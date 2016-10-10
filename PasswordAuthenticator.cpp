#include "Logging.h"
#include "PasswordAuthenticator.h"

#include <QNetworkRequest>

using namespace qtredmine;

PasswordAuthenticator::PasswordAuthenticator( QString login, QString password, QObject* parent )
    : Authenticator( parent ),
      login_( login ),
      password_( password )
{
    ENTER()(login)(password);
    RETURN();
}

void PasswordAuthenticator::addAuthentication( QNetworkRequest* request )
{
    ENTER()(request);

    QByteArray auth = QByteArray(QString("%1:%2").arg(login_).arg(password_).toLatin1().toBase64());
    request->setRawHeader( "Authorization", "Basic " + auth );

    RETURN();
}
