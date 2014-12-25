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

#include <QGraphicsItem>
#include "component.h"
#include "connection.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Component;
class Connection;

/**
 * A component connector
 */
class Connector : public QGraphicsItem
{
public:
    enum { Type = UserType + 6 };
    int type() const { return Type;}
    enum ConnectorType { Input=1, Output=2 };
    ConnectorType connectorType() const {
        return m_connectorType;
    }
    void setConnectorType(ConnectorType p) {
        m_connectorType = p;
    }
    Connector(Component *self, QGraphicsScene *scene, QWidget* widget,
         ConnectorType  ct = Input, const int radius = 7);

    Component *getComponent() const {
        return m_self;
    }

    virtual ~Connector();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    int m_radius;
    QWidget m_widget;
    ConnectorType m_connectorType;
    bool m_isConnected;
    const char m_name;
    Connection m_connection;
    Component *m_self;
    QColor m_darkColor;
    bool m_highlight;


};

#endif // CONNECTOR_H
