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

#include "symbolic-packet-set.h"

SymbolicPacketSet::SymbolicPacketSet() : values() {
}

void SymbolicPacketSet::greaterAs(interval_type b) {
    for (auto &packet : values) {
        for (auto &field : packet.fields) {
            SymbolicIntervalField *interval = dynamic_cast<SymbolicIntervalField*>(field.second.get());
            if (interval)
                interval->greaterAs(b);
            SymbolicAnyField *any = dynamic_cast<SymbolicAnyField*>(field.second.get());
            if (any)
                field.second = std::make_shared<SymbolicIntervalField>(b+1, std::numeric_limits<SymbolicIntervalField::interval_type>::max());
        }
    }
}

void SymbolicPacketSet::greaterEqualAs(interval_type b) {
    for (auto &packet : values) {
        for (auto &field : packet.fields) {
            SymbolicIntervalField *interval = dynamic_cast<SymbolicIntervalField*>(field.second.get());
            if (interval)
                interval->greaterEqualAs(b);
            SymbolicAnyField *any = dynamic_cast<SymbolicAnyField*>(field.second.get());
            if (any)
                field.second = std::make_shared<SymbolicIntervalField>(b, std::numeric_limits<SymbolicIntervalField::interval_type>::max());
        }
    }
}

void SymbolicPacketSet::lessEqualAs(interval_type b) {
    for (auto &packet : values) {
        for (auto &field : packet.fields) {
            SymbolicIntervalField *interval = dynamic_cast<SymbolicIntervalField*>(field.second.get());
            if (interval)
                interval->lessEqualAs(b);
            SymbolicAnyField *any = dynamic_cast<SymbolicAnyField*>(field.second.get());
            if (any)
                //field.second = std::make_shared<SymbolicIntervalField>(std::numeric_limits<SymbolicIntervalField::interval_type>::min(), b+1);
                field.second = std::make_shared<SymbolicIntervalField>(0, b+1);
        }
    }
}

void SymbolicPacketSet::lessAs(interval_type b) {
    for (auto &packet : values) {
        for (auto &field : packet.fields) {
            SymbolicIntervalField *interval = dynamic_cast<SymbolicIntervalField*>(field.second.get());
            if (interval)
                interval->lessAs(b);
            SymbolicAnyField *any = dynamic_cast<SymbolicAnyField*>(field.second.get());
            if (any)
                //field.second = std::make_shared<SymbolicIntervalField>(std::numeric_limits<SymbolicIntervalField::interval_type>::min(), b);
                field.second = std::make_shared<SymbolicIntervalField>(0, b);
        }
    }
}

void SymbolicPacketSet::negate() {
    std::vector<SymbolicPacket> before = std::move(values);
    for (auto &packet : before) {
        auto result = packet.negate();
        values.insert(values.end(), result.begin(), result.end());
    }
    //std::cout << "after negate there are " << values.size() << " packets" << std::endl;
}

void SymbolicPacketSet::print(std::ostream &out) const {
    out << "{";
    for (auto &packet : values) {
        if (&packet != &*values.begin())
            out << ",";
        out << packet;
    }
    out << "}";
}

void SymbolicPacketSet::updateHash()
{
    for (auto &v : values)
        v.updateHash();
}


