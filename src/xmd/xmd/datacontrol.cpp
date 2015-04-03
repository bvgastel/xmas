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

#include <iostream>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <QQmlContext>
#include <QDebug>
#include <QtQml>
#include <QVariant>
#include "canvascomponentextension.h"
#include "composite-network-extension.h"
#include "model/network.h"
#include "model/component.h"
#include "model/port.h"
#include "model/channel.h"
#include "export.h"
#include "parse.h"
#include "datacontrol.h"

DataControl::DataControl(QObject *parent)
    : QObject(parent),
      m_logger("datacontrol")
{
    m_project = std::make_shared<XMASProject>();
    QObject::connect(&m_logger, &Logger::writeLog, this, &DataControl::writeLog );
}

DataControl::~DataControl()
{
}

void DataControl::registerTypes() const{
    qmlRegisterType<model::Network>("XMAS.model", 1, 0, "XNetwork");
    qRegisterMetaType<model::Network*>("XNetwork*");
    qmlRegisterType<model::Component>("XMAS.model", 1, 0, "XComponent");
    qmlRegisterType<model::Channel>("XMAS.model", 1, 0, "XChannel");
    qmlRegisterType<model::XPort>("XMAS.model", 1, 0, "XPort");
    qRegisterMetaType<model::XPort*>("XPort*");
}

std::shared_ptr<XMASProject> DataControl::project() {
    return m_project;
}
