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

#include "exception.h"
#include "parsed-xmas-function.h"

std::vector<SymbolicPacket> ParsedXMASFunction::operator()(const std::vector<SymbolicPacket> &packets) const
{
    std::vector<SymbolicPacket> retval;
    std::vector<SymbolicPacket> current;
    if (hasCondition) {
        std::vector<SymbolicPacket> next;
        next.insert(next.end(), packets.begin(), packets.end());
        for (auto &condition : conditions) {
            std::vector<SymbolicPacket> newNext;
            for (auto &packet : next) {
                packet.getDifference(condition, [&newNext,this](SymbolicPacket &&p) {
                    //std::cout << "difference between " << packet << " and " << funcPacket << " -> " << p << std::endl;
                    newNext.emplace_back(std::move(p));
                    //retval.push_back(p);
                });
            }
            next = std::move(newNext);
        }

        if (!next.empty()) {
            if (!this->next)
                throw bitpowder::lib::Exception("no else part defined");
            retval = std::move(this->next->operator()(next));
        }

        for (auto &packet : packets) {
            for (auto &condition : conditions) {
                auto result = packet.getIntersection(condition);
                current.insert(current.end(), result.begin(), result.end());
            }
        }
    } else {
        current = packets;
    }

    //std::cout << "ParsedXMASFunction apply function" << std::endl;
    std::vector<SymbolicPacket> currentRetval;
    currentRetval.push_back(SymbolicPacket());
    for (auto& f : fields) {
        std::vector<SymbolicPacket> before = std::move(currentRetval);
        for (auto &c : current) {
            auto result = (*f.second)(c);
            for (auto& r : result) {
                for (auto& b : before) {
                    SymbolicPacket p(b);
                    p.fields[f.first] = r;
                    currentRetval.push_back(p);
                }
            }
        }
    }
    for (auto &c : currentRetval)
        c.updateHash();
    retval.insert(retval.end(), currentRetval.begin(), currentRetval.end());
    return retval;
}

void ParsedXMASFunction::printOldCSyntax(std::ostream& out, std::map<bitpowder::lib::String, int>& enumMap) const
{
    if (hasCondition) {
        out << "if (";
        bool first = true;
        for (const auto& condition : conditions) {
            if (!first)
                out << " || ";
            //out << condition;
            condition.printOldCSyntax(out, enumMap);
            first = false;
        }
        out << ") {";
    }
    bool first = true;
    for (const auto& field : fields) {
        if (!first)
            out << ";\\r\\n";
        out << "ret_" << field.first << " = ";
        field.second->printOldCSyntax(out, enumMap);
        first = false;
    }
    out << ";";
    if (next) {
        out << "} else {";
        next->printOldCSyntax(out, enumMap);
    }
    if (hasCondition)
        out << "}";
}



std::ostream &operator<<(std::ostream &out, const ParsedXMASFunction &c)
{
    if (c.hasCondition) {
        out << "if ";
        bool first = true;
        for (const auto& condition : c.conditions) {
            if (!first)
                out << " || ";
            out << condition;
            first = false;
        }
        out << " then ";
    }
    bool first = true;
    for (const auto& field : c.fields) {
        if (!first)
            out << ", ";
        out << field.first << " := " << *field.second;
        first = false;
    }
    if (c.next) {
        out << " else ";
        out << *c.next;
    }
    return out;
}
