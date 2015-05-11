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

#ifndef SYMBOLIC_INTERVAL_H
#define SYMBOLIC_INTERVAL_H

#include <sstream>

#include "symbolic-interval-field.h"

struct SymbolicInterval {
    typedef SymbolicIntervalField::interval_type interval_type;
    interval_type min;
    interval_type max;
    SymbolicInterval(interval_type min = 0, interval_type max = 0) : min(min), max(max) {
    }

    void print(std::ostream& out) const {
        out << "[" << min << ".." << (max-1) << "]";
    }
    static SymbolicInterval all() {
        return {std::numeric_limits<interval_type>::min(), std::numeric_limits<interval_type>::max()};
    }
};

namespace std {
    inline std::ostream& operator<< (std::ostream& out, const SymbolicInterval &n)
    {
        n.print(out);
        return out;
    }
} // namespace std



#endif // SYMBOLIC_INTERVAL_H

