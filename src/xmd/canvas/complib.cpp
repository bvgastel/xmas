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
 * @param contextMenu
 * @param parent
 * @param scene
 */
CompLib::CompLib()
{
    qmlRegisterType<Component>("XMAS", 1, 0, "XComponent");
    qmlRegisterType<Connector>("XMAS", 1, 0, "XConnector");
    qmlRegisterType<Connection>("XMAS", 1, 0, "XConnection");

    QDeclarativeEngine engine;

    //(todo) test menu : must be specefic for each component type
//    QMenu *qm = new QMenu("test");
//    qm->addMenu("help");
//    qm->addMenu("delete");
//    qm->addMenu("edit");

    QDeclarativeComponent componentQueue(&engine, QUrl::fromLocalFile("../xmd/qml/queue.qml"));
    m_queue = qobject_cast<Component*>(componentQueue.create());

    QDeclarativeComponent componentFunction(&engine, QUrl::fromLocalFile("../xmd/qml/function.qml"));
    m_function = qobject_cast<Component*>(componentFunction.create());

    QDeclarativeComponent componentFork(&engine, QUrl::fromLocalFile("../xmd/qml/fork.qml"));
    m_fork = qobject_cast<Component*>(componentFork.create());

    QDeclarativeComponent componentJoin(&engine, QUrl::fromLocalFile("../xmd/qml/join.qml"));
    m_join = qobject_cast<Component*>(componentJoin.create());

    QDeclarativeComponent componentSwitch(&engine, QUrl::fromLocalFile("../xmd/qml/switch.qml"));
    m_switch = qobject_cast<Component*>(componentSwitch.create());

    QDeclarativeComponent componentMerge(&engine, QUrl::fromLocalFile("../xmd/qml/merge.qml"));
    m_merge = qobject_cast<Component*>(componentMerge.create());

    QDeclarativeComponent componentSink(&engine, QUrl::fromLocalFile("../xmd/qml/sink.qml"));
    m_sink = qobject_cast<Component*>(componentSink.create());

    QDeclarativeComponent componentSource(&engine, QUrl::fromLocalFile("../xmd/qml/source.qml"));
    m_source = qobject_cast<Component*>(componentSource.create());

    QDeclarativeComponent componentIn(&engine, QUrl::fromLocalFile("../xmd/qml/in.qml"));
    m_in = qobject_cast<Component*>(componentIn.create());

    QDeclarativeComponent componentOut(&engine, QUrl::fromLocalFile("../xmd/qml/out.qml"));
    m_out = qobject_cast<Component*>(componentOut.create());

    QDeclarativeComponent componentComposite(&engine, QUrl::fromLocalFile("../xmd/qml/spidergon.qml"));
    m_composite = qobject_cast<Component*>(componentComposite.create());

}

/**
 * @brief CompLib::getComponent
 * @param type
 * @return
 */
Component *CompLib::getComponent(int type)
{
    switch (type)
    {
    case CompLib::Queue :
        return m_queue;
    case CompLib::Function :
        return m_function;
    case CompLib::Fork :
        return m_fork;
    case CompLib::Join :
        return m_join;
    case CompLib::Switch :
        return m_switch;
    case CompLib::Merge :
        return m_merge;
    case CompLib::Source :
        return m_source;
    case CompLib::Sink :
        return m_sink;
    case CompLib::In :
        return m_in;
    case CompLib::Out :
        return m_out;
    case CompLib::Composite :
        return m_composite;
    }
    return nullptr;
}
