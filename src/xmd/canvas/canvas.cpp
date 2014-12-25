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

/**
 * @brief Canvas::Canvas
 * @param itemMenu
 * @param parent
 */
Canvas::Canvas(QMenu *itemMenu,
               QObject *parent)
    : QGraphicsScene(parent)
{
    m_itemMenu = itemMenu;
    m_mode = MoveItem;
    m_tmpConnection = nullptr;
    m_tmpConnector = nullptr;
    m_lastHighlighted = nullptr;
}

/**
 * @brief Canvas::setMode
 * @param mode
 */
void Canvas::setMode(Mode mode)
{
    m_mode = mode;
}

/**
 * @brief Canvas::mousePressEvent
 * @param mouseEvent
 */
void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        QList<QGraphicsItem *> startConnectors = items(mouseEvent->scenePos());
        foreach(QGraphicsItem * g, startConnectors)
        {
            if (g->type() == Connector::Type)
            {
                Connector* start = dynamic_cast<Connector*>(g);
                start->deleteConnections();
                m_tmpConnector = new Connector(nullptr, this, nullptr,Connector::Input);
                m_tmpConnector->setPos(mouseEvent->scenePos());
                m_tmpConnector->setVisible(false);
                m_tmpConnector->m_radius = 0;
                if (start->connectorType() != Connector::Input)
                {
                    m_tmpConnection = new Connection(start, m_tmpConnector, nullptr, this);
                }
                else
                {
                    m_tmpConnection = new Connection(m_tmpConnector, start, nullptr, this);
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

/**
 * @brief Canvas::mouseMoveEvent
 * @param mouseEvent
 */
void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_tmpConnection != nullptr)
    {

        if (m_tmpConnection->startConnector()->mapToScene(0,0).x()
                < m_tmpConnection->endConnector()->mapToScene(0,0).x())
        {
            if (m_tmpConnector == m_tmpConnection->endConnector())
                m_tmpConnector->setConnectorType(Connector::Input);
            else
                m_tmpConnector->setConnectorType(Connector::Output);
        }
        else
        {
            if (m_tmpConnector == m_tmpConnection->endConnector())
            {
                m_tmpConnector->setConnectorType(Connector::Output);
            }
            else
            {
                m_tmpConnector->setConnectorType(Connector::Input);
            }
        }

        QList<QGraphicsItem *> intersectedItems = items(mouseEvent->scenePos());
        while(intersectedItems.count()
              && (intersectedItems.first() == m_tmpConnection
                  || intersectedItems.first() == m_tmpConnector
                  || intersectedItems.first()->type() != Connector::Type))
        {
            intersectedItems.removeFirst();
        }
        if (intersectedItems.count()
                && intersectedItems.first() != m_tmpConnector
                && intersectedItems.first() != m_existingConnector)
        {
            Connector *conn = qgraphicsitem_cast<Connector *>(intersectedItems.first());
            if (m_existingConnector->connectorType() == Connector::InOut)
            {
                 if ((conn->connectorType() == Connector::Output
                     && m_tmpConnection->startConnector() == m_existingConnector)
                         || (conn->connectorType() == Connector::Input
                             && m_tmpConnection->endConnector() == m_existingConnector))
                 {
                    Connector* old1 = m_tmpConnection->startConnector();
                    Connector* old2 = m_tmpConnection->endConnector();
                    delete m_tmpConnection;
                    m_tmpConnection = new Connection(old2, old1, nullptr, this);
                }
            }
            else if (conn->connectorType() == m_existingConnector->connectorType())
            {
                return;
            }

            conn->setHighlight(true);
            conn->updatePosition();
            conn->update();

            m_tmpConnector->setConnectorType(conn->connectorType());
            m_tmpConnector->updatePosition();
            m_tmpConnector->update();
            m_lastHighlighted = conn;
        }
        else if (m_lastHighlighted != nullptr)
        {
            m_lastHighlighted->setHighlight(false);
            m_lastHighlighted->updatePosition();
            m_lastHighlighted->update();

            m_lastHighlighted = nullptr;
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

/**
 * @brief Canvas::mouseReleaseEvent
 * @param mouseEvent
 */
void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_tmpConnection != nullptr)
    {
        Connector* startC = m_tmpConnection->startConnector();
        Connector* endC = m_tmpConnection->endConnector();
        QPointF startPos(startC->mapToScene(0, 0));
        QPointF endPos(endC->mapToScene(0, 0));

        m_tmpConnection->startConnector()->setHighlight(false);
        m_tmpConnection->endConnector()->setHighlight(false);

        QList<QGraphicsItem *> startConnectors = items(startPos);
        while(startConnectors.count()
              && (startConnectors.first() == m_tmpConnection
                  || startConnectors.first() == m_tmpConnector
                  || startConnectors.first()->type() != Connector::Type))
        {
            startConnectors.removeFirst();
        }
        QList<QGraphicsItem *> endConnectors = items(endPos);
        while(endConnectors.count()
              && (endConnectors.first() == m_tmpConnection
                  || endConnectors.first() == m_tmpConnector
                  || endConnectors.first()->type() != Connector::Type))
        {
            endConnectors.removeFirst();
        }
        delete m_tmpConnection;
        m_tmpConnection = nullptr;
        delete m_tmpConnector;
        m_tmpConnector = nullptr;

        if (startConnectors.count() > 0 && endConnectors.count() > 0
                && startConnectors.first()->type() == Connector::Type
                && endConnectors.first()->type() == Connector::Type
                && startConnectors.first() != endConnectors.first())
        {
            Connector *startConnector =
                qgraphicsitem_cast<Connector *>(startConnectors.first());
            Connector *endConnector =
                qgraphicsitem_cast<Connector *>(endConnectors.first());

            if (!((startConnector->connectorType() == Connector::Input
                   && endConnector->connectorType() == Connector::Input)
                  || (startConnector->connectorType() == Connector::Output
                      && endConnector->connectorType() == Connector::Output)))
            {
                Connection *connection = new Connection(startConnector, endConnector, nullptr, this);
                connection->setColor(m_lineColor);
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

        if (m_lastHighlighted != nullptr)
        {
            m_lastHighlighted->setHighlight(false);
            m_lastHighlighted = nullptr;
        }
        return;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/**
 * @brief Canvas::isItemChange
 * @param type
 * @return
 */
bool Canvas::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems())
    {
        if (item->type() == type)
        {
            return true;
        }
    }
    return false;
}
