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

#include <QStyleOptionGraphicsItem>
#include <QCursor>

#include "connector.h"

class Component;
class Connection;

/**
 * @brief Connector::Connector
 */
Connector::Connector()
    : QDeclarativeItem(),
    m_type(Connector::Input)
{
    setCacheMode(DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemHasNoContents,false);
    setAcceptHoverEvents(true);
    m_connection = nullptr;
    setZValue(1);
}

/**
 * Deconstructor
 */
Connector::~Connector()
{
    if (scene() != nullptr)
    {
        this->scene()->removeItem(this);
    }
}

/**
 * @brief Connector::boundingRect
 * @return
 */
QRectF Connector::boundingRect() const
{
    qreal adjust = 1; //pen
    return QRectF(-m_size - adjust, -m_size - adjust,
                  2*(m_size + adjust), 2*(m_size + adjust));
}

/**
 * @brief Connector::shape
 * @return
 */
QPainterPath Connector::shape() const
{
        QPainterPath path;
    path.addEllipse(-m_size, -m_size, 2*m_size, 2*m_size);
    return path;
}

/**
 * @brief Connector::name
 * @return
 */
QString Connector::name() const
{
    return m_name;
}

/**
 * @brief Connector::setName
 * @param name
 */
void Connector::setName(const QString &name)
{
    m_name = name;
}

/**
 * @brief Connector::paint
 * @param painter
 * @param option
 * @param w
 */
void Connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* w)
{
    Q_UNUSED(w);

    if (m_connection == nullptr)
    {
        painter->setBrush(Qt::red);
    }
    else
    {
        painter->setBrush(Qt::black);
    }

    if (option->state.testFlag(QStyle::State_MouseOver))
    {
        setCursor(Qt::CrossCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
    painter->setPen(Qt::NoPen);
    painter->drawRect(-m_size,-m_size,2*m_size,2*m_size);
}

/**
 * @brief Connector::mousePressEvent
 * @param event
 */
void Connector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QDeclarativeItem::mousePressEvent(event);
}

/**
 * @brief Connector::mouseReleaseEvent
 * @param event
 */
void Connector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QDeclarativeItem::mouseReleaseEvent(event);
}

/**
 * @brief Connector::deleteConnection
 * @param connection
 */
void Connector::deleteConnection()
{
    m_connection = nullptr;
}

