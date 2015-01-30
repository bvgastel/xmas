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

#include "controller.h"
#include "common.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("controller", this);
    engine.load(QUrl(QStringLiteral("qrc:///mainWindow.qml")));


}

Controller::~Controller() {
    delete m_window;

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
    QQmlComponent component(&engine, QUrl("qrc:/fork.qml"));
    QObject *fork = component.create();
    bitpowder::lib::unused(fork);
    return true;
}

/**
 * @brief Controller::componentCreated
 * @param comp
 * @return
 */
bool Controller::componentCreated(QVariant object)
{
    qDebug() << "Component created by designer";
    qDebug() << "object = " << object << " typename = " << object.typeName() << "data " << object.data();
    qDebug() << "conversion possible? "
             << (object.canConvert<int>()? "int;": "not to int;")
             << (object.canConvert<QString>() ? "QString;" : "not to QString;")
//             << (object.canConvert<XComponent>() ? "XComponent;" : "not to XComponent;") // How to do?
             ;
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


//TODO : testClicked is a tempory test method and must be removed once done (stefan)
bool Controller::testClicked()
{
    QVariant qv;
    qDebug() << "test clicked";
    emit componentCreate(qv);
    return true;
}




