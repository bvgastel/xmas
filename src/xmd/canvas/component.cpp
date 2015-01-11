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
 */
Component::Component(): QDeclarativeItem()
{
    setCacheMode(ItemCoordinateCache);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemHasNoContents,false);
    setAcceptedMouseButtons(Qt::AllButtons);
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
    QRectF rect = QRect(0,0,this->width(),this->height());
    rect.adjust(21,21,-21,-21);
    return rect;
}


/**
 * @brief Component::shape
 * @return
 */
QPainterPath Component::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

/**
 * @brief Component::connectors
 * @return
 */
QDeclarativeListProperty<Connector> Component::connectors()
{
    return QDeclarativeListProperty<Connector>(this, 0,
                                               &Component::append_connector,
                                               0,
                                               0,
                                               0);
}

/**
 * @brief Component::append_connector
 * @param list
 * @param connector
 */
void Component::append_connector(QDeclarativeListProperty<Connector> *list, Connector *connector)
{
    Component *component = qobject_cast<Component *>(list->object);
    if (component) {
        connector->setParentItem(component);
        component->m_connectors.append(connector);
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
        //painter->drawRoundRect(bodyRect(),10,10);
    }
}



