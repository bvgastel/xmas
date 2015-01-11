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

#include <math.h>

#include "connection.h"
#include "connector.h"

const qreal Pi = 3.14;

/**
 * @brief Connection::Connection
 * @param startConnector
 * @param endConnector
 * @param parent
 * @param scene
 */
Connection::Connection(Connector *startConnector,
                       Connector *endConnector,
                       QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    m_startConnector = startConnector;
    m_endConnector = endConnector;
    setFlag(QDeclarativeItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemHasNoContents,false);

}

/**
 * @brief Connection::boundingRect
 * @return
 */
QRectF Connection::boundingRect() const
{
    return QDeclarativeItem::boundingRect();
}

/**
 * @brief Connection::shape
 * @return
 */
QPainterPath Connection::shape() const
{
    return QDeclarativeItem::shape();
}


/**
 * @brief Connection::~Connection
 */
Connection::~Connection()
{
}

void Connection::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *w)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(w);


}

