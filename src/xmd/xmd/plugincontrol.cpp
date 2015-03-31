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

#include <QPluginLoader>
#include <QApplication>
#include <QVariantList>
#include <QDebug>

#include "commoninterface.h"
#include "logger.h"
#include "plugincontrol.h"

PluginControl::PluginControl(QObject *parent)
    : QObject(parent), m_logger(new Logger("plugin control"))
{
    std::shared_ptr<QDir> dir = pluginDir();
    m_pluginDir = QDir(*dir);
}

PluginControl::~PluginControl()
{

}

bool PluginControl::startPlugin(QString vtPlugin) {
   VtPluginInterface *plugin = m_vtMap[vtPlugin];
    auto xmap = getXmasComponents();
    if (!xmap.empty()) {
        plugin->start(xmap);
        return true;
    }
    m_logger->log(QString("[PluginControl] Plugin not started due to empty map."));
    return false;
}

bool PluginControl::stopPlugin(QString vtPlugin) {
    Q_UNUSED(vtPlugin)
//    VtPluginInterface *plugin = m_vtMap[vtPlugin];
//        plugin->stop();
        return true;
    m_logger->log(QString("[PluginControl] Plugin stopped."));
}

/*
 * Return the map of xmas components by name
 * Return an empty map if things go wrong.
 * But do show some message of what went wrong.
 *
 */
XMap PluginControl::getXmasComponents() {
    std::shared_ptr<XMASProject> project = dataControl->project();
    if (!project) {
        QString msg = "[PluginControl::startPlugin] Plugin could not run due to null xmas_project.";
        m_logger->log(msg);
        std::cerr << msg.toStdString() << std::endl;
        return XMap();
    }
    XMASNetwork *network = project->getRootNetwork();
    if (!network) {
        QString msg = "[PluginControl::startPlugin] Plugin could not run due to null network from xmas_project.";
        m_logger->log(msg);
        std::cerr << msg.toStdString() << std::endl;
        return XMap();
    }
    auto xmap = network->getComponentMap();
    return xmap;
}


std::shared_ptr<QDir> PluginControl::pluginDir() {
    std::shared_ptr<QDir> pluginDir = std::make_shared<QDir>(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginDir->dirName().toLower() == "debug" || pluginDir->dirName().toLower() == "release")
        pluginDir->cdUp();
#elif defined(Q_OS_MAC)
    if (pluginDir->dirName() == "MacOS") {
        pluginDir->cdUp();
        pluginDir->cdUp();
        pluginDir->cdUp();
    }
#endif
    pluginDir->cdUp(); // to build dir
    pluginDir->cdUp(); // to src
    pluginDir->cdUp(); // to xmas
    pluginDir->cd("lib");
    pluginDir->cd("plugins");
//    qApp->addLibraryPath(pluginDir->absolutePath());
    QString msg = QString("Plugin directory is ")+pluginDir->absolutePath();
    m_logger->log(msg);
    qDebug() << msg;
    return pluginDir;
}

bool PluginControl::loadPlugins() {
    QVariantList vtNameList;
    foreach (QString fileName, m_pluginDir.entryList(QDir::Files)) {
        m_logger->log("Found " + fileName + " in " + m_pluginDir.absolutePath());
        QPluginLoader pluginLoader(m_pluginDir.absoluteFilePath(fileName));
        pluginLoader.load();
        if (!pluginLoader.isLoaded()) {
            QString errorString = pluginLoader.errorString();
            QString msg = "[Load plugins] Error loading plugin, msg = " + errorString;
            qDebug() << msg;
            m_logger->log(msg);
            return false;
        }
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            VtPluginInterface *vtPluginInterface = qobject_cast<VtPluginInterface *>(plugin);
            if (vtPluginInterface) {
                m_logger->log("Success!! Plugin found is vtPlugin in " + fileName);
                QString vtname = vtPluginInterface->name();
                if (m_vtMap.contains(vtname)) {
                    m_logger->log("Duplicate plugin found: " + vtname);
                    m_logger->log(QString("Second occurrence ignored."));
                } else {
                    m_vtMap[vtname] = vtPluginInterface;
                    vtNameList.append(vtname);
                }
            } else {
                m_logger->log("Fail!! Loader found no vtPlugin in " + fileName);
            }
        } else {
            m_logger->log("Fail! Loader found no plugin in "+ fileName);
        }
    }
    emit pluginsLoaded(vtNameList);
    return m_vtMap.size()>0;
}
/**
 * @brief PluginControl::pluginParams
 * @param name
 * @return
 */
QVariantMap PluginControl::pluginParams(QString vtname) {
    QVariantMap map;
    if (m_vtMap.contains(vtname)) {
        VtPluginInterface *vtPluginInterface = m_vtMap[vtname];
        map = vtPluginInterface->paramMap();
    }
    qDebug() << "plugin param count : " << (map.size());

    return map;
}

bool PluginControl::pluginParam(QString name, QString key, QString value) {
    const char *cname = name.toStdString().c_str();
    const char *ckey = key.toStdString().c_str();
    if (m_vtMap.contains(cname)) {
        auto vtplugin = m_vtMap[cname];
        auto paramMap = vtplugin->paramMap();
        if (paramMap.contains(ckey)) {
            paramMap[ckey] = value;
            return true;
        }
    }
    return false;
}
