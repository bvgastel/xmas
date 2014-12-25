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

Connector::Connector(Component *parent1, QGraphicsScene *scene1, QWidget* widget, Connector::ConnectorType conType1,  int radius)
        : QGraphicsItem(parent1, scene1),
     m_connectorType(conType1), m_self(parent1), m_widget(widget), m_radius(radius)
{

    setCacheMode(DeviceCoordinateCache);
    setZValue(2);
    updatePosition();

    if (conType1 == ConnectorType::Input) {
        m_darkColor =  QColor(Qt::darkYellow);
    }
    else if (conType1 == ConnectorType::Output) {
        m_darkColor =  QColor(Qt::darkRed).light(80);
    }
    else {
        m_darkColor =  QColor(Qt::darkGreen);
    }
    m_highlight = false;
}


void Connector::updatePosition() {
    this->prepareGeometryChange();

    if (m_self == NULL || m_widget == NULL) {
        return;
    }

    QPointF pPos = m_self->pos();
    QSize widgetSize = m_widget->size();

    QPointF newPos;
    if (connectorAlignment() == Connector::Left) {
        newPos.setX(-m_radius);
        newPos.setY(parent->subWidgetRect(m_widget).y() + m_self->subWidgetRect(m_widget).height()/2.0);
    }
    else if (connectorAlignment() == Connector::Right) {
        newPos.setX(m_self->rect().width() + m_radius);
        newPos.setY(m_self->subWidgetRect(m_widget).y()+ m_self->subWidgetRect(m_widget).height()/2.0);
    }
    else if (connectorAlignment() == Connector::Bottom) {
        newPos.setX(m_self->subWidgetRect(m_widget).x() + m_self->subWidgetRect(m_widget).width()/2.0);
        newPos.setY(m_self->rect().height() + m_radius);
    }
    else if (connectorAlignment() == Connector::Top) {
        newPos.setX(m_self->subWidgetRect(m_widget).x() + m_self->subWidgetRect(m_widget).width()/2.0);
        newPos.setY(-m_radius);
    }

    this->setPos(newPos);
    this->m_connection.updatePosition();
    update();
}

Connector::~Connector()
{
    this->deleteConnections();
    if (scene() != NULL) {
        this->scene()->removeItem(this);
    }
}

