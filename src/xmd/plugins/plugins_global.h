#ifndef PLUGINS_GLOBAL_H
#define PLUGINS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGINS_LIBRARY)
#  define PLUGINSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLUGINSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGINS_GLOBAL_H
