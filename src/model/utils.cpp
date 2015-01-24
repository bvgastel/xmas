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

/**
 * @brief readNetwork read the network and return a tuple with
 *        the result.
 *
 * @param urlString A Url as a c string that indicates where to read from.
 *
 * @return std::tuple<Network *, QQmlComponent *> which either
 *  contains a network pointer without errors in component
 *  or a nullptr for network and a qml component for the errors.
 *  Use as follows:
 *
 *  Network *n;
 *  QQmlComponent *q;
 *  std::tie(n, q) = readNetwork("some uri");
 *
 *  if(!n) {
 *      // Print the errors : its a QList in c->errors();
 *  }
 *
 * Both pointers were created on the heap.
 *
 * TODO: FINISH THIS!
 *
 */
std::tuple<model::Network *, QQmlComponent *> readNetwork(const char *urlString) {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent *component = new QQmlComponent(&engine, QUrl(urlString));
    model::Network *network = qobject_cast<model::Network *>(component->create());

    return std::make_tuple(network, component);
}

std::tuple<model::Network *, QQmlComponent *> readNetwork(QUrl url) {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent *component = new QQmlComponent(&engine, url);
    model::Network *network = qobject_cast<model::Network *>(component->create());

    return std::make_tuple(network, component);
}
