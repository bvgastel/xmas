/*********************************************************************
  *
  * Copyright (C) Guus Bonnema, 2015
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

#ifndef PARSEDXMASFUNCTION_H
#define PARSEDXMASFUNCTION_H

#include "simplestring.h"
#include "symbolic.h"
#include "parse-parsed-xmas-expression-interface.h"

class ParsedXMASFunction {
public:
    int refcount = 0;
    virtual ~ParsedXMASFunction() {}

    bool hasCondition = false;
    std::vector<SymbolicPacket> conditions;
    std::shared_ptr<ParsedXMASFunction> next; // if condition was not met

    std::map<bitpowder::lib::String, std::shared_ptr<ParsedXMASExpression>> fields; // function to apply for each field

    std::vector<SymbolicPacket> operator()(const std::vector<SymbolicPacket> &packet) const;

    virtual void printOldCSyntax(std::ostream &out, std::map<bitpowder::lib::String,int>& enumMap) const;
};

std::ostream &operator<<(std::ostream &out, const ParsedXMASFunction &c);


#endif // PARSEDXMASFUNCTION_H
