#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "SimpleRedmineClient.h"

#include <QObject>

/**
 * @brief Example class for using QtRedmine
 */
class Example : public QObject
{
    Q_OBJECT

private:
    /// Redmine connection object
    qtredmine::SimpleRedmineClient* redmine_;

    /// Issues cache
    qtredmine::Issues issues_;

public:
    /**
     * @brief Constructor
     *
     * @param parent Parent QObject
     */
    explicit Example( QObject* parent = nullptr );

    /**
     * @brief Cache issues
     */
    void cacheIssues();

    /**
     * @brief Print projects using DEBUG()
     */
    void printProjects();
};

#endif // EXAMPLE_H
