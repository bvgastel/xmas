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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

#include "connector.h"

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsSvgItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
QT_END_NAMESPACE

class Connector;
class ConnectorType;

/**
 * @brief The Component class
 */
class Component : public QGraphicsSvgItem
{
public:
    enum { Type = UserType + 0 };
    int type() const Q_DECL_OVERRIDE { return Type; }
    Component(QMenu *contextMenu,
              QGraphicsItem *parent = 0,
              QGraphicsScene *scene = 0);
    virtual ~Component();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;

//  void addConnection(Connection* connection);
    void Component::addConnector(int type);
    void deleteConnections();
    QList<Connector *> connectors;

public slots:
     void deleted();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    bool m_controlResizeHandles;
    bool m_noResize;

private:
    QMenu *m_contextMenu;
    void updateConnectorsPos();
    bool m_isMoving;
    int count(int type) const;
    QRect bodyRect() const;
    void updateConnectorPositions();
};

#endif // COMPONENT_H
