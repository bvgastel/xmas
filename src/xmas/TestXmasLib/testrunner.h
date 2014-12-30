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

#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QList>
#include <QTimer>
#include <QCoreApplication>
#include <QtTest/QtTest>

class TestRunner: public QObject
{
    Q_OBJECT

public:
    TestRunner()
        : m_overallResult(0)
    {}

    void addTest(QObject * test) {
        test->setParent(this);
        m_tests.append(test);
    }

    bool runTests() {
        int argc =0;
        char * argv[] = {0};
        QCoreApplication app(argc, argv);
        QTimer::singleShot(0, this, SLOT(run()) );
        app.exec();

        return m_overallResult == 0;
    }
private slots:
    void run() {
        doRunTests();
        QCoreApplication::instance()->quit();
    }
private:
    void doRunTests() {
        foreach (QObject * test, m_tests) {
            m_overallResult|= QTest::qExec(test);
        }
    }

    QList<QObject *> m_tests;
    int m_overallResult;
};

#endif // TESTRUNNER_H
