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
#include "complib.h"
#include <iostream>

/**
 * @brief CompLib::CompLib
 */
CompLib::CompLib()
{
    qmlRegisterType<Component>("XMAS", 1, 0, "XComponent");
    qmlRegisterType<Connector>("XMAS", 1, 0, "XConnector");
    qmlRegisterType<Connection>("XMAS", 1, 0, "XConnection");

}

/**
 * @brief CompLib::getComponent
 * @param type
 * @return
 */
Component *CompLib::createComponent(int type)
{

    QQmlEngine *engine = new QQmlEngine;
    QQmlComponent component(engine);

    switch (type)
    {
    case CompLib::Queue :
        component.loadUrl(QUrl("qrc:/qml/queue.qml"));
        break;
    case CompLib::Function :
        component.loadUrl(QUrl("qrc:/qml/function.qml"));
        break;
    case CompLib::Fork :
        component.loadUrl(QUrl("qrc:/qml/fork.qml"));
        break;
    case CompLib::Join :
        component.loadUrl(QUrl("qrc:/qml/join.qml"));
        break;
    case CompLib::Switch :
        component.loadUrl(QUrl("qrc:/qml/switch.qml"));
        break;
    case CompLib::Merge :
        component.loadUrl(QUrl("qrc:/qml/merge.qml"));
        break;
    case CompLib::Source :
        component.loadUrl(QUrl("qrc:/qml/source.qml"));
        break;
    case CompLib::Sink :
        component.loadUrl(QUrl("qrc:/qml/sink.qml"));
        break;
    case CompLib::In :
        component.loadUrl(QUrl("qrc:/qml/in.qml"));
        break;
    case CompLib::Out :
        component.loadUrl(QUrl("qrc:/qml/out.qml"));
        break;
    case CompLib::Composite :
        component.loadUrl(QUrl("qrc:/qml/spidergon.qml"));
        break;
    }
    return qobject_cast<Component*>(component.create());
}
