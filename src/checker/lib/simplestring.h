/**
Copyright 2010-2015 Bernard van Gastel, bvgastel@bitpowder.com.
This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bit Powder Libraries.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BP_STRING_H_
#define _BP_STRING_H_

#ifdef _MSC_VER

// To avoid compilation issues on Visual C++, use a standard std::string based implementation of String
#include <string>

class String : public std::string {
public:
    String(const std::string& str) : std::string(str) {
    }

    std::string stl() const {
        return *this;
    }

};

#else

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifdef __MINGW32__
// FIXME: why is strncasecmp redeclared for MINGW32 ??
//        it causes a warning: 'int _strnicmp(const char*, const char*, size_t)' redeclared without dllimport attribute: previous dllimport ignored [-Wattributes]
//        and the project builds without errors if the declaration below is commented out.
//int strncasecmp (const char *sz1, const char *sz2, size_t sizeMaxCompare);
#endif
}

#include <cctype>
#include <limits>
#include <cstddef>
#include <string>
#include "common.h"
#include "memorypool.h"

namespace bitpowder {
namespace lib {

class String;
class StringT;
class MemoryPool;

template <class FirstString>
class Substring;

// Basic 'string'
class StringOperation {
public:
    constexpr StringOperation(size_t _size) : _size(_size) {
    }
    size_t _size;
    inline size_t size() const {
        return _size;
    }
    virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const = 0;
    String operator()(MemoryPool *mp) const;
    StringT c_str(MemoryPool *mp) const;
    // use StaticMemoryPool<128> or something like that
    template <size_t N> String operator()(char (&buffer)[N]) const;
    template <size_t N> StringT c_str(char (&buffer)[N]) const;
    String operator()(char* buffer, std::size_t size) const;
    String to(char*& buffer, std::size_t& size) const;
    Substring<StringOperation> _substring(size_t offset, size_t maxLength = std::numeric_limits<size_t>::max()) const;
    /* some compile errors, like "lala" + String are harder to spot, as by autocasting this becomes "lala" + 1 -> "ala", instead of "lala"_S + String
    operator bool() {
        return _size() > 0;
    }
    */
    explicit operator std::string() const;
    static size_t writeToHelper(char * __restrict__ &buffer, size_t &bufferLength, size_t &offset, const char *src, size_t len);
    static size_t writeToHelper(char * __restrict__ &buffer, size_t &bufferLength, size_t &offset, const StringOperation &op, size_t max = std::numeric_limits<size_t>::max());
    static size_t writeToHelper(char * __restrict__ &buffer, size_t &bufferLength, size_t &offset, const String &str, size_t max = std::numeric_limits<size_t>::max());
};

// support for String methods
template <typename A, typename B>
struct CharOr;

template <class T>
struct Complement {
    T t;
    constexpr Complement(const T &t) : t(t) {
    }
    constexpr bool operator()(char in) const {
        return !t(in);
    }
    template <typename B>
    constexpr CharOr<Complement,B> operator+(const B &b) const {
        return CharOr<Complement,B>(*this, b);
    }
    constexpr T operator !() const {
        return t;
    }
};

template <typename A, typename B>
struct CharOr {
    A a;
    B b;
    constexpr CharOr() : a(), b() {
    }
    constexpr CharOr(const A &a, const B &b) : a(a), b(b) {
    }
    constexpr bool operator()(char in) const {
        return a(in) || b(in);
    }
    template <typename C>
    constexpr CharOr<CharOr<A,B>,C> operator+(const C &c) const {
        return CharOr< CharOr<A,B>, C>(*this, c);
    }
    constexpr Complement<CharOr> operator !() const {
        return Complement<CharOr>(*this);
    }
};

template <char m>
struct Char {
    constexpr inline bool operator()(char in) const {
        return in == m;
    }
    template <typename B>
    constexpr CharOr<Char,B> operator+(const B &b) const {
        return CharOr<Char,B>(*this, b);
    }
    constexpr Complement<Char> operator !() const {
        return Complement<Char>(*this);
    }
};

struct Alpha {
    constexpr inline bool operator()(char in) const {
        return ('a' <= in && in <= 'z') ||
                ('A' <= in && in <= 'Z');
    }
    template <typename B>
    constexpr CharOr<Alpha,B> operator+(const B &b) const {
        return CharOr<Alpha,B>(*this, b);
    }
    constexpr Complement<Alpha> operator !() const {
        return Complement<Alpha>(*this);
    }
};

struct Num {
    constexpr inline bool operator()(char in) const {
        return ('0' <= in && in <= '9');
    }
    template <typename B>
    constexpr CharOr<Num,B> operator+(const B &b) const {
        return CharOr<Num,B>(*this, b);
    }
    constexpr Complement<Num> operator !() const {
        return Complement<Num>(*this);
    }
};

struct Hex {
    constexpr inline bool operator()(char in) const {
        return ('0' <= in && in <= '9') ||
                ('a' <= in && in <= 'f') ||
                ('A' <= in && in <= 'F');
    }
    template <typename B>
    constexpr CharOr<Hex,B> operator+(const B &b) const {
        return CharOr<Hex,B>(*this, b);
    }
    constexpr Complement<Hex> operator !() const {
        return Complement<Hex>(*this);
    }
};

template <class T>
constexpr Complement<T> CharComplement(const T &t) {
    return Complement<T>(t);
}

#define C(c) Char<c>()

class StringOperationString;

inline int toUpper(int c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

inline int toLower(int c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

#define SMALL_MEM_COPY_LIMIT 128
// avoids function call, can be faster for small copies
inline void smallmemcpy(char * __restrict__ dst, const char * __restrict__ src, size_t length) {
    char *dstEnd = &dst[length];
    while (dst != dstEnd) {
        *dst = *src;
        ++dst; ++src;
    }
}

inline void memcpywrap(char * __restrict__ dst, const char * __restrict__ src, size_t length) {
    if (length < SMALL_MEM_COPY_LIMIT)
        smallmemcpy(dst, src, length);
    else
        memcpy(dst, src, length);
}

class HashedString;
class StableHashedString;

// the main string
class String {
protected:
    const char *ptr;
    size_t len;
public:
    constexpr static size_t UNKNOWN_LENGTH = std::numeric_limits<size_t>::max();
    constexpr inline String(const char *pointer, size_t len = UNKNOWN_LENGTH) : ptr(pointer), len(len < UNKNOWN_LENGTH ? len : pointer ? strlen(pointer) : 0) {
    }
    String(const std::string& str) : ptr(str.c_str()), len(str.length()) {
    }
    constexpr inline String() : ptr(nullptr), len(0) {
    }
    String(std::nullptr_t) : String() {
    }
    template <size_t N>
    constexpr inline String(const char (&str)[N]) : ptr(str), len(N-1) {
    }
    constexpr inline String(const char *begin, const char *end) : ptr(begin < end ? begin : nullptr), len(begin < end ? end-begin : 0) {
    }
    constexpr inline String(const String &c) : ptr(c.ptr), len(c.len) {
    }
    String &operator=(const String &str) {
        ptr = str.ptr;
        len = str.len;
        return *this;
    }
    std::string stl() const {
        return std::string(ptr, len);
    }
    explicit operator std::string() const {
        return this->stl();
    }

    void clear() {
        ptr = nullptr;
        len = 0;
    }
    typedef const char* const_iterator;
    inline const_iterator begin() const {
        return ptr;
    }
    inline const_iterator end() const {
        return ptr+len;
    }
    inline constexpr size_t length() const __attribute__ ((pure)) {
        return len;
    }
    inline constexpr size_t size() const __attribute__ ((pure)) {
        return len;
    }
    size_t _size() const __attribute__ ((pure)) {
        return len;
    }
    inline constexpr const char *pointer() const __attribute__ ((pure)) {
        return ptr;
    }
    // stable hashes are stable over execution runs and over multiple systems/installations
    unsigned int stableHash() const __attribute__ ((pure));
    unsigned int stableHashIgnoreCase() const __attribute__ ((pure));
    // in order to counter denial of service attacks, these hashes are only stable during a executing.
    unsigned int hash() const __attribute__ ((pure));
    unsigned int hashIgnoreCase() const __attribute__ ((pure));
    // It uses the randomizeStringHash to counter semi-collisions (last few bytes that are the same, so the same modulo is used)
    static unsigned int randomizeStringHash(unsigned int value);
    // in order to counter denial of service attacks, these hashes are only stable during a executing.
    inline bool empty() const __attribute__ ((pure)) {
        return !ptr || len == 0;
    }
    inline size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t l = std::min((size_t)(len - offset), bufferLength);
        memcpywrap(buffer, &ptr[offset], l);
        return l;
    }
    constexpr String substring(size_t pos) const __attribute ((pure)) {
        return pos >= length() ? String() : String(pointer() + pos, length() - pos);
    }
    constexpr String substring(size_t pos, size_t len) const __attribute ((pure)) {
        return pos >= length() ? String() : String(pointer() + pos, std::min(length() - pos, len));
    }
    template <size_t N>
    static size_t printf(const char (&str)[N], const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        int retval = snprintf(str, N, format, ap);
        va_end(ap);
        return retval;
    }
    bool startsWith(const String &str) const {
        if (str.length() > length())
            return false;
        return memcmp(pointer(), str.pointer(), str.length()) == 0;
    }
    bool startsWithIgnoreCase(const String &b) const {
        return len >= b.len && strncasecmp(ptr, b.ptr, b.len) == 0;
    }
    bool endsWith(const String &str) const {
        if (str.length() > length())
            return false;
        return memcmp(pointer()+length()-str.length(), str.pointer(), str.length()) == 0;
    }
    bool endsWithIgnoreCase(const String &b) const {
        return len >= b.len && strncasecmp(ptr+len-b.len, b.ptr, b.len) == 0;
    }
    String dir() const {
        String retval;
        rsplitOn(Char<'/'>(), retval);
        if (!retval.empty())
            return retval;
        // empty so the part after "/" was empty, this can mean two things:
        // first: /filename (in the root of the filesystem)
        // second: filename (in the current dir)
        if (length() > 0 && ptr[0] == '/')
            return "/";
        return ".";
    }
    String base() const {
        return rsplitOn(Char<'/'>(), nullptr);
    }
    String extension() const {
        String b = base();
        String retval = b.doRSplitOn(Char<'.'>());
        return b.empty() ? String() : retval;
    }

    void print() const {
        write(STDOUT_FILENO, ptr, len);
        write(STDOUT_FILENO, "\n", 1);
    }
    template <typename T>
    String span(const T &t, String *next = nullptr) const {
        const char *p = pointer();
        if (p) {
            for (size_t l = 0; l < len; ++l) {
                if (!t(p[l])) {
                    String retval = substring(0, l);
                    if (next)
                        *next = substring(l);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String span(const T &t, String &next) const {
        return span(t, &next);
    }
    template <typename T>
    String doSpan(const T &t) {
        return span<T>(t, this);
    }
    template <typename T>
    String rspan(const T &t, String *next = nullptr) const {
        const char *p = pointer();
        if (p) {
            for (ssize_t l = len-1; l >= 0; --l) {
                if (!t(p[l])) {
                    String retval = substring(l+1);
                    if (next)
                        *next = substring(0, l+1);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String rspan(const T &t, String &next) const {
        return rspan(t, &next);
    }
    template <typename T>
    String doRSpan(const T &t) {
        return rspan<T>(t, this);
    }
    String trim() const {
        String retval;

        static Char<' '> space;
        span(space, &retval);

        retval.doRSpan(space);
        return retval;
    }

    template <typename T>
    String splitOn(const T &t, String &next, char *delimPtr = nullptr) const {
        return splitOn(t, &next, delimPtr);
    }
    template <typename T>
    String splitOn(const T &t, String *next = nullptr, char *delimPtr = nullptr) const {
        const char *p = pointer();
        if (p) {
            for (size_t l = 0; l < len; ++l) {
                if (t(p[l])) {
                    String retval = substring(0, l);
                    if (delimPtr)
                        *delimPtr = p[l];
                    if (next)
                        *next = substring(l+1);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (delimPtr)
            *delimPtr = '\0';
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String doSplitOn(const T &t, char *delimPtr = nullptr) {
        return splitOn<T>(t, this, delimPtr);
    }

    template <typename T>
    String rsplitOn(const T &t, String &next, char *delimPtr = nullptr) const {
        return rsplitOn(t, &next, delimPtr);
    }
    template <typename T>
    String rsplitOn(const T &t, String *next = nullptr, char *delimPtr = nullptr) const {
        const char *p = pointer();
        if (p) {
            for (ssize_t l = len-1; l >= 0; --l) {
                if (t(p[l])) {
                    String retval = substring(l+1);
                    if (delimPtr)
                        *delimPtr = p[l];
                    if (next)
                        *next = substring(0, l);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (delimPtr)
            *delimPtr = '\0';
        if (next)
            *next = String();
        return retval;
    }
    template <typename T>
    String doRSplitOn(const T &t, char *delimPtr = nullptr) {
        return rsplitOn<T>(t, this, delimPtr);
    }


    String doSplit(const String &s) {
        return split(s, this);
    }
    String doSplitIgnoreCase(const String &s) {
        return splitIgnoreCase(s, this);
    }

    String split(const String &s, String &next) const {
        return split(s, &next);
    }
    String split(const String &s, String *next = nullptr) const {
        if (!s.empty() && s.length() <= length() && ptr) {
            const char *sPtr = s.pointer();
            char sFirst = sPtr[0];
            size_t sLen = s.length();
            ssize_t end = len - sLen;
            for (ssize_t l = 0; l <= end; ++l) {
                if (ptr[l] == sFirst && memcmp(&ptr[l], sPtr, sLen) == 0) {
                    String retval = substring(0, l);
                    if (next)
                        *next = substring(l+sLen);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }

    String splitIgnoreCase(const String &s, String &next) const {
        return splitIgnoreCase(s, &next);
    }
    String splitIgnoreCase(const String &s, String *next = nullptr) const {
        if (!s.empty() && s.length() <= length() && ptr) {
            const char *sPtr = s.pointer();
            //char sFirst = sPtr[0];
            size_t sLen = s.length();
            ssize_t end = len - sLen;
            for (ssize_t l = 0; l <= end; ++l) {
                if (/*ptr[l] == sFirst &&*/ strncasecmp(&ptr[l], sPtr, sLen) == 0) {
                    String retval = substring(0, l);
                    if (next)
                        *next = substring(l+sLen);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }
    String doRSplit(const String &s) {
        return rsplit(s, this);
    }
    String doRSplitIgnoreCase(const String &s) {
        return rsplitIgnoreCase(s, this);
    }
    String rsplit(const String &s, String &next) const {
        return rsplit(s, &next);
    }
    String rsplit(const String &s, String *next = nullptr) const {
        if (!s.empty() && s.length() <= length() && ptr) {
            const char *sPtr = s.pointer();
            size_t sLen = s.length();
            char sFirst = sPtr[0];
            for (ssize_t l = len-sLen; l >= 0; --l) {
                if (ptr[l] == sFirst && memcmp(&ptr[l], sPtr, sLen) == 0) {
                    String retval = substring(l+sLen);
                    if (next)
                        *next = substring(0, l);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }
    String rsplitIgnoreCase(const String &s, String *next = nullptr) const {
        if (!s.empty() && s.length() <= length() && ptr) {
            const char *sPtr = s.pointer();
            size_t sLen = s.length();
            for (ssize_t l = len-sLen; l >= 0; --l) {
                if (strncasecmp(&ptr[l], sPtr, sLen) == 0) {
                    String retval = substring(l+sLen);
                    if (next)
                        *next = substring(0, l);
                    return retval;
                }
            }
        }
        String retval = *this;
        if (next)
            *next = String();
        return retval;
    }
    bool contains(const String &str) const {
        String first = split(str);
        return first.length() < length();
    }
    bool containsIgnoreCase(const String &str) const {
        String second;
        String first = splitIgnoreCase(str, second);
        return first.length() < length();
    }
    char operator[](off_t off) const {
        if (length() == 0)
            throw std::exception();
        if (off < 0)
            off += length();
        off %= length();
        if (off >= 0)
            return ptr[off];
        return 0;
    }
    String unescape(MemoryPool *mp) const;
    String escape(MemoryPool *mp) const;

    String base64decode(MemoryPool *mp) const;

    bool equalsIgnoreCase(const String &b) const;

    int compareUsingVersion(const String &rhs) const;

    template <typename T, int _base = 10>
    inline T toNumber(String *next = nullptr) const {
        if (length() == 0) {
            if (next)
                *next = *this;
            return 0;
        }
        T result = 0;
        bool isNegative = ptr[0] == '-';
        int i = isNegative;
        ssize_t l = length();
        while (i < l &&
                ((ptr[i] >= '0' && ptr[i] <= '9')
                 || (_base > 10 && ptr[i] >= 'a' && ptr[i] <= 'z')
                 || (_base > 10 && ptr[i] >= 'A' && ptr[i] <= 'Z')
                )) {
            result *= _base;
            if ((ptr[i] >= '0') && (ptr[i] <= '9')) {
                result += ptr[i] - '0';
            } else if ((ptr[i] >= 'a') && (ptr[i] <= 'z')) {
                result += ptr[i] - 'a' + 10;
            } else if ((ptr[i] >= 'A') && (ptr[i] <= 'Z')) {
                result += ptr[i] - 'A' + 10;
            }
            i++;
        }
        if (next)
            *next = i > isNegative ? substring(i) : *this;
        return isNegative ? -result : result;
    }
    template <typename T, int _base = 10>
    inline T toNumber(String &next) const {
        return toNumber<T, _base>(&next);
    }

    template <typename T>
    inline T hexToNumber(String *next = nullptr) const {
        return toNumber<T, 16>(next);
    }
    template <typename T>
    inline T hexToNumber(String &next) const {
        return hexToNumber<T>(&next);
    }

    template <typename T, int _base = 64>
    inline T maxToNumber(String *next = nullptr) const {
        if (length() == 0) {
            if (next)
                *next = *this;
            return 0;
        }
        T result = 0;
        bool isNegative = ptr[0] == '-';
        int i = isNegative;
        ssize_t l = length();
        while (i < l &&
                ((ptr[i] >= '0' && ptr[i] <= '9')
                 || (_base > 10 && ptr[i] >= 'a' && ptr[i] <= 'z')
                 || (_base > 36 && ptr[i] >= 'A' && ptr[i] <= 'Z')
                 || (_base > 62 && ptr[i] == '_')
                 || (_base > 63 && ptr[i] == '.')
                )) {
            result *= _base;
            if ((ptr[i] >= '0') && (ptr[i] <= '9')) {
                result += ptr[i] - '0';
            } else if ((ptr[i] >= 'a') && (ptr[i] <= 'z')) {
                result += ptr[i] - 'a' + 10;
            } else if ((ptr[i] >= 'A') && (ptr[i] <= 'Z')) {
                result += ptr[i] - 'A' + 36;
            } else if (ptr[i] == '_') {
                result += 62;
            } else if (ptr[i] == '.') {
                result += 63;
            }
            i++;
        }
        if (next)
            *next = i > isNegative ? substring(i) : *this;
        return isNegative ? -result : result;
    }

    template <typename T>
    inline T maxToNumber(String &next) const {
        return maxToNumber<T, 64>(next);
    }

    template <typename T>
    inline T doToNumber() {
        return toNumber<T, 10>(this);
    }
    template <typename T>
    inline T doHexToNumber() {
        return hexToNumber<T>(this);
    }
    template <typename T>
    inline T doMaxToNumber() {
        return maxToNumber<T>(this);
    }

    String append(const String &a, MemoryPool &mp) const;

    template <class F>
    String filter(F &f, MemoryPool *mp) const {
        int newSize = 0;
        for (char c : *this)
            if (f(c))
                newSize++;
        char *retval = (char*)mp->alloc(newSize, sizeof(char));
        newSize = 0;
        for (char c : *this)
            if (f(c))
                retval[newSize++] = c;
        return String(retval, newSize);
    }

    StringOperationString op() const;
    operator StringOperationString () const;
    explicit constexpr operator StableHashedString () const;
    explicit operator HashedString () const;

    String operator()(MemoryPool *mp) const;
    StringT c_str(MemoryPool *mp) const;
    template <size_t N> String operator()(char (&buffer)[N]) const;
    template <size_t N> StringT c_str(char (&buffer)[N]) const;
    String operator()(char* buffer, std::size_t size) const;
    String to(char*& buffer, std::size_t& size) const;
    String copy() const;
};

class StringOperationString : public StringOperation {
    String str;
public:
    constexpr StringOperationString(const String &str) : StringOperation(str.size()), str(str) {
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        return str.writeTo(buffer, bufferLength, offset);
    }
};

inline StringOperationString String::op() const {
    return StringOperationString(*this);
}
inline String::operator StringOperationString () const {
    return op();
}

// terminating string
class StringT : public String {
public:
    inline StringT(const char *ptr, size_t len = std::numeric_limits<size_t>::max()) : String(ptr, len) {
    }
    inline StringT(const std::string &s) : String(s.c_str(), s.size()) {
    }
    inline StringT() : String() {
    }
    template <size_t N>
    inline StringT(const char (&str)[N]) : String(str) {
    }

    StringT c_str(MemoryPool *mp) const {
        return String::c_str(mp);
    }
    const char *c_str() const {
        return pointer();
    }
};

inline String String::operator()(MemoryPool *mp) const {
    return op()(mp);
}
inline StringT String::c_str(MemoryPool *mp) const {
    return op().c_str(mp);
}
template <size_t N> String String::operator ()(char (&buffer)[N]) const {
    return op()(buffer);
}
template <size_t N> StringT String::c_str(char (&buffer)[N]) const {
    return op().c_str(buffer);
}

// operators on strings
inline bool operator==(const String &lhs, const String &rhs) {
    size_t len = lhs.length();
    if (len != rhs.length())
        return false;
    return lhs.pointer() == rhs.pointer() || memcmp(lhs.pointer(), rhs.pointer(), len) == 0;
}
inline bool operator<(const String &lhs, const String &rhs) {
    size_t len = lhs.length();
    if (len != rhs.length())
        return len < rhs.length();
    return lhs.pointer() != rhs.pointer() && memcmp(lhs.pointer(), rhs.pointer(), len) < 0;
}
inline bool operator>(const String &lhs, const String &rhs) {
    size_t len = lhs.length();
    if (len != rhs.length())
        return len > rhs.length();
    return lhs.pointer() != rhs.pointer() && memcmp(lhs.pointer(), rhs.pointer(), len) > 0;
}

inline bool operator>=(const String &lhs, const String &rhs) {
    return !(lhs < rhs);
}
inline bool operator<=(const String &lhs, const String &rhs) {
    return !(lhs > rhs);
}
inline bool operator!=(const String &lhs, const String &rhs) {
    return !(lhs == rhs);
}

class StableHashedString {
    String str;
    unsigned int hashCache;
    friend class StringC;
    friend class StringTC;
    static constexpr unsigned int _hash(const String &str, unsigned int hash) {
        return str.length() == 0 ? hash : _hash(str.substring(1), hash*33 ^ toUpper(str[0]));
    }
    static constexpr unsigned int _hashRaw(const char *str, int length, unsigned int hash) {
        return length == 0 ? hash : _hashRaw(&str[1], length-1, hash*33 ^ toUpper(str[0]));
    }
    static constexpr unsigned int hash(const String &str) {
        return _hashRaw(str.pointer(), str.length(), 5381);
    }
public:
    constexpr StableHashedString(const String &str) : str(str), hashCache(hash(str)) {
    }
    constexpr StableHashedString(const char * value, size_t size) : str(value, size), hashCache(hash(str)) {
    }
    template <size_t N>
    constexpr inline StableHashedString(const char (&str)[N]) : str(String(str, N-1)), hashCache(hash(this->str)) {
    }
    inline unsigned int stableHashIgnoreCase() const {
        return hashCache;
    }
    StableHashedString &operator=(const String &str) {
        this->str = str;
        hashCache = str.stableHashIgnoreCase();
        return *this;
    }
    StableHashedString &operator=(const StableHashedString &str) {
        this->str = str.str;
        hashCache = str.hashCache;
        return *this;
    }
    template <size_t N>
    StableHashedString &operator=(const char (&str)[N]) {
        this->str = str;
        hashCache = hash(this->str);
        return *this;
    }
    explicit operator String&() {
        return str;
    }
    String* operator->() {
        return &str;
    }
    const String* operator->() const {
        return &str;
    }
    String &string() {
        return str;
    }
    operator const String&() const {
        return str;
    }
    const String &string() const {
        return str;
    }
    explicit operator HashedString () const;
    bool equalsIgnoreCase(const StableHashedString &b) const {
        return hashCache == b.hashCache && str.equalsIgnoreCase(b.str);
    }
};

// for std::less<StableHashedString>
inline bool operator<(const StableHashedString &lhs, const StableHashedString &rhs) {
    if (lhs.stableHashIgnoreCase() != rhs.stableHashIgnoreCase())
        return lhs.stableHashIgnoreCase() < rhs.stableHashIgnoreCase();
    return lhs.string() < rhs.string();
}
inline bool operator>(const StableHashedString &lhs, const StableHashedString &rhs) {
    if (lhs.stableHashIgnoreCase() != rhs.stableHashIgnoreCase())
        return lhs.stableHashIgnoreCase() > rhs.stableHashIgnoreCase();
    return lhs.string() > rhs.string();
}
// for StableHashedString
inline bool operator==(const StableHashedString &lhs, const StableHashedString &rhs) {
    if (lhs.stableHashIgnoreCase() != rhs.stableHashIgnoreCase())
        return false;
    return lhs.string() == rhs.string();
}

inline bool operator>=(const StableHashedString &lhs, const StableHashedString &rhs) {
    return !(lhs < rhs);
}
inline bool operator<=(const StableHashedString &lhs, const StableHashedString &rhs) {
    return !(lhs > rhs);
}
inline bool operator!=(const StableHashedString &lhs, const StableHashedString &rhs) {
    return !(lhs == rhs);
}

class HashedString {
    String str;
    unsigned int hashCache;
    friend class StringC;
    friend class StringTC;
public:
    HashedString(const StableHashedString &str) : str(str), hashCache(String::randomizeStringHash(str.stableHashIgnoreCase())) {
    }
    HashedString(const String &str) : str(str), hashCache(String::randomizeStringHash(str.stableHashIgnoreCase())) {
    }
    HashedString(const HashedString &str) : str(str.str), hashCache(str.hashCache) {
    }
    inline unsigned int hashIgnoreCase() const {
        return hashCache;
    }
    HashedString &operator=(const String &str) {
        this->str = str;
        hashCache = str.hashIgnoreCase();
        return *this;
    }
    HashedString &operator=(const StableHashedString &str) {
        this->str = str;
        hashCache = String::randomizeStringHash(str.stableHashIgnoreCase());
        return *this;
    }
    HashedString &operator=(const HashedString &str) {
        this->str = str.str;
        hashCache = str.hashCache;
        return *this;
    }
    template <size_t N>
    HashedString &operator=(const char (&str)[N]) {
        return *this = String(str);
    }
    explicit operator String&() {
        return str;
    }
    String* operator->() {
        return &str;
    }
    const String* operator->() const {
        return &str;
    }
    String &string() {
        return str;
    }
    operator const String&() const {
        return str;
    }
    const String &string() const {
        return str;
    }
    bool equalsIgnoreCase(const HashedString &b) const {
        return hashCache == b.hashCache && str.equalsIgnoreCase(b.str);
    }
};

// for std::less<HashedString>
inline bool operator<(const HashedString &lhs, const HashedString &rhs) {
    if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
        return lhs.hashIgnoreCase() < rhs.hashIgnoreCase();
    return lhs.string() < rhs.string();
}
inline bool operator>(const HashedString &lhs, const HashedString &rhs) {
    if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
        return lhs.hashIgnoreCase() > rhs.hashIgnoreCase();
    return lhs.string() > rhs.string();
}
// for HashedString
inline bool operator==(const HashedString &lhs, const HashedString &rhs) {
    if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
        return false;
    return lhs.string() == rhs.string();
}

inline bool operator>=(const HashedString &lhs, const HashedString &rhs) {
    return !(lhs < rhs);
}
inline bool operator<=(const HashedString &lhs, const HashedString &rhs) {
    return !(lhs > rhs);
}
inline bool operator!=(const HashedString &lhs, const HashedString &rhs) {
    return !(lhs == rhs);
}


// STRING CONTAINER

class StringC {
    String str;
    std::shared_ptr<char> container;
    unsigned int hashCache;
    friend class HashedString;
public:
    StringC(const String &string);
    StringC(const StringC &str) : str(str.str), container(str.container), hashCache(str.hashCache) {
    }
    StringC &operator=(const StringC &string) {
        str = string.str;
        container = string.container;
        hashCache = string.hashCache;
        return *this;
    }
    bool equalsIgnoreCase(const HashedString &b) const {
        return hashCache == b.hashCache && str.equalsIgnoreCase(b.str);
    }
    bool equalsIgnoreCase(const StringC &b) const {
        return hashCache == b.hashCache && str.equalsIgnoreCase(b.str);
    }
    inline unsigned int hashIgnoreCase() const {
        return hashCache;
    }
    operator String() const {
        return str;
    }
    operator String&() {
        return str;
    }
    String &string() {
        return str;
    }
    operator const String&() const {
        return str;
    }
    const String &string() const {
        return str;
    }
};

inline bool operator<(const StringC &lhs, const StringC &rhs) {
    if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
        return lhs.hashIgnoreCase() < rhs.hashIgnoreCase();
    const String &lh = lhs;
    const String &rh = rhs;
    size_t len = lh.length();
    if (len != rh.length())
        return len < rh.length();
    return lh.pointer() != rh.pointer() && memcmp(lh.pointer(), rh.pointer(), len) < 0;
}
inline bool operator==(const StringC &a, const StringC &b) {
    return a.hashIgnoreCase() == b.hashIgnoreCase() && a.string() == b.string();
}

class StringTC {
    StringT str;
    std::shared_ptr<char> container;
    unsigned int hashCache;
    friend class HashedString;
public:
    StringTC(const StringT &string, bool copy = false) {
        if (copy) {
            char *rawstring = new char[string.size()+1];
            memcpy(rawstring, string.pointer(), string.size());
            rawstring[string.size()] = '\0';
            container.reset(rawstring);
            str = StringT(rawstring, string.size());
        } else {
            str = string;
        }
        hashCache = str.hashIgnoreCase();
    }
    StringTC(const StringTC &str) : str(str.str), container(str.container), hashCache(str.hashCache) {
    }
    bool equalsIgnoreCase(const HashedString &b) const {
        return hashCache == b.hashCache && str.equalsIgnoreCase(b.str);
    }
    bool equalsIgnoreCase(const StringTC &b) const {
        return hashCache == b.hashCache && str.equalsIgnoreCase(b.str);
    }
    bool operator==(const StringTC &b) const {
        return hashCache == b.hashCache && str == b.str;
    }
    inline unsigned int hashIgnoreCase() const {
        return hashCache;
    }
    operator StringT() const {
        return str;
    }
    operator StringT&() {
        return str;
    }
    StringT &string() {
        return str;
    }
    operator const StringT&() const {
        return str;
    }
    const StringT &string() const {
        return str;
    }
};

// CONVERSIONS
class precision {
    unsigned short p;
public:
    precision(unsigned short p = 1) : p(p) {
    }
    unsigned short getPrecision() const {
        return p;
    }
};

class capital {
    bool capitals;
public:
    capital(bool capitals) : capitals(capitals) {
    }
    bool getCapitals() const {
        return capitals;
    }
};

template <class T,
         unsigned int bufferSize = std::numeric_limits<T>::digits10+1+std::numeric_limits<T>::is_signed,
         int base = 10>
class NumberToStringObject : public StringOperation {
    char buffer[bufferSize];
    unsigned char pos;
    template <int v>
    struct Int2Type {
        enum { value = v };
    };

    inline char chr(unsigned char r) {
        checkAssert(r < 64);
        return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_."[r];
    }

    void doConversion(T i, unsigned int precision, char buffer[], unsigned char &pos, Int2Type<false>) {
        while (i != 0) {
            T q = i / base;
            unsigned char r = i%base;
            buffer[--pos] = chr(r);
            i = q;
        }
        while (pos > 0 && (bufferSize - pos) < precision)
            buffer[--pos] = '0';
    }

    void doConversion(T i, unsigned int precision, char buffer[], unsigned char &pos, Int2Type<true>) {
        if (i >= 0) {
            doConversion(i, precision, buffer, pos, Int2Type<false>());
            return;
        }
        while (i != 0) {
            T q = i / base;
            unsigned char r = -(i % base);
            buffer[--pos] = chr(r);
            i = q;
        }
        while (pos > 0 && (bufferSize - pos) < precision)
            buffer[--pos] = '0';
        buffer[--pos] = '-';
    }

public:
    NumberToStringObject(T i, precision p = precision(1)) : StringOperation(0), /*buffer(),*/ pos(bufferSize) {
        // Potential helpful hints: http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
        doConversion(i, p.getPrecision(), buffer, pos, Int2Type<std::numeric_limits<T>::is_signed>());
        _size = bufferSize - pos;
    }
    virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t l = std::min((size_t)(_size - offset), bufferLength);
        const char *representation = &this->buffer[pos+offset];
        smallmemcpy(buffer, representation, l);
        return l;
    }
};

static inline char chrToHex(unsigned char r) {
    checkAssert(r < 16);
    return "0123456789abcdef"[r];
}

template <int N, int bufferSize = 2*N>
class ArrayToHexStringObject : public StringOperation {
    char buffer[bufferSize];
public:
    ArrayToHexStringObject(const unsigned char (&in)[N]) : StringOperation(0) {
        for (int i = 0; i < N; ++i) {
            buffer[_size++] = chrToHex(in[i] >> 4);
            buffer[_size++] = chrToHex(in[i] & 0xF);
        }
    }
    virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t l = std::min((size_t)(_size - offset), bufferLength);
        const char *representation = &this->buffer[offset];
        memcpywrap(buffer, representation, l);
        return l;
    }
};

class StringToHexStringObject : public StringOperation {
    String str;
public:
    StringToHexStringObject(const String& str) : StringOperation(str.size()*2), str(str) {
    }
    virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t l = std::min((size_t)(_size - offset), bufferLength);
        for (size_t i = 0; i < l; ++i) {
            size_t pos = offset + i;
            if (pos % 2 == 0) {
                buffer[i] = chrToHex(((unsigned char)str[pos/2]) >> 4);
            } else {
                buffer[i] = chrToHex(((unsigned char)str[pos/2]) & 0xF);
            }
        }
        return l;
    }
};

class StringFromHexStringObject : public StringOperation {
    String str;

    static int fromHex(unsigned char c) {
        if ((c >= '0') && (c <= '9')) {
            return c - '0';
        } else if ((c >= 'a') && (c <= 'f')) {
            return c - 'a' + 10;
        } else if ((c >= 'A') && (c <= 'F')) {
            return c - 'A' + 10;
        }
        return 0;
    }

public:
    StringFromHexStringObject(const String& str) : StringOperation(str.size()/2), str(str) {
    }
    virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t l = std::min((size_t)(_size - offset), bufferLength);
        for (size_t i = 0; i < l; ++i) {
            size_t pos = offset + i;
            buffer[i] = (fromHex(str[pos*2]) << 4) | fromHex(str[pos*2+1]);
        }
        return l;
    }
};


template <typename T>
class StringConversion {
public:
    typedef T ConversionResult;
};

template <>
class StringConversion<StringC> {
public:
    typedef String ConversionResult;
};

template <>
class StringConversion<StringTC> {
public:
    typedef StringT ConversionResult;
};

#define NUMBER_TO_STRING(type) \
template <> \
class StringConversion< type > { \
public: \
    typedef NumberToStringObject< type > ConversionResult; \
}

NUMBER_TO_STRING(char);
NUMBER_TO_STRING(unsigned char);
NUMBER_TO_STRING(short);
NUMBER_TO_STRING(unsigned short);
NUMBER_TO_STRING(int);
NUMBER_TO_STRING(unsigned int);
NUMBER_TO_STRING(long);
NUMBER_TO_STRING(unsigned long);
NUMBER_TO_STRING(long long);
NUMBER_TO_STRING(unsigned long long);

template <>
class StringConversion<std::string> {
public:
    class STLString : public StringOperation {
    public:
        const std::string &str;
        STLString(const std::string &str) : StringOperation(str.size()), str(str) {
        }

        virtual size_t _size() const {
            return str.length();
        }
        virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
            size_t l = std::min((size_t)(_size() - offset), bufferLength);
            const char *representation = &str.c_str()[offset];
            memcpywrap(buffer, representation, l);
            return l;
        }
    };

    typedef STLString ConversionResult;
};

class CharString : public StringOperation {
public:
    String str;
    CharString(const char *str) : StringOperation(0), str(str) {
        _size = this->str.length();
    }
    virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t l = std::min((size_t)(_size - offset), bufferLength);
        const char *representation = &str.pointer()[offset];
        memcpywrap(buffer, representation, l);
        return l;
    }
};

template <>
class StringConversion<const char*> {
public:
    typedef CharString ConversionResult;
};

template <>
class StringConversion<char*> {
public:
    typedef CharString ConversionResult;
};

template <size_t N>
class StringConversion<char[N]> {
public:
    class FixedCharString : public StringOperation {
    public:
        const char (&str)[N];
        constexpr FixedCharString(const char (&str)[N]) : StringOperation(N-1), str(str) {
        }
        virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
            size_t l = std::min((size_t)(N-1 - offset), bufferLength);
            const char *representation = &str[offset];
            memcpywrap(buffer, representation, l);
            return l;
        }
    };
    typedef FixedCharString ConversionResult;
};

template <>
class StringConversion<bool> {
public:
    class BoolString : public StringOperation {
        bool value;
    public:
        constexpr BoolString(bool value) : StringOperation(value ? 4 : 5), value(value) {
        }

        virtual size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
            size_t l = std::min((size_t)(_size - offset), bufferLength);
            //const char *representation = &(value ? "true" : "false")[offset];
            const char *representation = (value ? "true" : "false");
            smallmemcpy(buffer, representation, l);
            return l;
        }
    };
    typedef BoolString ConversionResult;
};

// conversion functions

template <class T>
typename StringConversion<T>::ConversionResult ToString(const T &t) {
    return typename StringConversion<T>::ConversionResult(t);
}

template <class T, typename... Args>
typename StringConversion<T>::ConversionResult ToString(const T &t, Args &&... p) {
    return typename StringConversion<T>::ConversionResult(t, std::forward<Args...>(p...));
}

template <class T>
NumberToStringObject<T,std::numeric_limits<T>::digits/4+1+std::numeric_limits<T>::is_signed,16> ToHexString(const T &t, precision p = precision()) {
    return NumberToStringObject<T,std::numeric_limits<T>::digits/4+1+std::numeric_limits<T>::is_signed,16>(t, p);
}

template <int N>
ArrayToHexStringObject<N> ArrayToHexString(const char (&in)[N]) {
    return ArrayToHexStringObject<N>((const unsigned char(&)[N])in);
}

template <int N>
ArrayToHexStringObject<N> ArrayToHexString(const unsigned char (&in)[N]) {
    return ArrayToHexStringObject<N>(in);
}

inline StringToHexStringObject ToHexString(const String& str) {
    return StringToHexStringObject(str);
}

inline StringFromHexStringObject FromHexString(const String& str) {
    return StringFromHexStringObject(str);
}

template <class T>
NumberToStringObject<T,std::numeric_limits<T>::digits/6+1+std::numeric_limits<T>::is_signed,64> ToMaxString(const T &t, precision p = precision()) {
    return NumberToStringObject<T,std::numeric_limits<T>::digits/6+1+std::numeric_limits<T>::is_signed,64>(t, p);
}

// OPERATIONS

template <class FirstString, class B>
class StringConcat : public StringOperation {
    const FirstString &a;
    const B b;
public:
    constexpr StringConcat(const FirstString &a, const B &b) : StringOperation(a.size() + b.size()), a(a), b(b) {
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t l = 0;
        size_t lengthA = a.size();
        if (offset < lengthA) {
            l += a.writeTo(buffer, bufferLength, offset);
            l += b.writeTo(buffer + l, bufferLength - l, 0);
        } else {
            l += b.writeTo(buffer + l, bufferLength - l, offset - lengthA);
        }
        return l;
    }
};

template <class B>
constexpr StringConcat<StringOperation, typename StringConversion<B>::ConversionResult> operator+(const StringOperation &a, const B &b) {
    return StringConcat<StringOperation, typename StringConversion<B>::ConversionResult>(a, ToString(b));
}

template <class B>
constexpr StringConcat<String, typename StringConversion<B>::ConversionResult> operator+(const String &a, const B &b) {
    return StringConcat<String, typename StringConversion<B>::ConversionResult>(a, ToString(b));
}

template <class FirstString, class State>
class StateConcat : public StringOperation {
    const FirstString & t;
    State st;
public:
    constexpr StateConcat(const FirstString &t, State st) : StringOperation(t.size()), t(t), st(st) {
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        return t.writeTo(buffer, bufferLength, offset);
    }
    operator State() const {
        return st;
    }
};

inline StateConcat<StringOperation, precision> operator+(const StringOperation &a, const precision &b) {
    return StateConcat<StringOperation, precision>(a, b);
}

inline StateConcat<StringOperation, capital> operator+(const StringOperation &a, const capital &b) {
    return StateConcat<StringOperation, capital>(a, b);
}

inline StateConcat<String, precision> operator+(const String &a, const precision &b) {
    return StateConcat<String, precision>(a, b);
}

inline StateConcat<String, capital> operator+(const String &a, const capital &b) {
    return StateConcat<String, capital>(a, b);
}

template <class FirstString, class State, class B>
constexpr StringConcat<StringOperation, typename StringConversion<B>::ConversionResult> operator+(const StateConcat<FirstString,State> &a, const B &b) {
    //return StringConcat<StringOperation, typename StringConversion<B>::ConversionResult>(a, ToString(b, (State)a));
    return {a, ToString(b, (State)a)};
}

/*
template <class FirstString, class State, class B>
constexpr StateConcat<StringConcat<StringOperation, typename StringConversion<B>::ConversionResult>, State> operator+(const StateConcat<FirstString,State> &a, const B &b) {
    return {StringConcat<StringOperation, typename StringConversion<B>::ConversionResult>(a, ToString(b, (State)a)), (State)a};
}
*/

template <class FirstString>
class StringMultiply : public StringOperation {
    const FirstString &a;
    const short i;
    ssize_t len;
public:
    StringMultiply(const FirstString &a, short i) : a(a), i(i), len(i*a._size()) {
    }
    inline size_t _size() const {
        return len;
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, off_t offset) const {
        if (offset >= len)
            return 0;
        short k = 0; // how many times the string is already written out
        if (offset < len) {
            ssize_t aSize = a._size();
            k += offset / aSize;
            size_t l = a.writeTo(buffer, bufferLength, offset % aSize);
            buffer = &buffer[l];
            bufferLength -= l;
        }
        // write once full
        size_t first = a.writeTo(buffer, bufferLength, 0);
        k++;
        size_t l = first;
        // repeat if needed
        for (; l < bufferLength && k < i; k++) {
            size_t len = std::min(first, bufferLength - l);
            memcpy(&buffer[l], buffer, len);
            l += len;
        }
        return l;
    }
};
template <class FirstString>
StringMultiply<FirstString> operator*(const FirstString &a, short i) {
    return StringMultiply<FirstString>(a, i);
}

template <class FirstString>
class Substring : public StringOperation {
    const FirstString &a;
    size_t offset;
public:
    Substring(const FirstString &a, size_t offset, size_t length = std::numeric_limits<size_t>::max()) : StringOperation(std::min(length, size_t(a.size()-offset))), a(a), offset(offset) {
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset2) const {
        return a.writeTo(buffer, std::min(bufferLength, _size), offset+offset2);
    }
};

template <class FirstString>
class EscapeOperation : public StringOperation {
    const FirstString &a;
public:
    constexpr EscapeOperation(const FirstString &a) : StringOperation(a.size()), a(a) {
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t size = a.writeTo(buffer, bufferLength, offset);
        char *end = buffer + size;
        for (char *it = buffer; it < end; ++it)
            if (*it == '\'')
                *it = '"';
        return size;
    }
};

template <class FirstString>
EscapeOperation<FirstString> Escape(const FirstString &str) {
    return EscapeOperation<FirstString>(str);
}

template <class FirstString>
class UppercaseOperation : public StringOperation {
    const FirstString &a;
public:
    constexpr UppercaseOperation(const FirstString &a) : StringOperation(a._size()), a(a) {
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t size = a.writeTo(buffer, bufferLength, offset);
        char *end = buffer + size;
        for (char *it = buffer; it < end; ++it)
            *it = toUpper(*it);
        return size;
    }
};

template <class FirstString>
UppercaseOperation<FirstString> ToUppercase(const FirstString &str) {
    return UppercaseOperation<FirstString>(str);
}

template <class FirstString>
class LowercaseOperation : public StringOperation {
    const FirstString &a;
public:
    constexpr LowercaseOperation(const FirstString &a) : StringOperation(a._size()), a(a) {
    }
    size_t writeTo(char * __restrict__ buffer, size_t bufferLength, size_t offset) const {
        size_t size = a.writeTo(buffer, bufferLength, offset);
        char *end = buffer + size;
        for (char *it = buffer; it < end; ++it)
            *it = toLower(*it);
        return size;
    }
};

template <class FirstString>
LowercaseOperation<FirstString> ToLowercase(const FirstString &str) {
    return LowercaseOperation<FirstString>(str);
}

/*
class StringEscape : public StringOperation {
    const StringOperation &a,
    off_t offset;
    size_t len;
    size_t extra;
    StringEscape(const StringOperation &a, off_t offset, size_t length) : a(a), offset(offset), len(std::min(length, size_t(a.length()-offset))), extra(0) {
        for (const_iterator it = begin(); it != end(); ++it) {
            switch (*it) {
            case '-':
            case '~':
            case '.':
            case '/':
            case ',':
            case '=':
            case '&':
            //case ' ':
                continue;
            }
            if ('a' <= *it && *it <= 'z')
                continue;
            if ('A' <= *it && *it <= 'Z')
                continue;
            if ('0' <= *it && *it <= '9')
                continue;
            extra++;
        }
    }
    ssize_t length() const {
        return len+extra;
    }
    size_t writeTo(char *buffer, size_t bufferLength, off_t offset2) const {
        return a.writeTo(buffer, std::min(bufferLength, len), offset+offset2);
    }
};
*/
// remaining

template <size_t N> String StringOperation::operator()(char (&buffer)[N]) const {
    size_t len = writeTo(buffer, N, 0);
    return String(buffer, len);
}

template <size_t N> StringT StringOperation::c_str(char (&buffer)[N]) const {
    size_t len = writeTo(buffer, N-1, 0);
    buffer[len] = '\0';
    return StringT(buffer, len);
}

template <size_t N>
const String ConstString(const char (&string)[N]) {
    return String((char*)string, N-1);
}

class StringNameCompare {
public:
    bool operator()(const String &lhs, const String &rhs);
};

template <class T>
struct lessIgnoreCaseAlfa : std::binary_function<T, T, bool>, std::binary_function<unsigned char,unsigned char,bool> {
    inline static const char* getPointer(const std::string &lhs) {
        return lhs.c_str();
    }
    inline static const char* getPointer(const String &lhs) {
        return lhs.pointer();
    }
    bool operator() (const unsigned char& a, const unsigned char& b) const {
        return toLower(a) < toLower(b);
    }
    bool operator() (const T & lhs, const T & rhs) const {
      return getPointer(lhs) != getPointer(rhs) && std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), *this);
    }
};

template <class T>
struct lessIgnoreCase : public lessIgnoreCaseAlfa<T> {
    bool operator() (const T & lhs, const T & rhs) const {
        size_t len = lhs.length();
        if (len != rhs.length())
            return len < rhs.length();
        return lessIgnoreCaseAlfa<T>::operator()(lhs, rhs);
    }
};

template <>
struct lessIgnoreCase<HashedString> : public lessIgnoreCase<String> {
    bool operator() (const HashedString & lhs, const HashedString & rhs) const {
        if (lhs.hashIgnoreCase() != rhs.hashIgnoreCase())
            return lhs.hashIgnoreCase() < rhs.hashIgnoreCase();
        return lessIgnoreCase<String>::operator()(lhs.string(), rhs.string());
    }
};

}
}

constexpr bitpowder::lib::String operator "" _S(const char * value, size_t size) {
    return {value, size};
}

constexpr bitpowder::lib::StableHashedString operator "" _HS(const char * value, size_t size) {
    return {value, size};
}

namespace std {
inline ostream& operator<< (ostream& out, const bitpowder::lib::String &str)
{
    out.write(str.pointer(), str.size());
    return out;
}
inline ostream& operator<< (ostream& out, const bitpowder::lib::StringOperation &op)
{
    int size = op._size;
    char buffer[size];
    op.writeTo(buffer, size, 0);
    out.write(buffer, size);
    return out;
}

template <>
struct hash<bitpowder::lib::String> : public unary_function<bitpowder::lib::String, size_t>
{
    size_t operator()(const bitpowder::lib::String &str) const {
        return str.hashIgnoreCase();
    }
};

template <>
struct hash<bitpowder::lib::HashedString> : public unary_function<bitpowder::lib::HashedString, size_t>
{
    size_t operator()(const bitpowder::lib::HashedString &str) const {
        return str.hashIgnoreCase();
    }
};

}
#endif // _MSC_VER

#endif
