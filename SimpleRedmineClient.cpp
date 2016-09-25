#include "SimpleRedmineClient.h"
#include "logging.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace qtredmine;

// Fill items
void
fillItem( Item& item, QJsonObject* obj, QString value)
{
    QJsonObject itemObj = obj->value(value).toObject();

    if( !itemObj.isEmpty() )
    {
        item.id   = itemObj.value("id").toInt();
        item.name = itemObj.value("name").toString();
    }
}

// Fill default fields
template<typename T>
void
fillDefaultFields( T& item, QJsonObject* obj)
{
    item.createdOn = obj->value("created_on").toVariant().toDateTime();
    item.updatedOn = obj->value("updated_on").toVariant().toDateTime();

    fillItem( item.user, obj, "user" );
}

QStringList
getErrorList( QNetworkReply* reply, QJsonDocument* json )
{
    ENTER()(reply->error())(json->toJson());

    QJsonArray jsonErrors = json->object().find("errors").value().toArray();
    QStringList errors;
    errors.push_back( reply->errorString() );

    for( const auto& error : jsonErrors )
        errors.push_back( error.toString() );

    RETURN( errors );
}

SimpleRedmineClient::SimpleRedmineClient( QObject* parent )
    : RedmineClient( parent )
{
    ENTER();
    init();
    RETURN();
}

SimpleRedmineClient::SimpleRedmineClient( QString url, QObject* parent )
    : RedmineClient( url, parent )
{
    ENTER()(url);
    init();
    RETURN();
}

SimpleRedmineClient::SimpleRedmineClient( QString url, QString apiKey, bool checkSsl, QObject* parent )
    : RedmineClient( url, apiKey, checkSsl, parent )
{
    ENTER()(url)(apiKey)(checkSsl);
    init();
    RETURN();
}

SimpleRedmineClient::SimpleRedmineClient( QString url, QString login, QString password, bool checkSsl,
                                          QObject* parent )
    : RedmineClient( url, login, password, parent )
{
    ENTER()(url)(login)(password)(checkSsl);
    init();
    RETURN();
}

void
SimpleRedmineClient::init()
{
    ENTER();

    // Connect the network accessible signal to the isConnected slot
    connect( this, &RedmineClient::networkAccessibleChanged,
             this, &SimpleRedmineClient::checkConnectionStatus );

    // Connect the initialised signal to the isConnected slot
    connect( this, &SimpleRedmineClient::initialised, [&](){ checkConnectionStatus(); } );

    RETURN();
}

void
SimpleRedmineClient::checkConnectionStatus( QNetworkAccessManager::NetworkAccessibility accessible )
{
    ENTER()(accessible);

    auto setConnectionState = [&]( QNetworkAccessManager::NetworkAccessibility connected )
    {
        ENTER()(connected);

        if( connected != connected_ )
        {
            DEBUG( "Emitting signal connectionChanged()" )(connected);
            connectionChanged( connected );
        }

        connected_ = connected;

        RETURN();
    };

    // If there is no network connection available, there will be no Redmine connection as well
    if( accessible == QNetworkAccessManager::NotAccessible )
    {
        setConnectionState( QNetworkAccessManager::NotAccessible );
        RETURN();
    }

    // Otherwise, check the Redmine connection
    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER()(reply)(json);

        if( reply->error() == QNetworkReply::NoError )
            setConnectionState( QNetworkAccessManager::Accessible );
        else
        {
            DEBUG()(reply->errorString());
            setConnectionState( QNetworkAccessManager::NotAccessible );
        }

        RETURN();
    };

    // Try to fetch one issue
    sendRequest( "issues", cb, QNetworkAccessManager::GetOperation, "limit=1" );

    RETURN();
  }

void
SimpleRedmineClient::sendIssue( Issue item, SuccessCb callback, int id, QString parameters )
{
    ENTER()(id)(parameters);

    QJsonObject attr;

    if( item.project.id != NULL_ID )
        attr["project_id"] = item.project.id;

    if( item.tracker.id != NULL_ID )
        attr["tracker_id"] = item.tracker.id;

    if( item.status.id != NULL_ID )
        attr["status_id"] = item.status.id;

    if( item.priority.id != NULL_ID )
        attr["priority_id"] = item.priority.id;

    if( !item.subject.isEmpty() )
        attr["subject"] = item.subject;

    if( !item.description.isEmpty() )
        attr["description"] = item.description;

    if( item.category.id != NULL_ID )
        attr["category_id"] = item.category.id;

    if( item.version.id != NULL_ID )
        attr["fixed_version_id"] = item.version.id;

    if( item.assignedTo.id != NULL_ID )
        attr["assigned_to_id"] = item.assignedTo.id;

    if( item.parentId != NULL_ID )
        attr["parent_issue_id"] = item.parentId;

    if( item.customFields.size() )
    {
        QJsonArray customFields;

        for( const auto& customField : item.customFields )
        {
            QJsonObject cf;
            cf["id"] = customField.id;
            cf["value"] = customField.values.at( 0 );
            customFields.append( cf );
        }

        attr["custom_fields"] = customFields;
    }

//    if( item.watcher_user_ids != NULL_ID )
//        attr["watcher_user_ids"] = item.watcher_user_ids;

//    if( item.is_private != NULL_ID )
//        attr["is_private"] = item.is_private;

    if( item.estimatedHours )
        attr["estimated_hours"] = item.estimatedHours;

    QJsonObject data;
    data["issue"] = attr;

    QJsonDocument json;
    json.setObject( data );

    DEBUG()(json.toJson());

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        DEBUG()(json->toJson());

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( false, NULL_ID, ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        // Iterate over the document
        QJsonObject jsonIssue = json->object().find("issue").value().toObject();
        int issueId = jsonIssue.find("id").value().toInt();

        callback( true, issueId, NO_ERROR, QStringList() );
    };

    RedmineClient::sendIssue( json, cb, id, parameters );

    RETURN();
}

void
SimpleRedmineClient::sendTimeEntry( TimeEntry item, SuccessCb callback, int id, QString parameters )
{
    ENTER()(id)(parameters);

    if( (item.hours * 60) < 1 )
    {
        DEBUG() << "Time entry has to be at least 1 minute";
        callback( false, NULL_ID, ERR_TIME_ENTRY_TOO_SHORT, QStringList() );
        RETURN();
    }

    if( id == NULL_ID && item.issue.id == NULL_ID && item.project.id == NULL_ID )
    {
        DEBUG() << "No issue and no project specified";
        callback( false, NULL_ID, ERR_INCOMPLETE_DATA, QStringList() );
        RETURN();
    }

    QJsonObject attr;

    attr["hours"] = item.hours;

    if( item.activity.id != NULL_ID )
        attr["activity_id"] = item.activity.id;

    if( !item.comment.isEmpty() )
        attr["comments"] = item.comment;

    if( item.issue.id != NULL_ID )
        attr["issue_id"] = item.issue.id;

    if( item.project.id != NULL_ID )
        attr["project_id"] = item.project.id;

    if( item.spentOn.isValid() )
        attr["spent_on"] = item.spentOn.toString( Qt::ISODate );

    QJsonObject data;
    data["time_entry"] = attr;

    QJsonDocument json;
    json.setObject( data );

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( false, NULL_ID, ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        callback( true, NULL_ID, NO_ERROR, QStringList() );
    };

    RedmineClient::sendTimeEntry( json, cb, id, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveCustomFields( CustomFieldsCb callback, CustomFieldFilter filter )
{
    ENTER();

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( CustomFields(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        CustomFields customFields;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all customFields
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                CustomField customField;

                // Simple fields
                customField.id   = obj.value("id").toInt();
                customField.name = obj.value("name").toString();

                customField.defaultValue = obj.value("default_value").toString();

                customField.type = obj.value("customized_type").toString();
                if( !filter.type.isEmpty() && filter.type != customField.type )
                {
                    DEBUG("Skipping custom field without type")(filter.type);
                    continue;
                }

                customField.format    = obj.value("field_format").toString();
                customField.regex     = obj.value("regex").toString();
                customField.minLength = obj.value("min_length").toInt();
                customField.maxLength = obj.value("max_length").toInt();

                customField.allProjects = obj.value("is_for_all").toBool();
                customField.isRequired  = obj.value("is_required").toBool();
                customField.isFilter    = obj.value("is_filter").toBool();
                customField.searchable  = obj.value("searchable").toBool();
                customField.multiple    = obj.value("multiple").toBool();
                customField.visible     = obj.value("visible").toBool();

                // Iterate over all possible values
                for( const auto& j3 : obj.value("possible_values").toArray() )
                    customField.possibleValues.push_back( j3.toObject().value("value").toString() );

                // Iterate over all projects
                bool foundProject = false;
                for( const auto& j3 : obj.value("projects").toArray() )
                {
                    Item project;
                    project.id = j3.toObject().value("id").toInt();
                    project.name = j3.toObject().value("name").toString();
                    customField.projects.push_back( project );

                    if( project.id == filter.projectId )
                        foundProject = true;
                }

                if( !customField.allProjects && filter.projectId != NULL_ID && !foundProject )
                {
                    DEBUG("Skipping custom field without project")(filter.projectId);
                    continue;
                }

                // Iterate over all trackers
                bool foundTracker = false;
                for( const auto& j3 : obj.value("trackers").toArray() )
                {
                    Item tracker;
                    tracker.id = j3.toObject().value("id").toInt();
                    tracker.name = j3.toObject().value("name").toString();
                    customField.trackers.push_back( tracker );

                    if( tracker.id == filter.trackerId )
                        foundTracker = true;
                }

                if( filter.trackerId != NULL_ID && !foundTracker )
                {
                    DEBUG("Skipping custom field without tracker")(filter.trackerId);
                    continue;
                }

                customFields.push_back( customField );
            }
        }

        callback( customFields, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveCustomFields( cb );

    RETURN();
}

void
SimpleRedmineClient::retrieveEnumerations(QString enumeration, EnumerationsCb callback, QString parameters )
{
    ENTER()(enumeration)(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Enumerations(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Enumerations enumerations;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all enumerations
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Enumeration enumeration;

                // Simple fields
                enumeration.id        = obj.value("id").toInt();
                enumeration.name      = obj.value("name").toString();
                enumeration.isDefault = obj.value("is_default").toBool();

                fillDefaultFields( enumeration, &obj );

                enumerations.push_back( enumeration );
            }
        }

        callback( enumerations, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveEnumerations( enumeration, cb, parameters );

    RETURN();
}

void
parseIssue( Issue& issue, QJsonObject* obj )
{
    ENTER();

    // Simple fields
    issue.id          = obj->value("id").toInt();
    issue.description = obj->value("description").toString();
    issue.doneRatio   = obj->value("done_ratio").toInt();
    issue.subject     = obj->value("subject").toString();

    QJsonObject parent = obj->value("parent").toObject();
    if( !parent.isEmpty() )
        issue.parentId = parent.value("id").toInt();

    fillItem( issue.author,   obj, "author" );
    fillItem( issue.category, obj, "category" );
    fillItem( issue.priority, obj, "priority" );
    fillItem( issue.project,  obj, "project" );
    fillItem( issue.status,   obj, "status" );
    fillItem( issue.tracker,  obj, "tracker" );
    fillItem( issue.version,  obj, "fixed_version" );

    // Dates and times
    issue.dueDate        = obj->value("due_date").toVariant().toDate();
    issue.estimatedHours = obj->value("estimated_hours").toDouble();
    issue.startDate      = obj->value("start_date").toVariant().toDate();

    // Custom field
    auto addCustomField = [&](QJsonObject cfObj)
    {
        CustomField customField;
        customField.id   = cfObj.value("id").toInt();
        customField.name = cfObj.value("name").toString();

        if( cfObj.value("value").isString() )
            customField.values.push_back( cfObj.value("value").toString() );
        else if( cfObj.value("value").isArray() )
            for( const auto& v : cfObj.value("value").toArray() )
                customField.values.push_back( v.toString() );

        customField.multiple = cfObj.value("multiple").toBool();
        customField.type     = "issue";

        issue.customFields.push_back( customField );
    };

    for( const auto& cf : obj->value("custom_fields").toArray() )
        addCustomField( cf.toObject() );

    fillDefaultFields( issue, obj );

    RETURN();
  }

void
SimpleRedmineClient::retrieveIssue( IssueCb callback, int issueId, QString parameters )
{
    ENTER()(issueId)(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Issue(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Issue issue;
        QJsonObject obj = json->object().value("issue").toObject();
        parseIssue( issue, &obj );
        callback( issue, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveIssue( cb, issueId, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveIssues( IssuesCb callback, RedmineOptions options )
{
    ENTER()(options);

    struct Data
    {
        Issues issues;
        int offset = 0;
        JsonCb jsonCb;
    };

    Data* data = new Data();

    data->jsonCb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER()(json->toJson());

        Issues& issues = data->issues;
        int&    offset = data->offset;
        JsonCb& cb     = data->jsonCb;

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Issues(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        int count = 0;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all issues
            for( const auto& j2 : j1.toArray() )
            {
                Issue issue;
                QJsonObject obj = j2.toObject();
                parseIssue( issue, &obj );
                issues.push_back( issue );
                ++count;
                ++offset;
            }
        }

        if( options.getAllItems && count == limit_ )
        {
            // In the last run, as many issues as the limit is were found - so there might be more
            RedmineClient::retrieveIssues(
                        cb,
                        QString("%1&offset=%2&limit=%3")
                            .arg(options.parameters)
                            .arg(offset)
                            .arg(limit_) );
        }
        else
        {
            // No more issues to fetch
            callback( issues, NO_ERROR, QStringList() );
            delete data;
        }

        RETURN();
    };

    RedmineClient::retrieveIssues( data->jsonCb,
                                   QString("%1&offset=%2&limit=%3").arg(options.parameters).arg(0).arg(limit_) );

    RETURN();
}

void
SimpleRedmineClient::retrieveIssueCategories( IssueCategoriesCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( IssueCategories(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        IssueCategories issueCategories;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all issueStatuss
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                IssueCategory issueCategory;

                // Simple fields
                issueCategory.id   = obj.value("id").toInt();
                issueCategory.name = obj.value("name").toString();

                fillItem( issueCategory.project, &obj, "project" );
                fillItem( issueCategory.assignedTo, &obj, "assigned_to" );

                issueCategories.push_back( issueCategory );
            }
        }

        callback( issueCategories, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveIssueCategories( cb, projectId, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveIssuePriorities( EnumerationsCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "issue_priorities", callback, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveIssueStatuses( IssueStatusesCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( IssueStatuses(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        IssueStatuses issueStatuses;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all issueStatuss
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                IssueStatus issueStatus;

                // Simple fields
                issueStatus.id        = obj.value("id").toInt();
                issueStatus.name      = obj.value("name").toString();
                issueStatus.isDefault = obj.value("is_default").toBool();

                fillDefaultFields( issueStatus, &obj );

                issueStatuses.push_back( issueStatus );
            }
        }

        callback( issueStatuses, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveIssueStatuses( cb, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveMemberships( MembershipsCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Memberships(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Memberships memberships;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all issueStatuss
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Membership membership;

                // Simple fields
                membership.id = obj.value("id").toInt();

                fillItem( membership.project, &obj, "project" );
                fillItem( membership.user, &obj, "user" );
                fillItem( membership.group, &obj, "group" );

                memberships.push_back( membership );
            }
        }

        callback( memberships, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveMemberships( cb, projectId, parameters );

    RETURN();
}

void
parseProject( Project& project, QJsonObject* obj )
{
    ENTER();

    // Simple fields
    project.id          = obj->value("id").toInt();
    project.description = obj->value("description").toString();
    project.identifier  = obj->value("identifier").toString();
    project.isPublic    = obj->value("is_public").toBool();
    project.name        = obj->value("name").toString();

    fillItem( project.parent, obj, "parent" );

    // Iterate over all issue categories
    for( const auto& j3 : obj->value("issue_categories").toArray() )
    {
        Item category;
        category.id = j3.toObject().value("id").toInt();
        category.name = j3.toObject().value("name").toString();

        project.categories.push_back( category );
    }

    // Iterate over all trackers
    for( const auto& j3 : obj->value("trackers").toArray() )
    {
        Item tracker;
        tracker.id = j3.toObject().value("id").toInt();
        tracker.name = j3.toObject().value("name").toString();

        project.trackers.push_back( tracker );
    }

    fillDefaultFields( project, obj );

    RETURN();
  }

void
SimpleRedmineClient::retrieveProject( ProjectCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Project(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Project project;
        QJsonObject obj = json->object().value("project").toObject();
        parseProject( project, &obj );
        callback( project, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveProject( cb, projectId, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveProjects( ProjectsCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Projects(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Projects projects;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all projects
            for( const auto& j2 : j1.toArray() )
            {
                Project project;
                QJsonObject obj = j2.toObject();
                parseProject( project, &obj );
                projects.push_back( project );
            }
        }

        callback( projects, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveProjects( cb, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveTimeEntries( TimeEntriesCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( TimeEntries(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        TimeEntries timeEntries;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all trackers
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                TimeEntry timeEntry;

                // Simple fields
                timeEntry.comment    = obj.value("comments").toString();
                timeEntry.hours      = obj.value("hours").toDouble();

                // Dates and times
                timeEntry.spentOn    = obj.value("spent_on").toVariant().toDate();

                fillItem( timeEntry.activity, &obj, "activity" );
                fillItem( timeEntry.issue,    &obj, "issue" );
                fillItem( timeEntry.project,  &obj, "project" );

                fillDefaultFields( timeEntry, &obj );

                timeEntries.push_back( timeEntry );
            }
        }

        callback( timeEntries, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveTimeEntries( cb, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveTimeEntryActivities( EnumerationsCb callback, QString parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "time_entry_activities", callback, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveTrackers( TrackersCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Trackers(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Trackers trackers;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all trackers
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Tracker tracker;

                // Simple fields
                tracker.id   = obj.value("id").toInt();
                tracker.name = obj.value("name").toString();

                fillDefaultFields( tracker, &obj );

                trackers.push_back( tracker );
            }
        }

        callback( trackers, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveTrackers( cb, parameters );

    RETURN();
}

void
parseUser( User& user, QJsonObject* obj )
{
  ENTER();

  // Simple fields
  user.id   = obj->value("id").toInt();

  user.login     = obj->value("login").toString();
  user.firstname = obj->value("firstname").toString();
  user.lastname  = obj->value("lastname").toString();

  user.mail        = obj->value("mail").toString();
  user.lastLoginOn = obj->value("last_login_on").toVariant().toDateTime();

  fillDefaultFields( user, obj );

  RETURN();
}

void
SimpleRedmineClient::retrieveCurrentUser( UserCb callback )
{
    ENTER();

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( User(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        User user;
        QJsonObject obj = json->object().value("user").toObject();
        parseUser( user, &obj );
        callback( user, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveCurrentUser( cb );

    RETURN();
}

void
SimpleRedmineClient::retrieveUsers( UsersCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Users(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Users users;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all users
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                User user;
                parseUser( user, &obj );
                users.push_back( user );
            }
        }

        callback( users, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveUsers( cb, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveVersions( VersionsCb callback, int projectId, QString parameters )
{
    ENTER()(projectId)(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            callback( Versions(), ERR_NETWORK, getErrorList(reply, json) );
            RETURN();
        }

        Versions versions;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all issueStatuss
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Version version;

                // Simple fields
                version.id = obj.value("id").toInt();
                version.name = obj.value("name").toString();
                version.description = obj.value("description").toString();
                version.dueDate = obj.value("due_date").toVariant().toDate();

                versions.push_back( version );
            }
        }

        callback( versions, NO_ERROR, QStringList() );

        RETURN();
    };

    RedmineClient::retrieveVersions( cb, projectId, parameters );

    RETURN();
}
