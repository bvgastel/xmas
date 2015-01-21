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

#include "board.h"
#include "testnetwork.h"

TestNetwork::TestNetwork(QObject *parent) : QObject(parent)
{

}

TestNetwork::~TestNetwork()
{

}

void TestNetwork::testBoardCreation() {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testboard_1.qml"));
    model::Board *board = qobject_cast<model::Board *>(component.create());
    if (board) {
        QCOMPARE(board->network(), QString("network1"));
        QCOMPARE(board->width(), 1);
        QCOMPARE(board->height(), 1);
    } else {
        QWARN("Creation of Board not successful. Recheck qml and qrc files.");
        QVERIFY(false);
    }

}

void TestNetwork::testNetworkCreation() {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testnetwork_1.qml"));
    model::Network *network = qobject_cast<model::Network *>(component.create());
    if (network) {
        QCOMPARE(network->name(), QString("network1"));
    } else {
        QString msg;
        foreach(QQmlError error, component.errors()) {
            msg.append(error.toString());
        }

        QVERIFY2(false, msg.toStdString().c_str());
    }

}

