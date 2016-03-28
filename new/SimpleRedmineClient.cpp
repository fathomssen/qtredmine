#include "SimpleRedmineClient.h"
#include "logging.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace qtredmine;

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
                QJsonObject obj = j2.toObject();

                Issue issue;

                // Simple fields
                issue.id          = obj.value("id").toInt();
                issue.description = obj.value("description").toString();
                issue.doneRatio   = obj.value("done_ratio").toInt();
                issue.subject     = obj.value("subject").toString();

                // Items
                auto fillItem = [&]( Item& item, QString value)
                {
                    QJsonObject itemObj = obj.value(value).toObject();

                    item.id   = itemObj.value("id").toInt();
                    item.name = itemObj.value("name").toString();
                };

                fillItem( issue.author,   "author" );
                fillItem( issue.category, "category" );
                fillItem( issue.priority, "priority" );
                fillItem( issue.project,  "project" );
                fillItem( issue.status,   "status" );
                fillItem( issue.tracker,  "tracker" );

                // Dates and times
                issue.createdOn      = obj.value("created_on").toVariant().toDateTime();
                issue.dueDate        = obj.value("due_date").toVariant().toDate();
                issue.estimatedHours = obj.value("estimated_hours").toVariant().toTime();
                issue.startDate      = obj.value("start_date").toVariant().toDate();
                issue.updatedOn      = obj.value("updated_on").toVariant().toDateTime();

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

                for( const auto& cf : obj.value("custom_fields").toArray() )
                    addCustomField( cf.toObject() );

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

                // Dates and times
                project.createdOn = obj.value("created_on").toVariant().toDateTime();
                project.updatedOn = obj.value("updated_on").toVariant().toDateTime();

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

                trackers.push_back( tracker );
            }
        }

        callback( trackers );

        RETURN();
    };

    retrieveTrackers( cb, parameters );

    RETURN();
}
