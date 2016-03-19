#include "Redmine.h"

#include "KeyAuthenticator.h"
#include "PasswordAuthenticator.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>

#include "logging.h"

using namespace qtredmine;

Redmine::Redmine( QObject* parent )
    : QObject( parent )
{
    qEnter();
}

Redmine::Redmine( QString url, QObject* parent )
    : QObject( parent )
{
    qEnter() << _(url);
    setUrl( url );
}

Redmine::Redmine( QString url, QString apiKey, bool checkSsl, QObject* parent )
    : QObject( parent )
{
    qEnter();

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( apiKey );
}

Redmine::Redmine( QString url, QString login, QString password, bool checkSsl, QObject* parent )
    : QObject( parent )
{
    qEnter();

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( login, password );
}

Redmine::~Redmine()
{
    qEnter();
}

QString
Redmine::getUrl() const
{
    qEnter();
    qReturn( url_ );
}

void
Redmine::init()
{
    qEnter();

    // Create QNetworkAccessManager object
    nma_ = new QNetworkAccessManager( this );

    // When a reqest to the network access manager has finished, call this->replyFinished()
    connect( nma_, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)) );
}

void
Redmine::replyFinished( QNetworkReply* reply )
{
    qEnter();

    // Search for callback function
    if( callbacks_.contains(reply) )
    {
        QByteArray data_raw = reply->readAll();
        QJsonDocument data_json = QJsonDocument::fromJson( data_raw );

        JsonCb callback = callbacks_[reply];
        callback( reply, &data_json );

        callbacks_.remove( reply );
    }
}

void
Redmine::retrieveCustomFields( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "custom_fields", callback, Mode::GET, parameters );
}

void
Redmine::retrieveEnumerations(QString enumeration, JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "enumerations/"+enumeration, callback, Mode::GET, parameters );
}

void
Redmine::retrieveEnumerations(QString enumeration, EnumerationsCb callback, QString parameters )
{
    qEnter();

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        qEnter();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
            qReturn();

        Enumerations enumerations;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all enumerations
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Enumeration enumeration;

                // Simple fields
                enumeration.id        = obj.value("id").toInt();
                enumeration.name      = obj.value("name").toString();
                enumeration.isDefault = obj.value("is_default").toBool();

                enumerations.push_back( enumeration );
            }
        }

        callback( enumerations );
    };

    retrieveEnumerations( enumeration, cb, parameters );
}

void
Redmine::retrieveIssueCategories( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "issue_categories", callback, Mode::GET, parameters );
}

void
Redmine::retrieveIssuePriorities( JsonCb callback, QString parameters )
{
    qEnter();
    retrieveEnumerations( "issue_priorities", callback, parameters );
}

void
Redmine::retrieveIssuePriorities( EnumerationsCb callback, QString parameters )
{
    qEnter();
    retrieveEnumerations( "issue_priorities", callback, parameters );
}

void
Redmine::retrieveIssues( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "issues", callback, Mode::GET, parameters );
}

void
Redmine::retrieveIssues( IssuesCb callback, QString parameters )
{
    qEnter();

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        qEnter();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
            qReturn();

        Issues issues;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all issues
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Issue issue;

                // Simple fields
                issue.id          = obj.value("id").toInt();
                issue.description = obj.value("description").toString();
                issue.doneRatio   = obj.value("done_ratio").toInt();
                issue.subject     = obj.value("subject").toString();

                // Items
                auto fillItem = [&]( Item& item, QString value)
                {
                    QJsonObject itemObj = obj.value(value).toObject();

                    item.id   = itemObj.value("id").toInt();
                    item.name = itemObj.value("name").toString();
                };

                fillItem( issue.author,   "author" );
                fillItem( issue.category, "category" );
                fillItem( issue.priority, "priority" );
                fillItem( issue.project,  "project" );
                fillItem( issue.status,   "status" );
                fillItem( issue.tracker,  "tracker" );

                // Dates and times
                issue.createdOn      = obj.value("created_on").toVariant().toDateTime();
                issue.dueDate        = obj.value("due_date").toVariant().toDate();
                issue.estimatedHours = obj.value("estimated_hours").toVariant().toTime();
                issue.startDate      = obj.value("start_date").toVariant().toDate();
                issue.updatedOn      = obj.value("updated_on").toVariant().toDateTime();

                // Custom field
                auto addCustomField = [&](QJsonObject cfObj)
                {
                    CustomField customField;
                    customField.id   = cfObj.value("id").toInt();
                    customField.name = cfObj.value("name").toString();

                    for( const auto& v : cfObj.value("value").toArray() )
                      customField.value.push_back( v.toString() );

                    issue.customFields.push_back( customField );
                };

                for( const auto& cf : obj.value("custom_fields").toArray() )
                    addCustomField( cf.toObject() );

                issues.push_back( issue );
            }
        }

        callback( issues );
    };

    retrieveIssues( cb, parameters );
}

void
Redmine::retrieveIssueStatuses( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "issue_statuses", callback, Mode::GET, parameters );
}

void
Redmine::retrieveIssueStatuses( IssueStatusesCb callback, QString parameters )
{
    qEnter();

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        qEnter();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
            qReturn();

        IssueStatuses issueStatuses;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all issueStatuss
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                IssueStatus issueStatus;

                // Simple fields
                issueStatus.id        = obj.value("id").toInt();
                issueStatus.name      = obj.value("name").toString();
                issueStatus.isDefault = obj.value("is_default").toBool();

                issueStatuses.push_back( issueStatus );
            }
        }

        callback( issueStatuses );
    };

    retrieveIssueStatuses( cb, parameters );
}

void
Redmine::retrieveProjects( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "projects", callback, Mode::GET, parameters );
}

void
Redmine::retrieveProjects( ProjectsCb callback, QString parameters )
{
    qEnter();

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        qEnter();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
            qReturn();

        Projects projects;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all projects
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Project project;

                // Simple fields
                project.id          = obj.value("id").toInt();
                project.description = obj.value("description").toString();
                project.identifier  = obj.value("identifier").toString();
                project.isPublic    = obj.value("is_public").toBool();
                project.name        = obj.value("name").toString();

                // Dates and times
                project.createdOn = obj.value("created_on").toVariant().toDateTime();
                project.updatedOn = obj.value("updated_on").toVariant().toDateTime();

                projects.push_back( project );
            }
        }

        callback( projects );
    };

    retrieveProjects( cb, parameters );
}

void
Redmine::retrieveTimeEntries( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "time_entries", callback, Mode::GET, parameters );
}

void
Redmine::retrieveTimeEntryActivities( JsonCb callback, QString parameters )
{
    qEnter();
    retrieveEnumerations( "time_entry_activities", callback, parameters );
}

void
Redmine::retrieveTimeEntryActivities( EnumerationsCb callback, QString parameters )
{
    qEnter();
    retrieveEnumerations( "time_entry_activities", callback, parameters );
}

void
Redmine::retrieveTrackers( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "trackers", callback, Mode::GET, parameters );
}

void
Redmine::retrieveTrackers( TrackersCb callback, QString parameters )
{
    qEnter();

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        qEnter();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
            qReturn();

        Trackers trackers;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all trackers
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Tracker tracker;

                // Simple fields
                tracker.id   = obj.value("id").toInt();
                tracker.name = obj.value("name").toString();

                trackers.push_back( tracker );
            }
        }

        callback( trackers );
    };

    retrieveTrackers( cb, parameters );
}

void
Redmine::retrieveUsers( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "users", callback, Mode::GET, parameters );
}

void
Redmine::retrieveVersions( JsonCb callback, QString parameters )
{
    qEnter();
    sendRequest( "versions", callback, Mode::GET, parameters );
}

QNetworkReply*
Redmine::sendRequest( QString resource, JsonCb callback, Mode mode, const QString& queryParams,
                      const QByteArray& postData )
{
    qEnter();

    //
    // Initial checks
    //

    if( resource.isEmpty() )
    {
        qDebug() << "No resource specified";
        qReturn( nullptr );
    }

    if( mode == Mode::GET && !callback )
    {
        qDebug() << "No callback specified for HTTP GET mode";
        qReturn( nullptr );
    }

    //
    // Build the Redmine REST URL
    //

    QUrl url = url_ + "/" + resource + ".json?" + queryParams;

    if( !url.isValid() )
    {
        qDebug() <<  "Invalid URL:"  << url;
        qReturn( nullptr );
    }
    else
      qDebug() <<  "Using URL:"  << url;

    //
    // Build the network request
    //

    QNetworkRequest request( url );
    request.setRawHeader( "User-Agent",          userAgent_ );
    request.setRawHeader( "X-Custom-User-Agent", userAgent_ );
    request.setRawHeader( "Content-Type",        "application/json" );
    request.setRawHeader( "Content-Length",      QByteArray::number(postData.size()) );
    auth_->addAuthentication( &request );

    //
    // Perform the network action
    //

    QNetworkReply* reply;

    if( !checkSsl_ )
        reply->ignoreSslErrors();

    switch( mode )
    {
    case Mode::GET:
        reply = nma_->get( request );
        break;

    case Mode::ADD:
        reply = nma_->post( request, postData );
        break;

    case Mode::UPDATE:
        reply = nma_->put( request, postData );
        break;

    case Mode::DEL:
        reply = nma_->deleteResource( request );
        break;
    }

    if( reply && callback )
        callbacks_[reply] = callback;

    qReturn( reply );
}

void
Redmine::setAuthenticator( QString apiKey )
{
    qEnter();

    auth_ = new KeyAuthenticator( apiKey.toLatin1(), this );
    init();
}

void
Redmine::setAuthenticator( QString login, QString password )
{
    qEnter();

    auth_ = new PasswordAuthenticator( login, password, this );
    init();
}

void
Redmine::setCheckSsl( bool checkSsl )
{
    qEnter();
    checkSsl_ = checkSsl;
}

void
Redmine::setUrl( QString url )
{
    qEnter();
    url_ = url;
}

void
Redmine::setUserAgent( QByteArray userAgent )
{
    qEnter();
    userAgent_ = userAgent;
}
