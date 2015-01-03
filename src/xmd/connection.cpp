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
                       QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
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
 * @brief Connection::boundingRect
 * @return
 */
QRectF Connection::boundingRect() const
{
    qreal extra = 3;
    return QGraphicsPathItem::boundingRect().normalized().adjusted(-extra, -extra, extra, extra);
}

/**
 * @brief Connection::shape
 * @return
 */
QPainterPath Connection::shape() const
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
        combinedSize.setWidth(5 * leftConn->boundingRect().width());
        combinedSize.setHeight(5 * leftConn->boundingRect().height());
    }
    if (rightConn->parentItem() != nullptr) {
        combinedSize.setWidth(combinedSize.width() + rightConn->parentItem()->boundingRect().width());
        combinedSize.setHeight(combinedSize.height() + rightConn->parentItem()->boundingRect().height());
    }
    else {
        combinedSize.setWidth(combinedSize.width() + 5 * rightConn->boundingRect().width());
        combinedSize.setHeight(combinedSize.height() + 5 * rightConn->boundingRect().height());
    }



    if (leftConn->getType() == rightConn->getType()
            && leftConn->getType() == Connector::Input
            && right.x() - left.x() < combinedSize.width()/2.0) {
        controlPoint1.setX(controlPoint1.x() - moveY/2.0);
        controlPoint2.setX(controlPoint2.x() - moveY/2.0);
    }
    else if (leftConn->getType() == rightConn->getType()
             && leftConn->getType() == Connector::Output
             && right.x() - left.x() < combinedSize.width()/2.0) {
        controlPoint1.setX(controlPoint1.x() + moveY/2.0);
        controlPoint2.setX(controlPoint2.x() + moveY/2.0);
    }
    else if (leftConn->getType() != Connector::Input
             && rightConn->getType() != Connector::Output) {
        controlPoint1 = a;
        controlPoint2 = b;
        qreal moveX = 0.45 * diffx;
        qreal moveY = 0.45 * diffy;
        if (dist > 5 * (m_startConnector->boundingRect().width())) {

            if (moveX < 3 * (m_startConnector->boundingRect().width())) {
                moveX = 3 * (m_startConnector->boundingRect().width());
            }
            if (moveY < 3 * (m_startConnector->boundingRect().height())) {
                moveY = 3 * (m_startConnector->boundingRect().height());
            }
        }

        if (m_startConnector->getType() == Connector::Input) {
            controlPoint1.setX(controlPoint1.x() - moveX);
        }
        else if (m_startConnector->getType() == Connector::Output) {
            controlPoint1.setX(controlPoint1.x() + moveX);
        }

        if (m_endConnector->getType() == Connector::Input) {
            controlPoint2.setX(controlPoint2.x() - moveX);
        }
        else if (m_endConnector->getType() == Connector::Output) {
            controlPoint2.setX(controlPoint2.x() + moveX);
        }
    }
     else {
        controlPoint1 = a;
        controlPoint2 = b;
        qreal maxMove = 0.5 * dist;
        moveX = 0.5 * dist;
        moveY = 0.5 * dist;
        if (m_startConnector->component() != nullptr) {
            maxMove = 1 * (m_startConnector->component()->boundingRect().width() + m_startConnector->component()->boundingRect().height());
        }
        else if (m_endConnector->component() != nullptr) {
            maxMove = 1 * (m_endConnector->component()->boundingRect().width() + m_endConnector->component()->boundingRect().height());
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
        if (m_startConnector->component() != nullptr && m_endConnector->component() != nullptr) {
            maxMove = 1 * (m_endConnector->component()->boundingRect().width() + m_endConnector->component()->boundingRect().height());
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
        this->m_startConnector->deleteConnection();
        this->m_startConnector = nullptr;
    }
    if (this->m_endConnector != nullptr) {
        this->m_endConnector->deleteConnection();
        this->m_endConnector = nullptr;
    }

//    if (scene() != nullptr) {
//        this->scene()->removeItem(this);
//    }
}

void Connection::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *w)
{
    Q_UNUSED(option);
    Q_UNUSED(w);

    painter->setRenderHint(QPainter::Antialiasing);

    if (m_startConnector == nullptr
            || m_endConnector == nullptr
            || m_startConnector->collidesWithItem(m_endConnector))
        return;


    QPointF controlPoint1;
    QPointF controlPoint2;
    recreatePath(controlPoint1, controlPoint2);

    QPen mPen = pen();
    mPen.setColor(Qt::black);
    painter->setPen(mPen);
    painter->setBrush(Qt::NoBrush);

    if (isSelected())
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));

    QPainterPath& p = this->path();
    painter->drawPath(p);


}

