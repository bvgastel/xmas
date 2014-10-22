#ifndef PARSER_JSON_H
#define PARSER_JSON_H

#include "simplestring.h"
#include "stringparse.h"
#include "parser.h"
#include "type_hash.h"
#include <map>
#include <vector>

/**
 * @brief JSON Parser
 * Ideas
 * - use memorypool during parsing (with linked list for arrays and objects OR allocating the structures inside a memorypool), making a final datastructure out of it after parsing
 * - use String during parsing, convert to std::string after parsing
 * - make a version that uses no input buffer, so directly copies Strings etc to a MemoryPool
 * - automatically convert to certain type, e.g. (actually, should also support enumerate<std::vector<int>>, and maybe custom types with a convertor [](const JSONData &d) { if (error) return Maybe(); return Maybe(MyCustomObject); })
 *      for (int i : object.field("foo").nth(2).enumerate<int>())
 *          std::cout << i << std::endl;
 *      alternatively: auto e = object.field("foo").nth(2).enumerate<int>();
 *                     std::vector<int> v(e.begin(), e.end());
 * - add error reporting
 * - automatically convert to Lua objects
 * - custom error types (not only ints)
 * - currently it is a DOM parser, maybe also implement a SAX parser?
 */

namespace bitpowder {
namespace lib {

#define JSON_PARSER_USE_MEMORYPOOL

class JSONData {
public:
    typedef String Key;
    typedef MemoryPool::Allocator<JSONData> AllocatorVector;
    typedef MemoryPool::Allocator<std::pair<Key,JSONData>> AllocatorMap;

#ifdef JSON_PARSER_USE_MEMORYPOOL
    typedef std::vector<JSONData, AllocatorVector> Vector;
    typedef std::map<Key, JSONData, std::less<Key>, AllocatorMap> Map;
#else
    typedef std::vector<JSONData> Vector;
    typedef std::map<Key, JSONData, std::less<Key>> Map;
#endif

    static Map AllocateMap(MemoryPool &mp) {
#ifdef JSON_PARSER_USE_MEMORYPOOL
        return JSONData::Map(std::less<Key>(), mp.allocator<std::pair<Key,JSONData>>());
#else
        return JSONData::Map();
#endif
    }
    static Vector AllocateVector(MemoryPool &mp) {
#ifdef JSON_PARSER_USE_MEMORYPOOL
        return JSONData::Vector(mp.allocator<JSONData>());
#else
        return JSONData::Vector();
#endif
    }
private:
    enum {JSONNull, JSONString, JSONNumber, JSONArray, JSONObject} type;
    MemoryPool *mp;
    union {
        String str;
        int number;
        Vector *array;
        Map *object;
    };
public:
    JSONData() : type(JSONNull), mp(nullptr) {
    }
    JSONData(int number) : type(JSONNumber), mp(nullptr), number(number) {
    }
    JSONData(const String &str) : type(JSONString), mp(nullptr), str(str) {
    }
    JSONData(const Vector &array, MemoryPool &mp) : type(JSONArray), mp(&mp), array(new (mp) Vector(array)) {
    }
    JSONData(Vector &&array, MemoryPool &mp) : type(JSONArray), mp(&mp), array(new (mp) Vector(std::move(array))) {
    }
    JSONData(const Map &object, MemoryPool &mp) : type(JSONObject), mp(&mp), object(new (mp) Map(object)) {
    }
    JSONData(Map &&object, MemoryPool &mp) : type(JSONObject), mp(&mp), object(new (mp) Map(std::move(object))) {
    }
    ~JSONData() {
        clear();
    }
    void clear();

    JSONData(JSONData &&b) noexcept;
    JSONData(const JSONData &b) noexcept;
    JSONData& operator=(JSONData &&b);
    JSONData& operator=(const JSONData &b);
    bool operator==(const JSONData &b) const;

    bool isVector() {
        return type == JSONArray;
    }

    bool isObject() {
        return type == JSONObject;
    }

    bool isString() {
        return type == JSONString;
    }

    bool isNumber() {
        return type == JSONNumber;
    }

    bool isNull() {
        return type == JSONNull;
    }

    Vector& asVector() {
        if (!isVector())
            throw Exception("not an array");
        return *array;
    }

    Map& asObject() {
        if (!isObject())
            throw Exception("not an object");
        return *object;
    }

    String asString() {
        if (!isString())
            throw Exception("not a string");
        return str;
    }

    int asNumber() {
        if (!isNumber())
            throw Exception("not a number");
        return number;
    }

    operator Vector&() {
        return asVector();
    }

    operator Map&() {
        return asObject();
    }

    operator String() {
        return asString();
    }

    operator HashedString() {
        return asString();
    }

    explicit operator int() {
        return asNumber();
    }

    virtual void print(std::ostream& out) const;

    JSONData& operator[](const String &lookup) {
        return asObject()[lookup];
    }
    JSONData& operator[](int lookup) {
        return asVector()[lookup];
    }

    class iterator {
        friend class JSONData;
        Vector *contents;
        unsigned long index;
        iterator(Vector *contents, unsigned long index) : contents(contents), index(index) {
        }
    public:
        bool operator==(const iterator& it) const {
            return contents == it.contents && index == it.index;
        }
        bool operator!=(const iterator& it) const {
            return !(*this == it);
        }
        JSONData& operator*() const {
            return (*contents)[index];
        }
        iterator& operator++() {
            index++;
            return *this;
        }
    };
    iterator begin();
    iterator end();
};

class JSONParseResult {
    bool success;
    int pos;
    String errorMessage;
    JSONData retval;
public:
    JSONParseResult(int position, String errorMessage) : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    JSONParseResult(JSONData &&retval) : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() const {
        return success;
    }
    JSONData& result() {
        return retval;
    }
    String error() const {
        return errorMessage;
    }
    int position() const {
        return pos;
    }
};

JSONParseResult ParseJSON(const String &str, MemoryPool &mp);

}
}

namespace std {
inline std::ostream& operator<< (std::ostream& out, const bitpowder::lib::JSONData &json)
{
    json.print(out);
    return out;
}
std::ostream& operator<< (std::ostream& out, bitpowder::lib::JSONParseResult &parseResult);
}

#endif // PARSER_JSON_H
