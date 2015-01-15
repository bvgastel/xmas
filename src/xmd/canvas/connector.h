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

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QQuickItem>

#include "component.h"
#include "connection.h"

class Component;
class Connection;

/**
 * @brief The Connector class
 */
class Connector : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(ConnectorType)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(ConnectorType connectorType READ connectorType CONSTANT)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged )

public:

    enum ConnectorType { Input=1, Output=2 };
    ConnectorType connectorType() const {return m_type;}

    Connector(QQuickItem * parent=0);
    virtual ~Connector();

    QString name() const;
    void setName(const QString &name);

    bool connected(){ return m_connected; }

    void setConnection(Connection *connection){m_connection = connection;}
    void deleteConnection();
    const Component *component() const {return this->m_parent;}

signals:
    void connectedChanged();

private:
    QString m_name;
    const ConnectorType m_type;
    const Component *m_parent;
    Connection *m_connection;
    bool m_connected;
    static const int m_size = 5;
};

#endif // CONNECTOR_H
