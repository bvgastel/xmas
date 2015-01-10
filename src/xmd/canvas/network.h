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

#ifndef NETWORK_H
#define NETWORK_H

#include <QtDeclarative>

#include "component.h"
#include "connection.h"

class Component;
class Connection;

/**
 * @brief The Network class
 */
class Network : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QDeclarativeListProperty<Component> components READ components)
    Q_PROPERTY(QDeclarativeListProperty<Connection> connections READ connections)

public:
    Network();
    virtual ~Network();

    QString name() const { return m_name; }
    void setName(const QString &name) {m_name = name;}

    QDeclarativeListProperty<Component> components();
    QDeclarativeListProperty<Connection> connections();

private:
    static void append_component(QDeclarativeListProperty<Component> *list, Component *component);
    static void append_connection(QDeclarativeListProperty<Connection> *list, Connection *connection);
    QString m_name;
    QList<Component *> m_components;
    QList<Connection *> m_connections;

};

#endif // NETWORK_H
