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
//#include "common.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{

}

Controller::~Controller() {

}

//TODO : to be implemented (empty at the moment)
bool Controller::xmv2xmd()
{

    //@Guus : ik weet niet of het makkelijk is om
    // QVariant om te zetten maar onderstaande
    // voorbeeld kan ook. (kende je reeds dacht ik)
    // alleen dat er een probleem was met de qml
    // files , deze werkt met resource qrc
    // Het kan ook zonder emit door object
    // als parent sheet te geven.
    // sheet kun je vinden in de rootObjects
    // via de main engine
    QQmlEngine engine;
    QQmlComponent component(&engine,
            QUrl(QStringLiteral("qrc:///qml/fork.qml")));
    QObject *object = component.create();
    object->setProperty("x",200); //de 200 is nu fixed maar komt normaal van het xmv argument
    object->setProperty("y",200);
    object->setProperty("name","fork000");
    qDebug() << "Object " << object->property("name").toString()
             << " of type " << object->property("type").toString()
             << " has been created." ;
    //emit componentCreate(QVariant(object));
    delete object;
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
    for (QObject *child : qobject->children()) {
        if (child->objectName() == "port") {
            QVariant vpname = child->property("name");
            QString pname = vpname.toString();
            qDebug() << " port: " << pname;
        }
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




