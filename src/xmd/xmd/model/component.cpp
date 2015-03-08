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
    QString m_compType = this->property("comptype").toString();

    m_component = createComponent(m_compType, m_name);
}

// TODO: What to do with IN and OUT?
XMASComponent *model::Component::createComponent(QString typeName, QString qname) {
    XMASComponent *component = nullptr;
    std::string name = qname.toStdString();
    if (xsource == typeName) {
        component = new XMASSource(name);
    } else if (xsink == typeName) {
        component = new XMASSink(name);
    } else if (xfunction == typeName) {
        component = new XMASFunction(name);
    } else if (xqueue == typeName) {
        component = new XMASQueue(name);
    } else if (xjoin == typeName) {
        component = new XMASJoin(name);
    } else if (xmerge == typeName) {
        component = new XMASMerge(name);
    } else if (xswitch == typeName) {
        component = new XMASSwitch(name);
    } else if (xfork == typeName) {
        component = new XMASFork(name);
    } else if (xin == typeName) {
        emit writeLog(QString("type in is not implemented .... yet"), Qt::red);
    } else if (xout == typeName) {
        emit writeLog(QString("type out is not implemented .... yet"), Qt::red);
    } else if (xcomposite == typeName) {
        emit writeLog(QString("type composite is not implemented .... yet"), Qt::red);
    }

    return component;
}



