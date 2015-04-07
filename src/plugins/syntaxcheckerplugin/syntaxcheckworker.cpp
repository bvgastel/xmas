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

#include <QList>

#include "commoninterface.h"
#include "cycle.h"
#include "parse.h"
#include "syntaxcheckworker.h"
#include "result.h"

SyntaxCheckWorker::SyntaxCheckWorker(QObject *parent) : WorkerInterface(parent) {

}

SyntaxCheckWorker::~SyntaxCheckWorker() {

}

// Never tested yet. Not implemented.
void SyntaxCheckWorker::doProcessWork(const QString &json) {
    Q_UNUSED(json)
    std::cout << "[SyntaxCheckWorker::doProcessWork()] Not implemented." << std::endl;
}

// Never tested yet.
void SyntaxCheckWorker::doThreadWork(const QString &json) {

    std::string networkName = "syntax checker network";
    std::string basePath;
    std::shared_ptr<XProject> project = std::make_shared<XMASProject>(json.toStdString(), networkName, basePath);
    doWork(project);
}

void SyntaxCheckWorker::doWork(std::shared_ptr<XProject> project) {
    Result result;
    bool success;
    tpoint start, end, nextstart, nextend;
    QString stepName;

    XMap componentMap = project->getRootNetwork()->getComponentMap();
    std::set<XMASComponent *> componentSet;
    project->getRootNetwork()->getComponentSet(componentSet);
    success = true;

    if (success) {
        // STEP 1: check if the topology is well formed
        stepName = "syntax check for valid ports";
        std::tie(nextstart, nextend) = checkSyntax(componentMap, result, stepName);
        end = nextend;
        reportTimer(nextstart, nextend, stepName, result);
        success = success && extractSuccess(result);
    }

    if (success) {
        // STEP 2: check for cycles
        stepName = "check for cycles";
        std::tie(nextstart, nextend) = checkCycles(componentSet, result, stepName);
        end = nextend;
        reportTimer(nextstart, nextend, stepName, result);
        success = success && extractSuccess(result);
    }

    if (success) {
        // STEP 3: check symbolic types
        stepName = "check for symbolic types";
        std::tie(nextstart, nextend) = checkSymbolicTypes(componentSet, result, stepName);
        end = nextend;
        reportTimer(nextstart, nextend, stepName, result);
        success = success && extractSuccess(result);
    }

    // Final STEP: Reporting final timers
    stepName = "Complete check duration";
    reportTimer(start, end, stepName, result);
    emit resultReady(result);
}

std::pair<tpoint, tpoint>
SyntaxCheckWorker::checkSyntax(XMap componentMap,
                               Result &result,
                               QString stepName) {
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
    QString msg;
    if (valid) {
        msg = QString("All ports connected");
        result.addStep(stepName, msg);
    } else {
        msg = "Some ports not connected";
        result.addError(stepName, msg, QString());
    }

    std::cout << msg.toStdString() << std::endl;


    auto end = std::chrono::high_resolution_clock::now();

    return make_pair(start, end);
}

std::pair<tpoint, tpoint>
SyntaxCheckWorker::checkCycles(XSet componentSet,
                               Result &result,
                               QString stepName) {
    auto start = std::chrono::high_resolution_clock::now();

    bool cycles = CombinatorialCycleDetector(componentSet);

    QString msg;
    if (cycles) {
        msg = QString("Network contains cycles\n");
        result.addError(stepName, msg, QString());
    } else {
        msg = "No cycles in network\n";
        result.addStep(stepName, msg);
    }
    std::cout << msg.toStdString() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    return make_pair(start, end);
}

/**
 * @brief SyntaxCheckWorker::checkSymbolicTypes
 *
 * This check will never return an error. All errors are in std::cout or std::cerr
 *
 * @param componentSet
 * @param result
 * @return
 */
std::pair<tpoint, tpoint>
SyntaxCheckWorker::checkSymbolicTypes(XSet componentSet,
                               Result &result, QString stepName) {
    auto start = std::chrono::high_resolution_clock::now();

    SymbolicTypes(componentSet);


    QString msg = "Network symbolic types executed: check your messages.\n";
    result.addStep(stepName, msg);

    std::cout << msg.toStdString() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    return make_pair(start, end);
}

bool SyntaxCheckWorker::extractSuccess(Result &result) {
    bool success = true;
    for (auto e : result.errorList()) {
        success = success && !e->m_error;
    }
    return success;
}

void SyntaxCheckWorker::reportTimer(tpoint start, tpoint end, QString name, Result &result) {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    QString duration_qstr = name + QString(" in \t")+duration+" ms";
    result.addStep(name, duration_qstr);
    std::cout << duration_qstr.toStdString() << std::endl;
}


