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

#include "network.h"

/**
 * @brief Network::Network
 */
Network::Network(QQuickItem * parent): QQuickItem(parent)
{
    //setFlag(QQuickItem::ItemHasNoContents,false);
}

/**
 * @brief Network::~Network
 */
Network::~Network()
{

}

/**
 * @brief Network::components
 * @return
 */
QQmlListProperty<Component> Network::components()
{
    return QQmlListProperty<Component>(this, 0,
                                               &Network::append_component,
                                               0,
                                               0,
                                               0);
}

/**
 * @brief Network::append_component
 * @param list
 * @param component
 */
void Network::append_component(QQmlListProperty<Component> *list, Component *component)
{
    Network *network = qobject_cast<Network *>(list->object);
    if (network) {
        component->setParentItem(network);
        network->m_components.append(component);
    }
}

/**
 * @brief Network::connections
 * @return
 */
QQmlListProperty<Connection> Network::connections()
{
    return QQmlListProperty<Connection>(this, 0,
                                               &Network::append_connection,
                                               0,
                                               0,
                                               0);
}

/**
 * @brief Network::append_connection
 * @param list
 * @param connection
 */
void Network::append_connection(QQmlListProperty<Connection> *list, Connection *connection)
{
    Network *network = qobject_cast<Network *>(list->object);
    if (network) {
        connection->setParentItem(network);
        //todo : connections list is kept with the network object, simple and easy.
        //Remove connection properties in connector object, what's the advantage
        //We do not need any tracking of the network, right?
        network->m_connections.append(connection);
    }
}
