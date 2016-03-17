#include "Example.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace qtredmine;

Example::Example( QObject *parent )
    : QObject( parent )
{
    redmine_ = new Redmine( "https://redmine.site", "34z58c7346btv847brcw478c6br434f234", this );

    printProjects();

}

void
Example::printProjects()
{
    auto callback = [=]( QNetworkReply* reply, QJsonDocument* data )
    {
        // Quit on network error
        if( reply->error() != QNetworkReply::NoError )
            return;

        // Iterate over the document
        for( const auto& doc : data->object() )
        {
            // Iterate over all projects
            for( const auto& project : doc.toArray() )
            {
                QJsonObject obj = project.toObject();
                qDebug() << obj.value("name").toString() << obj.value("id").toInt();
            }
        }
    };

    redmine_->retrieveProjects( callback );
}
