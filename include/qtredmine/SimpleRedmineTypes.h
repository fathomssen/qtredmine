#ifndef SIMPLEREDMINETYPES_H
#define SIMPLEREDMINETYPES_H

#include "Logging.h"
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

/// Custom field filter
struct CustomFieldFilter
{
    int projectId = NULL_ID; ///< Project
    int trackerId = NULL_ID; ///< Tracker
    QString type;   ///< Customised type
    QString format; ///< Format
};

/// @name Redmine data structures
/// @{

/// Structure representing a Redmine item
struct Item {
    int     id = NULL_ID; ///< ID
    QString name;         ///< Name
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

/// Structure representing an enumeration
struct Enumeration : RedmineResource
{
    int     id = NULL_ID;   ///< ID
    QString name;           ///< Project name
    bool    isDefault;      ///< Default entry
};

/// @}

/// @name Redmine data structures
/// @{

/// Structure representing a custom field
struct CustomField // no RedmineResource
{
    int                  id = NULL_ID; ///< ID
    QString              name;         ///< Name

    QVector<QString>     values;         ///< Value(s)
    QVector<QString>     possibleValues; ///< Possible
    QString              defaultValue;   ///< Default value

    QString type;   ///< Customised type
    QString format; ///< Field format
    QString regex;  ///< Regular expression
    int minLength;  ///< Minimum length
    int maxLength;  ///< Maximum length

    bool allProjects; ///< Custom field may be used by all projects
    bool isRequired;  ///< Custom field is required
    bool isFilter;    ///< Custom field may be used as filter
    bool searchable;  ///< Custom field is searchable
    bool multiple;    ///< Custom field may contain multiple values
    bool visible;     ///< Custom field is visible

    Items projects; ///< Custom field is allowed in these projects
    Items trackers; ///< Custom field is allowed in these trackers
};

/// @}

/// @name Redmine data containers
/// @{

/// Custom field vector
using CustomFields = QVector<CustomField>;

/// @}

/// @name Redmine data structures
/// @{

/// Structure representing a group
struct Group : RedmineResource
{
    int     id = NULL_ID; ///< ID
    QString name;         ///< Group name
    Items   members;      ///< Group members
};

/// Structure representing an issue
struct Issue : RedmineResource
{
    int          id = NULL_ID;        ///< ID
    int          parentId = NULL_ID;  ///< Parent issue ID

    QString      description;    ///< Description
    double       doneRatio = 0;  ///< Done ratio
    QString      subject;        ///< Subject

    Item         assignedTo;     ///< Assigned to user
    Item         author;         ///< Author
    Item         category;       ///< Category
    Item         priority;       ///< Priority
    Item         project;        ///< Project
    Item         status;         ///< Status
    Item         tracker;        ///< Tracker
    Item         version;        ///< Version

    QDate        dueDate;        ///< Due date
    double       estimatedHours = 0; ///< Estimated hours
    QDate        startDate;      ///< Start date

    CustomFields customFields;   ///< Custom fields vector
};

/// Structure representing an issue category
struct IssueCategory // no RedmineResource
{
    int     id = NULL_ID;   ///< ID
    QString name;           ///< Issue category name
    Item    project;        ///< Associated project
    Item    assignedTo;     ///< Assigned to user
};

/// Structure representing an issue status
struct IssueStatus : RedmineResource
{
    int     id = NULL_ID;   ///< ID
    QString name;           ///< Issue status name
    bool    isClosed;       ///< Closed status
    bool    isDefault;      ///< Default entry
};

/// Structure representing a membership
struct Membership : RedmineResource
{
    int     id = NULL_ID;   ///< ID
    Item    project;        ///< Project this membership is related to
    Item    user;           ///< User within the project (exclusive with group)
    Item    group;          ///< Group within the project (exclusive with user)
    Items   roles;          ///< Roles of the user or group
};

/// Structure representing a project
struct Project : RedmineResource
{
    int       id = NULL_ID; ///< ID

    QString   description; ///< Description
    QString   identifier;  ///< Internal identifier
    bool      isPublic;    ///< Public project
    QString   name;        ///< Project name
    Item      parent;      ///< Parent project

    Items trackers;   ///< Trackers
    Items categories; ///< Issue categories
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

    CustomFields customFields;   ///< Custom fields vector
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

/// Structure representing a version
struct Version : RedmineResource
{
    int     id = NULL_ID;   ///< ID
    QString name;           ///< Version name
    ///< Status (missing)
    ///< Sharing (missing)
    QDate   dueDate;        ///< Due date
    QString description;    ///< Description
};

/// @}

/// @name Redmine data containers
/// @{

/// Custom fields vector
using CustomFields = QVector<CustomField>;

/// Enumeration vector
using Enumerations = QVector<Enumeration>;

/// Group vector
using Groups = QVector<Group>;

/// Issue vector
using Issues = QVector<Issue>;

/// Issue category vector
using IssueCategories = QVector<IssueCategory>;

/// Issue statuses vector
using IssueStatuses = QVector<IssueStatus>;

/// Membership vector
using Memberships = QVector<Membership>;

/// Project vector
using Projects = QVector<Project>;

/// TimeEntry vector
using TimeEntries = QVector<TimeEntry>;

/// Tracker vector
using Trackers = QVector<Tracker>;

/// User vector
using Users = QVector<User>;

/// Versions vector
using Versions = QVector<Version>;

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

/**
 * Typedef for an custom fields callback function
 *
 * @param CustomFields Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using CustomFieldsCb = std::function<void(CustomFields, RedmineError, QStringList)>;

/**
 * Typedef for an enumerations callback function
 *
 * @param Enumerations Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using EnumerationsCb = std::function<void(Enumerations, RedmineError, QStringList)>;

/**
 * Typedef for an group callback function
 *
 * @param Group Redmine resource
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using GroupCb = std::function<void(Group, RedmineError, QStringList)>;

/**
 * Typedef for an issue callback function
 *
 * @param Issue Redmine resource
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using IssueCb = std::function<void(Issue, RedmineError, QStringList)>;

/**
 * Typedef for an issues callback function
 *
 * @param Issues Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using IssuesCb = std::function<void(Issues, RedmineError, QStringList)>;

/**
 * Typedef for an issue categories callback function
 *
 * @param IssueCategories Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using IssueCategoriesCb = std::function<void(IssueCategories, RedmineError, QStringList)>;

/**
 * Typedef for an issue statuses callback function
 *
 * @param IssueStatuses Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using IssueStatusesCb = std::function<void(IssueStatuses, RedmineError, QStringList)>;

/**
 * Typedef for an membership callback function
 *
 * @param Memberships Vector of Redmine resource
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using MembershipsCb = std::function<void(Memberships, RedmineError, QStringList)>;

/**
 * Typedef for an project callback function
 *
 * @param Project Redmine resource
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using ProjectCb = std::function<void(Project, RedmineError, QStringList)>;

/**
 * Typedef for a projects callback function
 *
 * @param Projects Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using ProjectsCb = std::function<void(Projects, RedmineError, QStringList)>;

/**
 * Typedef for a time entry callback function
 *
 * @param TimeEntry Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using TimeEntriesCb = std::function<void(TimeEntries, RedmineError, QStringList)>;

/**
 * Typedef for a trackers callback function
 *
 * @param Trackers Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using TrackersCb = std::function<void(Trackers, RedmineError, QStringList)>;

/**
 * Typedef for a user callback function
 *
 * @param User Redmine resource
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using UserCb = std::function<void(User, RedmineError, QStringList)>;

/**
 * Typedef for a users callback function
 *
 * @param Users Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using UsersCb = std::function<void(Users, RedmineError, QStringList)>;

/**
 * Typedef for a version callback function
 *
 * @param Version Redmine resource
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using VersionCb = std::function<void(Version, RedmineError, QStringList)>;

/**
 * Typedef for a versions callback function
 *
 * @param Versions Vector of Redmine resources
 * @param RedmineError Redmine error code
 * @param QStringList Errors that Redmine returned
 */
using VersionsCb = std::function<void(Versions, RedmineError, QStringList)>;

/// @}

} // qtredmine

/**
 * @brief QDebug stream operator for items
 * @return QDebug object
 */
inline QDebug
operator<<( QDebug debug, const qtredmine::Item& data )
{
    QDebugStateSaver saver( debug );
    DEBUGFIELDS(id)(name);
    return debug;
}

/**
 * @brief QDebug stream operator for custom fields
 * @return QDebug object
 */
inline QDebug
operator<<( QDebug debug, const qtredmine::CustomField& data )
{
    QDebugStateSaver saver( debug );
    DEBUGFIELDS(id)(name)(values)(possibleValues)(defaultValue)(type)(format)(regex)(minLength)(maxLength)
            (allProjects)(isRequired)(isFilter)(searchable)(multiple)(visible)(projects)(trackers);
    return debug;
}

/**
 * @brief QDebug stream operator for issues
 * @return QDebug object
 */
inline QDebug
operator<<( QDebug debug, const qtredmine::Issue& data )
{
    QDebugStateSaver saver( debug );
    DEBUGFIELDS(id)(parentId)(description)(doneRatio)(subject)(assignedTo)(author)(category)(priority)
            (project)(status)(tracker)(version)(dueDate)(estimatedHours)(startDate)(customFields);
    return debug;
}

/**
 * @brief QDebug stream operator for memberships
 * @return QDebug object
 */
inline QDebug
operator<<( QDebug debug, const qtredmine::Membership& data )
{
    QDebugStateSaver saver( debug );
    DEBUGFIELDS(id)(project)(user)(group)(roles);
    return debug;
}

/**
 * @brief QDebug stream operator for RedmineOptions
 * @return QDebug object
 */
inline QDebug
operator<<( QDebug debug, const qtredmine::RedmineOptions& options )
{
    QDebugStateSaver saver( debug );
    debug.nospace() << "[" << options.parameters << ", " << options.getAllItems << "]";

    return debug;
}

Q_DECLARE_METATYPE( qtredmine::RedmineResource )

Q_DECLARE_METATYPE( qtredmine::Item )
Q_DECLARE_METATYPE( qtredmine::Enumeration )

Q_DECLARE_METATYPE( qtredmine::CustomField )
Q_DECLARE_METATYPE( qtredmine::Group )
Q_DECLARE_METATYPE( qtredmine::Issue )
Q_DECLARE_METATYPE( qtredmine::IssueCategory )
Q_DECLARE_METATYPE( qtredmine::IssueStatus )
Q_DECLARE_METATYPE( qtredmine::Membership )
Q_DECLARE_METATYPE( qtredmine::Project )
Q_DECLARE_METATYPE( qtredmine::TimeEntry )
Q_DECLARE_METATYPE( qtredmine::Tracker )
Q_DECLARE_METATYPE( qtredmine::User )
Q_DECLARE_METATYPE( qtredmine::Version )

#endif // SIMPLEREDMINETYPES_H
