/*
 *  exception.cpp
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 08-05-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#include "exception.h"
#include "simplestring.h"

namespace bitpowder {
namespace lib {

std::ostream &operator <<(std::ostream &out, const Exception &e) {
    out<< "Exception " << e.err << " (" << strerror(e.err) << ")";
    if (e.sourceFile)
        out << " [" << e.sourceFile << ":" << e.lineNo << "]";
    if (e.desc.size() > 0)
        out << " due to " << e.desc;
    return out;
}

String Exception::toString(MemoryPool& mp) const
{
    return ("Exception "_S + err + " (" + strerror(err) + ")" +
    " ["_S + sourceFile + ":" + lineNo + "]" +
    " due to "_S + desc)(mp);
}

}
}
