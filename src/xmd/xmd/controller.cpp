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

#include "controller.h"
//#include "common.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
//    QQmlContext* context = engine.rootContext();
//    context->setContextProperty("controller", this);
//    engine.load(QUrl(QStringLiteral("qrc:///mainWindow.qml")));



//    QObject *rootObject = engine.rootObjects().first();
//    QObject *sheet = rootObject->findChild<QObject*>("sheet");

//    Controller controller(sheet);
//    if (sheet)
//    {
//        qDebug() << "test" << sheet << ", rootobjects : " << engine.rootObjects().count();

//    }
//    else
//    {
//       qDebug() << "no sheet found" ;
//    }



//    QObject::connect(sheet, SIGNAL(qmlSignal(QString)),
//                         &controller, SLOT(cppSlot(QString)));





}

Controller::~Controller() {
    delete m_window;

}

//TODO : to be implemented (empty at the moment)
bool Controller::xmv2xmd()
{

    //QVariant qv;

    //info --> http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html

    //qDebug() << "test clicked";
    //emit componentCreate(qv);
    return true;
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
    QObjectList children = qobject->children();
    qDebug() << "children: " << children;
    for (QObject *child : qobject->children()) {
        qDebug() << "child->objectName == '" << child->objectName() << "'";
        qDebug() << "child->dynamicPropertyNames() == " << child->dynamicPropertyNames();
        qDebug() << "child" << child;
//        if (child->objectName() == "XConnector") {  // does not work: is empty
            QVariant vpname = child->property("name");
            QString pname = vpname.toString();
            qDebug() << " port: " << pname;
//        }
    }
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
bool Controller::componentDestroyed(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Component destroyed by designer";
    return true;
}

/**
 * @brief Controller::componentChanged
 * @param object
 * @return
 */
bool Controller::componentChanged(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Component changed by designer";
    return true;
}


bool Controller::connectionCreated(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Connection created by designer";
    return true;
}

bool Controller::connectionDestroyed(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Connection destroyed by designer";
    return true;
}

bool Controller::connectionChanged(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Connection changed by designer";
    return true;
}




