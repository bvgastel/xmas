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

#include "testnoc.h"

TestNoc::TestNoc(QObject *parent) : QObject(parent)
{

}

TestNoc::~TestNoc()
{

}

void TestNoc::initTestCase()
{

}

void TestNoc::cleanupTestCase()
{

}

void TestNoc::emptyNoc() {
    QString name = "empty network";
    std::shared_ptr<Noc> noc = std::make_shared<Noc>(name);
    std::cout << "Network == noc" << noc << std::endl;

    QString nameExpected = "empty network";
    QCOMPARE(noc->name(), nameExpected);

}

void TestNoc::equalNoc() {
    QString name1 = "one noc";
    QString name2 = "one noc";

    Noc noc1(name1);
    Noc noc2(name2);

    QCOMPARE(noc1, noc2);
    QVERIFY(!(noc1 != noc2));
}

void TestNoc::unequalNoc() {
    QString name1 = "one noc";
    QString name2 = "two noc";

    Noc noc1(name1);
    Noc noc2(name2);

    QVERIFY(!(noc1 == noc2));
    QVERIFY(noc1 != noc2);
}
