#ifndef REDMINEQT_GLOBAL_HPP
#define REDMINEQT_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(REDMINEQT_LIBRARY)
#  define REDMINEQTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define REDMINEQTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // REDMINEQT_GLOBAL_HPP
