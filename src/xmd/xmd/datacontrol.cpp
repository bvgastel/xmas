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

#include <iostream>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <QQmlContext>
#include <QDebug>
#include <QtQml>
#include <QVariant>
#include "canvascomponentextension.h"
#include "model/component.h"
#include "model/port.h"
#include "model/channel.h"
#include "datacontrol.h"

DataControl::DataControl(QObject *parent) : QObject(parent), m_logger("datacontrol")
{
}

DataControl::~DataControl()
{

}

void DataControl::registerTypes() {
    qmlRegisterType<model::Component>(m_modelName, m_modelMajor, m_modelMinor, "xcomponent");
    qmlRegisterType<model::Channel>(m_modelName, m_modelMajor, m_modelMinor, "xconnection");
    qmlRegisterType<model::Port>(m_modelName, m_modelMajor, m_modelMinor, "xport");
}

bool DataControl::fileOpen(QUrl fileUrl) {
    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    m_logger.log("Opening file " + filename);

    std::tie(m_componentMap, std::ignore) = parse_xmas_from_file(filename, m_mp);

    if (m_componentMap.empty()) {
        m_logger.log("[Component.cpp/fileOpen(fileUrl)] File "+ filename + " was parsed as empty. Maybe the file is invalid json input.",Qt::red);
        return false;
    }
    return emitNetwork();
}

bool DataControl::emitNetwork() {
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
        qDebug() << "CPU time used: " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms"
                       << " for " << compList.length() << " components and " << channelList.length() << " channels";

    return true;
}

void DataControl::connectInQml(QVariantList &list, XMASComponent *comp) {
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


void DataControl::convertToQml(QVariantMap &map, XMASComponent *comp) {
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

/**
 * @brief DataControl::componentCreated
 * @param comp
 * @return
 */
bool DataControl::componentCreated(const QVariant &qvariant)
{
    m_logger.log(QString("Creating component (C++)"));
    QString msg("created ");
    qDebug() << "Component created by designer (C++)";
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString type = QQmlProperty::read(qobject, "type").toString();
    QString name = QQmlProperty::read(qobject, "name").toString();
    int x = QQmlProperty::read(qobject, "x").toInt();
    int y = QQmlProperty::read(qobject, "x").toInt();
    int orientation = QQmlProperty::read(qobject, "orientation").toInt();
    int scale = QQmlProperty::read(qobject, "scale").toInt();
    msg += type + "(" + name + ") at [" + x + "," + y + "]"
            + "looking at " + orientation + " scaled " + scale;
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
        m_logger.log(msg,Qt::black);
        return true;
    }
    m_logger.log("component "+name+ " was not created.",Qt::red);
    return false;
}

/**
 * @brief DataControl::componentDestroyed
 * @param object
 * @return
 */
bool DataControl::componentDestroyed(const QVariant &qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Component " + name + " destroyed by designer";

    return true;
}

/**
 * @brief DataControl::componentChanged
 * @param object
 * @return
 */
bool DataControl::componentChanged(const QVariant &qvariant)
{
    Q_UNUSED(qvariant)
    qDebug() << "Component changed by designer";
    return true;
}


bool DataControl::channelCreated(const QVariant &qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Channel " + name + " created by designer";
    return true;
}

bool DataControl::channelDestroyed(const QVariant &qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Channel " + name + " destroyed by designer";
    return true;
}

bool DataControl::channelChanged(const QVariant &qvariant)
{
    Q_UNUSED(qvariant)
    qDebug() << "Channel changed by designer";
    return true;
}
