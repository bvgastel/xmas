/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#include "parse.h"
#include "syntaxcheckworker.h"

void SyntaxCheckWorker::doWork(const QString &json) {
    Result result;

    // STEP 0: parse the json string toward a component map
    bitpowder::lib::MemoryPool mp;
    std::map<bitpowder::lib::String, XMASComponent *> componentMap;
    std::tie(componentMap, std::ignore) = parse_xmas_from_json(json.toStdString(), mp);

    // STEP 1: check if the topology is well formed
    std::chrono::high_resolution_clock::time_point start, end;
    std::tie(start, end) = checkSyntax(componentMap, result);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    QString msg = QString("syntactic check in \t")+duration+" ms";
    result.add2ResultString(msg+"\n");
    std::cout << msg.toStdString() << std::endl;

    emit resultReady(result);
}

std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>
SyntaxCheckWorker::checkSyntax(std::map<bitpowder::lib::String, XMASComponent *> componentMap,
                               Result &result) {
    auto start = std::chrono::high_resolution_clock::now();

    // check that each pointer is reflexive
    auto cbegin = componentMap.cbegin();
    auto cend = componentMap.cend();
    auto it = cbegin;

    bool valid = true;
    while(it != cend) {
        it++;
        XMASComponent *c = it->second;
        valid = valid & c->valid();
    }
    QString msg = valid ? "Syntax ok" : "Syntax not ok";
    result.add2ResultString(msg+"\n");
    std::cout << msg.toStdString() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    return make_pair(start, end);
}

