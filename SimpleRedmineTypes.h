#ifndef SIMPLEREDMINETYPES_H
#define SIMPLEREDMINETYPES_H

#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <QTime>
#include <QVector>
#include <QtGlobal>

#include <functional>

namespace qtredmine {

#define DEFAULT_FIELDS \
    QDateTime createdOn;     /**< @brief Created on */   \
    QDateTime updatedOn;     /**< @brief Updated on */   \
    Item      user;          /**< @brief Redmine user */ \
    bool      valid = false; /**< @brief Valid record */

/// @name Redmine data structures
/// @{

/// Structure representing a Redmine item
struct Item {
    int     id;   ///< ID
    QString name; ///< Name
};

/// Structure representing a Redmine custom field
struct CustomField {
    int                  id;    ///< ID
    QString              name;  ///< Name
    std::vector<QString> value; ///< Value
};

/// @}

/// @name Redmine data containers
/// @{

/// Item vector
using Items = QVector<Item>;

/// Custom field vector
using CustomFields = QVector<CustomField>;

/// Structure representing an enumeration
struct Enumeration {
    int     id;        ///< ID
    QString name;      ///< Project name
    bool    isDefault; ///< Default entry

    DEFAULT_FIELDS
};

/// @}

/// @name Redmine data structures
/// @{

/// Structure representing an issue
struct Issue {
    int          id;             ///< ID

    QString      description;    ///< Description
    double       doneRatio;      ///< Done ratio
    QString      subject;        ///< Subject

    Item         author;         ///< Author
    Item         category;       ///< Category
    Item         priority;       ///< Priority
    Item         project;        ///< Project
    Item         status;         ///< Status
    Item         tracker;        ///< Tracker

    QDate        dueDate;        ///< Due date
    QTime        estimatedHours; ///< Estimated hours
    QDate        startDate;      ///< Start date

    CustomFields customFields;   ///< Custom fields vector

    DEFAULT_FIELDS
};

/// Structure representing an issue status
struct IssueStatus {
    int     id;        ///< ID
    QString name;      ///< Project name
    bool    isClosed;  ///< Closed status
    bool    isDefault; ///< Default entry

    DEFAULT_FIELDS
};

/// Structure representing a project
struct Project {
    int       id;          ///< ID

    QString   description; ///< Description
    QString   identifier;  ///< Internal identifier
    bool      isPublic;    ///< Public project
    QString   name;        ///< Project name

    DEFAULT_FIELDS
};

/// Structure representing a time entry
struct TimeEntry {
    Item    activity; ///< Activity
    QString comment;  ///< Additional comment
    double  hours;    ///< Hours spent
    Item    issue;    ///< Issue (required if no project was specified)
    Item    project;  ///< Project (required if no issue was specified)
    QDate   spentOn;  ///< Date of the time spent

    DEFAULT_FIELDS
};

/// Structure representing a tracker
struct Tracker {
    int     id;   ///< ID
    QString name; ///< Project name

    DEFAULT_FIELDS
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

/// @}

/// @name Callbacks
/// @{

/// Success callback which passes a boolean value
using SuccessCb = std::function<void(bool)>;

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

Q_DECLARE_METATYPE( qtredmine::Item )
Q_DECLARE_METATYPE( qtredmine::CustomField )
Q_DECLARE_METATYPE( qtredmine::Enumeration )

Q_DECLARE_METATYPE( qtredmine::Issue )
Q_DECLARE_METATYPE( qtredmine::IssueStatus )
Q_DECLARE_METATYPE( qtredmine::Project )
Q_DECLARE_METATYPE( qtredmine::TimeEntry )
Q_DECLARE_METATYPE( qtredmine::Tracker )

#endif // SIMPLEREDMINETYPES_H
