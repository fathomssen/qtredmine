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

/// Structure representing a Redmine item
struct Item {
  int     id;   ///< ID
  QString name; ///< Name
};

/// Item vector
using Items = QVector<Item>;

/// Structure representing a Redmine custom field
struct CustomField {
  int                  id;    ///< ID
  QString              name;  ///< Name
  std::vector<QString> value; ///< Value
};

/// Custom field vector
using CustomFields = QVector<CustomField>;

/// Structure representing an enumeration
struct Enumeration {
    int     id;        ///< ID
    QString name;      ///< Project name
    bool    isDefault; ///< Default entry
};

/// Enumeration vector
using Enumerations = QVector<Enumeration>;

/// Typedef for a Redmine enumerations callback function
using EnumerationsCb = std::function<void(Enumerations)>;

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

    QDateTime    createdOn;      ///< Created on
    QDate        dueDate;        ///< Due date
    QTime        estimatedHours; ///< Estimated hours
    QDate        startDate;      ///< Start date
    QDateTime    updatedOn;      ///< Updated on

    CustomFields customFields;   ///< Custom fields vector
};

/// Issue vector
using Issues = QVector<Issue>;

/// Typedef for a Redmine issues callback function
using IssuesCb = std::function<void(Issues)>;

/// Structure representing an issue status
struct IssueStatus {
    int     id;        ///< ID
    QString name;      ///< Project name
    bool    isClosed;  ///< Closed status
    bool    isDefault; ///< Default entry
};

/// Issue statuses vector
using IssueStatuses = QVector<IssueStatus>;

/// Typedef for a Redmine issue statuses callback function
using IssueStatusesCb = std::function<void(IssueStatuses)>;

/// Structure representing a project
struct Project {
    int       id;          ///< ID

    QString   description; ///< Description
    QString   identifier;  ///< Internal identifier
    bool      isPublic;    ///< Public project
    QString   name;        ///< Project name

    QDateTime createdOn;   ///< Created on
    QDateTime updatedOn;   ///< Updated on
};

/// Project vector
using Projects = QVector<Project>;

/// Typedef for a Redmine projects callback function
using ProjectsCb = std::function<void(Projects)>;

/// Structure representing a tracker
struct Tracker {
    int     id;   ///< ID
    QString name; ///< Project name
};

/// Tracker vector
using Trackers = QVector<Tracker>;

/// Typedef for a Redmine Trackers callback function
using TrackersCb = std::function<void(Trackers)>;

/**
 * @brief QDebug stream for issues
 *
 * @param debug QDebug object
 * @Param item Issue item
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
Q_DECLARE_METATYPE( qtredmine::Tracker )

#endif // SIMPLEREDMINETYPES_H
