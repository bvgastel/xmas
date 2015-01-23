/*********************************************************************
  *
  * Copyright (C) Guus Bonnema, 2014
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

#include <QtQml>

#include "utils.h"

#include "gridpoint.h"
#include "borderpos.h"
#include "port.h"
#include "channel.h"
#include "chipcomponent.h"
#include "network.h"

model::Utils::Utils()
{

}

model::Utils::~Utils()
{

}

QString model::Utils::qmlBuildError(QQmlComponent &component) {
    QString msg;
    msg.append(QString("QML error: "));
    foreach(QQmlError error, component.errors()) {
        msg.append(error.toString());
    }
    return msg;
}

void model::Utils::registreModel() {
    const char *nameSpace = "Model";
    const int major = 1;
    const int minor = 0;
    qmlRegisterType<model::GridPoint>       (nameSpace, major, minor, "GridPoint");
    qmlRegisterType<model::BorderPos>       (nameSpace, major, minor, "BorderPos");
    qmlRegisterUncreatableType<model::Port> (nameSpace, major, minor, "Port", QString("Port is an abstract class"));
    qmlRegisterType<model::Inport>          (nameSpace, major, minor, "Inport");
    qmlRegisterType<model::Outport>         (nameSpace, major, minor, "Outport");
    qmlRegisterType<model::ChipComponent>   (nameSpace, major, minor, "ChipComponent");
    qmlRegisterType<model::Channel>         (nameSpace, major, minor, "Channel");
    qmlRegisterType<model::Network>         (nameSpace, major, minor, "Network");
}
