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

    /**
     * @brief Retrieve issue priorities from Redmine
     *
     * @param callback Callback function with an Enumeration vector
     * @param parameters  Additional enumeration parameters
     */
    void retrieveIssuePriorities( EnumerationsCb callback,
                                  QString parameters = "" );

    /**
     * @brief Retrieve issues from Redmine
     *
     * @param callback Callback function with an Issue vector
     * @param parameters  Additional issue parameters
     */
    void retrieveIssues( IssuesCb callback,
                         QString parameters = "" );

    /**
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function with a IssueStatus vector
     * @param parameters  Additional issue status parameters
     */
    void retrieveIssueStatuses( IssueStatusesCb callback,
                                QString parameters = "" );

    /**
     * @brief Retrieve projects from Redmine
     *
     * @param callback Callback function with a Project vector
     * @param parameters  Additional project parameters
     */
    void retrieveProjects( ProjectsCb callback,
                           QString parameters = "" );

    /**
     * @brief Retrieve time entry activities from Redmine
     *
     * @param callback Callback function with an Enumeration vector
     * @param parameters  Additional enumeration parameters
     */
    void retrieveTimeEntryActivities( EnumerationsCb callback,
                                      QString parameters = "" );

    /**
     * @brief Retrieve trackers from Redmine
     *
     * @param callback Callback function with a Tracker vector
     * @param parameters  Additional tracker parameters
     */
    void retrieveTrackers( TrackersCb callback,
                           QString parameters = "" );

protected:
    // Inherited and overloaded methods
    using RedmineClient::retrieveEnumerations;
    using RedmineClient::retrieveIssuePriorities;
    using RedmineClient::retrieveIssues;
    using RedmineClient::retrieveIssueStatuses;
    using RedmineClient::retrieveProjects;
    using RedmineClient::retrieveTimeEntryActivities;
    using RedmineClient::retrieveTrackers;

    /**
     * @brief Retrieve enumerations from Redmine
     *
     * @param enumeration The enumeration to load
     * @param callback    Callback function with an Enumeration vector
     * @param parameters  Additional enumeration parameters
     */
    void retrieveEnumerations( QString enumeration,
                               EnumerationsCb callback,
                               QString parameters = "" );

};

} // qtredmine

#endif // SIMPLEREDMINECLIENT_H
