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
 */
bool Controller::fileOpen(QUrl fileUrl)
{
    bitpowder::lib::MemoryPool mp;

<<<<<<< HEAD
    if (fileUrl == "") {
        fileUrl = QString("../../testfiles/2_queues.fjson");
        controllerLog("fileUrl was empty. using default input file: "+fileUrl.toStdString());
    }
    controllerLog("Opening file "+fileUrl.toStdString());
    std::string fileName = QUrl(fileUrl).toLocalFile().toStdString();
    auto parsePair  = Parse(fileName, mp);
    auto &componentMap = parsePair.first;
    auto &globals = parsePair.second;

    if (componentMap.empty()) {
        controllerLog("[Component.cpp/fileOpen(fileUrl)] File "+fileUrl + " is empty. Oops ..... ");
=======
    std::string filename = fileUrl.toLocalFile().toStdString();
    if (filename == "") {
        filename = "../../testfiles/2_queues.fjson";
        log("Ivalid filename!! Using default input file: " + filename, Qt::red);
    }
    log("Opening file " + filename);
    auto parse  = Parse(filename, mp);
    auto componentMap = parse.first;
    if (componentMap.empty()) {
        log("[Component.cpp/fileOpen(fileUrl)] File "+ filename + " is empty. Oops ..... ",Qt::red);
        return false;
>>>>>>> 2beab07c95ceca3c6277fd9d68413301cc91fe40
    }
    std::set<XMASComponent *> allComponents;
    for(auto &it : componentMap) {
        if (it.second) {
            allComponents.insert(it.second);
            emitComponent(it.second);
        }
    }
    return true;
}

void Controller::emitComponent(XMASComponent *comp) {
    std::string name = comp->getName().stl();
    controllerLog("name = "+ name, Qt::black);

    QString typeName = typeid(comp).name();
    QString type = m_type_map[typeName];
    QObject object;
    object.setProperty("type", type);
    QString qname = QString::fromStdString(name);
    object.setProperty("name", qname);
    //QVariant qvariant = QVariant(object);
    // FIXME: object vervangen door qml object
    emit createComponent(type, object);
}

//QVariant createPropObject(XMASComponent *comp) {
//    QObject object;
//    object.setProperty("name", comp->getName().stl());
//    return QVariant(object);
//}


// TODO: to be implemented toward xmv.
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
    qDebug() << name << ", " << type << "TODO: connect to xmv";
    for (QObject *child : qobject->children()) {
        if (child->objectName() == "port") {
            QVariant vpname = child->property("name");
            QString pname = vpname.toString();
            qDebug() << " port: " << pname;
        }
    }
    controllerLog(QString("Hello from Controller to qml"),Qt::red);
    return true;
}


/**
 * @brief Controller::scratch
 *
 * Scratch method to try out stuff, that we never execute, just a tryout.
 *
 * @return
 */
bool Controller::scratch() {

    // Creating a fork: what can we do with it? It's a QObject. To what can we cast it?
    // @Guus : creation is easier when done at qml site on the right context (sheet)
    //         here all we need is translating xmas component to QVariant(?) methode
    //         argument. see the testClick methode. For now this one is
    //         simply called from the view test button. But it shows how c++ can request
    //         a fork on the sheet. To send also properties I'm working on it :)

    //    QQmlComponent component(&engine, QUrl("qrc:/fork.qml"));
    //    QObject *fork = component.create();
    //    bitpowder::lib::unused(fork);
    return true;
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

void Controller::controllerLog(const QString message){
    controllerLog(message, Qt::black);
}

<<<<<<< HEAD
void Controller::controllerLog(const bitpowder::lib::String message) {
    controllerLog(message, Qt::black);
}

void Controller::controllerLog(const std::string message){
    controllerLog(message, Qt::black);
}
=======
>>>>>>> 2beab07c95ceca3c6277fd9d68413301cc91fe40

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
