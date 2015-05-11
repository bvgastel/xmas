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

#ifndef PARSEPARSEDXMASEXPRESSIONINTERFACE_H
#define PARSEPARSEDXMASEXPRESSIONINTERFACE_H

#include "simplestring.h"
#include "symbolic.h"

class ParsedXMASExpression {
public:
    virtual ~ParsedXMASExpression() {}
    virtual std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const = 0;
    virtual void print(std::ostream &out) const = 0;
    virtual void printOldCSyntax(std::ostream &out, std::map<bitpowder::lib::String,int>& enumMap) const = 0;
};

std::ostream &operator <<(std::ostream &out, const ParsedXMASExpression &c);



#endif // PARSEPARSEDXMASEXPRESSIONINTERFACE_H

