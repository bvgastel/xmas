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
//#include <QColor>

#include "controller.h"
#include "simplestring.h"
#include "memorypool.h"
#include "parse.h"
//#include "common.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
}

Controller::~Controller() {

}

bool Controller::networkFromJson(QString filename)
{
    bitpowder::lib::MemoryPool mp;

    filename = QString("../../testfiles/2_queues.fjson");
    output("Opening file "+filename.toStdString(), Qt::black);
    auto parse  = Parse(filename.toStdString(), mp);
    auto componentMap = parse.first;
    if (componentMap.empty()) {
        output("Opening file "+filename + " is empty.", Qt::black);
    }
    for(auto compMapEntry : componentMap) {
        emitComponent(compMapEntry.second);

//        std::string name = compMapEntry.first.stl();
//        output("name = "+ name, Qt::black);
//        if (typeid(component) == typeid(XMASSink)) {
//            QObject *object = new Object();
//            XMASSink *comp = compMapEntry.second;
//            std::string name = comp->getName();
//            object->setProperty("name", name);
//            emit createComponent(CompType::Sink, QVariant(object));
//        }
//        if (typeid(component) == typeid(XMASQueue)) {
//            QObject *object = new Object();
//            XMASQueue *queue = compMapEntry.second;
//            std::string name = queue.getName();
//            int qlen = queue.c;
//            object->setProperty("name", name);
//            object->setProperty("size", qlen);
//            emit createComponent(CompType::Queue, QVariant(object));
//        }
    }

    return true;
}

std::pair<Controller::CompType, std::shared_ptr<std::list>> convertComp2Object(XMASComponent *comp) {
    std::shared_ptr<std::list> paramList;
    std::pair<std::string, std::string> param;
    std::pair<Controller::CompType, std::shared_ptr<std::list>> result;
    param = std::make_pair("name", comp->getName);
    paramList->push_back(param);
    param = std::make_pair("x", "100");
    paramList->push_back(param);
    param = std::make_pair("y", "100");
    if (typeid(comp) == typeid(XMASSource)) {
        result = std::make_pair(Controller::CompType::Source, paramList);
        return result;
    } else if (typeid(*comp) == typeid(XMASSink)) {
        result = std::make_pair(Controller::CompType::Sink, paramList);
        return result;
    } else if (typeid(*comp) == typeid(XMASFunction)) {
        result = std::make_pair(Controller::CompType::Function, paramList);
        return result;
    } else if (typeid(*comp) == typeid(XMASQueue)) {
        XMASQueue *q = std::dynamic_pointer_cast<XMASQueue>(comp);
        paramList->push_back("size", q->c);
        result = std::make_pair(Controller::CompType::Queue, paramList);
        return result;
    } else if (typeid(*comp) == typeid(XMASJoin)) {
        result = std::make_pair(Controller::CompType::Join, paramList);
        return result;
    } else if (typeif(*comp) == typeid(XMASMerge)) {
        result = std::make_pair(Controller::CompType::Merge, paramList);
        return result;
    } else if (typeif(*comp) == typeid(XMASSwitch)) {
        result = std::make_pair(Controller::CompType::Switch, paramList);
        return result;
    } else if (typeif(*comp) == typeid(XMASFork)) {
        result = std::make_pair(Controller::CompType::Fork, paramList);
        return result;
    }
    throw new Exception("Unknown type of XMASComponent");

}

void Controller::emitComponent(XMASComponent *comp) {
    output("name = "+ comp->getName(), Qt::black);
    std::pair<CompType, std::shared_ptr<std::list>> result = convertComp2Object(comp);
    CompType type = result.first;
    std::shared_ptr<std::list> paramList = result.second;
    QObject object;
    object.setProperty("type", type);
    for (auto prop : paramList) {
        object.setProperty(prop.first, prop.second);
    }
    emit createComponent(type, QVariant(object));
}



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
    output("Hello from Controller to qml",Qt::black);
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

/**
 * @brief Controller::output
 * @param message
 * @param color
 */
void Controller::output(const std::string message, QColor color){
    emit log(QString::fromUtf8(message.c_str()),color);
}

/**
 * @brief Controller::output
 * @param message
 * @param color
 */
void Controller::output(const QString message, QColor color){
    emit log(message,color);
}
