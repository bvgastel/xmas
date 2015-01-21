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
#include <QDebug>

#include "testrunner.h"
#include "testport.h"
#include "testchipcomponent.h"
#include "testchannel.h"
#include "testnetwork.h"

/**
 * @brief main The testdriver.
 *
 * This module runs all tests added to the testset.
 *
 * @return 0
 */
int main() {

    TestRunner runner;
    runner.addTest(new TestPort());
    runner.addTest(new TestComponent());
    runner.addTest(new TestChannel());
    runner.addTest(new TestNetwork());

    runner.runTests();
    return 0;
}
