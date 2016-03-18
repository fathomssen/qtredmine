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

namespace qtredmine {

/**
 * @brief Redmine connection class
 *
 * This class handles the connections to a Redmine instance and provides access to
 * the Redmine REST API (@sa http://www.redmine.org/projects/redmine/wiki/Rest_api).
 *
 * @example Example.h
 * @example Example.cpp
 */
class QTREDMINESHARED_EXPORT Redmine : public QObject
{
    Q_OBJECT

public:
    /// Typedef for a JSON callback function
    using cbJson = std::function<void(QNetworkReply*, QJsonDocument*)>;

    /// HTTP operation mode
    enum class Mode {
        GET,  ///< HTTP GET operation
        POST, ///< HTTP POST operation
        PUT,  ///< HTTP PUT operation
        DEL   ///< HTTP DELETE operation
    };

    //
    // Internal representations
    //

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
    using cbEnumerations = std::function<void(Enumerations)>;

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
    using cbIssues = std::function<void(Issues)>;

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
    using cbIssueStatuses = std::function<void(IssueStatuses)>;

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
    using cbProjects = std::function<void(Projects)>;

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
    using cbTrackers = std::function<void(Trackers)>;

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
    QMap<QNetworkReply*, cbJson> callbacks_;

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
     * @param callback    Callback function with a QJsonDocument object
     * @param enumeration The enumeration to load
     * @param filters     Additional enumeration filters
     */
    void retrieveEnumerations( cbJson  callback,
                               QString enumeration,
                               QString filters = "" );

    /**
     * @brief Retrieve enumerations from Redmine
     *
     * @param callback    Callback function with an Enumeration vector
     * @param enumeration The enumeration to load
     * @param filters     Additional enumeration filters
     */
    void retrieveEnumerations( cbEnumerations callback,
                               QString enumeration,
                               QString filters = "" );

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
     * @brief Retrieve custom fields from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional custom field filters
     */
    void retrieveCustomFields( cbJson callback,
                               QString filters = "" );

    /**
     * @brief Retrieve issue categories from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional issue category filters
     */
    void retrieveIssueCategories( cbJson callback,
                                  QString filters = "" );

    /**
     * @brief Retrieve issue priorities from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional enumeration filters
     */
    void retrieveIssuePriorities( cbJson callback,
                                  QString filters = "" );

    /**
     * @brief Retrieve issue priorities from Redmine
     *
     * @param callback Callback function with an Enumeration vector
     * @param filters  Additional enumeration filters
     */
    void retrieveIssuePriorities( cbEnumerations callback,
                                  QString filters = "" );

    /**
     * @brief Retrieve issues from Redmine
     *
     * @param callback Callback function with an Issue vector
     * @param filters  Additional issue filters
     */
    void retrieveIssues( cbIssues callback,
                         QString filters = "" );

    /**
     * @brief Retrieve issues from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional issue filters
     */
    void retrieveIssues( cbJson callback,
                         QString filters = "" );

    /**
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional issue status filters
     */
    void retrieveIssueStatuses( cbJson callback,
                                QString filters = "" );

    /**
     * @brief Retrieve issue statuses from Redmine
     *
     * @param callback Callback function with a IssueStatus vector
     * @param filters  Additional issue status filters
     */
    void retrieveIssueStatuses( cbIssueStatuses callback,
                                QString filters = "" );

    /**
     * @brief Retrieve projects from Redmine
     *
     * @param callback Callback function with a Project vector
     * @param filters  Additional project filters
     */
    void retrieveProjects( cbProjects callback,
                           QString filters = "" );

    /**
     * @brief Retrieve projects from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional project filters
     */
    void retrieveProjects( cbJson callback,
                           QString filters = "" );

    /**
     * @brief Retrieve time entries from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional time entry filters
     */
    void retrieveTimeEntries( cbJson callback,
                              QString filters = "" );

    /**
     * @brief Retrieve time entry activities from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional enumeration filters
     */
    void retrieveTimeEntryActivities( cbJson callback,
                                      QString filters = "" );

    /**
     * @brief Retrieve time entry activities from Redmine
     *
     * @param callback Callback function with an Enumeration vector
     * @param filters  Additional enumeration filters
     */
    void retrieveTimeEntryActivities( cbEnumerations callback,
                                      QString filters = "" );

    /**
     * @brief Retrieve trackers from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional tracker filters
     */
    void retrieveTrackers( cbJson callback,
                           QString filters = "" );

    /**
     * @brief Retrieve trackers from Redmine
     *
     * @param callback Callback function with a Tracker vector
     * @param filters  Additional tracker filters
     */
    void retrieveTrackers( cbTrackers callback,
                           QString filters = "" );

    /**
     * @brief Retrieve versions from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional version filters
     */
    void retrieveVersions( cbJson callback,
                           QString filters = "" );

    /**
     * @brief Retrieve users from Redmine
     *
     * @param callback Callback function with a QJsonDocument object
     * @param filters  Additional user filters
     */
    void retrieveUsers( cbJson callback,
                        QString filters = "" );

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
                                cbJson callback             = nullptr,
                                Mode mode                  = Mode::GET,
                                const QString& queryParams = "",
                                const QByteArray& postData = "" );

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
    void requestFinished( cbJson callback, QNetworkReply* reply, QJsonDocument* data );
};

} // qtredmine

#endif // REDMINE_H
