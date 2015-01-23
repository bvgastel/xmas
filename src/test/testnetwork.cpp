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

#include <iostream>

#include <QtQml>
#include <QDebug>

#include "testnetwork.h"

TestNetwork::TestNetwork(QObject *parent) : QObject(parent)
{

}

TestNetwork::~TestNetwork()
{

}

void TestNetwork::testNetworkCreation() {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testnetwork_1.qml"));
    model::Network *network = qobject_cast<model::Network *>(component.create());
    if (network) {
        QCOMPARE(network->name(), QString("network1"));
        model::Channel *channel = network->channel(0);
        model::ChipComponent *initiator = channel->initiator();
        QCOMPARE(initiator->name(), QString("testcomp1"));
        QCOMPARE(initiator->network(), network->name());
    } else {
        QString msg = model::Utils::qmlBuildError(component);
        QVERIFY2(false, msg.toStdString().c_str());
    }

}

