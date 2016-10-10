#include "KeyAuthenticator.h"
#include "Logging.h"

#include <QNetworkRequest>

using namespace qtredmine;

KeyAuthenticator::KeyAuthenticator( QByteArray apiKey, QObject* parent )
    : Authenticator( parent ),
      apiKey_( apiKey )
{
    ENTER()(apiKey);
    RETURN();
}

void KeyAuthenticator::addAuthentication( QNetworkRequest* request )
{
    ENTER()(request);

    request->setRawHeader( "X-Redmine-API-Key", apiKey_ );

    RETURN();
}
