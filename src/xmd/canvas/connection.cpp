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



Connection::Connection(Connector *startConnector, Connector *endConnector,
                       QGraphicsItem *parent, QGraphicsScene *scene)
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

QRectF NodeConnection::boundingRect() const
{
    qreal extra = 3;
    return QGraphicsPathItem::boundingRect().normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath NodeConnection::shape() const
{
    QPainterPath path = QGraphicsPathItem::shape();
    return path;
}

void Connection::updatePosition() {
    prepareGeometryChange();
    update();
}

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

    QPointF bottom;
    QPointF top;
    Connector* bottomConn;
    Connector* topConn;
    if (a.y() < b.y()) {
        top = a;
        topConn = m_startConnector;
        bottom = b;
        bottomConn = m_endConnector;
    }
    else {
        top = b;
        topConn = m_endConnector;
        bottom = a;
        bottomConn = m_startConnector;
    }


    controlPoint1 = a;
    controlPoint2 = b;
    //how much to move control point from start or end point, default as used for simple case
    qreal moveX = 0.45 * diffx;
    qreal moveY = 0.45 * diffy;

    QSizeF combinedSize(30,30);
    if (leftConn->parentItem() != NULL) {
        combinedSize.setWidth(leftConn->parentItem()->boundingRect().width());
        combinedSize.setHeight(leftConn->parentItem()->boundingRect().height());
    }
    else {
        combinedSize.setWidth(5 * leftConn->mRadius);
        combinedSize.setWidth(5 * leftConn->mRadius);
    }
    if (rightConn->parentItem() != NULL) {
        combinedSize.setWidth(combinedSize.width() + rightConn->parentItem()->boundingRect().width());
        combinedSize.setHeight(combinedSize.height() + rightConn->parentItem()->boundingRect().height());
    }
    else {
        combinedSize.setWidth(combinedSize.width() + 5 * rightConn->mRadius);
        combinedSize.setHeight(combinedSize.height() + 5 * rightConn->mRadius);
    }



    if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == Connector::Left && right.x() - left.x() < combinedSize.width()/2.0) {
        controlPoint1.setX(controlPoint1.x() - moveY/2.0 /*- combinedSize.width()/2.0*/);
        controlPoint2.setX(controlPoint2.x() - moveY/2.0 /*- combinedSize.width()/2.0*/);
    }
    else if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == Connector::Right && right.x() - left.x() < combinedSize.width()/2.0) {
        controlPoint1.setX(controlPoint1.x() + moveY/2.0 /*+ combinedSize.width()/2.0*/);
        controlPoint2.setX(controlPoint2.x() + moveY/2.0 /*+ combinedSize.width()/2.0*/);
    }
    else if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == Connector::Top && bottom.y() - top.y() < combinedSize.height()/2.0) {
        controlPoint1.setY(controlPoint1.y() - moveX/2.0 /*- combinedSize.height()/2.0*/);
        controlPoint2.setY(controlPoint2.y() - moveX/2.0 /*- combinedSize.height()/2.0*/);
    }
    else if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == Connector::Bottom && bottom.y() - top.y() < combinedSize.height()/2.0) {
        controlPoint1.setY(controlPoint1.y() + moveX/2.0 /*+ combinedSize.height()/2.0*/);
        controlPoint2.setY(controlPoint2.y() + moveX/2.0 /*+ combinedSize.height()/2.0*/);
    }
    else
    //the simple case, they face each other the "good" way
    if (leftConn->connectorAlignment() != Connector::Left && rightConn->connectorAlignment() != Connector::Right
            && topConn->connectorAlignment() != Connector::Top && bottomConn->connectorAlignment() != Connector::Bottom) {

        //very simple: straight line
        //controlPoint1 = a + 0.3 * (b-a);
        //controlPoint2 = a + 0.7 * (b-a);

        controlPoint1 = a;
        controlPoint2 = b;
        //how much to move control point from start or end point
        qreal moveX = 0.45 * diffx;
        qreal moveY = 0.45 * diffy;
        if (dist > 5 * (m_startConnector->mRadius)) {

            if (moveX < 3 * (m_startConnector->mRadius)) {
                moveX = 3 * (m_startConnector->mRadius);
            }
            if (moveY < 3 * (m_startConnector->mRadius)) {
                moveY = 3 * (m_startConnector->mRadius);
            }
        }

        if (m_startConnector->connectorAlignment() == Connector::Left) {
            controlPoint1.setX(controlPoint1.x() - moveX);
        }
        else if (m_startConnector->connectorAlignment() == Connector::Right) {
            controlPoint1.setX(controlPoint1.x() + moveX);
        }
        else if (m_startConnector->connectorAlignment() == Connector::Bottom) {
            controlPoint1.setY(controlPoint1.y() + moveY);
        }
        else if (m_startConnector->connectorAlignment() == Connector::Top) {
            controlPoint1.setY(controlPoint1.y() - moveY);
        }

        if (m_endConnector->connectorAlignment() == Connector::Left) {
            controlPoint2.setX(controlPoint2.x() - moveX);
        }
        else if (m_endConnector->connectorAlignment() == Connector::Right) {
            controlPoint2.setX(controlPoint2.x() + moveX);
        }
        else if (m_endConnector->connectorAlignment() == Connector::Bottom) {
            controlPoint2.setY(controlPoint2.y() + moveY);
        }
        else if (m_endConnector->connectorAlignment() == Connector::Top) {
            controlPoint2.setY(controlPoint2.y() - moveY);
        }
    }
    //the bad case, method needs cleanup
    else {
        controlPoint1 = a;
        controlPoint2 = b;
        qreal maxMove = 0.5 * dist;
        moveX = 0.5 * dist;
        moveY = 0.5 * dist;
        if (m_startConnector->parent != NULL) {
            maxMove = 1 * (m_startConnector->parent->rect().width() + m_startConnector->parent->rect().height());
        }
        else if (m_endConnector->parent != NULL) {
            maxMove = 1 * (m_endConnector->parent->rect().width() + m_endConnector->parent->rect().height());
        }
        if (moveX > maxMove) {
            moveX = maxMove + 0.1 * (moveX-maxMove);
        }
        if (moveY > maxMove) {
            moveY = maxMove + 0.1 * (moveY-maxMove);
        }
        if (m_startConnector->connectorAlignment() == Connector::Left) {
            moveX *= -1;
            if ((m_startConnector == topConn) == (m_startConnector == rightConn)) {
                moveY *= -1;//relevantHeight;
            }
        }
        else if (m_startConnector->connectorAlignment() == Connector::Right) {
            //moveX *= 1;
            if ((m_startConnector == topConn) == (m_startConnector == leftConn)) {
                moveY *= -1;//relevantHeight;
            }
        }
        else if (m_startConnector->connectorAlignment() == Connector::Bottom) {
            //moveY *= 1;
            if ((m_startConnector == leftConn) == (m_startConnector == topConn)) {
                moveX *= -1;
            }
        }
        else if (m_startConnector->connectorAlignment() == Connector::Top) {
            moveY *= -1;
            if ((m_startConnector == leftConn) == (m_startConnector == bottomConn)) {
                moveX *= -1;
            }
        }


        //ugly shit: handle some cases that don't look nice
        if (m_endConnector == topConn && topConn->connectorAlignment() == Connector::Top && (bottomConn->connectorAlignment() == Connector::Left || bottomConn->connectorAlignment() == Connector::Right)) {
            moveY *= -1;
            //moveY = 0;
        }
        else if (m_endConnector == bottomConn && bottomConn->connectorAlignment() == Connector::Bottom && (topConn->connectorAlignment() == Connector::Left || topConn->connectorAlignment() == Connector::Right)) {
            moveY *= -1;
            //moveY = 0;
        }
        else if (m_endConnector == leftConn && leftConn->connectorAlignment() == Connector::Left && (rightConn->connectorAlignment() == Connector::Top || rightConn->connectorAlignment() == Connector::Bottom)) {
            moveX *= -1;
            //moveX = 0;
        }
        else if (m_endConnector == rightConn && rightConn->connectorAlignment() == Connector::Right && (leftConn->connectorAlignment() == Connector::Top || leftConn->connectorAlignment() == Connector::Bottom)) {
            moveX *= -1;
            //moveX = 0;
        }

        controlPoint1.setX(controlPoint1.x() + moveX);
        controlPoint1.setY(controlPoint1.y() + moveY);


        moveX = 0.5 * dist;
        moveY = 0.5 * dist;
        // if start was null, then it was already set to end.
        if (m_startConnector->parent != NULL && m_endConnector->parent != NULL) {
            maxMove = 1 * (m_endConnector->parent->rect().width() + m_endConnector->parent->rect().height());
        }
        if (moveX > maxMove) {
            moveX = maxMove + 0.1 * (moveX-maxMove);
        }
        if (moveY > maxMove) {
            moveY = maxMove + 0.1 * (moveY-maxMove);
        }
        if (m_endConnector->connectorAlignment() == Connector::Left) {
            moveX *= -1;
            if ((m_endConnector == topConn) == (m_endConnector == rightConn)) {
                moveY *= -1;//relevantHeight;
            }
        }
        else if (m_endConnector->connectorAlignment() == Connector::Right) {
            //moveX *= 1;
            if ((m_endConnector == topConn) == (m_endConnector == leftConn)) {
                moveY *= -1;//relevantHeight;
            }
        }
        else if (m_endConnector->connectorAlignment() == Connector::Bottom) {
            //moveY *= 1;
            if ((m_startConnector == leftConn) == (m_startConnector == topConn)) {
                moveX *= -1;
            }
        }
        else if (m_endConnector->connectorAlignment() == Connector::Top) {
            moveY *= -1;
            if ((m_startConnector == leftConn) == (m_startConnector == bottomConn)) {
                moveX *= -1;
            }
        }

        /*
        if (mStartConnector->connectorAlignment() == mEndConnector->connectorAlignment()) {
            moveX *= 2;
            moveY *= 2;
        }*/


        //ugly shit: handle some cases that don't look nice
        if (m_startConnector == topConn && topConn->connectorAlignment() == Connector::Top && (bottomConn->connectorAlignment() == Connector::Left || bottomConn->connectorAlignment() == Connector::Right)) {
            moveY *= -1;
            //moveY = 0;
        }
        else if (m_startConnector == bottomConn && bottomConn->connectorAlignment() == Connector::Bottom && (topConn->connectorAlignment() == Connector::Left || topConn->connectorAlignment() == Connector::Right)) {
            moveY *= -1;
            //moveY = 0;
        }
        else if (m_startConnector == leftConn && leftConn->connectorAlignment() == Connector::Left && (rightConn->connectorAlignment() == Connector::Top || rightConn->connectorAlignment() == Connector::Bottom)) {
            moveX *= -1;
            //moveX = 0;
        }
        else if (m_startConnector == rightConn && rightConn->connectorAlignment() == Connector::Right && (leftConn->connectorAlignment() == Connector::Top || leftConn->connectorAlignment() == Connector::Bottom)) {
            moveX *= -1;
            //moveX = 0;
        }

        controlPoint2.setX(controlPoint2.x() + moveX);
        controlPoint2.setY(controlPoint2.y() + moveY);
    }


    QPainterPath p(a);
    p.cubicTo(controlPoint1, controlPoint2, b);
    this->setPath(p);
}




Connection::~Connection(){
    if (this->m_startConnector != NULL) {
        this->m_startConnector->removeConnection(this);
        this->m_startConnector = NULL;
    }
    if (this->m_endConnector != NULL) {
        this->m_endConnector->removeConnection(this);
        this->m_endConnector = NULL;
    }

    if (scene() != NULL) {
        this->scene()->removeItem(this);
    }
}

