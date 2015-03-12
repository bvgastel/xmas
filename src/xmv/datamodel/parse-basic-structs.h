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

#ifndef PARSE_BASIC_STRUCTS_H
#define PARSE_BASIC_STRUCTS_H

#include "symbolic-interval-field.h"
#include "symbolic-enum-field.h"


struct Interval {
    typedef SymbolicIntervalField::interval_type interval_type;
    interval_type min;
    interval_type max;
    Interval(interval_type min = 0, interval_type max = 0) : min(min), max(max) {
    }

    void print(std::ostream& out) const {
        out << "[" << min << ".." << (max-1) << "]";
    }
    static Interval all() {
        return {std::numeric_limits<interval_type>::min(), std::numeric_limits<interval_type>::max()};
    }
};

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

struct SymbolicPacketSet {
    typedef SymbolicIntervalField::interval_type interval_type;
    std::vector<SymbolicPacket> values;
    SymbolicPacketSet();

    void greaterAs(interval_type b);
    void greaterEqualAs(interval_type b);
    void lessEqualAs(interval_type b);
    void lessAs(interval_type b);
    void negate();

    bool operator==(const SymbolicPacketSet &rhs) const {
        return values == rhs.values;
    }
    void print(std::ostream& out) const;
    void updateHash();
};




#endif // PARSE_BASIC_STRUCTS_H

