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

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QMetaObject>
#include <QQmlEngine>
#include <QtQml>

#include "controller.h"
#include "simplestring.h"
#include "memorypool.h"
#include "parse.h"
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
//    bitpowder::lib::MemoryPool mp1;

    std::string filename = fileUrl.toLocalFile().toStdString();
    if (filename == "") {
        filename = "../../testfiles/2_queues.fjson";
        controllerLog("Ivalid filename!! Using default input file: " + filename, Qt::red);
    }
    controllerLog("Opening file " + filename);

    decltype(Parse(filename, mp).first) componentMap;
    std::tie(componentMap, std::ignore) = Parse(filename, mp);

    if (componentMap.empty()) {
        controllerLog("[Component.cpp/fileOpen(fileUrl)] File "+ filename + " is empty. ",Qt::red);
        return false;
    }

    for(auto &it : componentMap) {
        if (it.second) {
            bool inserted;
            std::tie(std::ignore, inserted) = m_componentMap.insert({it.second->getStdName(), it.second});
            if (inserted) {
                m_allComponents.insert(it.second);
                emitComponent(it.second);
            } else {
                emitDuplicate(it.second);
                continue;       // don't stop at a duplicate
            }
        }
    }
    return true;
}

void Controller::emitComponent(XMASComponent *comp) {
    std::string name = comp->getStdName();
    controllerLog("name = "+ name + " slot for creation called", Qt::green);

    std::type_index typeIndex = std::type_index(typeid(*comp));
    QString type = m_type_map[typeIndex];
    QObject *object = new QObject();
    object->setProperty("type", type);
    QString qname = QString::fromStdString(name);
    object->setProperty("name", qname);

    emit createComponent(type, object);
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
bool Controller::componentCreated(QVariant qvariant)
{
    qDebug() << "Component created by designer";
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString type = QQmlProperty::read(qobject, "type").toString();
    QString name = QQmlProperty::read(qobject, "name").toString();
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
bool Controller::componentDestroyed(QVariant qvariant)
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
bool Controller::componentChanged(QVariant qvariant)
{
    Q_UNUSED(qvariant)
    qDebug() << "Component changed by designer";
    return true;
}


bool Controller::channelCreated(QVariant qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Channel " + name + " created by designer";
    return true;
}

bool Controller::channelDestroyed(QVariant qvariant)
{
    QObject *qobject = qvariant_cast<QObject *>(qvariant);
    QString name = QQmlProperty::read(qobject, "name").toString();
    qDebug() << "Channel " + name + " destroyed by designer";
    return true;
}

bool Controller::channelChanged(QVariant qvariant)
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
