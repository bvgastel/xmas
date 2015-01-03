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
#include "complib.h"
#include <iostream>

/**
 * @brief CompLib::CompLib
 * @param contextMenu
 * @param parent
 * @param scene
 */
CompLib::CompLib()
{
   //(todo) test menu : must be specefic for each component type
    QMenu *qm = new QMenu("test");
    qm->addMenu("help");
    qm->addMenu("delete");
    qm->addMenu("edit");

    m_queue = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/queue.svg"));
    m_queue->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_queue->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_queue->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_function = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/function.svg"));
    m_function->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_function->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_function->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_fork = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/fork.svg"));
    m_fork->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_fork->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_fork->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_join = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/join.svg"));
    m_join->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_join->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_join->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_switch = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/switch.svg"));
    m_switch->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_switch->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_switch->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_merge = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/merge.svg"));
    m_merge->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_merge->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_merge->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_sink = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/sink.svg"));
    m_sink->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_sink->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_sink->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_source = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/source.svg"));
    m_source->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_source->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_source->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_in = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/in.svg"));
    m_in->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_in->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_in->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_out = (Component *) new QGraphicsSvgItem(QString(":/images/svgs/out.svg"));
    m_out->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_out->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_out->setFlag(QGraphicsItem::ItemIsFocusable, true);

    m_composite = new Component(qm,nullptr,nullptr);
    m_composite->addConnector(Connector::Input);
    m_composite->addConnector(Connector::Input);
    m_composite->addConnector(Connector::Input);
    m_composite->addConnector(Connector::Output);
    m_composite->addConnector(Connector::Output);

}

/**
 * @brief CompLib::getComponent
 * @param type
 * @return
 */
Component *CompLib::getComponent(int type)
{
    switch (type)
    {
    case CompLib::Queue :
        return m_queue;
    case CompLib::Function :
        return m_function;
    case CompLib::Fork :
        return m_fork;
    case CompLib::Join :
        return m_join;
    case CompLib::Switch :
        return m_switch;
    case CompLib::Merge :
        return m_merge;
    case CompLib::Source :
        return m_source;
    case CompLib::Sink :
        return m_sink;
    case CompLib::In :
        return m_in;
    case CompLib::Out :
        return m_out;
    case CompLib::Composite :
        return m_composite;
    }
    return nullptr;
}
