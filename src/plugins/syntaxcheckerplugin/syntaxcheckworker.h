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

#ifndef SYNTAXCHECKWORKER_H
#define SYNTAXCHECKWORKER_H

#include "workerinterface.h"
#include "commoninterface.h"
#include "xmas.h"
#include "simplestring.h"
#include "result.h"

class SyntaxCheckWorker : public WorkerInterface
{
    Q_OBJECT
public:
    SyntaxCheckWorker(QObject *parent = 0);
    virtual ~SyntaxCheckWorker();


protected:
    void reportTimer(tpoint start, tpoint end, QString name, Result &result);

    std::tuple<tpoint, tpoint, XSet> convertComponentMap2Set(XMap componentMap);

    std::pair<tpoint, tpoint> checkSyntax(XMap componentMap, Result &result, QString stepName);

    std::pair<tpoint, tpoint> checkCycles(XSet componentSet, Result &result, QString stepName);

    std::pair<tpoint, tpoint> checkSymbolicTypes(XSet componentSet, Result &result, QString stepName);

public slots:
    void doProcessWork(const QString &json) override;
    void doThreadWork(const QString &json) override;
    void doWork(XMap &componentMap) override;


signals:
    void resultReady(const Result &result);

private:
    bool extractSuccess(Result &result);
};


#endif // SYNTAXCHECKWORKER_H
