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

#include "component.h"

Component::Component(QMenu *contextMenu,
                   QGraphicsItem *parent, QGraphicsScene *scene, Qt::WindowFlags wFlags)
    : QGraphicsProxyWidget(parent,wFlags) , m_maxRadius(1)
{

    setCacheMode(DeviceCoordinateCache);

    m_contextMenu = contextMenu;
    m_isMoving = false;

    setZValue(1);
    //param
    m_noResize = true;
    if (m_noResize) {
        m_controlResizeHandles = true;
    }
    else {
        //param
        m_controlResizeHandles = false;
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    if (scene != NULL){
        scene->addItem(this);
    }

}

Component::~Component()
{
    removeWidgetFromConnectors();
    if (scene() != NULL) {
        this->scene()->removeItem(this);
    }
}

QRectF Component::boundingRect() const
{
    QRectF rec(QGraphicsProxyWidget::boundingRect());
    qreal extra = 3;
    rec = rec.normalized().adjusted(-extra, 0, extra, 0);
    return rec;
}

QPainterPath Component::shape() const {
    QPainterPath p;
    QRectF rec(QGraphicsProxyWidget::boundingRect());
    p.addRect(rec);
   return p;
}

void Component::setWidget(QWidget *widget) {
    QGraphicsProxyWidget::setWidget(widget);
    connect(widget, SIGNAL(destroyed()), this, SLOT(deleted()));
}

void Component::addConnector(Connector* connector) {
    connectors.append(connector);
    if (connector->m_radius > m_maxRadius) {
        m_maxRadius = connector->m_radius;
    }

    updateConnectorsPos();
    prepareGeometryChange();
}

void Component::deleteConnections()
{
    foreach (NodeConnector *c, connectors) {
         c->deleteConnections();
    }
}

void Component::removeWigetFromConnectors() {
    foreach (Connector *c, connectors) {
        c->removeWidget();
    }
}

void Component::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
   if (m_contextMenu != NULL) {
        m_contextMenu->exec(event->screenPos());
    }
}

QVariant Component::itemChange(GraphicsItemChange change,
                              const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        updateConnectorsPos();
    }

    if (change == QGraphicsItem::ItemVisibleHasChanged && !value.value<bool>()) {
        deleted();
        return value;
    }

    if (m_controlResizeHandles && change == QGraphicsItem::ItemCursorChange) {
        QCursor cur = qVariantValue<QCursor>(value);
        if (cur.shape() == Qt::SizeVerCursor || cur.shape() == Qt::SizeHorCursor || cur.shape() == Qt::SizeBDiagCursor || cur.shape() == Qt::SizeFDiagCursor) {
            if (m_noResize) {
                return Qt::ArrowCursor;
            }
            else {
                foreach (Connector *con, connectors) {
                    if (con->isUnderMouse()) {
                        return Qt::ArrowCursor;
                    }
                }
            }
        }
    }
    if (change == QGraphicsItem::ItemSelectedChange || change == QGraphicsItem::ItemTransformChange || change == QGraphicsItem::ItemScaleChange
            || change == QGraphicsItem::ItemSendsGeometryChanges || change == QGraphicsItem::ItemMatrixChange) {
        updateConnectorsPos();
    }
    if (change == QGraphicsItem::ItemPositionHasChanged || change == QGraphicsItem::ItemSelectedChange) {
        updateConnectorsPos();
    }

    return QGraphicsProxyWidget::itemChange(change, value);
}

void Component::resizeEvent ( QGraphicsSceneResizeEvent * event ) {
    QGraphicsProxyWidget::resizeEvent(event);
    updateConnectorsPos();
}


void Component::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )  {
    event->ignore();
    return;
}

void Component::updateConnectorsPos() {
    foreach (NodeConnector *con, connectors) {
        con->updatePosition();
    }
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    QGraphicsProxyWidget::paint(painter, option, w);

}

const char* Component::shouldMoveOnClickTypes[] = {"QDialog", "QFrame", "QGroupBox"};


bool Component::shouldMoveNode(QGraphicsSceneMouseEvent *mouseEvent) {
    QPointF pos = mouseEvent->pos();
    QPointer<QWidget> hitWidget = widget()->childAt(pos.toPoint());
    if (hitWidget == NULL) {
        return true;
    }
    const size_t len = sizeof(shouldMoveOnClickTypes) / sizeof(shouldMoveOnClickTypes[0]);
    for (size_t i = 0; i < len; ++i) {
        if (hitWidget->inherits(shouldMoveOnClickTypes[i])) {
            return true;
        }
    }
    return false;
}

void Component::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (shouldMoveNode(mouseEvent)) {
        QGraphicsItem::mousePressEvent(mouseEvent);
        m_isMoving = true;
        // what if we have to remove that?
        scene()->clearSelection();
        scene()->clearFocus();
        setSelected(true);
    }
    else {
        QGraphicsProxyWidget::mousePressEvent(mouseEvent);
    }
}

void Component::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (m_isMoving) {
        QGraphicsItem::mouseMoveEvent(mouseEvent);
    }
    else {
        QGraphicsProxyWidget::mouseMoveEvent(mouseEvent);
    }
}

void Component::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (m_isMoving) {
        m_isMoving = false;
    }
    QGraphicsItem::mouseReleaseEvent(mouseEvent);
    QGraphicsProxyWidget::mouseReleaseEvent(mouseEvent);
    scene()->clearSelection();
    setSelected(true);
}

void Component::hide() {
    this->widget()->close();
}

void Component::deleted() {
    this->widget()->close();
}

void Component::deleted(int result) {
    this->widget()->close();
}
