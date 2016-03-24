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
    ENTER();
    RETURN();
}

Redmine::Redmine( QString url, QObject* parent )
    : QObject( parent )
{
    ENTER()(url);

    setUrl( url );

    RETURN();
}

Redmine::Redmine( QString url, QString apiKey, bool checkSsl, QObject* parent )
    : QObject( parent )
{
    ENTER()(url)(apiKey)(checkSsl);

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( apiKey );

    RETURN();
}

Redmine::Redmine( QString url, QString login, QString password, bool checkSsl,
                  QObject* parent )
    : QObject( parent )
{
    ENTER()(url)(login)(password)(checkSsl);

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( login, password );

    RETURN();
}

Redmine::~Redmine()
{
    ENTER();
    RETURN();
}

QString
Redmine::getUrl() const
{
    ENTER();
    RETURN( url_ );
}

void
Redmine::init()
{
    ENTER();

    // Create QNetworkAccessManager object
    nma_ = new QNetworkAccessManager( this );

    // When a reqest to the network access manager has finished, call this->replyFinished()
    connect( nma_, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)) );

    RETURN();
}

void
Redmine::replyFinished( QNetworkReply* reply )
{
    ENTER();

    // Search for callback function
    if( callbacks_.contains(reply) )
    {
        QByteArray data_raw = reply->readAll();
        QJsonDocument data_json = QJsonDocument::fromJson( data_raw );

        JsonCb callback = callbacks_[reply];
        callback( reply, &data_json );

        callbacks_.remove( reply );
    }

    RETURN();
}

void
Redmine::retrieveCustomFields( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "custom_fields", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveEnumerations(QString enumeration, JsonCb callback, QString parameters )
{
    ENTER()(enumeration)(parameters);

    sendRequest( "enumerations/"+enumeration, callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveEnumerations(QString enumeration, EnumerationsCb callback, QString parameters )
{
    ENTER()(enumeration)(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            RETURN();
        }

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

        RETURN();
    };

    retrieveEnumerations( enumeration, cb, parameters );

    RETURN();
}

void
Redmine::retrieveIssueCategories( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "issue_categories", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveIssuePriorities( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "issue_priorities", callback, parameters );

    RETURN();
}

void
Redmine::retrieveIssuePriorities( EnumerationsCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "issue_priorities", callback, parameters );

    RETURN();
}

void
Redmine::retrieveIssues( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "issues", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveIssues( IssuesCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            RETURN();
        }

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

        RETURN();
    };

    retrieveIssues( cb, parameters );

    RETURN();
}

void
Redmine::retrieveIssueStatuses( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "issue_statuses", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveIssueStatuses( IssueStatusesCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            RETURN();
        }

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

        RETURN();
    };

    retrieveIssueStatuses( cb, parameters );

    RETURN();
}

void
Redmine::retrieveProjects( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "projects", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveProjects( ProjectsCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            RETURN();
        }

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

        RETURN();
    };

    retrieveProjects( cb, parameters );

    RETURN();
}

void
Redmine::retrieveTimeEntries( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "time_entries", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveTimeEntryActivities( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "time_entry_activities", callback, parameters );

    RETURN();
}

void
Redmine::retrieveTimeEntryActivities( EnumerationsCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "time_entry_activities", callback, parameters );

    RETURN();
}

void
Redmine::retrieveTrackers( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "trackers", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveTrackers( TrackersCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            RETURN();
        }

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

        RETURN();
    };

    retrieveTrackers( cb, parameters );

    RETURN();
}

void
Redmine::retrieveUsers( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "users", callback, Mode::GET, parameters );

    RETURN();
}

void
Redmine::retrieveVersions( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "versions", callback, Mode::GET, parameters );

    RETURN();
}

QNetworkReply*
Redmine::sendRequest( QString resource, JsonCb callback, Mode mode, QString queryParams,
                      const QByteArray& postData )
{
    ENTER()(resource)(mode)(queryParams)(postData);

    //
    // Initial checks
    //

    if( resource.isEmpty() )
    {
        DEBUG() << "No resource specified";
        RETURN( nullptr );
    }

    if( mode == Mode::GET && !callback )
    {
        DEBUG() << "No callback specified for HTTP GET mode";
        RETURN( nullptr );
    }

    //
    // Build the Redmine REST URL
    //

    QUrl url = url_ + "/" + resource + ".json?" + queryParams;

    if( !url.isValid() )
    {
        DEBUG() <<  "Invalid URL:"  << url;
        RETURN( nullptr );
    }
    else
      DEBUG() <<  "Using URL:"  << url;

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

    RETURN( reply );
}

void
Redmine::setAuthenticator( QString apiKey )
{
    ENTER()(apiKey);

    auth_ = new KeyAuthenticator( apiKey.toLatin1(), this );
    init();

    RETURN();
}

void
Redmine::setAuthenticator( QString login, QString password )
{
    ENTER()(login)(password);

    auth_ = new PasswordAuthenticator( login, password, this );
    init();

    RETURN();
}

void
Redmine::setCheckSsl( bool checkSsl )
{
    ENTER()(checkSsl);

    checkSsl_ = checkSsl;

    RETURN();
}

void
Redmine::setUrl( QString url )
{
    ENTER()(url);

    url_ = url;

    RETURN();
}

void
Redmine::setUserAgent( QByteArray userAgent )
{
    ENTER()(userAgent);

    userAgent_ = userAgent;

    RETURN();
}
