#ifndef PASSWORD_AUTHENTICATOR_H
#define PASSWORD_AUTHENTICATOR_H

#include "qtredmine_global.h"
#include "Authenticator.h"

#include <QString>

namespace qtredmine {

/**
 * @brief Basic login and password authenticator
 *
 * Adds an "Authorization" header using HTTP Basic authentication.
 */
class QTREDMINESHARED_EXPORT PasswordAuthenticator : public Authenticator
{
private:
    QString login_;
    QString password_;

public:
    /**
     * @brief Constructor
     *
     * @param login    Redmine login
     * @param password Redmine password
     * @param parent   Parent QObject
     */
    PasswordAuthenticator( QString login, QString password, QObject* parent = nullptr );

    /**
     * @brief Destructor
     */
    virtual ~PasswordAuthenticator() {}

    /**
     * @brief Add authentication header to request
     *
     * @param request Request to add header to
     */
    virtual void addAuthentication( QNetworkRequest* request );
};

} // qtredmine

#endif // PASSWORD_AUTHENTICATOR_H
