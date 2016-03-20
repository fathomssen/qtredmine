#include "KeyAuthenticator.h"

#include "logging.h"

#include <QNetworkRequest>

using namespace qtredmine;

KeyAuthenticator::KeyAuthenticator( QByteArray apiKey, QObject* parent )
    : Authenticator( parent ),
      apiKey_( apiKey )
{
    qEnter() << _(apiKey);
    qReturn();
}

void KeyAuthenticator::addAuthentication( QNetworkRequest* request )
{
    qEnter() << _(request);

    request->setRawHeader( "X-Redmine-API-Key", apiKey_ );

    qReturn();
}
