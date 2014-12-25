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

#include "canvas.h"

Canvas::Canvas(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    m_itemMenu = itemMenu;
    m_mode = MoveItem;
    m_tmpConnection = NULL;
    m_tmpConnector = NULL;
    m_lastHighlighted = NULL;
}

void Canvas::setMode(Mode mode)
{
    m_mode = mode;
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        QList<QGraphicsItem *> startConnectors = items(mouseEvent->scenePos());
        foreach(QGraphicsItem * g, startConnectors) {
            if (g->type() == Connector::Type) {
                Connector* start = dynamic_cast<Connector*>(g);
                start->deleteConnections();

                m_tmpConnector = new Connector(NULL, this, NULL,Connector::Input);
                m_tmpConnector->setPos(mouseEvent->scenePos());
                m_tmpConnector->setVisible(false);
                m_tmpConnector->m_radius = 0;

                if (start->connectorType() != Connector::Input) {
                    m_tmpConnection = new NodeConnection(start, m_tmpConnector, NULL, this);
                }
                else {
                    m_tmpConnection = new NodeConnection(m_tmpConnector, start, NULL, this);
                }
                start->setHighlight(true);
                m_existingConnector = start;


                m_existingConnector->updatePosition();
                m_existingConnector->update();
                m_tmpConnector->updatePosition();
                m_tmpConnector->update();
                m_tmpConnection->updatePosition();
                m_tmpConnection->update();
                return;
            }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_tmpConnection != 0) {

        if (m_tmpConnection->startConnector()->mapToScene(0,0).x() < m_tmpConnection->endConnector()->mapToScene(0,0).x()) {
            if (m_tmpConnector == m_tmpConnection->endConnector())
                m_tmpConnector->setConnectorAlignment(Connector::ConnectorAlignment::Left);// right = false;
            else
                //m_tmpConnector->right = true;
                m_tmpConnector->setConnectorAlignment(Connector::ConnectorAlignment::Right);
        }
        else {
            if (m_tmpConnector == m_tmpConnection->endConnector())
                m_tmpConnector->setConnectorAlignment(Connector::ConnectorAlignment::Right);
            else
                m_tmpConnector->setConnectorAlignment(Connector::ConnectorAlignment::Left);
        }

        QList<QGraphicsItem *> intersectedItems = items(mouseEvent->scenePos());
        while(intersectedItems.count() && (intersectedItems.first() == m_tmpConnection || intersectedItems.first() == m_tmpConnector || intersectedItems.first()->type() != Connector::Type)) {
            intersectedItems.removeFirst();
        }
        if (intersectedItems.count() && intersectedItems.first() != m_tmpConnector && intersectedItems.first() != m_existingConnector) {
            Connector *node = qgraphicsitem_cast<Connector *>(intersectedItems.first());
            if (m_existingConnector->connectorType() == Connector::InOut) {
                //switch if non matching
                if ((node->connectorType() == Connector::Out && m_tmpConnection->startConnector() == m_existingConnector)
                    || (node->connectorType() == Connector::In && m_tmpConnection->endConnector() == m_existingConnector)) {
                    Connector* old1 = m_tmpConnection->startConnector();
                    Connector* old2 = m_tmpConnection->endConnector();
                    delete m_tmpConnection;
                    m_tmpConnection = new NodeConnection(old2, old1, NULL, this);
                }
            }
            else if (node->connectorType() == m_existingConnector->connectorType()) {
                return;
            }

            node->setHighlight(true);
            node->updatePosition();
            node->update();

            m_tmpConnector->setConnectorAlignment(node->connectorAlignment());
            m_tmpConnector->setConnectorType(node->connectorType());
            m_tmpConnector->updatePosition();
            m_tmpConnector->update();
            m_lastHighlighted = node;
        }
        else if (m_lastHighlighted != NULL) {
            m_lastHighlighted->setHighlight(false);
            m_lastHighlighted->updatePosition();
            m_lastHighlighted->update();

            m_lastHighlighted = NULL;
            m_tmpConnector->updatePosition();
            m_tmpConnector->update();
        }

        m_tmpConnector->setPos(mouseEvent->scenePos());
        m_tmpConnection->updatePosition();
        m_tmpConnection->update();
        m_tmpConnector->updatePosition();

        m_tmpConnector->update();
        return;
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_tmpConnection != 0) {
        Connector* startC = m_tmpConnection->startConnector();
        Connector* endC = m_tmpConnection->endConnector();
        QPointF startPos(startC->mapToScene(0, 0));
        QPointF endPos(endC->mapToScene(0, 0));

        m_tmpConnection->startConnector()->setHighlight(false);
        m_tmpConnection->endConnector()->setHighlight(false);

        QList<QGraphicsItem *> startConnectors = items(startPos);
        while(startConnectors.count() && (startConnectors.first() == m_tmpConnection || startConnectors.first() == m_tmpConnector || startConnectors.first()->type() != Connector::Type)) {
            startConnectors.removeFirst();
        }
        QList<QGraphicsItem *> endConnectors = items(endPos);
        while(endConnectors.count() && (endConnectors.first() == m_tmpConnection || endConnectors.first() == m_tmpConnector || endConnectors.first()->type() != Connector::Type)) {
            endConnectors.removeFirst();
        }
        delete m_tmpConnection;
        m_tmpConnection = 0;
        delete m_tmpConnector;
        m_tmpConnector = 0;

        if (startConnectors.count() > 0 && endConnectors.count() > 0 &&
            startConnectors.first()->type() == Connector::Type &&
            endConnectors.first()->type() == Connector::Type &&
            startConnectors.first() != endConnectors.first())
        {
            Connector *startConnector =
                qgraphicsitem_cast<Connector *>(startConnectors.first());
            Connector *endConnector =
                qgraphicsitem_cast<Connector *>(endConnectors.first());

            //dw new: verify again:
            if (!((startConnector->connectorType() == Connector::In && endConnector->connectorType() == Connector::In) || (startConnector->connectorType() == Connector::Out && endConnector->connectorType() == Connector::Out)))
            {
                Connection *connection = new NodeConnection(startConnector, endConnector, NULL, this);
                connection->setColor(mLineColor);
                startConnector->addConnection(connection);
                endConnector->addConnection(connection);
                connection->setZValue(-1000.0);

                connection->startConnector()->setHighlight(false);
                connection->endConnector()->setHighlight(false);
                connection->startConnector()->update();
                connection->endConnector()->update();
            }

            startConnector->setHighlight(false);
            endConnector->setHighlight(false);
        }

        if (m_lastHighlighted != NULL) {
            m_lastHighlighted->setHighlight(false);
            m_lastHighlighted = NULL;
        }
        return;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool Canvas::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
