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
public:
    // Inherited constructors
    using RedmineClient::RedmineClient;

    /// @name Redmine data creators
    /// @{

    /**
     * @brief Create issue priority in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param parameters Additional enumeration parameters
     */
    void createIssuePriority( Enumeration item,
                              SuccessCb callback = nullptr,
                              QString parameters = "" );

    /**
     * @brief Create issue in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param parameters Additional issue parameters
     */
    void createIssue( Issue item,
                      SuccessCb callback = nullptr,
                      QString parameters = "" );

    /**
     * @brief Create issue status in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param parameters Additional issue status parameters
     */
    void createIssueStatus( IssueStatus item,
                            SuccessCb callback = nullptr,
                            QString parameters = "" );

    /**
     * @brief Create project in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param parameters Additional project parameters
     */
    void createProject( Project item,
                        SuccessCb callback = nullptr,
                        QString parameters = "" );

    /**
     * @brief Create time entry in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param parameters Additional time entry parameters
     */
    void createTimeEntry( TimeEntry item,
                          SuccessCb callback = nullptr,
                          QString parameters = "" );

    /**
     * @brief Create time entry activity in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param parameters Additional enumeration parameters
     */
    void createTimeEntryActivity( Enumeration item,
                                  SuccessCb callback = nullptr,
                                  QString parameters = "" );

    /**
     * @brief Create tracker in Redmine
     *
     * @param item Data to be stored in Redmine
     * @param callback Success callback function
     * @param parameters Additional tracker parameters
     */
    void createTracker( Tracker item,
                        SuccessCb callback = nullptr,
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
     * @param parameters Additional issue parameters
     */
    void retrieveIssues( IssuesCb callback,
                         QString parameters = "" );

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

    /// @}

protected:
    // Inherited and overloaded methods
    using RedmineClient::createEnumeration;
    using RedmineClient::createIssuePriority;
    using RedmineClient::createIssue;
    using RedmineClient::createIssueStatus;
    using RedmineClient::createProject;
    using RedmineClient::createTimeEntry;
    using RedmineClient::createTimeEntryActivity;
    using RedmineClient::createTracker;

    using RedmineClient::retrieveEnumerations;
    using RedmineClient::retrieveIssuePriorities;
    using RedmineClient::retrieveIssue;
    using RedmineClient::retrieveIssues;
    using RedmineClient::retrieveIssueStatuses;
    using RedmineClient::retrieveProjects;
    using RedmineClient::retrieveTimeEntries;
    using RedmineClient::retrieveTimeEntryActivities;
    using RedmineClient::retrieveTrackers;

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
