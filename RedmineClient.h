#ifndef REDMINECLIENT_H
#define REDMINECLIENT_H

#include "qtredmine_global.h"

#include "Authenticator.h"

#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

#include <functional>

/// QtRedmine namespace
namespace qtredmine {

/**
 * @example Example.h
 * @example Example.cpp
 */

/**
 * @brief Redmine connection class
 *
 * This class handles the connections to a Redmine instance and provides access to
 * the Redmine REST API.
 *
 * @sa http://www.redmine.org/projects/redmine/wiki/Rest_api
 */
class QTREDMINESHARED_EXPORT RedmineClient : public QObject
{
    Q_OBJECT

public:
    /// Operation mode
    enum class Mode {
        GET,    ///< Get items from Redmine
        ADD,    ///< Add a new item to Redmine
        UPD,    ///< Update a Redmine item
        DEL     ///< Delete a Redmine item
    };

public:
    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param parent Parent QObject (default: nullptr)
     */
    RedmineClient( QObject* parent = nullptr );

    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param url    Redmine base URL
     * @param parent Parent QObject (default: nullptr)
     */
    RedmineClient( QString url, QObject* parent = nullptr );

    /**
     * @brief Constructor for a Redmine connection using API key authentication
     *
     * @param url      Redmine base URL
     * @param apiKey   Redmine API key
     * @param checkSsl Check the SSL certificate (default: true)
     * @param parent   Parent QObject (default: nullptr)
     */
    RedmineClient( QString url, QString apiKey,
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
    RedmineClient( QString url, QString login, QString password,
                   bool checkSsl   = true,
                   QObject* parent = nullptr );

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

    /// Typedef for a JSON callback function
    using JsonCb = std::function<void(QNetworkReply*, QJsonDocument*)>;

    /**
     * @brief Retrieve custom fields from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional custom field parameters
     */
    void retrieveCustomFields( JsonCb callback,
                               QString parameters = "" );

    /**
     * @brief Retrieve issue categories from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional issue category parameters
     */
    void retrieveIssueCategories( JsonCb callback,
                                  QString parameters = "" );

    /**
     * @brief Retrieve issue priorities from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional enumeration parameters
     */
    void retrieveIssuePriorities( JsonCb callback,
                                  QString parameters = "" );

    /**
     * @brief Retrieve issues from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional issue parameters
     */
    void retrieveIssues( JsonCb callback,
                         QString parameters = "" );

    /**
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional issue status parameters
     */
    void retrieveIssueStatuses( JsonCb callback,
                                QString parameters = "" );

    /**
     * @brief Retrieve projects from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional project parameters
     */
    void retrieveProjects( JsonCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve time entries from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional time entry parameters
     */
    void retrieveTimeEntries( JsonCb callback,
                              QString parameters = "" );

    /**
     * @brief Retrieve time entry activities from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional enumeration parameters
     */
    void retrieveTimeEntryActivities( JsonCb callback,
                                      QString parameters = "" );

    /**
     * @brief Retrieve trackers from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional tracker parameters
     */
    void retrieveTrackers( JsonCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve versions from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional version parameters
     */
    void retrieveVersions( JsonCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve users from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional user parameters
     */
    void retrieveUsers( JsonCb callback,
                        QString parameters = "" );

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
                                JsonCb callback            = nullptr,
                                Mode mode                  = Mode::GET,
                                const QString& queryParams = "",
                                const QByteArray& postData = "" );

    /**
     * @brief Retrieve enumerations from Redmine
     *
     * @param enumeration The enumeration to load
     * @param callback    Callback function with a QJsonDocument object
     * @param parameters  Additional enumeration parameters
     */
    void retrieveEnumerations( QString enumeration,
                               JsonCb  callback,
                               QString parameters = "" );

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
    QMap<QNetworkReply*, JsonCb> callbacks_;

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
    void requestFinished( JsonCb callback, QNetworkReply* reply, QJsonDocument* json );
};

inline QDebug
operator<<( QDebug debug, const RedmineClient::Mode& mode )
{
    QDebugStateSaver saver( debug );

    switch( mode )
    {
    case RedmineClient::Mode::GET: debug.nospace() << "GET"; break;
    case RedmineClient::Mode::ADD: debug.nospace() << "ADD"; break;
    case RedmineClient::Mode::UPD: debug.nospace() << "UPD"; break;
    case RedmineClient::Mode::DEL: debug.nospace() << "DEL"; break;
    }

    return debug;
}

} // qtredmine

#endif // REDMINECLIENT_H
