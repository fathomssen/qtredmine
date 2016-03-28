#include "Example.h"

#include "logging.h"

#include <QDebug>

using namespace qtredmine;

Example::Example( QObject *parent )
    : QObject( parent )
{
    redmine_ = new SimpleRedmineClient( "https://redmine.site", "34z58c7346btv847brcw478c6br434f234", this );

    printProjects();

}

void
Example::cacheIssues()
{
    redmine_->retrieveIssues( [&]( Issues issues ){ issues_ = issues; } );
}

void
Example::printProjects()
{
    redmine_->retrieveProjects( []( Projects projects )
    {
        for( const auto& project : projects )
            DEBUG() << project.name;
    } );
}
