#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>

//
// If debug mode is enabled, use advanced logging methods
//
#ifdef DEBUG_OUTPUT

#include <QFileInfo>

enum normal
{
    ARG0
};

enum close
{
    ARG1,
    ARG2
};

inline QDebug operator<<( QDebug debug, normal )
{
    return debug;
}

inline QDebug operator<<( QDebug debug, close )
{
    return debug << ")";
}

#define C(s) s.c_str()
#define STR(s) std::string(s)
#define TOS(i) std::to_string(i)

// Function name
#define FUNC C( STR(__func__).append("()") )

// Filename and position
#define POS C( QFileInfo(__FILE__).fileName().toStdString().append(":").append(TOS(__LINE__)).append(":") )

// Enter and return helpers with filename and position
#define DBG(...) qDebug().noquote().nospace() << POS << " " << #__VA_ARGS__ << " "

#define ARG0(x)  "(" #x "=" << x << ARG1
#define ARG1(x) ", " #x "=" << x << ARG2
#define ARG2(x) ", " #x "=" << x << ARG1
#define ARGS ARG0

/*
 * This is how the variable printing works
 *
 * 1. A call to DEBUG() will translate to
 *    a. DBG() << ARGS
 *    b. DBG() << ARG0  // ARG0 is no macro but an enum
 *    c. DBG() << ""    // Empty QDebug
 *
 * 2. A call to DEBUG()(var1) will translate to
 *    a. DBG() << ARGS()(var1)
 *    b. DBG() << ARG0(var1)                        // ARG0 is a macro
 *    c. DBG() << "(" #var1 "=" << #var1 << ARG1    // ARG1 is no macro but an enum
 *    d. DBG() << "(" #var1 "=" << #var1 << ")"     // QDebug << ")"
 *
 * 3. A call to DEBUG()(var1)(var2) will translate to
 *    a. DBG() << ARGS()(var1)(var2)
 *    b. DBG() << ARG0(var1)(var2)                                          // ARG0 is a macro
 *    c. DBG() << "(" #var1 "=" << #var1 << ARG1(var2)                      // ARG1 is a macro
 *    c. DBG() << "(" #var1 "=" << #var1 << ", " #var1 "=" << #var2 << ARG2 // ARG2 is no macro but an enum
 *    d. DBG() << "(" #var1 "=" << #var1 << ", " #var1 "=" << #var2 << ")"  // QDebug << ")"
 */

#define DEBUG(...) DBG(__VA_ARGS__) << ARGS
#define ENTER(...) DBG(__VA_ARGS__) << "Entering " << FUNC << ARGS
#define RETURN(x) {DBG() << "Leaving " << FUNC; return x;}

#else

#define DEBUG(...) qDebug(__VA_ARGS__)
#define ENTER(...) DEBUG(__VA_ARGS__) << "Entering" << Q_FUNC_INFO
#define RETURN(x) {DEBUG() << "Leaving" << Q_FUNC_INFO; return x;}

#endif

#endif // LOGGING_H
