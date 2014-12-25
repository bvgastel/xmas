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

#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include "component.h"
#include "connection.h"
#include "connector.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class Component;

class Canvas : public QGraphicsScene
{
        Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem, InsertNode, InsertNode2 };

    Canvas(QMenu *itemMenu, QObject *parent = 0);
    QColor lineColor() const
        { return m_lineColor; }
    void setLineColor(const QColor &color);

    QMenu *m_itemMenu;

public slots:
    void setMode(Mode mode);

signals:
    void nodeInserted(QGraphicsItem *item);
    void nodeItemInserted(Component *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    QColor m_lineColor;
    Mode m_mode;
private:
    bool isItemChange(int type);
    bool m_leftButtonDown;
    QPointF m_startPoint;

    Connection* m_tmpConnection;
    Connector* m_tmpConnector;
    Connector* m_existingConnector;
    Connector* m_lastHighlighted;

};

#endif // CANVAS_H
