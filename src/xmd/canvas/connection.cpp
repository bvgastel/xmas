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

/**
 * @brief Connection::Connection
 * @param startConnector
 * @param endConnector
 * @param parent
 * @param scene
 */
Connection::Connection(Connector *startConnector,
                       Connector *endConnector,
                       QGraphicsItem *parent,
                       QGraphicsScene *scene)
    : QGraphicsLineItem(parent)
{
    m_startConnector = startConnector;
    m_endConnector = endConnector;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setZValue(-1000.0);
    QPointF controlPoint1;
    QPointF controlPoint2;
    recreatePath(controlPoint1, controlPoint2);
}

/**
 * @brief NodeConnection::boundingRect
 * @return
 */
QRectF NodeConnection::boundingRect() const
{
    qreal extra = 3;
    return QGraphicsPathItem::boundingRect().normalized().adjusted(-extra, -extra, extra, extra);
}

/**
 * @brief NodeConnection::shape
 * @return
 */
QPainterPath NodeConnection::shape() const
{
    QPainterPath path = QGraphicsPathItem::shape();
    return path;
}

/**
 * @brief Connection::updatePosition
 */
void Connection::updatePosition()
{
    prepareGeometryChange();
    update();
}

/**
 * @brief Connection::recreatePath
 * @param controlPoint1
 * @param controlPoint2
 */
void Connection::recreatePath(QPointF& controlPoint1, QPointF& controlPoint2)
{
    QPointF a(mapFromItem(m_startConnector, 0, 0));
    QPointF b(mapFromItem(m_endConnector, 0, 0));

    qreal dist = QLineF(a, b).length();
    qreal diffx = abs(a.x() - b.x());
    qreal diffy = abs(a.y() - b.y());

    QPointF left;
    QPointF right;
    Connector* leftConn;
    Connector* rightConn;
    if (a.x() < b.x()) {
        left = a;
        leftConn = m_startConnector;
        right = b;
        rightConn = m_endConnector;
    }
    else {
        left = b;
        leftConn = m_endConnector;
        right = a;
        rightConn = m_startConnector;
    }

    controlPoint1 = a;
    controlPoint2 = b;
    //how much to move control point from start or end point,
    //default as used for simple case
    qreal moveX = 0.45 * diffx;
    qreal moveY = 0.45 * diffy;

    QSizeF combinedSize(30,30);
    if (leftConn->parentItem() != nullptr) {
        combinedSize.setWidth(leftConn->parentItem()->boundingRect().width());
        combinedSize.setHeight(leftConn->parentItem()->boundingRect().height());
    }
    else {
        combinedSize.setWidth(5 * leftConn->m_radius);
        combinedSize.setWidth(5 * leftConn->m_radius);
    }
    if (rightConn->parentItem() != nullptr) {
        combinedSize.setWidth(combinedSize.width() + rightConn->parentItem()->boundingRect().width());
        combinedSize.setHeight(combinedSize.height() + rightConn->parentItem()->boundingRect().height());
    }
    else {
        combinedSize.setWidth(combinedSize.width() + 5 * rightConn->m_radius);
        combinedSize.setHeight(combinedSize.height() + 5 * rightConn->m_radius);
    }



    if (leftConn->connectorType() == rightConn->connectorType()
            && leftConn->connectorType() == Connector::Input
            && right.x() - left.x() < combinedSize.width()/2.0) {
        controlPoint1.setX(controlPoint1.x() - moveY/2.0);
        controlPoint2.setX(controlPoint2.x() - moveY/2.0);
    }
    else if (leftConn->connectorType() == rightConn->connectorType()
             && leftConn->connectorType() == Connector::Output
             && right.x() - left.x() < combinedSize.width()/2.0) {
        controlPoint1.setX(controlPoint1.x() + moveY/2.0);
        controlPoint2.setX(controlPoint2.x() + moveY/2.0);
    }
    else if (leftConn->connectorType() != Connector::Input
             && rightConn->connectorType() != Connector::Output) {
        controlPoint1 = a;
        controlPoint2 = b;
        qreal moveX = 0.45 * diffx;
        qreal moveY = 0.45 * diffy;
        if (dist > 5 * (m_startConnector->m_radius)) {

            if (moveX < 3 * (m_startConnector->m_radius)) {
                moveX = 3 * (m_startConnector->m_radius);
            }
            if (moveY < 3 * (m_startConnector->m_radius)) {
                moveY = 3 * (m_startConnector->m_radius);
            }
        }

        if (m_startConnector->connectorType() == Connector::Input) {
            controlPoint1.setX(controlPoint1.x() - moveX);
        }
        else if (m_startConnector->connectorAlignment() == Connector::Output) {
            controlPoint1.setX(controlPoint1.x() + moveX);
        }

        if (m_endConnector->connectorType() == Connector::Input) {
            controlPoint2.setX(controlPoint2.x() - moveX);
        }
        else if (m_endConnector->connectorType() == Connector::Output) {
            controlPoint2.setX(controlPoint2.x() + moveX);
        }
    }
     else {
        controlPoint1 = a;
        controlPoint2 = b;
        qreal maxMove = 0.5 * dist;
        moveX = 0.5 * dist;
        moveY = 0.5 * dist;
        if (m_startConnector->parent != nullptr) {
            maxMove = 1 * (m_startConnector->parent->rect().width() + m_startConnector->parent->rect().height());
        }
        else if (m_endConnector->parent != nullptr) {
            maxMove = 1 * (m_endConnector->parent->rect().width() + m_endConnector->parent->rect().height());
        }
        if (moveX > maxMove) {
            moveX = maxMove + 0.1 * (moveX-maxMove);
        }
        if (moveY > maxMove) {
            moveY = maxMove + 0.1 * (moveY-maxMove);
        }
        controlPoint1.setX(controlPoint1.x() + moveX);
        controlPoint1.setY(controlPoint1.y() + moveY);


        moveX = 0.5 * dist;
        moveY = 0.5 * dist;
        if (m_startConnector->parent != nullptr && m_endConnector->parent != nullptr) {
            maxMove = 1 * (m_endConnector->parent->rect().width() + m_endConnector->parent->rect().height());
        }
        if (moveX > maxMove) {
            moveX = maxMove + 0.1 * (moveX-maxMove);
        }
        if (moveY > maxMove) {
            moveY = maxMove + 0.1 * (moveY-maxMove);
        }
        controlPoint2.setX(controlPoint2.x() + moveX);
        controlPoint2.setY(controlPoint2.y() + moveY);
    }


    QPainterPath p(a);
    p.cubicTo(controlPoint1, controlPoint2, b);
    this->setPath(p);
}

/**
 * @brief Connection::~Connection
 */
Connection::~Connection(){
    if (this->m_startConnector != nullptr) {
        this->m_startConnector->removeConnection(this);
        this->m_startConnector = nullptr;
    }
    if (this->m_endConnector != nullptr) {
        this->m_endConnector->removeConnection(this);
        this->m_endConnector = nullptr;
    }

    if (scene() != nullptr) {
        this->scene()->removeItem(this);
    }
}

