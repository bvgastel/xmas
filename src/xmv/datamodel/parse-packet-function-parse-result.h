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

#ifndef PARSEPACKETFUNCTIONPARSERESULT_H
#define PARSEPACKETFUNCTIONPARSERESULT_H

#include "parsed-xmas-function.h"

class PacketFunctionParseResult {
    bool success;
    int pos;
    bitpowder::lib::String errorMessage;
    std::shared_ptr<ParsedXMASFunction> retval;
public:
    PacketFunctionParseResult(int position, bitpowder::lib::String errorMessage)
        : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    PacketFunctionParseResult(std::shared_ptr<ParsedXMASFunction> &&retval)
        : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    std::shared_ptr<ParsedXMASFunction> result() {
        return retval;
    }
    bitpowder::lib::String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};



#endif // PARSEPACKETFUNCTIONPARSERESULT_H

