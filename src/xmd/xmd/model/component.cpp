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
        break;
    case Sink:
        qDebug() << "Sink created";
        //component = new XMASSink(name);
        break;
    case Function:
        //component = new XMASFunction(name);
        break;
    case Queue:
        //component = new XMASQueue(name);
        break;
    case Join:
        //component = new XMASJoin(name);
        break;
    case Merge:
        //component = new XMASMerge(name);
        break;
    case Switch:
        //component = new XMASSwitch(name);
        break;
    case Fork:
        //component = new XMASFork(name);
        break;
    default:
        qDebug() << "Unknown c++ type : " << m_type;
        break;
    }
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

//void model::Component::onXChanged() {
//    emit writeLog(QString("x=")+x());
//}

//void model::Component::onYChanged() {
//    emit writeLog(QString("y=")+y());
//}
//void model::Component::onScaleChanged() {
//    emit writeLog(QString("scale changed: ")+scale());
//}
//void model::Component::onRotationChanged() {

//    emit writeLog(QString("orientation = ")+rotation());
//}

//void model::Component::onTypeChanged() {
//    //emit writeLog(QString("type = "+type()));
//    emit writeLog(QString("type = ????"));
//}

//void model::Component::onItemChanged() {
//    emit writeLog(QString("onItemChanged received."));
//}



