#ifndef KEY_AUTHENTICATOR_H
#define KEY_AUTHENTICATOR_H

#include "qtredmine_global.h"
#include "Authenticator.h"

#include <QString>

namespace qtredmine {

/**
 * @brief API key authenticator
 *
 * Adds an "X-Redmine-API-Key" header with API key.
 */
class QTREDMINESHARED_EXPORT KeyAuthenticator : public Authenticator
{
private:
    /// Redmine API Key
    QByteArray apiKey_;

public:
    /**
     * @brief Constructor
     *
     * @param apiKey Redmine API key
     * @param parent Parent QObject
     */
    KeyAuthenticator( QByteArray apiKey, QObject* parent = nullptr );

    /**
     * @brief Destructor
     */
    virtual ~KeyAuthenticator() {}

    /**
     * @brief Add authentication header to request
     *
     * @param request Request to add header to
     */
    virtual void addAuthentication( QNetworkRequest* request );
};

} // qtredmine

#endif // KEY_AUTHENTICATOR_H
