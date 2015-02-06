#ifndef EXPORT_H
#define EXPORT_H

#include <set>
#include "simplestring.h"
#include "parser_json.h"
#include "xmas.h"

void Export(std::set<XMASComponent*> allComponents, const bitpowder::lib::JSONData& globals);

#endif // EXPORT_H
