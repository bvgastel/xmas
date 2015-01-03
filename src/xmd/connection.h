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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QGraphicsPathItem>
#include <QPen>
#include <QPainter>

#include "connector.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsPathItem;
class QGraphicsScene;
class QRectF;
class QPen;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Connector;

/**
 * @brief The Connection class
 */
class Connection : public QGraphicsPathItem
{
public:
    enum { Type = UserType + 2 };

    Connection(Connector *startConnector,
               Connector *endConnector,
               QGraphicsItem *parent = 0);

    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;

    Connector *startConnector() const
    {
        return m_startConnector;
    }
    Connector *endConnector() const
    {
        return m_endConnector;
    }

    void updatePosition();

    virtual ~Connection();

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) Q_DECL_OVERRIDE;

    void recreatePath(QPointF& controlPoint1,
                      QPointF& controlPoint2);

private:
    Connector *m_startConnector;
    Connector *m_endConnector;

};

#endif // CONNECTION_H
