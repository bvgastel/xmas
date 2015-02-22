#ifndef PLUGINS_H
#define PLUGINS_H

#include <QString>

#include "vtplugininterface.h"

class ValidPlugin : public QObject, public VtPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "nl.ou.xmd.VtPluginInterface/1.0" FILE "vtplugininterface.json")
    Q_INTERFACES(VtPluginInterface)

public:
    QString <method>(const QString &message) Q_DECL_OVERRIDE;
};


#endif // PLUGINS_H
