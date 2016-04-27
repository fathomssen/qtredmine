#ifndef SIMPLEREDMINETYPES_H
#define SIMPLEREDMINETYPES_H

#include "RedmineClient.h"

#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QMetaType>
#include <QNetworkAccessManager>
#include <QString>
#include <QTime>
#include <QVector>
#include <QtGlobal>

#include <functional>

namespace qtredmine {

/// Redmine error codes
enum RedmineError {
    NO_ERROR,
    ERR_INCOMPLETE_DATA,
    ERR_NETWORK,
    ERR_NOT_SAVED,
    ERR_TIME_ENTRY_TOO_SHORT,
    ERR_TIMEOUT,
};

/// Redmine options
struct RedmineOptions
{
    QString parameters;
    bool getAllItems = false;

    RedmineOptions( QString parameters = "", bool getAllItems = false )
        : parameters( parameters ),
          getAllItems( getAllItems )
    {}
};

/// @name Redmine data structures
/// @{

/// Structure representing a Redmine item
struct Item {
    int     id = NULL_ID; ///< ID
    QString name;    ///< Name
};

/// Structure representing a Redmine custom field
struct CustomField {
    int                  id = NULL_ID; ///< ID
    QString              name;    ///< Name
    std::vector<QString> value;   ///< Value
};

/// @}

/// @name Redmine data containers
/// @{

/// Redmine resource
struct RedmineResource
{
    QDateTime createdOn; ///< Created on
    QDateTime updatedOn; ///< Updated on
    Item      user;      ///< Redmine user
};

/// Item vector
using Items = QVector<Item>;

/// Custom field vector
using CustomFields = QVector<CustomField>;

/// Structure representing an enumeration
struct Enumeration : RedmineResource
{
    int     id = NULL_ID;   ///< ID
    QString name;      ///< Project name
    bool    isDefault; ///< Default entry
};

/// @}

/// @name Redmine data structures
/// @{

/// Structure representing an issue
struct Issue : RedmineResource
{
    int          id = NULL_ID;        ///< ID
    int          parentId = NULL_ID;  ///< Parent issue ID

    QString      description;    ///< Description
    double       doneRatio = 0;  ///< Done ratio
    QString      subject;        ///< Subject

    Item         assignedTo;     ///< Assigned to
    Item         author;         ///< Author
    Item         category;       ///< Category
    Item         priority;       ///< Priority
    Item         project;        ///< Project
    Item         status;         ///< Status
    Item         tracker;        ///< Tracker

    QDate        dueDate;        ///< Due date
    double       estimatedHours = 0; ///< Estimated hours
    QDate        startDate;      ///< Start date

    CustomFields customFields;   ///< Custom fields vector
};

/// Structure representing an issue status
struct IssueStatus : RedmineResource
{
    int     id = NULL_ID;   ///< ID
    QString name;      ///< Issue status name
    bool    isClosed;  ///< Closed status
    bool    isDefault; ///< Default entry
};

/// Structure representing a project
struct Project : RedmineResource
{
    int       id = NULL_ID;     ///< ID

    QString   description; ///< Description
    QString   identifier;  ///< Internal identifier
    bool      isPublic;    ///< Public project
    QString   name;        ///< Project name
};

/// Structure representing a time entry
struct TimeEntry : RedmineResource
{
    Item    activity; ///< Activity
    QString comment;  ///< Additional comment
    double  hours = 0;///< Hours spent
    Item    issue;    ///< Issue (required if no project was specified)
    Item    project;  ///< Project (required if no issue was specified)
    QDate   spentOn;  ///< Date of the time spent
};

/// Structure representing a tracker
struct Tracker : RedmineResource
{
    int     id = NULL_ID; ///< ID
    QString name;         ///< Tracker name

};

/// Structure representing a user
struct User : RedmineResource
{
    int     id = NULL_ID; ///< ID

    QString login;        ///< Login name
    QString firstname;    ///< First name
    QString lastname;     ///< Last name

    QString mail;         ///< E-mail address
    QDateTime lastLoginOn;///< Last login time and date
};

/// @}

/// @name Redmine data containers
/// @{

/// Enumeration vector
using Enumerations = QVector<Enumeration>;

/// Issue vector
using Issues = QVector<Issue>;

/// Issue statuses vector
using IssueStatuses = QVector<IssueStatus>;

/// Project vector
using Projects = QVector<Project>;

/// TimeEntry vector
using TimeEntries = QVector<TimeEntry>;

/// Tracker vector
using Trackers = QVector<Tracker>;

/// User vector
using Users = QVector<User>;

/// @}

/// @name Callbacks
/// @{

/**
 * Success callback
 *
 * @param bool True if successful, false otherwise
 * @param int ID of newly created Redmine item
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using SuccessCb = std::function<void(bool, int, RedmineError, QStringList)>;

/// Typedef for a enumerations callback function
using EnumerationsCb = std::function<void(Enumerations)>;

/// Typedef for a issue callback function
using IssueCb = std::function<void(Issue)>;

/// Typedef for a issues callback function
using IssuesCb = std::function<void(Issues)>;

/// Typedef for a issue statuses callback function
using IssueStatusesCb = std::function<void(IssueStatuses)>;

/// Typedef for a projects callback function
using ProjectsCb = std::function<void(Projects)>;

/// Typedef for a TimeEntry callback function
using TimeEntriesCb = std::function<void(TimeEntries)>;

/// Typedef for a Trackers callback function
using TrackersCb = std::function<void(Trackers)>;

/// Typedef for a User callback function
using UserCb = std::function<void(User)>;

/// Typedef for a Users callback function
using UsersCb = std::function<void(Users)>;

/// @}

/**
 * @brief QDebug stream operator for issues
 *
 * @param debug QDebug object
 * @param item Issue item
 *
 * @return QDebug object
 */
inline QDebug
operator<<( QDebug debug, const Issue& item )
{
    QDebugStateSaver saver( debug );
    debug.nospace() << "[" << item.id << ", " << item.subject << "]";

    return debug;
}

} // qtredmine

Q_DECLARE_METATYPE( qtredmine::RedmineResource )

Q_DECLARE_METATYPE( qtredmine::Item )
Q_DECLARE_METATYPE( qtredmine::CustomField )
Q_DECLARE_METATYPE( qtredmine::Enumeration )

Q_DECLARE_METATYPE( qtredmine::Issue )
Q_DECLARE_METATYPE( qtredmine::IssueStatus )
Q_DECLARE_METATYPE( qtredmine::Project )
Q_DECLARE_METATYPE( qtredmine::TimeEntry )
Q_DECLARE_METATYPE( qtredmine::Tracker )
Q_DECLARE_METATYPE( qtredmine::User )

#endif // SIMPLEREDMINETYPES_H
