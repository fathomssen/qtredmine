#ifndef SIMPLEREDMINECLIENT_H
#define SIMPLEREDMINECLIENT_H

#include "qtredmine_global.h"

#include "RedmineClient.h"
#include "SimpleRedmineTypes.h"

#include <QObject>
#include <QString>

namespace qtredmine {

/**
 * @brief Simple Redmine connection class
 *
 * This class handles the connections to a Redmine instance and provides access to
 * the Redmine REST API.
 *
 * It provides a simple API for accessing Redmine.
 *
 * @sa http://www.redmine.org/projects/redmine/wiki/Rest_api
 */
class QTREDMINESHARED_EXPORT SimpleRedmineClient : public RedmineClient
{
    Q_OBJECT

private:
    /// Maximum number of resources to fetch at once
    int limit_ = 100;

    /// Current connection status to Redmine
    QNetworkAccessManager::NetworkAccessibility connected_ = QNetworkAccessManager::UnknownAccessibility;

public:
    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param parent Parent QObject (default: nullptr)
     */
    SimpleRedmineClient( QObject* parent = nullptr );

    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param url    Redmine base URL
     * @param parent Parent QObject (default: nullptr)
     */
    SimpleRedmineClient( QString url, QObject* parent = nullptr );

    /**
     * @brief Constructor for a Redmine connection using API key authentication
     *
     * @param url      Redmine base URL
     * @param apiKey   Redmine API key
     * @param checkSsl Check the SSL certificate (default: true)
     * @param parent   Parent QObject (default: nullptr)
     */
    SimpleRedmineClient( QString url, QString apiKey,
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
    SimpleRedmineClient( QString url, QString login, QString password,
                         bool checkSsl   = true,
                         QObject* parent = nullptr );

    /**
     * @brief Initialise the Redmine client
     */
    void init();

    /// @name Redmine data creators and updaters
    /// @{

    /**
     * @brief Create or update issue in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Issue ID to update; if set to \c NULL_ID, create a new issue
     * @param parameters Additional issue parameters
     */
    void sendIssue( Issue item,
                    SuccessCb callback = nullptr,
                    int id = NULL_ID,
                    QString parameters = "" );

    /**
     * @brief Create or update issue priority in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Issue priority ID to update; if set to \c NULL_ID, create a new issue priority
     * @param parameters Additional enumeration parameters
     */
    void sendIssuePriority( Enumeration item,
                            SuccessCb callback = nullptr,
                            int id = NULL_ID,
                            QString parameters = "" );

    /**
     * @brief Create or update issue status in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Issue status ID to update; if set to \c NULL_ID, create a new issue status
     * @param parameters Additional issue status parameters
     */
    void sendIssueStatus( IssueStatus item,
                          SuccessCb callback = nullptr,
                          int id = NULL_ID,
                          QString parameters = "" );

    /**
     * @brief Create or update project in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Project ID to update; if set to \c NULL_ID, create a new project
     * @param parameters Additional project parameters
     */
    void sendProject( Project item,
                      SuccessCb callback = nullptr,
                      int id = NULL_ID,
                      QString parameters = "" );

    /**
     * @brief Create or update time entry in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Time entry ID to update; if set to \c NULL_ID, create a new time entry
     * @param parameters Additional time entry parameters
     */
    void sendTimeEntry( TimeEntry item,
                        SuccessCb callback = nullptr,
                        int id = NULL_ID,
                        QString parameters = "" );

    /**
     * @brief Create or update time entry activity in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Time entry activity ID to update; if set to \c NULL_ID, create a new time entry activity
     * @param parameters Additional enumeration parameters
     */
    void sendTimeEntryActivity( Enumeration item,
                                SuccessCb callback = nullptr,
                                int id = NULL_ID,
                                QString parameters = "" );

    /**
     * @brief Create or update tracker in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Tracker ID to update; if set to \c NULL_ID, create a new tracker
     * @param parameters Additional tracker parameters
     */
    void sendTracker( Tracker item,
                      SuccessCb callback = nullptr,
                      int id = NULL_ID,
                      QString parameters = "" );

    /**
     * @brief Create or update version in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param id Version ID to update; if set to \c NULL_ID, create a new version
     * @param parameters Additional version parameters
     */
    void sendTracker( Version item,
                      SuccessCb callback = nullptr,
                      int id = NULL_ID,
                      QString parameters = "" );

    /// @}

    /// @name Redmine data retrievers
    /// @{

    /**
     * @brief Retrieve custom fields from Redmine
     *
     * @param callback Callback function with a custom field vector
     * @param filter Additional custom field parameters
     */
    void retrieveCustomFields( CustomFieldsCb callback,
                               CustomFieldFilter filter );

    /**
     * @brief Retrieve an issue from Redmine
     *
     * @param callback Callback function with an issue object
     * @param issueId Issue ID
     * @param parameters Additional issue parameters
     */
    void retrieveIssue( IssueCb callback,
                        int issueId,
                        QString parameters = "" );

    /**
     * @brief Retrieve issues from Redmine
     *
     * @param callback Callback function with an issue vector
     * @param options Additional options
     */
    void retrieveIssues( IssuesCb callback, RedmineOptions options = RedmineOptions() );

    /**
     * @brief Retrieve issue categories for a project
     *
     * @param callback Callback function with a issue category vector
     * @param projectId Project ID
     * @param parameters Additional issue category parameters
     */
    void retrieveIssueCategories( IssueCategoriesCb callback,
                                  int projectId,
                                  QString parameters = "" );

    /**
     * @brief Retrieve issue priorities from Redmine
     *
     * @param callback Callback function with an enumeration vector
     * @param parameters Additional enumeration parameters
     */
    void retrieveIssuePriorities( EnumerationsCb callback,
                                  QString parameters = "" );

    /**
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function with a issue status vector
     * @param parameters Additional issue status parameters
     */
    void retrieveIssueStatuses( IssueStatusesCb callback,
                                QString parameters = "" );

    /**
     * @brief Retrieve an project from Redmine
     *
     * @param callback Callback function with an project object
     * @param projectId Project ID
     * @param parameters Additional project parameters
     */
    void retrieveProject( ProjectCb callback,
                          int projectId,
                          QString parameters = "" );

    /**
     * @brief Retrieve projects from Redmine
     *
     * @param callback Callback function with a project vector
     * @param parameters Additional project parameters
     */
    void retrieveProjects( ProjectsCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve time entries from Redmine
     *
     * @param callback Callback function with a time entries vector
     * @param parameters Additional time entry parameters
     */
    void retrieveTimeEntries( TimeEntriesCb callback,
                              QString parameters = "" );

    /**
     * @brief Retrieve time entry activities from Redmine
     *
     * @param callback Callback function with an enumeration vector
     * @param parameters Additional enumeration parameters
     */
    void retrieveTimeEntryActivities( EnumerationsCb callback,
                                      QString parameters = "" );

    /**
     * @brief Retrieve trackers from Redmine
     *
     * @param callback Callback function with a tracker vector
     * @param parameters Additional tracker parameters
     */
    void retrieveTrackers( TrackersCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve current user from Redmine
     *
     * @param callback Callback function with a user object
     */
    void retrieveCurrentUser( UserCb callback );

    /**
     * @brief Retrieve users from Redmine
     *
     * @param callback Callback function with a user vector
     * @param parameters Additional user parameters
     */
    void retrieveUsers( UsersCb callback,
                        QString parameters = "" );

    /**
     * @brief Retrieve versions from Redmine
     *
     * @param callback Callback function with a version vector
     * @param parameters Additional version parameters
     */
    void retrieveVersions( VersionsCb callback,
                           QString parameters = "" );

    /// @}

protected:
    // Inherited and overloaded methods
    using RedmineClient::sendEnumeration;
    using RedmineClient::sendIssuePriority;
    using RedmineClient::sendIssue;
    using RedmineClient::sendIssueStatus;
    using RedmineClient::sendProject;
    using RedmineClient::sendTimeEntry;
    using RedmineClient::sendTimeEntryActivity;
    using RedmineClient::sendTracker;
    using RedmineClient::sendVersion;

    using RedmineClient::retrieveCustomFields;
    using RedmineClient::retrieveEnumerations;
    using RedmineClient::retrieveIssue;
    using RedmineClient::retrieveIssues;
    using RedmineClient::retrieveIssueCategories;
    using RedmineClient::retrieveIssuePriorities;
    using RedmineClient::retrieveIssueStatuses;
    using RedmineClient::retrieveProject;
    using RedmineClient::retrieveProjects;
    using RedmineClient::retrieveTimeEntries;
    using RedmineClient::retrieveTimeEntryActivities;
    using RedmineClient::retrieveTrackers;
    using RedmineClient::retrieveCurrentUser;
    using RedmineClient::retrieveUsers;
    using RedmineClient::retrieveVersion;
    using RedmineClient::retrieveVersions;

    /**
     * @brief Retrieve enumerations from Redmine
     *
     * @param enumeration The enumeration to load
     * @param callback    Callback function with an Enumeration vector
     * @param parameters Additional enumeration parameters
     */
    void retrieveEnumerations( QString enumeration,
                               EnumerationsCb callback,
                               QString parameters = "" );

public slots:
    /**
     * @brief Check whether the connection currently works
     *
     * If the status has changed from \c Accessible to \c NotAccessible or vice versa, the
     * \c connectionChanged signal is emitted.
     *
     * @param accessible Specifies whether the network is accessible at all
     */
    void checkConnectionStatus( QNetworkAccessManager::NetworkAccessibility accessible =
                                QNetworkAccessManager::UnknownAccessibility );

signals:
    /**
     * @brief Singal that the connection to Redmine has changed
     *
     * @param connected true if connection is available, false otherwise
     */
    void connectionChanged( QNetworkAccessManager::NetworkAccessibility connected );
};

} // qtredmine

#endif // SIMPLEREDMINECLIENT_H
