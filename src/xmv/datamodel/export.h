#ifndef EXPORT_H
#define EXPORT_H

#include <set>
#include "simplestring.h"
#include "parser_json.h"
#include "symbolic.h"
#include "xmas.h"

void Export(std::ostream &out, std::set<XMASComponent *> allComponents, const bitpowder::lib::JSONData& globals);
void Export(std::set<XMASComponent*> allComponents, const bitpowder::lib::JSONData& globals);
bitpowder::lib::String ExportOldCStyleClause(const SymbolicPacket& packet, std::map<bitpowder::lib::String, int>& enumMap, bitpowder::lib::MemoryPool& mp);
bitpowder::lib::String ExportOldCStyle(XMASSource* source, std::map<bitpowder::lib::String, int>& enumMap, bitpowder::lib::MemoryPool& mp);
bitpowder::lib::String ExportOldCStyle(XMASSwitch* source, std::map<bitpowder::lib::String, int>& enumMap, bitpowder::lib::MemoryPool& mp);
bitpowder::lib::String ExportOldCStyle(std::set<XMASComponent*> allComponents, const bitpowder::lib::JSONData& globals, bitpowder::lib::MemoryPool& mp);

bitpowder::lib::String ExportClause(const SymbolicPacket& packet, bitpowder::lib::MemoryPool& mp);
bitpowder::lib::String Export(XMASSource* source, bitpowder::lib::MemoryPool& mp);
bitpowder::lib::String Export(XMASSwitch* sw, bitpowder::lib::MemoryPool& mp);
bitpowder::lib::String Export(XMASFunction* sw, bitpowder::lib::MemoryPool& mp);
bitpowder::lib::String Export(std::set<XMASComponent*> allComponents, const bitpowder::lib::JSONData& globals, bitpowder::lib::MemoryPool& mp);

#endif // EXPORT_H
