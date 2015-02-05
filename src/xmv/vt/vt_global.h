#ifndef VT_GLOBAL_H
#define VT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VT_LIBRARY)
#  define VTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VT_GLOBAL_H
