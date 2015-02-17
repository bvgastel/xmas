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

#include "controller.h"
#include "simplestring.h"
#include "memorypool.h"
#include "parse.h"
#include "canvascomponentextension.h"
//#include "common.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
}

Controller::~Controller()  {

}

/**
 * @brief Controller::fileOpen
 * @param fileUrl
 * @return
 *
 * NOTE: adjust qml to catch signal duplicate
 */
bool Controller::fileOpen(QUrl fileUrl)
{
    bitpowder::lib::MemoryPool mp;

    std::string filename = fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString() : fileUrl.fileName().toStdString();

    controllerLog("Opening file " + filename);

    std::tie(m_componentMap, std::ignore) = Parse(filename, mp);

    if (m_componentMap.empty()) {
        controllerLog("[Component.cpp/fileOpen(fileUrl)] File "+ filename + " is empty. ",Qt::red);
        return false;
    }

    return emitNetwork();
}

bool Controller::emitNetwork() {
    QVariantList compList;
    for(auto &it : m_componentMap) {
        XMASComponent *comp = it.second;
        if (comp) {
            // WARNING: Stefan: verdwijnt de map van de stack bij beeindigen van methode?
            // Of doet Qt iets om deze classes van destruction te behoeden?
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
            controllerLog(
             "connection created from " + out->getInitiator()->getStdName() +
             " to " + out->getTarget()->getStdName());
            list.append(map);
        } else {
            controllerLog("output port " + std::string(out->getName()) + " in comp "
                      + out->getComponent()->getStdName() + " is not connected");
        }
    }
}

void Controller::convertToQml(QVariantMap &map, XMASComponent *comp) {
    std::string name = comp->getStdName();
    controllerLog("name = "+ name + " slot for creation called", Qt::green);

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
    //map.insert("fx", "");

}

void Controller::emitDuplicate(XMASComponent *comp) {
    std::string name = comp->getStdName();
    std::string msg = "name = "+ name + " was a duplicate";
    controllerLog(msg, Qt::red);
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
        controllerLog(QString("Created component name = \"")+name+QString("\""),Qt::red);
        return true;
    }
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

void Controller::controllerLog(const bitpowder::lib::String message) {
    controllerLog(message, Qt::black);
}

void Controller::controllerLog(const std::string message){
    controllerLog(message, Qt::black);
}

/**
 * @brief Controller::output
 * @param message
 * @param color
 */
void Controller::controllerLog(const std::string message, QColor color){
    emit controllerLog(QString::fromUtf8(message.c_str()),color);
}

void Controller::controllerLog(const bitpowder::lib::String message, QColor color) {
    emit controllerLog(QString::fromUtf8(message.stl().c_str()),color);
}

/**
 * @brief Controller::output
 * @param message
 * @param color
 */
void Controller::controllerLog(const QString message, QColor color){
    emit log(message,color);
}
