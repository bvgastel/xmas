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
#ifndef PLUGINTHREAD_H
#define PLUGINTHREAD_H

#include <QString>
#include <QMap>

#include "xmas.h"
#include "logger.h"
#include "xmapper/vtplugininterface.h"

class PluginThread : public QObject, public VtPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VtPluginInterface_iid FILE "vtplugin.json")
    Q_INTERFACES(VtPluginInterface)

public:
    PluginThread():
        m_name("plugin in thread"),
        m_paramMap({{"parameter1", ""}, {"parameter2", ""}}),
        m_logger(new Logger("PluginThread")) {
        m_name = "simple checks in thread";
    }

    ~PluginThread() {

    }

    virtual QString name() override;
    virtual void name(QString name) override;

    virtual QMap<QString, QString> parameters() override;
    virtual void parameters(QMap<QString, QString> paramMap) override;

    // FIXME: why do I *not* get an error message for override of non-existing parent method?
    virtual void xstart(const QString &json) override;

    virtual bool run(const QUrl &fileUrl) override;
    virtual bool run(const QString &json) override;

    virtual Logger *logger() override;

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
     * @brief m_logger A specific logger for this verification tool.
     */
    Logger *m_logger;
};

#endif // PLUGINTHREAD_H
