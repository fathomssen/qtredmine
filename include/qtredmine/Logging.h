#pragma once

#include <QDebug>

/*
 * This is how the variable printing works (examples below for debug mode)
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

enum argNone{ ARG0 };
enum argLast{ ARG1, ARG2 };

inline QDebug operator<<( QDebug debug, argNone )
{
    return debug;
}

inline QDebug operator<<( QDebug debug, argLast )
{
    return debug << ")";
}

#define ARG0(x)  "(" #x "=" << x << ARG1
#define ARG1(x) ", " #x "=" << x << ARG2
#define ARG2(x) ", " #x "=" << x << ARG1
#define ARGS ARG0

enum fieldArgNone{ FIELDARG0 };
enum fieldArgLast{ FIELDARG1, FIELDARG2 };

inline QDebug operator<<( QDebug debug, fieldArgNone )
{
    return debug;
}

inline QDebug operator<<( QDebug debug, fieldArgLast )
{
    return debug << ")";
}

#define FIELDARG0(x)  "(" << #x << "=" << data.x << FIELDARG1
#define FIELDARG1(x) ", " << #x << "=" << data.x << FIELDARG2
#define FIELDARG2(x) ", " << #x << "=" << data.x << FIELDARG1
#define FIELDARGS FIELDARG0

#ifdef DEBUG_OUTPUT

#include <QFileInfo>
#include <QRegularExpression>

int  getLoggingIndent();
bool getLoggingNewline();
void increaseLoggingIndent();
void decreaseLoggingIndent();

#define C(s) s.c_str()
#define STR(s) std::string(s)
#define TOS(i) std::to_string(i)

// Function name
// - Remove the return type name
// - Remove the namespace name (expected to be lower-case letters only)
// - Remove the const indicator
// - Replace the signature by '()'
// - Replace the default lambda expression by 'lambda()'
#define FUNC \
    QString(Q_FUNC_INFO)\
    .replace(QRegularExpression("^[a-zA-Z:<>]+ "), "")\
    .replace(QRegularExpression("^(\\*?)[a-z]*::"), "\\1")\
    .replace(QRegularExpression(" +const$"), "")\
    .replace(QRegularExpression("\\([^)]*\\)$"), "()")\
    .replace(QRegularExpression("\\([^)]*\\)::\\(anonymous class\\)::operator\\(\\)\\(\\)$"), "()::lambda()")\
    .replace(QRegularExpression("\\([^)]*\\)::\\(anonymous class\\)::operator\\(\\)\\(\\)$"), "()::lambda()")\

// Filename and position
#define POS C( QFileInfo(__FILE__).fileName().toStdString().append(":").append(TOS(__LINE__)) )

// Enter and return helpers with filename and position
#define DBG(...) qDebug().noquote().nospace() << QString("  ").repeated(getLoggingIndent())

#define DEBUG(...) DBG(__VA_ARGS__) << POS << ": " << ARGS
#define DEBUGFIELDS debug.nospace() << FIELDARGS

#define ENTER() \
    if( getLoggingNewline() ) { qDebug().noquote().nospace() << " "; }\
    DBG() << FUNC << " { (" << POS << ")";\
    increaseLoggingIndent();\
    DBG() << ARGS

#define RETURN_0()  DBG() << "} (" << POS << ")"; return;
#define RETURN_1(A) DBG() << "} -> '" << A << "' (" << POS << ")"; return A;
#define RETURN_2(A,B) DBG() << "} -> '" << B << "' (" << POS << ")"; return A;
#define RETURN_X( x, A, B, FCT, ... ) FCT
#define RETURN(...) do{\
        decreaseLoggingIndent();\
        RETURN_X( , ##__VA_ARGS__, RETURN_2(__VA_ARGS__), RETURN_1(__VA_ARGS__), RETURN_0(__VA_ARGS__) )\
    }while(0)

#define CBENTER() \
    ENTER();\
    if( deleteLater_ ) CBRETURN();\
    DBG() << ARGS

#define CBRETURN(x) do{\
        --callbackCounter_;\
        if( deleteLater_ ) deleteLater();\
        RETURN(x);\
    }while(0)

#else

#define DEBUG(...) QNoDebug() << ARGS
#define DEBUGFIELDS QNoDebug() << FIELDARGS
#define ENTER() QNoDebug() << ARGS

#define RETURN_0()  return;
#define RETURN_1(A) return A;
#define RETURN_2(A,B) return A;
#define RETURN_X( x, A, B, FCT, ... ) FCT
#define RETURN(...) do{\
        RETURN_X( , ##__VA_ARGS__, RETURN_2(__VA_ARGS__), RETURN_1(__VA_ARGS__), RETURN_0(__VA_ARGS__) )\
    }while(0)

// Callbacks
#define CBENTER(...) \
    if( deleteLater_ ) CBRETURN();\
    QNoDebug() << ARGS

#define CBRETURN(x) do{\
        --callbackCounter_;\
        if( deleteLater_ ) deleteLater();\
        return;\
    }while(0)

#endif
