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

#ifndef SYMBOLICENUM_H
#define SYMBOLICENUM_H

#include "symbolic-enum-field.h"

struct Enum {
    std::vector<SymbolicEnumField::Type> values;
    Enum() : values() {
    }
    bool operator==(const Enum &n) const {
        return values == n.values;
    }
    void print(std::ostream& out) const {
        out << "{";
        for (auto &v : values) {
            out << v << " ";
        }
        out << "}";
    }
};



#endif // SYMBOLICENUM_H

