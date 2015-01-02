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

int main(int argc, char *argv[]) {

    std::cout << " number of arguments: " << argc << " and arguments are: ";
    std::string glue = "";
    for (int i = 0; i < argc; i++) {
        std::cout << glue << argv[i];
        glue = ", ";
    }
    std::cout << std::endl;

    TestRunner runner;
    runner.addTest(new TestNoc());
    runner.addTest(new TestComponent());

    runner.runTests();
}
