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

    item.id   = itemObj.value("id").toInt();
    item.name = itemObj.value("name").toString();
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

void
SimpleRedmineClient::createTimeEntry( TimeEntry item, SuccessCb callback, QString parameters )
{
    ENTER()(parameters);

    if( (item.hours * 60) < 1 )
    {
        DEBUG() << "Time entry has to be at least 1 minute";
        callback( false, Error::TIME_ENTRY_TOO_SHORT );
        RETURN();
    }

    if( item.issue.id == 0 && item.project.id == 0 )
    {
        DEBUG() << "No issue and no project specified";
        callback( false, Error::INCOMPLETE_DATA );
        RETURN();
    }

    QJsonObject attr;

    attr["hours"] = item.hours;

    if( item.activity.id )
        attr["activity_id"] = item.activity.id;

    if( !item.comment.isEmpty() )
        attr["comments"] = item.comment;

    if( item.issue.id )
        attr["issue_id"] = item.issue.id;

    if( item.project.id )
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
            callback( false, Error::NETWORK );
            RETURN();
        }

        callback( true, Error::NO_ERROR );
    };

    createTimeEntry( json, cb, parameters );

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

        callback( enumerations );

        RETURN();
    };

    retrieveEnumerations( enumeration, cb, parameters );

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
parseIssue( Issue& issue, QJsonObject* obj )
{
  ENTER();

  // Simple fields
  issue.id          = obj->value("id").toInt();
  issue.description = obj->value("description").toString();
  issue.doneRatio   = obj->value("done_ratio").toInt();
  issue.subject     = obj->value("subject").toString();

  fillItem( issue.author,   obj, "author" );
  fillItem( issue.category, obj, "category" );
  fillItem( issue.priority, obj, "priority" );
  fillItem( issue.project,  obj, "project" );
  fillItem( issue.status,   obj, "status" );
  fillItem( issue.tracker,  obj, "tracker" );

  // Dates and times
  issue.dueDate        = obj->value("due_date").toVariant().toDate();
  issue.estimatedHours = obj->value("estimated_hours").toVariant().toTime();
  issue.startDate      = obj->value("start_date").toVariant().toDate();

  // Custom field
  auto addCustomField = [&](QJsonObject cfObj)
  {
      CustomField customField;
      customField.id   = cfObj.value("id").toInt();
      customField.name = cfObj.value("name").toString();

      for( const auto& v : cfObj.value("value").toArray() )
        customField.value.push_back( v.toString() );

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
            RETURN();
        }

        Issue issue;
        QJsonObject obj = json->object().value("issue").toObject();
        parseIssue( issue, &obj );
        callback( issue );

        RETURN();
    };

    retrieveIssue( cb, issueId, parameters );

    RETURN();
}

void
SimpleRedmineClient::retrieveIssues( IssuesCb callback, QString parameters )
{
    ENTER()(parameters);

    auto cb = [=]( QNetworkReply* reply, QJsonDocument* json )
    {
        ENTER();

        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
        {
            DEBUG() << "Network error:" << reply->errorString();
            RETURN();
        }

        Issues issues;

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
            }
        }

        callback( issues );

        RETURN();
    };

    retrieveIssues( cb, parameters );

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

        callback( issueStatuses );

        RETURN();
    };

    retrieveIssueStatuses( cb, parameters );

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
            RETURN();
        }

        Projects projects;

        // Iterate over the document
        for( const auto& j1 : json->object() )
        {
            // Iterate over all projects
            for( const auto& j2 : j1.toArray() )
            {
                QJsonObject obj = j2.toObject();

                Project project;

                // Simple fields
                project.id          = obj.value("id").toInt();
                project.description = obj.value("description").toString();
                project.identifier  = obj.value("identifier").toString();
                project.isPublic    = obj.value("is_public").toBool();
                project.name        = obj.value("name").toString();

                fillDefaultFields( project, &obj );

                projects.push_back( project );
            }
        }

        callback( projects );

        RETURN();
    };

    retrieveProjects( cb, parameters );

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

                DEBUG()(timeEntry.issue.id)(timeEntry.project.id)(timeEntry.hours);

                timeEntries.push_back( timeEntry );
            }
        }

        callback( timeEntries );

        RETURN();
    };

    retrieveTimeEntries( cb, parameters );

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

        callback( trackers );

        RETURN();
    };

    retrieveTrackers( cb, parameters );

    RETURN();
}
