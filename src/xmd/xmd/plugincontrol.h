/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#ifndef PLUGINCONTROL_H
#define PLUGINCONTROL_H

#include <QObject>
#include <QDir>
#include <QMap>
#include <QColor>
#include <QVariantMap>

#include "vtplugininterface.h"

class PluginControl : public QObject
{
    Q_OBJECT
public:
    explicit PluginControl(QObject *parent = 0);
    ~PluginControl();

    Q_INVOKABLE QVariantMap pluginParams(QString name);
    Q_INVOKABLE bool pluginParam(QString name, QString key, QString value);

private:
    std::shared_ptr<QDir> pluginDir();

signals:
    void pluginsLoaded(QVariantList vtNameList);
    void writeLog(QString message,QColor color=Qt::black);

public slots:
    bool loadPlugins();

private:
    QDir m_pluginDir;
    QMap<QString, VtPluginInterface *> m_vtMap;
    LoggerInterface *m_logger;
};

#endif // PLUGINCONTROL_H
