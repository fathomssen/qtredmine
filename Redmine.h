#ifndef REDMINE_H
#define REDMINE_H

#include "qtredmine_global.h"

#include "Authenticator.h"

#include <QByteArray>
#include <QHash>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QUrl>

#include <functional>

namespace qtredmine {

/**
 * @brief Redmine connection class
 *
 * This class handles the connections to a Redmine instance and provides access to
 * the Redmine REST API (@sa http://www.redmine.org/projects/redmine/wiki/Rest_api).
 *
 * @example Example.h
 * @example Example.cpp
 */
class QTREDMINESHARED_EXPORT Redmine : public QObject
{
    Q_OBJECT

public:
    /// Typedef for a callback function
    using cbFct = std::function<void(QNetworkReply*, QJsonDocument*)>;

    /// HTTP operation mode
    enum class Mode {
        GET,  ///< HTTP GET operation
        POST, ///< HTTP POST operation
        PUT,  ///< HTTP PUT operation
        DEL   ///< HTTP DELETE operation
    };

private:
    /// Currently configured authenticator for Redmine
    Authenticator* auth_;

    /**
     * @brief Mapping from network reply to callback function
     *
     * This hash table can be used to find the correct callback for a network reply.
     * It is used by slot replyFinished() to call the desired callback function after
     * a reply from the network access manager has finished.
     */
    QHash<QNetworkReply*, cbFct> callbacks_;

    /// Determines whether SSL data (e.g. certificate validity) should be checked
    bool checkSsl_ = true;

    /// Network access manager for networking operations
    QNetworkAccessManager* nma_;

    /// Redmine base URL
    QString url_;

    /// User agent for Redmine connection (default: "qtredmine")
    QByteArray userAgent_ = "qtredmine";

    /**
     * @brief Initialise the Redmine connection
     *
     * Creates the network access manager and sets up the signal and slot for callbacks.
     */
    void init();

public:
    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param parent Parent QObject (default: nullptr)
     */
    Redmine( QObject* parent = nullptr );

    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param url    Redmine base URL
     * @param parent Parent QObject (default: nullptr)
     */
    Redmine( QString url, QObject* parent = nullptr );

    /**
     * @brief Constructor for a Redmine connection using API key authentication
     *
     * @param url      Redmine base URL
     * @param apiKey   Redmine API key
     * @param checkSsl Check the SSL certificate (default: true)
     * @param parent   Parent QObject (default: nullptr)
     */
    Redmine( QString url, QString apiKey,
             bool checkSsl   = true,
             QObject* parent = nullptr );

    /**
     * @brief Constructor for a Redmine connection using basic authentication
     *
     * @param url      Redmine base URL
     * @param login    Redmine login
     * @param password Redmine password
     * @param checkSsl Check the SSL certificate (default: true)
     * @param parent   Parent QObject (default: nullptr)
     */
    Redmine( QString url, QString login, QString password,
             bool checkSsl   = true,
             QObject* parent = nullptr );

    /**
     * @brief Destructor
     */
    virtual ~Redmine();

    /**
     * @brief Get the Redmine base URL
     *
     * @return Redmine base URL
     */
    QString getUrl() const;

    /**
     * @brief Set the Redmine base URL
     *
     * @param url Redmine base URL
     */
    void setUrl( QString url );

    /**
     * @brief Set the Redmine authentification parameters
     *
     * @param apiKey Redmine API key
     */
    void setAuthenticator( QString apiKey );

    /**
     * @brief Set the Redmine authentification parameters
     *
     * @param login    Redmine login name
     * @param password Redmine login password
     */
    void setAuthenticator( QString login, QString password );

    /**
     * @brief Set the user agent for the Redmine connection
     *
     * @param userAgent User agent
     */
    void setUserAgent( QByteArray userAgent );

    /**
     * @brief Set the SSL check data (e.g. certificate validity)
     *
     * @param checkSsl Check SSL data
     */
    void setCheckSsl( bool checkSsl );

    /**
     * @brief Retrieve custom fields from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional custom field filters
     */
    void retrieveCustomFields( cbFct callback,
                               QString filters = "" );

    /**
     * @brief Retrieve enumerations from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional enumeration filters
     */
    void retrieveEnumerations( cbFct callback,
                               QString filters = "" );

    /**
     * @brief Retrieve issue categories from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional issue category filters
     */
    void retrieveIssueCategories( cbFct callback,
                                  QString filters = "" );

    /**
     * @brief Retrieve issues from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional issue filters
     */
    void retrieveIssues( cbFct callback,
                         QString filters = "" );

    /**
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional issue status filters
     */
    void retrieveIssueStatuses( cbFct callback,
                                QString filters = "" );

    /**
     * @brief Retrieve projects from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional project filters
     */
    void retrieveProjects( cbFct callback,
                           QString filters = "" );

    /**
     * @brief Retrieve time entries from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional time entry filters
     */
    void retrieveTimeEntries( cbFct callback,
                              QString filters = "" );

    /**
     * @brief Retrieve trackers from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional tracker filters
     */
    void retrieveTrackers( cbFct callback,
                           QString filters = "" );

    /**
     * @brief Retrieve versions from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional version filters
     */
    void retrieveVersions( cbFct callback,
                           QString filters = "" );

    /**
     * @brief Retrieve users from Redmine
     *
     * @param callback Callback function
     * @param filters  Additional user filters
     */
    void retrieveUsers( cbFct callback,
                        QString filters = "" );

protected:
    /**
     * @brief Send a request to Redmine
     *
     * @param resource
     *   @parblock
     *     The resource specific part of the Redmine REST URL, e.g. \c issues or \c projects
     *
     *     For example, if the Redmine base URL is set to <tt>https://redmine.site</tt> and \c resource is set
     *     to \c issues, the resulting Redmine REST URL will be <tt>https://redmine.site/issues.json</tt>.
     *
     *     @sa http://www.redmine.org/projects/redmine/wiki/Rest_api
     *   @endparblock
     *
     * @param callback
     *   @parblock
     *     Callback function for the result of the request
     *
     *     The callback function is required for GET operations and optional for other operations
     *   @endparblock
     *
     * @param mode HTTP operation mode (default: GET) \sa Mode
     *
     * @param queryParams
     *   @parblock
     *     Query parameters that are appended to the Redmine REST URL
     *
     *     For example, if the Redmine base URL is set to <tt>https://redmine.site</tt>, \c resource is set to
     *     \c issues and \c queryParams is set to <tt>project_id=2&tracker_id=1</tt>, the resulting Redmine
     *     REST URL will be <tt>https://redmine.site/issues.json?project_id=2&tracker_id=1</tt>.
     *   @endparblock
     *
     * @param postData Data that will be sent by POST and PUT operations
     *
     * @return The network reply for this request
     */
    QNetworkReply* sendRequest( QString resource,
                                cbFct callback             = nullptr,
                                Mode mode                  = Mode::GET,
                                const QString& queryParams = "",
                                const QByteArray& postData = "" );

private slots:
    /**
     * @brief Process a reply from the network access manager
     *
     * @param reply Network reply object
     */
    void replyFinished( QNetworkReply* reply );

signals:
    /**
     * @brief Signals that the request has finished
     *
     * @param callback Callback function that will be called
     * @param reply    Reply that will be passed to the callback function
     * @param json     JSON data that will be passed to the callback function
     */
    void requestFinished( cbFct callback, QNetworkReply* reply, QJsonDocument* data );
};

} // qtredmine

#endif // REDMINE_H
