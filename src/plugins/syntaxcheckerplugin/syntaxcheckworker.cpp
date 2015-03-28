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
#include <set>
#include <map>
#include <tuple>

#include "commoninterface.h"
#include "cycle.h"
#include "parse.h"
#include "syntaxcheckworker.h"

SyntaxCheckWorker::SyntaxCheckWorker(QObject *parent) : WorkerInterface(parent) {

}

SyntaxCheckWorker::~SyntaxCheckWorker() {

}

void SyntaxCheckWorker::reportTimer(tpoint start, tpoint end, QString name, Result &result) {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    QString duration_qstr = name + QString(" in \t")+duration+" ms\n";
    result.add2ResultString(duration_qstr);
    std::cout << duration_qstr.toStdString() << std::endl;
}

void SyntaxCheckWorker::doWork(XMap &componentMap) {
    Result result;
    tpoint start, end, nextstart, nextend;

    // STEP 1: convert map to set
    std::set<XMASComponent *> componentSet;
    std::tie(nextstart, nextend, componentSet) = convertComponentMap2Set(componentMap);
    start = nextstart;
    end = nextend;
    reportTimer(nextstart, nextend, "convert to set", result);

    // STEP 2: check if the topology is well formed
    std::tie(nextstart, nextend) = checkSyntax(componentMap, result);
    end = nextend;
    reportTimer(nextstart, nextend, "syntax check (valid ports)", result);

    // STEP 2: check for cycles
    std::tie(nextstart, nextend) = checkCycles(componentSet, result);
    end = nextend;
    reportTimer(nextstart, nextend, "syntax check (valid ports)", result);

    // STEP 3: check symbolic types
    std::tie(nextstart, nextend) = checkSymbolicTypes(componentSet, result);
    end = nextend;
    reportTimer(nextstart, nextend, "symbolic types", result);

    // Final STEP: Reporting final timers
    reportTimer(start, end, "Complete check", result);
    emit resultReady(result);
}

void SyntaxCheckWorker::doThreadWork(const QString &json) {
    // STEP 0: parse the json string toward a component map
    bitpowder::lib::MemoryPool mp;
    std::map<bitpowder::lib::String, XMASComponent *> componentMap;
    std::tie(componentMap, std::ignore) = parse_xmas_from_json(json.toStdString(), mp);
    doWork(componentMap);
}

std::tuple<tpoint, tpoint, XSet>
    SyntaxCheckWorker::convertComponentMap2Set(XMap componentMap) {
    auto start = std::chrono::high_resolution_clock::now();

    XSet componentSet;

    // check that each pointer is reflexive
    auto cbegin = componentMap.cbegin();
    auto cend = componentMap.cend();
    auto it = cbegin;

    while(it != cend) {
        auto component = it->second;
        componentSet.insert(component);
        it++;
    }
    auto end = std::chrono::high_resolution_clock::now();

    return std::make_tuple(start, end, componentSet);
}

std::pair<tpoint, tpoint>
SyntaxCheckWorker::checkSyntax(XMap componentMap,
                               Result &result) {
    auto start = std::chrono::high_resolution_clock::now();

    // check that each pointer is reflexive
    auto cbegin = componentMap.cbegin();
    auto cend = componentMap.cend();
    auto it = cbegin;

    bool valid = true;
    while(it != cend) {
        XMASComponent *c = it->second;
        valid = valid & c->valid();
        it++;
    }
    QString msg = valid ? "All ports connected" : "Some ports not connected";
    result.add2ResultString(msg+"\n");

    std::cout << msg.toStdString() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    return make_pair(start, end);
}

std::pair<tpoint, tpoint>
SyntaxCheckWorker::checkCycles(XSet componentSet,
                               Result &result) {
    auto start = std::chrono::high_resolution_clock::now();

    bool cycles = CombinatorialCycleDetector(componentSet);

    QString msg = cycles ? "Network contains cycles\n" : "No cycles in network\n";
    result.add2ResultString(msg);

    std::cout << msg.toStdString() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    return make_pair(start, end);
}

std::pair<tpoint, tpoint>
SyntaxCheckWorker::checkSymbolicTypes(XSet componentSet,
                               Result &result) {
    auto start = std::chrono::high_resolution_clock::now();

    SymbolicTypes(componentSet);

    QString msg = "Network symbolic types executed: check your messages.\n";
    result.add2ResultString(msg);

    std::cout << msg.toStdString() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    return make_pair(start, end);
}

void SyntaxCheckWorker::doProcessWork(const QString &json) {
    Q_UNUSED(json)
    std::cout << "[SyntaxCheckWorker::doProcessWork()] Not implemented." << std::endl;
}

