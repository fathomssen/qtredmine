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
private:
    int limit_ = 100;

public:
    // Inherited constructors
    using RedmineClient::RedmineClient;

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

    /// @}

    /// @name Redmine data retrievers
    /// @{

    /**
     * @brief Retrieve issue priorities from Redmine
     *
     * @param callback Callback function with an Enumeration vector
     * @param parameters Additional enumeration parameters
     */
    void retrieveIssuePriorities( EnumerationsCb callback,
                                  QString parameters = "" );

    /**
     * @brief Retrieve an issue from Redmine
     *
     * @param callback Callback function with an Issue object
     * @param issueId Issue ID
     * @param parameters Additional issue parameters
     */
    void retrieveIssue( IssueCb callback, int issueId,
                        QString parameters = "" );

    /**
     * @brief Retrieve issues from Redmine
     *
     * @param callback Callback function with an Issue vector
     * @param options Additional options
     */
    void retrieveIssues( IssuesCb callback, RedmineOptions options = RedmineOptions() );

    /**
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function with a IssueStatus vector
     * @param parameters Additional issue status parameters
     */
    void retrieveIssueStatuses( IssueStatusesCb callback,
                                QString parameters = "" );

    /**
     * @brief Retrieve projects from Redmine
     *
     * @param callback Callback function with a Project vector
     * @param parameters Additional project parameters
     */
    void retrieveProjects( ProjectsCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve time entries from Redmine
     *
     * @param callback Callback function with a TimeEntries vector
     * @param parameters Additional time entry parameters
     */
    void retrieveTimeEntries( TimeEntriesCb callback,
                              QString parameters = "" );

    /**
     * @brief Retrieve time entry activities from Redmine
     *
     * @param callback Callback function with an Enumeration vector
     * @param parameters Additional enumeration parameters
     */
    void retrieveTimeEntryActivities( EnumerationsCb callback,
                                      QString parameters = "" );

    /**
     * @brief Retrieve trackers from Redmine
     *
     * @param callback Callback function with a Tracker vector
     * @param parameters Additional tracker parameters
     */
    void retrieveTrackers( TrackersCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve current user from Redmine
     *
     * @param callback Callback function with a User vector
     */
    void retrieveCurrentUser( UserCb callback );

    /**
     * @brief Retrieve users from Redmine
     *
     * @param callback Callback function with a User vector
     * @param parameters Additional user parameters
     */
    void retrieveUsers( UsersCb callback,
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

    using RedmineClient::retrieveEnumerations;
    using RedmineClient::retrieveIssuePriorities;
    using RedmineClient::retrieveIssue;
    using RedmineClient::retrieveIssues;
    using RedmineClient::retrieveIssueStatuses;
    using RedmineClient::retrieveProjects;
    using RedmineClient::retrieveTimeEntries;
    using RedmineClient::retrieveTimeEntryActivities;
    using RedmineClient::retrieveTrackers;
    using RedmineClient::retrieveCurrentUser;
    using RedmineClient::retrieveUsers;

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

};

} // qtredmine

#endif // SIMPLEREDMINECLIENT_H
