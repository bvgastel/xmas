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

#ifndef PARSESOURCEEXPRESSIONPARSERESULT_H
#define PARSESOURCEEXPRESSIONPARSERESULT_H

#include "simplestring.h"
#include "parse-specset.h"

class SourceExpressionParseResult {
    bool success;
    int pos;
    bitpowder::lib::String errorMessage;
    SpecSet retval;
public:
    SourceExpressionParseResult(int position, bitpowder::lib::String errorMessage)
        : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    SourceExpressionParseResult(SpecSet &&retval)
        : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    SpecSet& result() {
        return retval;
    }
    bitpowder::lib::String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};


#endif // PARSESOURCEEXPRESSIONPARSERESULT_H

