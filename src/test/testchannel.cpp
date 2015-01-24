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
#include "utils.h"

TestChannel::TestChannel(QObject *parent) : QObject(parent)
{

}

TestChannel::~TestChannel()
{

}

void TestChannel::testChannelCreate() {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testchannel_1.qml"));
    model::Channel *channel = qobject_cast<model::Channel *>(component.create());
    if (channel) {
        QCOMPARE(channel->name(), QString("testchannel1"));
        QVERIFY(channel->network() == nullptr);
        QVERIFY(channel->initiator() == nullptr);
        QVERIFY(channel->initiator_port() == nullptr);
        QVERIFY(channel->target() == nullptr);
        QVERIFY(channel->target_port() == nullptr);
        QCOMPARE(channel->ptSize(), 2);
        for (int i = 0; i < channel->ptSize(); i++) {
            QString n = QString(std::to_string(i+1).c_str());
            model::GridPoint *p = channel->pt(i);
            QCOMPARE(p->x(), i+1);
            QCOMPARE(p->y(), i+1);
        }
    } else {
        QString msg = model::Utils::qmlBuildError(component);
        QVERIFY2(false, msg.toStdString().c_str());
    }
}

