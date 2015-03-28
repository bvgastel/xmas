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
#ifndef SYNTAXCHECKERPLUGIN_H
#define SYNTAXCHECKERPLUGIN_H

#include <chrono>

#include <QString>
#include <QVariant>
#include <QThread>
#include <QProcess>

#include "resultinterface.h"
#include "commoninterface.h"
#include "vtplugininterface.h"

class SyntaxCheckerPlugin : public QObject, public VtPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VtPluginInterface_iid FILE "vtplugin.json")
    Q_INTERFACES(VtPluginInterface)

public:
    SyntaxCheckerPlugin(QObject *parent = 0);

    virtual ~SyntaxCheckerPlugin();

    virtual QString name() override;
    virtual void name(QString name) override;

    virtual void parameters(QVariantMap paramMap) override;
    virtual QVariantMap paramMap() override;

    virtual void start(XMap &componentMap) override;
    virtual void startThread(const QString &json) override;
    virtual void startProcess(const QString &json) override;

//    virtual LoggerInterface *logger() override;

public slots:
    virtual void handleResults(const ResultInterface &result);
signals:
    void operate(const QString &json);

private:

    /**
     * @brief m_name the name of the verification tool.
     */
    QString m_name;
    /**
     * @brief m_paramMap the parameters for the verification tool in a QMap.
     */
    QMap<QString, QVariant> m_paramMap;
    /**
     * @brief m_logger A specific logger for this verification tool.
     */
    //LoggerInterface *m_logger;
    /**
     * @brief m_workerThread The thread that will run the verification tool
     */
    QThread m_workerThread;
    QProcess m_process;
};


#endif // SYNTAXCHECKERPLUGIN_H
