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

    int m_type = this->property("comptype").toInt();

    if(m_type){
        switch (m_type) {
        case Source:
            qDebug() << "Source created";
            //component = new XMASSource(name);
            qDebug() << " c++ type : " << "source";
            break;
        case Sink:
            qDebug() << "Sink created";
            //component = new XMASSink(name);
            qDebug() << " c++ type : " << "sink";
            break;
        case Function:
            //component = new XMASFunction(name);
            qDebug() << " c++ type : " << "function";
            break;
        case Queue:
            //component = new XMASQueue(name);
            qDebug() << " c++ type : " << "queue";
            break;
        case Join:
            //component = new XMASJoin(name);
            qDebug() << " c++ type : " << "join";
            break;
        case Merge:
            //component = new XMASMerge(name);
            qDebug() << " c++ type : " << "merge";
            break;
        case Switch:
            //component = new XMASSwitch(name);
            qDebug() << " c++ type : " << "switch";
            break;
        case Fork:
            //component = new XMASFork(name);
            qDebug() << " c++ type : " << "fork";
            break;
        default:
            qDebug() << "Unknown c++ type : " << m_type;
            break;
        }
    } else {
        qDebug() << "c++ type is not defined";
    }
    //Todo : type must filled in from qml during construction
    //Todo : unique name should be created from xmas , but for now maybe the autoname out of qml ?
    m_component = createComponent(CompType::Queue, "x");

}

model::Component::~Component()
{
    delete m_component;
}

XMASComponent *model::Component::createComponent(CompType type, std::string name) {
    XMASComponent *component;
    switch (type) {
    case Source:
        component = new XMASSource(name);
        break;
    case Sink:
        component = new XMASSink(name);
        break;
    case Function:
        component = new XMASFunction(name);
        break;
    case Queue:
        component = new XMASQueue(name);
        break;
    case Join:
        component = new XMASJoin(name);
        break;
    case Merge:
        component = new XMASMerge(name);
        break;
    case Switch:
        component = new XMASSwitch(name);
        break;
    case Fork:
        component = new XMASFork(name);
        break;
    default:
        break;
    }
    return component;
}



