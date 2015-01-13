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

#include "testrunner.h"
#include "testnoc.h"
#include "testcomponent.h"
//#include "testnetwork.h"

/**
 * @brief main The testdriver.
 *
 * This module runs all added tests. Make
 * sure to add the test class you want to run tests off.
 *
 * @return
 */
int main() {

    TestRunner runner;
    runner.addTest(new TestNoc());
    runner.addTest(new TestComponent());
    //runner.addTest(new TestNetwork());

    runner.runTests();
}
