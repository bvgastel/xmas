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

#include <QDebug>

#include "testchannel.h"

TestChannel::TestChannel(QObject *parent) : QObject(parent)
{

}

TestChannel::~TestChannel()
{

}

void TestChannel::testChannelCreate() {
    qmlRegisterType<model::Port>("Model", 1, 0, "Port");
    qmlRegisterType<model::ChipComponent>("Model", 1, 0, "ChipComponent");
    qmlRegisterType<model::Channel>("Model", 1, 0, "Channel");
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testchannel_1.qml"));
    model::Channel *channel = qobject_cast<model::Channel *>(component.create());
    if (channel) {
        QCOMPARE(channel->name(), QString("testchannel1"));
        QCOMPARE(channel->initiator(), QString("testinitiator1"));
        QCOMPARE(channel->init_port(), QString("testinit_port1"));
        QCOMPARE(channel->target(), QString("testtarget1"));
        QCOMPARE(channel->target_port(), QString("testtarget_port1"));
        QCOMPARE(channel->ptSize(), 2);
        for (int i = 0; i < channel->ptSize(); i++) {
            QString n = QString(std::to_string(i+1).c_str());
            QPoint *p = channel->pt(i);
            QCOMPARE(p->x(), i+1);
            QCOMPARE(p->y(), i+1);
        }
    } else {
        QWARN("Creation of Channel not successful. Recheck qml and qrc files.");
        QVERIFY(false);
    }
}

