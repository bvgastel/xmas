/*
 *  string.cpp
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 21-05-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#include "simplestring.h"
#include <random>

/* djb2 algorithm */
/*
unsigned int strhash(const char *str) {
    unsigned char c;
    unsigned int hash = 5381;
    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c;
    return hash;
    //return *str == 0 ? 1 : *str;
}
*/

char fromDigit(char tmp);
char toDigit(unsigned char tmp);

/*
const char *base(const char *str) {
    const char *last = str ? strrchr(str, '/') : NULL;
    return last ? &last[1] : str;
}

const char *extension(const char *str) {
    str = base(str);
    const char *last = str ? strrchr(str, '.') : NULL;
    return last ? &last[1] : NULL;
}

char *strsep2(char **s, const char *delim, char *seperator) {
    char *retval = *s;
    size_t pos = strcspn(*s, delim);
    if (seperator != NULL)
        *seperator = (*s)[pos];
    if ((*s)[pos] == '\0') {
        *s = NULL;
    } else {
        (*s)[pos] = '\0';
        *s = (*s)[pos+1] ? &(*s)[pos+1] : NULL;
    }
    return retval;
}

char asciHexToChar(const char* hex) {
    return digit(hex[0]) << 4 | digit(hex[1]);
}

void unescapeString(char *queryString) {
    if (queryString == NULL)
        return;
    char *src = queryString;
    // faster loop till a % is encounterd
    while (*src != '\0' && *src != '%') {
        if (*src == '+')
            *src = ' ';
        src++;
    }
    char *dst = src;
    while (*src != '\0') {
        // convert '+' to a space
        if (*src == '+')
            *dst = ' ';
        // covert something with % to their real value
        else if (*src == '%' && *(src+1) != 0 && *(src+2) != 0) {
            *dst = digit(src[1]) << 4 | digit(src[2]);
            src+=2;
            // ohterwise copy character
        } else
            *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
}
*/

unsigned char fromDigit(unsigned char tmp) {
    if (tmp >= 'A' && tmp <= 'F')
        return 10 + tmp - 'A';
    else if (tmp >= 'a' && tmp <= 'f')
        return 10 + tmp - 'a';
    else if (tmp >= '0' && tmp <= '9')
        return tmp - '0';
    return 0;
}

char toDigit(unsigned char value) {
    return "0123456789ABCDEF"[value];
}

namespace bitpowder {
namespace lib {
/*
 * Hashing: good overview: http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
 *
    // SuperFastHash: http://www.azillionmonkeys.com/qed/hash.html BUT "SuperFastHash has very poor collision properties, which have been documented elsewhere"
    // Murmur2: http://www.team5150.com/~andrew/noncryptohashzoo/Murmur2.html
    inline unsigned int Murmur2( const unsigned char *key, unsigned int len, unsigned int seed ) {
            const unsigned int m = 0x5bd1e995;
            const unsigned char r = 24;
            unsigned int h = len + seed;
            const unsigned char * data = (const unsigned char *)key;

            for ( ; len >= 4; len -= 4, data += 4 ) {
                    unsigned int k = *(unsigned int *)data * m;
                    k ^= k >> r;
                    k *= m;
                    h = ( h * m ) ^ k;
            }

            switch ( len ) {
                    case 3: h ^= data[2] << 16;
                    case 2: h ^= data[1] << 8;
                    case 1: h ^= data[0];
                                    h *= m;
                    default:;
            }

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;
            return h;
    }
    typedef u_int32_t u32;
    typedef u_int8_t u8;
    typedef u_int64_t u64;
    // CrapWow: http://www.team5150.com/~andrew/noncryptohashzoo/CrapWow.html
    inline u32  CrapWow( const u8 *key, u32 len, u32 seed ) {
    #if !defined(__LP64__) && !defined(_MSC_VER) && ( defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) )
            // esi = k, ebx = h
            u32 hash;
            asm(
                    "leal 0x5052acdb(%%ecx,%%esi), %%esi\n"
                    "movl %%ecx, %%ebx\n"
                    "cmpl $8, %%ecx\n"
                    "jb DW%=\n"
            "QW%=:\n"
                    "movl $0x5052acdb, %%eax\n"
                    "mull (%%edi)\n"
                    "addl $-8, %%ecx\n"
                    "xorl %%eax, %%ebx\n"
                    "xorl %%edx, %%esi\n"
                    "movl $0x57559429, %%eax\n"
                    "mull 4(%%edi)\n"
                    "xorl %%eax, %%esi\n"
                    "xorl %%edx, %%ebx\n"
                    "addl $8, %%edi\n"
                    "cmpl $8, %%ecx\n"
                    "jae QW%=\n"
            "DW%=:\n"
                    "cmpl $4, %%ecx\n"
                    "jb B%=\n"
                    "movl $0x5052acdb, %%eax\n"
                    "mull (%%edi)\n"
                    "addl $4, %%edi\n"
                    "xorl %%eax, %%ebx\n"
                    "addl $-4, %%ecx\n"
                    "xorl %%edx, %%esi\n"
            "B%=:\n"
                    "testl %%ecx, %%ecx\n"
                    "jz F%=\n"
                    "shll $3, %%ecx\n"
                    "movl $1, %%edx\n"
                    "movl $0x57559429, %%eax\n"
                    "shll %%cl, %%edx\n"
                    "addl $-1, %%edx\n"
                    "andl (%%edi), %%edx\n"
                    "mull %%edx\n"
                    "xorl %%eax, %%esi\n"
                    "xorl %%edx, %%ebx\n"
            "F%=:\n"
                    "leal 0x5052acdb(%%esi), %%edx\n"
                    "xorl %%ebx, %%edx\n"
                    "movl $0x5052acdb, %%eax\n"
                    "mull %%edx\n"
                    "xorl %%ebx, %%eax\n"
                    "xorl %%edx, %%esi\n"
                    "xorl %%esi, %%eax\n"
                    : "=a"(hash), "=c"(len), "=S"(len), "=D"(key)
                    : "c"(len), "S"(seed), "D"(key)
                    : "%ebx", "%edx", "cc"
            );
            return hash;
    #else
            #define cwfold( a, b, lo, hi ) { p = (u32)(a) * (u64)(b); lo ^= (u32)p; hi ^= (u32)(p >> 32); }
            #define cwmixa( in ) { cwfold( in, m, k, h ); }
            #define cwmixb( in ) { cwfold( in, n, h, k ); }

            const u32 m = 0x57559429, n = 0x5052acdb, *key4 = (const u32 *)key;
            u32 h = len, k = len + seed + n;
            u64 p;

            while ( len >= 8 ) { cwmixb(key4[0]) cwmixa(key4[1]) key4 += 2; len -= 8; }
            if ( len >= 4 ) { cwmixb(key4[0]) key4 += 1; len -= 4; }
            if ( len ) { cwmixa( key4[0] & ( ( 1 << ( len * 8 ) ) - 1 ) ) }
            cwmixb( h ^ (k + n) )
            return k ^ h;
    #endif
    }
    */
/*
//////// BEGIN MurmurHash3_x86_32 from https://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
#define FORCE_INLINE inline __attribute__((always_inline))
inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}
inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}
#define ROTL32(x,y)     rotl32(x,y)
#define ROTL64(x,y)     rotl64(x,y)
#define BIG_CONSTANT(x) (x##LLU)
//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here
FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i )
{
  return p[i];
}
FORCE_INLINE uint64_t getblock64 ( const uint64_t * p, int i )
{
  return p[i];
}
//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche
FORCE_INLINE uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}
//----------
FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}
//-----------------------------------------------------------------------------
void MurmurHash3_x86_32 ( const void * key, int len,
                          uint32_t seed, void * out )
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

  for(int i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch(len & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  *(uint32_t*)out = h1;
}
//////// END MurmurHash3_x86_32
*/
unsigned int String::stableHash() const {
    /* djb2a algorithm */
    unsigned int hash = 5381;
    const unsigned char *last = (const unsigned char *)end();
    const unsigned char *current = (const unsigned char *)begin();
    while (current < last) {
        unsigned char c = *current++;
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}
unsigned int String::stableHashIgnoreCase() const {
    /* djb2a algorithm */
    unsigned int hash = 5381;
    const unsigned char *last = (const unsigned char *)end();
    const unsigned char *current = (const unsigned char *)begin();
    while (current < last) {
        unsigned char c = toUpper(*current++);
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

template <typename T>
T rotate_r(T value, int bits) {
    return (value >> ((-bits) & (sizeof(T)*8-1))) | (value << bits);
}

unsigned int String::randomizeStringHash(unsigned int value) {
    constexpr unsigned int bits = sizeof(value)*8;
    static unsigned int xorValue = std::random_device()();
    static unsigned int rotate = xorValue % bits;
    return rotate_r(value, rotate) ^ xorValue;
}

unsigned int String::hashIgnoreCase() const {
    return randomizeStringHash(stableHashIgnoreCase());
}
unsigned int String::hash() const {
    return randomizeStringHash(stableHash());
}
/*
 StringConcat operator+(const StringOperation &a, const StringOperation &b) {
 return StringConcat(a, b);
 }
 */



/*
 StringConstConcat operator+(const DataBase &a, const std::string &b) {
 return StringConstConcat(a, b);
 }

 StringConst2Concat operator+(const std::string &a, const DataBase &b) {
 return StringConst2Concat(a, b);
 }
 */

String String::unescape(MemoryPool *mp) const {
    if (empty())
        return String();
    unsigned char *dst = (unsigned char*)mp->alloc(len);
    unsigned int dst_i = 0;
    unsigned int ptr_i = 0;
    while (ptr_i < len) {
        // convert '+' to a space
        if (ptr[ptr_i] == '+')
            dst[dst_i] = ' ';
        // covert something with % to their real value
        else if (ptr[ptr_i] == '%' && ptr_i+2 < len) {
            dst[dst_i] = fromDigit((unsigned char)ptr[ptr_i+1]) << 4 | fromDigit((unsigned char)ptr[ptr_i+2]);
            ptr_i += 2;
        } else
            dst[dst_i] = ptr[ptr_i];
        dst_i++;
        ptr_i++;
    }
    return String((char*)dst, dst_i);
}

//a-z, A-Z, 0-9, '-._~/'
String String::escape(MemoryPool *mp) const {
    if (empty())
        return String();
    // count the needed extra charachters
    int extra = 0;
    for (const_iterator it = begin(); it != end(); ++it) {
        switch (*it) {
        case '-':
        case '~':
        case '.':
        case '/':
        case ',':
        case '=':
        case '&':
        case ':':
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
    // allocate
    char *dst = (char*)mp->alloc(len+2*extra);
    // really convert
    int i = 0;
    for (const_iterator it = begin(); it != end(); ++it) {
        switch (*it) {
        case '-':
        case '~':
        case '.':
        case '/':
        case ',':
        case '=':
        case '&':
        case ':':
            dst[i++] = *it;
            continue;
            /*
            case ' ':
            dst[i++] = '+';
            continue;
            */
        }
        if ('a' <= *it && *it <= 'z') {
            dst[i++] = *it;
            continue;
        }
        if ('A' <= *it && *it <= 'Z') {
            dst[i++] = *it;
            continue;
        }
        if ('0' <= *it && *it <= '9') {
            dst[i++] = *it;
            continue;
        }
        dst[i++] = '%';
        dst[i++] = toDigit(((unsigned char)*it) >> 4);
        dst[i++] = toDigit(((unsigned char)*it) & 0xF);
    }
    return String(dst, i);
}

#define BASE_64_BAD -1
#define BASE_64_PADDING -2
#define FROM_BASE_64(x)	( \
    'A' <= (x) && (x) <= 'Z' ? (x) - 'A' : \
    'a' <= (x) && (x) <= 'z' ? (x) - 'a' + 26 : \
    '0' <= (x) && (x) <= '9' ? (x) - '0' + 52 : \
    (x) == '+' ? 62 : \
    (x) == '/' ? 63 : \
    (x) == '=' ? BASE_64_PADDING : /* padding */ \
    BASE_64_BAD \
    )
String String::base64decode(MemoryPool *mp) const {
    int size = length() * 3 / 4;
    char *retval = (char*)mp->alloc(size, sizeof(char));
    int o = 0;
    unsigned int i;
    for (i = 0; i+4 < length(); i += 4) {
        char digit[4];
        for (int j = 0; j < 4; ++j) {
            digit[j] = FROM_BASE_64(ptr[i+j]);
            if (digit[j] == BASE_64_PADDING || digit[j] == BASE_64_BAD)
                goto end;
        }
        retval[o++] = (unsigned char)(digit[0] << 2 | digit[1] >> 4);
        retval[o++] = (unsigned char)((digit[1] & 0xf) << 4 | digit[2] >> 2);
        retval[o++] = (unsigned char)((digit[2] & 0x3) << 6 | digit[3]);
    }
end:
    if (i < length()) {
        char digit[4];
        for (int j = 0; j < 4; ++j) {
            digit[j] = i+j < length() ? FROM_BASE_64(ptr[i+j]) : -1;
            if (digit[j] == BASE_64_BAD)
                goto end2;
        }
        if (digit[1] >= 0) {
            retval[o++] = (unsigned char)(digit[0] << 2 | digit[1] >> 4);
            if (digit[2] >= 0) {
                retval[o++] = (unsigned char)((digit[1] & 0xf) << 4 | digit[2] >> 2);
                if (digit[3] >= 0)
                    retval[o++] = (unsigned char)((digit[2] & 0x3) << 6 | digit[3]);
            }
        }
    }
end2:
    return String(retval, o);
}

bool String::equalsIgnoreCase(const String &b) const {
    if (size() != b.size())
        return false;
    const char* aIt = begin();
    const char* bIt = b.begin();
    if (aIt == bIt)
        return true;
    while (aIt != end()) {
        if (toUpper(*aIt) != toUpper(*bIt))
            return false;
        ++aIt;
        ++bIt;
    }
    return true;
}

Substring<StringOperation> StringOperation::_substring(size_t offset, size_t length) const {
    return Substring<StringOperation>(*this, offset, length);
}

size_t StringOperation::writeToHelper(char * __restrict__ &buffer, size_t &bufferLength, size_t &offset, const StringOperation &op, size_t max) {
    max = std::min(bufferLength, max);
    size_t len = std::min(op._size, max);
    if (offset >= len) {
        offset -= len;
        return 0;
    }
    len = op.writeTo(buffer, max, offset);
    buffer += len;
    bufferLength -= len;
    offset = 0;
    return len;
}

size_t StringOperation::writeToHelper(char * __restrict__ &buffer, size_t &bufferLength, size_t &offset, const String &str, size_t max) {
    max = std::min(bufferLength, max);
    size_t len = std::min(str._size(), max);
    if (offset >= len) {
        offset -= len;
        return 0;
    }
    len = str.writeTo(buffer, max, offset);
    buffer += len;
    bufferLength -= len;
    offset = 0;
    return len;
}

size_t StringOperation::writeToHelper(char * __restrict__ &buffer, size_t &bufferLength, size_t &offset, const char *src, size_t len)
{
    len = std::min(bufferLength, len);
    if (offset >= len) {
        offset -= len;
        return 0;
    }
    len = len - offset;
    memcpywrap(buffer, src + offset, len);
    buffer += len;
    bufferLength -= len;
    offset = 0;
    return len;
}

String StringOperation::operator()(MemoryPool *mp) const {
    size_t l = _size;
    char * __restrict__ buffer = (char*__restrict__)mp->alloc(l, sizeof(char));
    size_t actual_length = writeTo(buffer, l, 0);
    if (actual_length < l)
        mp->returnUnused(buffer+actual_length, l-actual_length);
    return String(buffer, actual_length);
}

StringT StringOperation::c_str(MemoryPool *mp) const {
    size_t l = _size+1;
    char * __restrict__ buffer = (char*__restrict__)mp->alloc(l, sizeof(char));
    size_t actual_length = writeTo(buffer, l-1, 0);
    buffer[actual_length++] = '\0';
    if (actual_length < l)
        mp->returnUnused(buffer+actual_length, l-actual_length);
    return StringT(buffer, actual_length-1);
}

StringOperation::operator std::string() const {
    StaticMemoryPool<128> smp;
    return (*this)(smp).stl();
}

//StringT StringOperation::c_str(MemoryPool &mp) const {
//    return c_str(&mp);
//}
/*
 const String ConstString(const char *string) {
    return String((char*)string);
}
 */

//FROM: http://opensource.apple.com/source/gcc/gcc-5470.3/libiberty/strverscmp.c
/* states: S_N: normal, S_I: comparing integral part, S_F: comparing
           fractional parts, S_Z: idem but with leading Zeroes only */
#define  S_N    0x0
#define  S_I    0x4
#define  S_F    0x8
#define  S_Z    0xC

/* result_type: CMP: return diff; LEN: compare using len_diff/diff */
#define  CMP    2
#define  LEN    3
/* Compare S1 and S2 as strings holding indices/version numbers,
   returning less than, equal to or greater than zero if S1 is less than,
   equal to or greater than S2 (for more info, see the Glibc texinfo doc).  */
int String::compareUsingVersion(const String &rhs) const {
    const char *s1 = pointer();
    const char *s2 = rhs.pointer();

    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p1end = p1+size();
    const unsigned char *p2 = (const unsigned char *) s2;
    const unsigned char *p2end = p2+rhs.size();
    unsigned char c1, c2;
    int state;
    int diff;

    /* Symbol(s)    0       [1-9]   others  (padding)
         Transition   (10) 0  (01) d  (00) x  (11) -   */
    static const unsigned int next_state[] =
    {
        /* state    x    d    0    - */
        /* S_N */  S_N, S_I, S_Z, S_N,
        /* S_I */  S_N, S_I, S_I, S_I,
        /* S_F */  S_N, S_F, S_F, S_F,
        /* S_Z */  S_N, S_F, S_Z, S_Z
    };

    static const int result_type[] =
    {
        /* state   x/x  x/d  x/0  x/-  d/x  d/d  d/0  d/-
                             0/x  0/d  0/0  0/-  -/x  -/d  -/0  -/- */

        /* S_N */  CMP, CMP, CMP, CMP, CMP, LEN, CMP, CMP,
        CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
        /* S_I */  CMP, -1,  -1,  CMP, +1,  LEN, LEN, CMP,
        +1,  LEN, LEN, CMP, CMP, CMP, CMP, CMP,
        /* S_F */  CMP, CMP, CMP, CMP, CMP, LEN, CMP, CMP,
        CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
        /* S_Z */  CMP, +1,  +1,  CMP, -1,  CMP, CMP, CMP,
        -1,  CMP, CMP, CMP
    };

    if (p1 == p2)
        return 0;

    c1 = p1 == p1end ? '\0' : *p1++;
    c2 = *p2++;
    /* Hint: '0' is a digit too.  */
    state = S_N | ((c1 == '0') + (isdigit (c1) != 0));

    while ((diff = c1 - c2) == 0 && c1 != '\0')
        //while ((diff = c1 - c2) == 0 && p1 != p1end)
    {
        state = next_state[state];
        c1 = p1 == p1end ? '\0' : *p1++;
        c2 = p2 == p2end ? '\0' : *p2++;
        state |= (c1 == '0') + (isdigit (c1) != 0);
    }

    state = result_type[state << 2 | (((c2 == '0') + (isdigit (c2) != 0)))];

    switch (state)
    {
    case CMP:
        return diff;

    case LEN:
        while (isdigit (p1 == p1end ? '\0' : *p1++))
            if (!isdigit (p2 == p2end ? '\0' : *p2++))
                return 1;

        return isdigit (p2 == p2end ? '\0' : *p2++) ? -1 : diff;

    default:
        return state;
    }
}

bool StringNameCompare::operator()(const String &lhs, const String &rhs) {
    int retval = lhs.compareUsingVersion(rhs);
    return retval <= 0;
}

String String::append(const String &a, MemoryPool &mp) const {
    // is the tail the same as where we were going to be?
    const char *next = pointer()+length();
    if (mp.next(sizeof(char)) == next && mp.available() >= a.size()) {
        MemoryPoolStatus status = mp.status();
        // try append
        String retval = a(mp);
        if (retval.pointer() == next)
            return String(pointer(), length()+a.length());
        // apperantly broken memorypools, or it did not fit in, so restore the memorypool
        mp.restore(status);
        // fallback on default always correct behaviour
    }
    return (*this + a)(mp);
}

String::operator HashedString() const
{
    return HashedString((StableHashedString)*this);
}

constexpr String::operator StableHashedString() const
{
    return StableHashedString(*this);
}

String String::to(char*& buffer, std::size_t& size) const
{
    size_t len = writeTo(buffer, size, 0);
    String retval(buffer,len);
    buffer += len;
    size -= len;
    return retval;
}

String String::operator()(char* buffer, std::size_t size) const
{
    size_t len = writeTo(buffer, size, 0);
    return String(buffer, len);
}

StringC::StringC(const String &string)
{
    char *rawstring = new char[string.size()];
    memcpy(rawstring, string.pointer(), string.size());
    container.reset(rawstring, [](char* data) {
        delete[] data;
    });
    str = String(rawstring, string.size());
    hashCache = str.hashIgnoreCase();
}

String bitpowder::lib::StringOperation::operator()(char* buffer, std::size_t size) const {
    size_t len = writeTo(buffer, size, 0);
    return String(buffer, len);
}

String bitpowder::lib::StringOperation::to(char*& buffer, std::size_t& size) const {
    size_t len = writeTo(buffer, size, 0);
    String retval(buffer,len);
    buffer += len;
    size -= len;
    return retval;
}

StableHashedString::operator HashedString() const
{
    return HashedString(*this);
}

}
}

