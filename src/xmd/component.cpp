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
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "component.h"
#include "connector.h"

/**
 * @brief Component::Component
 * @param contextMenu
 * @param svg
 * @param scene
 */
Component::Component(QMenu *contextMenu,
                     QGraphicsItem *parent,
                     QGraphicsScene *scene)
    : QGraphicsSvgItem(parent)
{
    setCacheMode(ItemCoordinateCache);
    m_contextMenu = contextMenu;
    m_isMoving = false;

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    if (scene != nullptr)
    {
        scene->addItem(this);
    }
}

/**
 * @brief Component::~Component
 */
Component::~Component()
{

}

/**
 * @brief Component::boundingRect
 * @return
 */
QRectF Component::boundingRect() const
{
    QRect rect = bodyRect();
    rect.adjust(-10,-10,20,20);
    return rect;
}


/**
 * @brief Component::shape
 * @return
 */
QPainterPath Component::shape() const
{
    QPainterPath path;
    QRect rect = bodyRect();
    rect.adjust(-10,-10,20,20);
    path.addRect(rect);
    return path;
}


/**
 * @brief Component::addConnector
 * @param connector
 */
void Component::addConnector(int type)
{
    Connector *c = new Connector((Component*)this,static_cast<Connector::ConnectorType>(type));
    connectors.append(c);
    updateConnectorPositions();
}

/**
 * @brief Component::deleteConnections
 */
void Component::deleteConnections()
{
    foreach (Connector *c, connectors)
    {
         c->deleteConnection();
    }
}

/**
 * @brief Component::contextMenuEvent
 * @param event
 */
void Component::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   scene()->clearSelection();
   setSelected(true);
   if (m_contextMenu != nullptr)
   {
        m_contextMenu->exec(event->screenPos());
   }
}

/**
 * @brief Component::updateConnectorsPos
 */
void Component::updateConnectorsPos()
{

}

/**
 * @brief Component::paint
 * @param painter
 * @param option
 * @param w
 */
void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    //Q_UNUSED(painter);
    //Q_UNUSED(option);
    Q_UNUSED(w);
    if (option->state & QStyle::State_Selected)
    {
//        painter->setPen(QPen(Qt::black,0,Qt::DashLine));
//        QRectF rect = bodyRect();
//        rect.adjust(-10,-10,10,10);
//        painter->drawRect(rect);
    }
    if (this->parentItem() == nullptr)
    {
        painter->setBrush(Qt::white);
        painter->setPen(QPen(Qt::black,4));
        painter->drawRoundRect(bodyRect(),10,10);
    }
}

/**
 * @brief Component::mousePressEvent
 * @param mouseEvent
 */
void Component::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

/**
 * @brief Component::mouseMoveEvent
 * @param mouseEvent
 */
void Component::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem::mouseMoveEvent(mouseEvent);
}

/**
 * @brief Component::mouseReleaseEvent
 * @param mouseEvent
 */
void Component::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_isMoving)
    {
        m_isMoving = false;
    }
    QGraphicsItem::mouseReleaseEvent(mouseEvent);
    scene()->clearSelection();
    setSelected(true);
}

/**
 * @brief Component::count
 * @param type
 * @return
 */
int Component::count(int type) const
{
    int x = 0;
    foreach (Connector *c, connectors)
    {
       c->getType() == type ? ++x : x;
    }
    return x;
}

/**
 * @brief Component::body
 * @return
 */
QRect Component::bodyRect() const
{
    int x = qMax(count(Connector::Input),count(Connector::Output));
    (x > 0) ? --x : x;
    int h = 80 + x*60;
    return QRect(-90,-h/2,180,h);
}

/**
 * @brief Component::deleted
 */
void Component::deleted()
{

}

/**
 * @brief Component::updateConnectorPositions
 */
void Component::updateConnectorPositions()
{
    QPointF pos;
    int x = qMax(count(Connector::Input),count(Connector::Output));
    (x > 0) ? --x : x;
    int y1,y2;
    y1=y2=-(x*60)/2;
    foreach (Connector *c, connectors)
    {
       if (c->getType() == Connector::Input)
       {
           pos.setY(y1);
           pos.setX(-95);
           y1 += 60;
       }
       else
       {
           pos.setY(y2);
           pos.setX(95);
           y2 += 60;
       }
       c->setPos(pos);
    }
    prepareGeometryChange();

}



