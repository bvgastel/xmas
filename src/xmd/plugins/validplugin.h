/*********************************************************************
 *
 * Copyright (C) Guus Bonnema, 2015
 *
 * This file is part of the xmas-design tool.
 *
 * The xmas-design tool is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * The xmas-design tool is distributed in the hope that it will be
 * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the xmas-design tool.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 **********************************************************************/
#ifndef PLUGINS_H
#define PLUGINS_H

#include <QString>
#include <QMap>

#include "logger.h"
#include "vtplugininterface.h"

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