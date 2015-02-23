#ifndef PLUGINS_H
#define PLUGINS_H

#include <QString>

#include "vtplugininterface.h"
#include "logger.h"

class ValidPlugin : public QObject, public VtPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VtPluginInterface_iid FILE "vtplugin.json")
    Q_INTERFACES(VtPluginInterface)

public:
    ValidPlugin() : m_logger(new Logger("ValidPlugin")) {

    }

    virtual bool run_json_string(QString jsonString);
    virtual bool run_json_file(QUrl fileUrl);
    virtual bool run_json_xmas(std::map<std::string, XMASComponent * > );

    virtual Logger *logger() {
        return m_logger;
    }

private:

    bitpowder::lib::MemoryPool m_mp;
    Logger *m_logger;
};


#endif // PLUGINS_H
