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

#ifndef PARSESPECSET_H
#define PARSESPECSET_H

#include "symbolic-packet-set.h"
#include "messagespec.h"

struct SpecSet {
    std::vector<std::tuple<SymbolicPacketSet,MessageSpec::Ref>> spec;

    void add(SymbolicPacketSet &&a) {
        spec.push_back(std::tuple<SymbolicPacketSet, MessageSpec::Ref>(std::move(a), nullptr));
    }

    void add(MessageSpec::Ref &&b) {
        std::get<1>(spec.back()) = std::move(b);
    }

    void addAnd(MessageSpec::Ref &&b) {
        std::get<1>(spec.back()) = std::get<1>(spec.back()) & std::move(b);
    }

    void addOr(MessageSpec::Ref &&b) {
        std::get<1>(spec.back()) = std::get<1>(spec.back()) | std::move(b);
    }

    void updateHash();
};

namespace std {
    /**
     * @brief operator<< output operator for specSet
     * @param out the output stream reference
     * @param specSet The SpecSpec to be printed
     * @return the output stream reference
     */
    inline std::ostream &operator<< (std::ostream &out, const SpecSet &specSet) {
        SymbolicPacketSet packetSet;
        MessageSpec::Ref specRef;
        for (auto specLine : specSet.spec) {
            std::tie(packetSet, specRef) = specLine;
            packetSet.print(out);
            out << specRef;
        }
        return out;
    }
} // namespace std

#endif // PARSESPECSET_H

