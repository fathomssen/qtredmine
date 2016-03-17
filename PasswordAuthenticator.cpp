#include "PasswordAuthenticator.h"

#include "logging.h"

#include <QNetworkRequest>

using namespace qtredmine;

PasswordAuthenticator::PasswordAuthenticator( QString login, QString password, QObject* parent )
    : Authenticator( parent ),
      login_( login ),
      password_( password )
{
    qEnter() << _(login) << _(password);
}

void PasswordAuthenticator::addAuthentication( QNetworkRequest* request )
{
    qEnter() << _(request);

    QByteArray auth = QByteArray(QString("%1:%2").arg(login_).arg(password_).toLatin1().toBase64());
    request->setRawHeader( "Authorization", "Basic " + auth );
}
