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

#include <QGraphicsProxyWidget>
#include "connector.h"
#include "connection.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Port;
class Channel;

class Component : public QGraphicsProxyWidget
{
        Q_OBJECT
public:
    enum { Type = UserType + 15 };
    int type() const Q_DECL_OVERRIDE { return Type; }
    Component(QMenu *contextMenu,
              QGraphicsItem *parent = 0,
              QGraphicsScene *scene = 0,
              Qt::WindowFlags wFlags = 0);
    virtual ~Component();
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void setWidget(QWidget *widget);
    void addConnection(Connection* nc);
    void deleteConnections();
    QList<Connector *> connectors;

public slots:
     void deleted();
     void deleted(int result);
     void hide();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    bool shouldMoveNode(QGraphicsSceneMouseEvent *mouseEvent);
    const static char* shouldMoveOnClickTypes[];
    void hoverMoveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    void resizeEvent ( QGraphicsSceneResizeEvent * event ) Q_DECL_OVERRIDE;
    bool m_controlResizeHandles;
    bool m_noResize;

private:
    QMenu *m_contextMenu;
    void updateConnectorsPos();
    bool m_isMoving;
    void removeWidgetFromConnectors();
    int m_maxRadius;

};

#endif // COMPONENT_H
