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
        QCOMPARE(channel->network(), network);
        model::ChipComponent *initiator = channel->initiator();
        model::ChipComponent *target = channel->target();
        // Check initiator
        QCOMPARE(initiator->name(), QString("testcomp1"));
        QCOMPARE(initiator->network(), network);
        auto q_out = qobject_cast<model::Outport *> (channel->initiator_port());
        QVERIFY2(q_out != nullptr, "Initiator port is not an output port!");
        // Check target
        QCOMPARE(target->name(), QString("testcomp2"));
        QCOMPARE(target->network(), network);
        auto q_in = qobject_cast<model::Inport *> (channel->target_port());
        QVERIFY2(q_in != nullptr, "Target port is not an input port!");

        auto p = qobject_cast<model::Network *>(channel->parent());
        QVERIFY2(p == network, "Channel does not have correct parent pointer");
        for(int i=0; i < network->count_chipComponents(); i++) {
            model::ChipComponent *c = network->chipComponent(i);
            p = qobject_cast<model::Network *> (c->parent());
            QString msg("Component does not have correct parent pointer. i=");
            msg.append(std::to_string(i).c_str());
            QVERIFY2(p == network, msg.toStdString().c_str());
        }

    } else {
        QString msg = model::Utils::qmlBuildError(component);
        QVERIFY2(false, msg.toStdString().c_str());
    }

}

