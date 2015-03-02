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

#include <chrono>

#include "xmas.h"
#include "simplestring.h"
#include "../interfaces/result.h"


class SyntaxCheckWorker : public QObject
{
    Q_OBJECT
public:
    SyntaxCheckWorker(QObject *parent = 0) : QObject(parent) {

    }

    virtual ~SyntaxCheckWorker() {

    }

private:
    std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>
    checkSyntax(std::map<bitpowder::lib::String, XMASComponent *> componentMap,
                Result &result);

public slots:
    void doWork(const QString &json);

signals:
    void resultReady(const Result &result);
};


#endif // SYNTAXCHECKWORKER_H
