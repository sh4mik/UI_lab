#ifndef SET_GLOBAL_H
#define SET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SET_LIBRARY)
#  define SET_EXPORT Q_DECL_EXPORT
#else
#  define SET_EXPORT Q_DECL_IMPORT
#endif

#endif // SET_GLOBAL_H
