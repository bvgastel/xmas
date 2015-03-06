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

model::Component::Component(QQuickItem *parent) : QQuickItem(parent)
{
    // get datacontrol object from qml ---> Impossible
    //m_component = createXmasComponent(QString("source"), QString("test comp"));
}

model::Component::~Component()
{

}

void model::Component::onXChanged() {
    emit writeLog(QString("x=")+x());
}

void model::Component::onYChanged() {
    emit writeLog(QString("y=")+y());
}
void model::Component::onScaleChanged() {
    emit writeLog(QString("scale changed: ")+scale());
}
void model::Component::onRotationChanged() {

    emit writeLog(QString("orientation = ")+rotation());
}


