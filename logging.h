#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>

// "<variable name> = <variable value>"
#define _(x) #x << "=" << x

//
// If debug mode is enabled, use advanced logging methods
//
#ifdef DEBUG

#include <QFileInfo>
// Filename and position
#define POS QFileInfo(__FILE__).fileName() << ":" << __LINE__ << ": "
// Enter and return helpers with filename and position
#define qEnter() qDebug().nospace().noquote() << POS << "Entering " << Q_FUNC_INFO
#define qReturn(x) {qDebug().nospace().noquote() << POS << "Leaving " << Q_FUNC_INFO; return x;}

#else

// Enter and return helpers without filename and position
#define qEnter() qDebug() << "Entering " << Q_FUNC_INFO
#define qReturn(x) {qDebug() << "Leaving " << Q_FUNC_INFO; return x;}

#endif // DEBUG

#endif // LOGGING_H
