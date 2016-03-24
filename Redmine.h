#ifndef REDMINE_H
#define REDMINE_H

#include "qtredmine_global.h"

#include "Authenticator.h"

#include <QByteArray>
#include <QDate>
#include <QDateTime>
#include <QJsonDocument>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QTime>
#include <QUrl>
#include <QVector>

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
class QTREDMINESHARED_EXPORT Redmine : public QObject
{
    Q_OBJECT

public:
    /// Operation mode
    enum class Mode {
        GET,      ///< Get items from Redmine
        ADD,      ///< Add a new item to Redmine
        UPDATE,   ///< Update a Redmine item
        DEL       ///< Delete a Redmine item
    };
    Q_ENUM( Mode );

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

    //
    // Enumeration
    //

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

    //
    // Issue
    //

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

    //
    // Issue statuses
    //

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

    //
    // Project
    //

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

    //
    // Tracker
    //

    /// Structure representing a tracker
    struct Tracker {
        int     id;   ///< ID
        QString name; ///< Project name
    };

    /// Tracker vector
    using Trackers = QVector<Tracker>;

    /// Typedef for a Redmine Trackers callback function
    using TrackersCb = std::function<void(Trackers)>;

public:
    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param parent Parent QObject (default: nullptr)
     */
    Redmine( QObject* parent = nullptr );

    /**
     * @brief Constructor for an unconfigured Redmine connection
     *
     * @param url    Redmine base URL
     * @param parent Parent QObject (default: nullptr)
     */
    Redmine( QString url, QObject* parent = nullptr );

    /**
     * @brief Constructor for a Redmine connection using API key authentication
     *
     * @param url      Redmine base URL
     * @param apiKey   Redmine API key
     * @param checkSsl Check the SSL certificate (default: true)
     * @param parent   Parent QObject (default: nullptr)
     */
    Redmine( QString url, QString apiKey,
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
    Redmine( QString url, QString login, QString password,
             bool checkSsl   = true,
             QObject* parent = nullptr );

    /**
     * @brief Destructor
     */
    virtual ~Redmine();

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

    /**
     * @brief Retrieve enumerations from Redmine
     *
     * @param enumeration The enumeration to load
     * @param callback    Callback function with a QJsonDocument object
     * @param parameters     Additional enumeration parameters
     */
    void retrieveEnumerations( QString enumeration,
                               JsonCb  callback,
                               QString parameters = "" );

    /**
     * @brief Retrieve enumerations from Redmine
     *
     * @param enumeration The enumeration to load
     * @param callback    Callback function with an Enumeration vector
     * @param parameters     Additional enumeration parameters
     */
    void retrieveEnumerations( QString enumeration,
                               EnumerationsCb callback,
                               QString parameters = "" );


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

} // qtredmine

#endif // REDMINE_H
