#ifndef XMAS_GLOBAL_H
#define XMAS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XMAS_LIBRARY)
#  define XMASSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XMASSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XMAS_GLOBAL_H
