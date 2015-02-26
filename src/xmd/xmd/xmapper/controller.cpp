/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/

#include <QtQml>
#include <QDir>
#include <QPluginLoader>

#include "controller.h"
#include "simplestring.h"
#include "memorypool.h"
#include "parse.h"
#include "canvascomponentextension.h"

//#include "common.h"

Controller::Controller(QObject* parent)
    : QObject(parent), m_componentWalker(nullptr), m_logger("controller")
{
    // Connect the logger writeLog to our own signal
    QObject::connect(&m_logger, &Logger::writeLog, this, &Controller::writeLog);
}

Controller::~Controller()  {

}

void Controller::startMsg() {
    m_logger.log(QString("Controller created and starting."));
}

void Controller::endMsg() {
    m_logger.log(QString("Controller ending."));
}

bool Controller::loadPlugins() {
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    QVariantList vtNameList;
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            VtPluginInterface *vtPluginInterface = qobject_cast<VtPluginInterface *>(plugin);
            if (vtPluginInterface) {
                QString vtname = vtPluginInterface->name();
                if (!m_vtMap.contains(vtname)) {
                    m_vtMap[vtname] = vtPluginInterface;
                    vtNameList.append(vtname);
                }
            }
        }
    }
    emit pluginsLoaded(vtNameList);
    return m_vtMap.size()>0;
}

bool Controller::startPlugin(QString vtname) {
    auto plugin = m_vtMap[vtname];
    plugin->run("");
    return false;
}

/**
 * @brief Controller::pluginParams retrieves the parameters with current values
 * @param name the name of the plugin
 * @return a QVariantMap with the plugin parameters and values
 */
QVariantMap Controller::pluginParams(QString name) {
    QVariantMap map;
    if (m_vtMap.contains(name)) {
        VtPluginInterface *vtPluginInterface = m_vtMap[name];
        map.insert("name", vtPluginInterface->name());
        QMap<QString, QString> paramMap = vtPluginInterface->parameters();
        auto i = paramMap.constBegin();
        while (i != paramMap.constEnd()) {
            std::string propName = i.key().toStdString();
            map.insert(propName.c_str(), i.value());
        }
    }
    return map;
}
/**
 * @brief Controller::interfaceParam Sets one param in the interface
 * @param name the name of the interface
 * @param key the key of the parameter
 * @param value the value of the parameter
 * @return true only if the plugin and the parameter exists
 */
bool Controller::pluginParam(QString name, QString key, QString value) {
    const char *cname = name.toStdString().c_str();
    const char *ckey = key.toStdString().c_str();
    if (m_vtMap.contains(cname)) {
        auto vtplugin = m_vtMap[cname];
        auto paramMap = vtplugin->parameters();
        if (paramMap.contains(ckey)) {
            paramMap[ckey] = value;
            return true;
        }
    }
    return false;
}

/**
 * @brief Controller::fileOpen
 * @param fileUrl
 * @return true if the file was succesfully opened.
 *
 */
bool Controller::fileOpen(QUrl fileUrl)
{

    auto save_componentWalker = m_componentWalker;

    bitpowder::lib::MemoryPool mp;

    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    m_logger.log("Opening file " + filename);

    std::tie(m_componentMap, std::ignore) = parse_xmas_from_file(filename, mp);

    if (m_componentMap.empty()) {
        m_logger.log("[Component.cpp/fileOpen(fileUrl)] File "+ filename + " was parsed as empty. Maybe the file is invalid json input.",Qt::red);
        return false;
    }
    return emitNetwork();
}

bool Controller::emitNetwork() {
    std::clock_t c_start = std::clock();
    QVariantList compList;
    for(auto &it : m_componentMap) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantMap map;
            convertToQml(map, comp);
            compList.append(map);
        }
    }
    QVariantList channelList;
    for (auto &it : m_componentMap) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantList list;
            connectInQml(list, comp);
            channelList.append(list);
        }
    }
    QVariantMap network;
    network["complist"] = compList;
    network["channellist"] = channelList;

    emit createNetwork(network);
    std::clock_t c_end = std::clock();
        qDebug() << "CPU time used: " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n"
                       << " for " << compList.length() << " components and " << channelList.length() << " channels";

    return true;
}

void Controller::emitComponent(XMASComponent *comp) {
    QVariantMap map;
    convertToQml(map, comp);
    emit createComponent(map);
}

void Controller::connectInQml(QVariantList &list, XMASComponent *comp) {
    for (Output *out : comp->outputPorts()) {
        if (out->isConnected()) {
            QVariantMap map;
            map.insert("initiator", QString(out->getInitiator()->getStdName().c_str()));
            map.insert("initiatorport", QString(out->getName()));
            map.insert("target", QString(out->getTarget()->getStdName().c_str()));
            map.insert("targetport", QString(out->getTargetPort()->getName()));
//            m_logger.log(
//             "channel created from " + out->getInitiator()->getStdName() +
//             " to " + out->getTarget()->getStdName());
            list.append(map);
        } else {
            m_logger.log("output port " + std::string(out->getName()) + " in comp "
                      + out->getComponent()->getStdName() + " is not connected");
        }
    }
}

void Controller::convertToQml(QVariantMap &map, XMASComponent *comp) {
    std::string name = comp->getStdName();
//    m_logger.log("name = "+ name + " slot for creation called", Qt::darkGreen);

    std::type_index typeIndex = std::type_index(typeid(*comp));
    QString type = m_type_map[typeIndex];
    QString qname = QString::fromStdString(name);

    CanvasComponentExtension *ext = comp->getExtension<CanvasComponentExtension *>();
    if (ext) {
        map.insert("x", ext->x());
        map.insert("y", ext->y());
        map.insert("orientation", ext->orientation());
        map.insert("scale", ext->scale());
    }

    map.insert("type", type);
    map.insert("name", qname);
    //@Guus of Jeroen : in qml is property "param" gelinked, dus voor queue is dit de capacity.
    //map.insert("param", "");

}

void Controller::emitDuplicate(XMASComponent *comp) {
    std::string name = comp->getStdName();
    std::string msg = "name = "+ name + " was a duplicate";
    m_logger.log(msg, Qt::red);
}


/**
 * @brief Controller::componentCreated
 * @param comp
 * @return
 */
bool Controller::componentCreated(const QVariant &qvariant)
{
    qDebug() << "Component created by designer";
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString type = QQmlProperty::read(qobject, "type").toString();
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "type = " << type << ", name = " << name << ".";
    auto it = this->m_componentMap.find(name.toStdString());
    if (it == m_componentMap.end()) {
        qDebug() << name << ", " << type << "added to network";
        for (QObject *child : qobject->children()) {
            if (child->objectName() == "port") {
                QVariant vpname = child->property("name");
                QString pname = vpname.toString();
                qDebug() << " port: " << pname;
            }
        }
//        m_logger.log(QString("Created component name = \"")+name+QString("\""),Qt::black);
        return true;
    }
    m_logger.log("component "+name+ " was not created.",Qt::red);
    return false;
}


/**
 * @brief Controller::componentDestroyed
 * @param object
 * @return
 */
bool Controller::componentDestroyed(const QVariant &qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Component " + name + " destroyed by designer";

    return true;
}

/**
 * @brief Controller::componentChanged
 * @param object
 * @return
 */
bool Controller::componentChanged(const QVariant &qvariant)
{
    Q_UNUSED(qvariant)
    qDebug() << "Component changed by designer";
    return true;
}


bool Controller::channelCreated(const QVariant &qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Channel " + name + " created by designer";
    return true;
}

bool Controller::channelDestroyed(const QVariant &qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Channel " + name + " destroyed by designer";
    return true;
}

bool Controller::channelChanged(const QVariant &qvariant)
{
    Q_UNUSED(qvariant)
    qDebug() << "Channel changed by designer";
    return true;
}
