#include "KeyAuthenticator.h"
#include "Logging.h"
#include "PasswordAuthenticator.h"
#include "RedmineClient.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace qtredmine;

RedmineClient::RedmineClient( QObject* parent )
    : QObject( parent )
{
    ENTER();
    RETURN();
}

RedmineClient::RedmineClient( QString url, QObject* parent )
    : RedmineClient( parent )
{
    ENTER()(url);

    setUrl( url );

    RETURN();
}

RedmineClient::RedmineClient( QString url, QString apiKey, bool checkSsl, QObject* parent )
    : RedmineClient( url, parent )
{
    ENTER()(url)(apiKey)(checkSsl);

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( apiKey );

    RETURN();
}

RedmineClient::RedmineClient( QString url, QString login, QString password, bool checkSsl, QObject* parent )
    : RedmineClient( url, parent )
{
    ENTER()(url)(login)(password)(checkSsl);

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( login, password );

    RETURN();
}

void
RedmineClient::init()
{
    ENTER();

    reconnect();
    initialised();

    RETURN();
}

void
RedmineClient::reconnect()
{
    ENTER();

    // Create QNetworkAccessManager object
    nma_ = new QNetworkAccessManager( this );

    // When a reqest to the network access manager has finished, call this->replyFinished()
    connect( nma_, &QNetworkAccessManager::finished, this, &RedmineClient::replyFinished );

    // Handle SSL errors
    connect( nma_, &QNetworkAccessManager::sslErrors, this, &RedmineClient::handleSslErrors );

    // Handle network accessibility change
    connect( nma_, &QNetworkAccessManager::networkAccessibleChanged,
             [&](QNetworkAccessManager::NetworkAccessibility accessible)
             { ENTER(); networkAccessibleChanged( accessible ); } );

    RETURN();
}

QString
RedmineClient::getUrl() const
{
    ENTER();
    RETURN( url_ );
}

void
RedmineClient::handleSslErrors( QNetworkReply* reply, const QList<QSslError>& errors )
{
    ENTER()(reply)(errors);

    if( !checkSsl_ )
        reply->ignoreSslErrors();

    RETURN();
}

void
RedmineClient::setAuthenticator( QString apiKey )
{
    ENTER()(apiKey);

    if( apiKey == authApiKey_ )
        RETURN();

    authApiKey_ = apiKey;

    auth_ = new KeyAuthenticator( apiKey.toLatin1(), this );

    if( !url_.isEmpty() )
        init();

    RETURN();
}

void
RedmineClient::setAuthenticator( QString login, QString password )
{
    ENTER()(login)(password);

    if( login == authLogin_ && password == authPassword_ )
        RETURN();

    authLogin_ = login;
    authPassword_ = password;

    auth_ = new PasswordAuthenticator( login, password, this );

    if( !url_.isEmpty() )
        init();

    RETURN();
}

void
RedmineClient::setCheckSsl( bool checkSsl )
{
    ENTER()(checkSsl);

    if( checkSsl == checkSsl_ )
        RETURN();

    checkSsl_ = checkSsl;

    if( auth_ && !url_.isEmpty() )
        init();

    RETURN();
}

void
RedmineClient::setUrl( QString url )
{
    ENTER()(url);

    if( url == url_ )
        RETURN();

    url_ = url;

    if( auth_ )
        init();

    RETURN();
}

void
RedmineClient::setUserAgent( QByteArray userAgent )
{
    ENTER()(userAgent);

    userAgent_ = userAgent;

    RETURN();
}

QNetworkReply*
RedmineClient::sendRequest( QString resource, JsonCb callback, QNetworkAccessManager::Operation mode,
                            const QString& queryParams, const QByteArray& postData )
{
    ENTER()(resource)(mode)(queryParams)(postData);

    //
    // Initial checks
    //

    if( !nma_ )
    {
        DEBUG( "Network manager not yet initialised" );
        RETURN( nullptr );
    }

    if( resource.isEmpty() )
    {
        DEBUG( "No resource specified" );
        RETURN( nullptr );
    }

    if( mode == QNetworkAccessManager::GetOperation && !callback )
    {
        DEBUG( "No callback specified for HTTP GET mode" );
        RETURN( nullptr );
    }

    //
    // Build the Redmine REST URL
    //

    QUrl url = url_ + "/" + resource + ".json?" + queryParams;

    if( !url.isValid() )
    {
        DEBUG("Invalid URL")(url);
        RETURN( nullptr );
    }
    else
        DEBUG("Using URL")(url);

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

    switch( mode )
    {
    case QNetworkAccessManager::GetOperation:
        reply = nma_->get( request );
        break;

    case QNetworkAccessManager::PostOperation:
        reply = nma_->post( request, postData );
        break;

    case QNetworkAccessManager::PutOperation:
        reply = nma_->put( request, postData );
        break;

    case QNetworkAccessManager::DeleteOperation:
        reply = nma_->deleteResource( request );
        break;
    default:
        DEBUG( "Unknown operation" );
        RETURN( nullptr );
    }

    if( reply && callback )
        callbacks_[reply] = callback;

    RETURN( reply );
}

void
RedmineClient::replyFinished( QNetworkReply* reply )
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
getResMode( int id, QString& resource, QNetworkAccessManager::Operation& mode )
{
    ENTER()(id)(resource)(mode);

    if( id == NULL_ID )
        mode = QNetworkAccessManager::PostOperation;
    else
    {
        resource.append("/").append(QString::number(id));
        mode = QNetworkAccessManager::PutOperation;
    }

    RETURN();
}

void
RedmineClient::sendCustomField( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "custom_fields";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendEnumeration( QString enumeration, const QJsonDocument& data, JsonCb callback, int id,
                                QString parameters )
{
    ENTER()(enumeration)(parameters);

    QString resource = "enumerations/"+enumeration;
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendIssue( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(id)(parameters);

    QString resource = "issues";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendIssueCategory( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "issue_categories";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendIssuePriority( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "issue_priorities";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendEnumeration( resource, data, callback, id, parameters );

    RETURN();
}

void
RedmineClient::sendIssueStatus( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "issue_statuses";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendProject( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "projects";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendTimeEntry( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "time_entries";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendTimeEntryActivity( const QJsonDocument& data, JsonCb callback, int id,
                                      QString parameters )
{
    ENTER()(parameters);

    QString resource = "time_entry_activities";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendEnumeration( resource, data, callback, id, parameters );

    RETURN();
}

void
RedmineClient::sendTracker( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "trackers";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendUser( const QJsonDocument& data, JsonCb callback, int id, QString parameters )
{
    ENTER()(parameters);

    QString resource = "users";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::retrieveCustomFields( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "shared/custom_fields", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveEnumerations( QString enumeration, JsonCb callback, QString parameters )
{
    ENTER()(enumeration)(parameters);

    sendRequest( "enumerations/"+enumeration, callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssues( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "issues", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssueCategories( JsonCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1/issue_categories").arg(projectId), callback,
                 QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssuePriorities( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "issue_priorities", callback, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssue( JsonCb callback, int issueId, QString parameters )
{
    ENTER()(issueId)(parameters);

    sendRequest( QString("issues/%1").arg(issueId), callback, QNetworkAccessManager::GetOperation,
                 parameters );

    RETURN();
}

void
RedmineClient::retrieveIssueStatuses( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "issue_statuses", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveMemberships( JsonCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1/memberships").arg(projectId), callback,
                 QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveProject( JsonCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1").arg(projectId), callback, QNetworkAccessManager::GetOperation,
                 QString("%1&include=%2").arg(parameters).arg("enabled_modules,issue_categories,trackers") );

    RETURN();
}

void
RedmineClient::retrieveProjects( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "projects", callback, QNetworkAccessManager::GetOperation,
                 QString("%1&include=%2").arg(parameters).arg("enabled_modules,issue_categories,trackers") );

    RETURN();
}

void
RedmineClient::retrieveTimeEntries( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "time_entries", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveTimeEntryActivities( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "time_entry_activities", callback, parameters );

    RETURN();
}

void
RedmineClient::retrieveTrackers( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "trackers", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveCurrentUser( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "users/current", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveUsers( JsonCb callback, QString parameters )
{
    ENTER()(parameters);

    sendRequest( "users", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveVersions(JsonCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1/versions").arg(projectId), callback,
                 QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}
