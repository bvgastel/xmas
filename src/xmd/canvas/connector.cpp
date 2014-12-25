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

#include "connector.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Component;
class Connection;

/**
 * Constructor
 */
Connector::Connector(Component *parent1,
                     QGraphicsScene *scene1,
                     QWidget* widget,
                     Connector::ConnectorType conType1,
                     int radius)
        : QGraphicsItem(parent1, scene1),
          m_connectorType(conType1),
          m_parent(parent1),
          m_widget(widget),
          m_radius(radius)
{
    setCacheMode(DeviceCoordinateCache);
    setZValue(2);
    updatePosition();
    m_darkColor =  QColor(Qt::darkRed);
    m_highlight = false;
}

/**
 * Deconstructor
 */
Connector::~Connector()
{
    this->deleteConnections();
    if (scene() != NULL) {
        this->scene()->removeItem(this);
    }
}

/**
 * updatePosition
 */
void Connector::updatePosition() {
    this->prepareGeometryChange();
    if (m_parent == NULL || m_widget == NULL) {
        return;
    }

    QPointF newPos;
    if (conType1 == ConnectorType::Input) {
        newPos.setX(-m_radius);
        newPos.setY(m_parent->subWidgetRect(m_widget).y() + m_parent->subWidgetRect(m_widget).height()/2.0);
    }
    else if (conType1 == ConnectorType::Output) {
        newPos.setX(m_parent->rect().width() + m_radius);
        newPos.setY(m_parent->subWidgetRect(m_widget).y()+ m_parent->subWidgetRect(m_widget).height()/2.0);
    }

    this->setPos(newPos);
    this->m_connection.updatePosition();
    update();
}


