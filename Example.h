#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "Redmine.h"

#include <QObject>

class Example : public QObject
{
    Q_OBJECT

private:
    /// Redmine connection object
    qtredmine::Redmine* redmine_;

public:
    /**
     * @brief Constructor
     *
     * @param parent Parent QObject
     */
    explicit Example( QObject* parent = nullptr );

    /**
     * @brief Print projects using qDebug()
     */
    void printProjects();
};

#endif // EXAMPLE_H
