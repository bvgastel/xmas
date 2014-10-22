#include "parser.h"

std::ostream &std::operator<<(std::ostream &out, const bitpowder::lib::TokenBase *t) {
    if (t)
        t->print(out);
    else
        out << "(null)";
    return out;
}
