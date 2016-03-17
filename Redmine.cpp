#include "Redmine.h"

#include "KeyAuthenticator.h"
#include "PasswordAuthenticator.h"

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

        cbFct callback = callbacks_[reply];
        callback( reply, &data_json );

        callbacks_.remove( reply );
    }
}

void
Redmine::retrieveCustomFields( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "custom_fields", callback, Mode::GET, filters );
}

void
Redmine::retrieveEnumerations( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "enumerations", callback, Mode::GET, filters );
}

void
Redmine::retrieveIssueCategories( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "issue_categories", callback, Mode::GET, filters );
}

void
Redmine::retrieveIssues( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "issues", callback, Mode::GET, filters );
}

void
Redmine::retrieveIssueStatuses( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "issue_statuses", callback, Mode::GET, filters );
}

void
Redmine::retrieveProjects( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "projects", callback, Mode::GET, filters );
}

void
Redmine::retrieveUsers( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "users", callback, Mode::GET, filters );
}

void
Redmine::retrieveTimeEntries( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "time_entries", callback, Mode::GET, filters );
}

void
Redmine::retrieveTrackers( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "trackers", callback, Mode::GET, filters );
}

void
Redmine::retrieveVersions( cbFct callback, QString filters )
{
    qEnter();
    sendRequest( "versions", callback, Mode::GET, filters );
}

QNetworkReply*
Redmine::sendRequest( QString resource, cbFct callback, Mode mode, const QString& queryParams,
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

    case Mode::POST:
        reply = nma_->post( request, postData );
        break;

    case Mode::PUT:
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
