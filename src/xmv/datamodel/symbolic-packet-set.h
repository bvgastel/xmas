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

namespace std {
inline std::ostream& operator<< (std::ostream& out, const SymbolicPacketSet &n)
{
    n.print(out);
    return out;
}

} // namespace std




#endif // PARSE_BASIC_STRUCTS_H

