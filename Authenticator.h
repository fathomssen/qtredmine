#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include "qtredmine_global.h"

#include <QObject>

class QNetworkRequest;

namespace qtredmine {

/**
 * @brief Authenticator interface
 *
 * Handle the authentication for a given network request.
 */
class QTREDMINESHARED_EXPORT Authenticator : public QObject
{
    //Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param param Parent QObject
     */
    Authenticator( QObject* parent = nullptr ) : QObject( parent ) {}

    /**
     * @brief Destructor
     */
    virtual ~Authenticator() {}

    /**
     * @brief Add authentication header to request
     *
     * @param request Request to add header to
     */
    virtual void addAuthentication( QNetworkRequest* request ) = 0;
};

} // qtredmine

#endif // AUTHENTICATOR_H
