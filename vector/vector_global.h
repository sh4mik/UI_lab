#ifndef VECTOR_GLOBAL_H
#define VECTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VECTOR_LIBRARY)
#  define VECTOR_EXPORT Q_DECL_EXPORT
#else
#  define VECTOR_EXPORT Q_DECL_IMPORT
#endif

#endif // VECTOR_GLOBAL_H
