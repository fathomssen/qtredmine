#ifndef QTREDMINE_GLOBAL_H
#define QTREDMINE_GLOBAL_H

#include <QtGlobal>

#if defined(QT_STATIC_BUILD)
  #define QTREDMINESHARED_EXPORT
#else
  #if defined(QTREDMINE_LIBRARY)
    #define QTREDMINESHARED_EXPORT Q_DECL_EXPORT
  #else
    #define QTREDMINESHARED_EXPORT Q_DECL_IMPORT
  #endif
#endif

#endif // QTREDMINE_GLOBAL_H
