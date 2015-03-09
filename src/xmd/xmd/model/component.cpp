/*********************************************************************
  *
  * Copyright (C) <name>, <year>
  *
  * This file is part of the xmas-design tool.
  *
  * The xmas-design tool is free software: you can redistribute it
  * and/or modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation, either version 3 of
  * the License, or (at your option) any later version.
  *
  * The xmas-design tool is distributed in the hope that it will be
  * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
  * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with the xmas-design tool.  If not, see
  * <http://www.gnu.org/licenses/>.
  *
  **********************************************************************/

#include "component.h"

model::Component::Component(QQuickItem *parent)
    : QQuickItem(parent)
{

}

model::Component::~Component()
{
    delete m_component;
}

void model::Component::classBegin() {
    // No action needed.
}

void model::Component::componentComplete() {
    m_type = (CompType)this->property("type").toInt();
    m_component = createComponent(m_type, m_name);
}

// TODO: What to do with IN and OUT?
XMASComponent *model::Component::createComponent(CompType type, QString qname) {
    XMASComponent *component = nullptr;
    std::string name = qname.toStdString();
    switch(type) {
    case CompType::Source :
        component = new XMASSource(name);
        break;
    case CompType::Sink :
        component = new XMASSink(name);
        break;
    case CompType::Function :
        component = new XMASFunction(name);
        break;
    case CompType::Queue :
        component = new XMASQueue(name);
        break;
    case CompType::Join :
        component = new XMASJoin(name);
        break;
    case CompType::Merge :
        component = new XMASMerge(name);
        break;
    case CompType::Switch :
        component = new XMASSwitch(name);
        break;
    case CompType::Fork :
        component = new XMASFork(name);
        break;
    case CompType::In :
        emit writeLog(QString("type in is not implemented .... yet"), Qt::red);
        break;
    case CompType::Out :
        emit writeLog(QString("type out is not implemented .... yet"), Qt::red);
        break;
    case CompType::Composite :
        emit writeLog(QString("type composite is not implemented .... yet"), Qt::red);
        break;
    default :
        emit writeLog(QString("Unknown component type!"), Qt::red);
    }
    return component;
}



