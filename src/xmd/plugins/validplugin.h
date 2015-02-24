#ifndef PLUGINS_H
#define PLUGINS_H

#include <QString>
#include <QMap>

#include "vtplugininterface.h"
#include "logger.h"

class ValidPlugin : public QObject, public VtPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VtPluginInterface_iid FILE "vtplugin.json")
    Q_INTERFACES(VtPluginInterface)

public:
    ValidPlugin() :
        m_paramMap({{"parameter1", ""}, {"parameter2", ""}}),
        m_logger(new Logger("ValidPlugin"))
    {
    }

    virtual QString name();
    virtual void name(QString name);

    virtual QMap<QString, QString> parameters();
    virtual void parameters(QMap<QString, QString> paramMap);

    virtual bool run_json_string(QString jsonString);
    virtual bool run_json_file(QUrl fileUrl);
    virtual bool run_json_xmas(std::map<std::string, XMASComponent * > );

    virtual Logger *logger() {
        return m_logger;
    }

private:

    /**
     * @brief m_name the name of the verification tool.
     */
    QString m_name;
    /**
     * @brief m_paramMap the parameters for the verification tool in a QMap.
     */
    QMap<QString, QString> m_paramMap;
    /**
     * @brief m_mp memory pool
     */
    bitpowder::lib::MemoryPool m_mp;
    /**
     * @brief m_logger A specific logger for this verification tool.
     */
    Logger *m_logger;
};


#endif // PLUGINS_H
