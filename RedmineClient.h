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

#define NULL_ID -1

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
    /// Typedef for a JSON callback function
    using JsonCb = std::function<void(QNetworkReply*, QJsonDocument*)>;

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
     * @brief (Re-)Connect to Redmine
     */
    void reconnect();

    /// @name Getters
    /// @{

    /**
     * @brief Get the Redmine base URL
     *
     * @return Redmine base URL
     */
    QString getUrl() const;

    /// @}

    /// @name Setters
    /// @{

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

    /// @}

    /// @name Redmine data creators
    /// @{

    /**
     * @brief Create or update custom field in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Custom field ID to update; if set to \c NULL_ID, create a new custom field
     * @param parameters  Additional custom field parameters
     */
    void sendCustomField( const QJsonDocument& data,
                          JsonCb callback = nullptr,
                          int id = NULL_ID,
                          QString parameters = "" );

    /**
     * @brief Create or update issue in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Issue ID to update; if set to \c NULL_ID, create a new issue
     * @param parameters  Additional issue parameters
     */
    void sendIssue( const QJsonDocument& data,
                    JsonCb callback = nullptr,
                    int id = NULL_ID,
                    QString parameters = "" );

    /**
     * @brief Create or update issue category in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Issue category ID to update; if set to \c NULL_ID, create a new issue category
     * @param parameters  Additional issue category parameters
     */
    void sendIssueCategory( const QJsonDocument& data,
                            JsonCb callback = nullptr,
                            int id = NULL_ID,
                            QString parameters = "" );

    /**
     * @brief Create or update issue priority in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Issue priority ID to update; if set to \c NULL_ID, create a new issue priority
     * @param parameters  Additional enumeration parameters
     */
    void sendIssuePriority( const QJsonDocument& data,
                            JsonCb callback = nullptr,
                            int id = NULL_ID,
                            QString parameters = "" );

    /**
     * @brief Create or update issue status in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Issue status ID to update; if set to \c NULL_ID, create a new issue status
     * @param parameters  Additional issue status parameters
     */
    void sendIssueStatus( const QJsonDocument& data,
                          JsonCb callback = nullptr,
                          int id = NULL_ID,
                          QString parameters = "" );

    /**
     * @brief Create or update project in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Project ID to update; if set to \c NULL_ID, create a new project
     * @param parameters  Additional project parameters
     */
    void sendProject( const QJsonDocument& data,
                      JsonCb callback = nullptr,
                      int id = NULL_ID,
                      QString parameters = "" );

    /**
     * @brief Create or update time entry in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Time entry ID to update; if set to \c NULL_ID, create a new time entry
     * @param parameters  Additional time entry parameters
     */
    void sendTimeEntry( const QJsonDocument& data,
                        JsonCb callback = nullptr,
                        int id = NULL_ID,
                        QString parameters = "" );

    /**
     * @brief Create or update time entry activity in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Time entry activity ID to update; if set to \c NULL_ID, create a new time entry activity
     * @param parameters  Additional enumeration parameters
     */
    void sendTimeEntryActivity( const QJsonDocument& data,
                                JsonCb callback = nullptr,
                                int id = NULL_ID,
                                QString parameters = "" );

    /**
     * @brief Create or update tracker in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Tracker ID to update; if set to \c NULL_ID, create a new tracker
     * @param parameters  Additional tracker parameters
     */
    void sendTracker( const QJsonDocument& data,
                      JsonCb callback = nullptr,
                      int id = NULL_ID,
                      QString parameters = "" );

    /**
     * @brief Create or update user in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id User ID to update; if set to \c NULL_ID, create a new user
     * @param parameters  Additional user parameters
     */
    void sendUser( const QJsonDocument& data,
                   JsonCb callback = nullptr,
                   int id = NULL_ID,
                   QString parameters = "" );

    /**
     * @brief Create or update version in Redmine
     *
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Version ID to update; if set to \c NULL_ID, create a new version
     * @param parameters  Additional version parameters
     */
    void sendVersion( const QJsonDocument& data,
                      JsonCb callback = nullptr,
                      int id = NULL_ID,
                      QString parameters = "" );

    /// @}

    /// @name Redmine data retrievers
    /// @{

    /**
     * @brief Retrieve custom fields from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional custom field parameters
     */
    void retrieveCustomFields( JsonCb callback,
                               QString parameters = "" );

    /**
     * @brief Retrieve an issue from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param issueId Issue ID
     * @param parameters  Additional issue parameters
     */
    void retrieveIssue( JsonCb callback, int issueId,
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
     * @brief Retrieve issue categories from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param projectId Project ID
     * @param parameters Additional issue category parameters
     */
    void retrieveIssueCategories( JsonCb callback,
                                  int projectId,
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
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional issue status parameters
     */
    void retrieveIssueStatuses( JsonCb callback,
                                QString parameters = "" );

    /**
     * @brief Retrieve a project from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param projectId Project ID
     * @param parameters  Additional project parameters
     */
    void retrieveProject( JsonCb callback, int projectId,
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
     * @brief Retrieve current user from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional user parameters
     */
    void retrieveCurrentUser( JsonCb callback,
                              QString parameters = "" );

    /**
     * @brief Retrieve users from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional user parameters
     */
    void retrieveUsers( JsonCb callback,
                        QString parameters = "" );

    /**
     * @brief Retrieve versions from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param parameters  Additional version parameters
     */
    void retrieveVersions( JsonCb callback,
                           QString parameters = "" );

    /// @}

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
     * @param mode HTTP operation mode \sa Mode
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
                                JsonCb callback = nullptr,
                                QNetworkAccessManager::Operation mode = QNetworkAccessManager::GetOperation,
                                const QString& queryParams = "",
                                const QByteArray& postData = "" );

    /**
     * @brief Create or update enumeration in Redmine
     *
     * @param enumeration The enumeration to create
     * @param data Data to store in Redmine
     * @param callback Callback function with a QJsonDocument object
     * @param id Enumeration to update; if set to \c NULL_ID, create a enumeration
     * @param parameters Additional enumeration parameters
     */
    void sendEnumeration( QString enumeration,
                          const QJsonDocument& data,
                          JsonCb callback = nullptr,
                          int id = NULL_ID,
                          QString parameters = "" );

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
    Authenticator* auth_ = nullptr;

    /// Currently configured API key
    QString authApiKey_;

    /// Currently configured login
    QString authLogin_;

    /// Currently configured password
    QString authPassword_;

    /**
     * @brief Mapping from network reply to callback function
     *
     * This map can be used to find the correct callback for a network reply.
     * It is used by slot replyFinished() to call the desired callback function after
     * a reply from the network access manager has finished.
     *
     * A QMap is usually faster than a QHash for less than 20 elements.
     */
    QMap<QNetworkReply*, JsonCb> callbacks_;

    /// Determines whether SSL data (e.g. certificate validity) should be checked
    bool checkSsl_ = true;

    /// Network access manager for networking operations
    QNetworkAccessManager* nma_ = nullptr;

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
     * @brief Handle SSL errors
     *
     * @param reply The QNetworkReply the error occurred in
     * @param errors List of errors
     */
    void handleSslErrors( QNetworkReply* reply, const QList<QSslError>& errors );

    /**
     * @brief Process a reply from the network access manager
     *
     * @param reply Network reply object
     */
    void replyFinished( QNetworkReply* reply );

signals:
    /**
     * @brief Signal that the request has finished
     *
     * @param callback Callback function that will be called
     * @param reply    Reply that will be passed to the callback function
     * @param json     JSON data that will be passed to the callback function
     */
    void requestFinished( JsonCb callback, QNetworkReply* reply, QJsonDocument* json );

    /**
     * @brief Signal that the network accessibility has changed
     *
     * @param accessible Network accessibility
     */
    void networkAccessibleChanged( QNetworkAccessManager::NetworkAccessibility accessible );

    /**
     * @brief Signal that the connection has been initialised
     */
    void initialised();
};

} // qtredmine

#endif // REDMINECLIENT_H
