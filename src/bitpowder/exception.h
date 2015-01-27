/**
Copyright 2010-2014 Bernard van Gastel, bvgastel@bitpowder.com.
All rights reserved. This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

//#define DEBUG_EXCEPTIONS

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "common.h"
#include <string>
#include <iostream>

namespace bitpowder {
namespace lib {

class String;
class MemoryPool;

class Exception {
    std::string desc;
    int err;
    const char *sourceFile;
    int lineNo;
public:
    Exception(int err = 0) : desc(""), err(err), sourceFile(nullptr), lineNo(0) {
#ifdef DEBUG_EXCEPTIONS
        std::cerr << *this << std::endl;
#endif
    }
    Exception(int err, const char *sourceFile, const int lineNo) : desc(""), err(err), sourceFile(sourceFile), lineNo(lineNo) {
#ifdef DEBUG_EXCEPTIONS
        std::cerr << *this << std::endl;
#endif
    }
    Exception(const char *desc, int err = 0, const char *sourceFile = nullptr, const int lineNo = 0) : desc(desc), err(err), sourceFile(sourceFile), lineNo(lineNo) {
#ifdef DEBUG_EXCEPTIONS
        std::cerr << *this << std::endl;
#endif
    }
    Exception(const std::string &desc, int err = 0, const char *sourceFile = nullptr, const int lineNo = 0) : desc(desc), err(err), sourceFile(sourceFile), lineNo(lineNo) {
#ifdef DEBUG_EXCEPTIONS
        std::cerr << *this << std::endl;
#endif
    }

    Exception(const Exception &e) : desc(e.desc), err(e.err), sourceFile(e.sourceFile), lineNo(e.lineNo) {
    }
    Exception &operator=(const Exception &e) {
        desc = e.desc;
        err = e.err;
        sourceFile = e.sourceFile;
        lineNo = e.lineNo;
        return *this;
    }

    inline operator const char *() const {
        return strerror(err);
    }

    inline operator int() const {
        return err;
    }

    const char *description() const {
        return desc.c_str();
    }

    friend std::ostream &operator <<(std::ostream &out, const Exception &e);

    const char *getSourceFile() const
    {
        return sourceFile;
    }
    int getLineNumber() const
    {
        return lineNo;
    }

    String toString(MemoryPool &mp) const;
};

std::ostream &operator <<(std::ostream &out, const Exception &e);

}
}

#define checkAssert(expression) { if (!(expression)) throw bitpowder::lib::Exception(0,  __FILE__, __LINE__); }
#define checkRetval(expression) { if (expression) throw bitpowder::lib::Exception(errno, __FILE__, __LINE__); }

#endif

