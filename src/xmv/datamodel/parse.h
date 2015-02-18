#ifndef PARSE_H
#define PARSE_H

#include <map>
#include <sstream>

#include "memorypool.h"
#include "parser_json.h"
#include "xmas.h"
#include "symbolic-interval-field.h"
#include "symbolic-enum-field.h"
#include "messagespec.h"

struct Interval {
    typedef SymbolicIntervalField::interval_type interval_type;
    interval_type min;
    interval_type max;
    Interval(interval_type min = 0, interval_type max = 0) : min(min), max(max) {
    }

    void print(std::ostream& out) const {
        out << "[" << min << ".." << (max-1) << "]";
    }
    static Interval all() {
        return {std::numeric_limits<interval_type>::min(), std::numeric_limits<interval_type>::max()};
    }
};

struct Enum {
    std::vector<SymbolicEnumField::Type> values;
    Enum() : values() {
    }
    bool operator==(const Enum &n) const {
        return values == n.values;
    }
    void print(std::ostream& out) const {
        out << "{";
        for (auto &v : values) {
            out << v << " ";
        }
        out << "}";
    }
};

struct SymbolicPacketSet {
    typedef SymbolicIntervalField::interval_type interval_type;
    std::vector<SymbolicPacket> values;
    SymbolicPacketSet();

    void greaterAs(interval_type b);
    void greaterEqualAs(interval_type b);
    void lessEqualAs(interval_type b);
    void lessAs(interval_type b);
    void negate();

    bool operator==(const SymbolicPacketSet &rhs) const {
        return values == rhs.values;
    }
    void print(std::ostream& out) const;
    void updateHash();
};

class PacketExpressionParseResult {
    bool success;
    int pos;
    bitpowder::lib::String errorMessage;
    SymbolicPacketSet retval;
public:
    PacketExpressionParseResult(int position, bitpowder::lib::String errorMessage)
        : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    PacketExpressionParseResult(SymbolicPacketSet &&retval)
        : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    SymbolicPacketSet& result() {
        return retval;
    }
    bitpowder::lib::String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};

struct SpecSet {
    std::vector<std::tuple<SymbolicPacketSet,MessageSpec::Ref>> spec;

    void add(SymbolicPacketSet &&a) {
        spec.push_back(std::tuple<SymbolicPacketSet, MessageSpec::Ref>(std::move(a), nullptr));
    }

    void add(MessageSpec::Ref &&b) {
        std::get<1>(spec.back()) = std::move(b);
    }

    void addAnd(MessageSpec::Ref &&b) {
        std::get<1>(spec.back()) = std::get<1>(spec.back()) & std::move(b);
    }

    void addOr(MessageSpec::Ref &&b) {
        std::get<1>(spec.back()) = std::get<1>(spec.back()) | std::move(b);
    }

    void updateHash();
};

class SourceExpressionParseResult {
    bool success;
    int pos;
    bitpowder::lib::String errorMessage;
    SpecSet retval;
public:
    SourceExpressionParseResult(int position, bitpowder::lib::String errorMessage)
        : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    SourceExpressionParseResult(SpecSet &&retval)
        : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    SpecSet& result() {
        return retval;
    }
    bitpowder::lib::String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};
PacketExpressionParseResult ParsePacketExpression(const bitpowder::lib::String &str,
                                                  bitpowder::lib::MemoryPool &memoryPool);
SourceExpressionParseResult ParseSourceExpression(const bitpowder::lib::String &str,
                                                  bitpowder::lib::MemoryPool &memoryPool);

class ParsedXMASExpression {
public:
    virtual ~ParsedXMASExpression() {}
    virtual std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const = 0;
    virtual void print(std::ostream &out) const = 0;
    virtual void printOldCSyntax(std::ostream &out, std::map<bitpowder::lib::String,int>& enumMap) const = 0;
};

std::ostream &operator <<(std::ostream &out, const ParsedXMASExpression &c);

class ParsedXMASFunction {
public:
    int refcount = 0;
    virtual ~ParsedXMASFunction() {}

    bool hasCondition = false;
    std::vector<SymbolicPacket> conditions;
    std::shared_ptr<ParsedXMASFunction> next; // if condition was not met

    std::map<bitpowder::lib::String, std::shared_ptr<ParsedXMASExpression>> fields; // function to apply for each field

    std::vector<SymbolicPacket> operator()(const std::vector<SymbolicPacket> &packet) const;

    virtual void printOldCSyntax(std::ostream &out, std::map<bitpowder::lib::String,int>& enumMap) const;
};

struct ParsedXMASFunctionExtension : public XMASComponentExtension {
    std::shared_ptr<ParsedXMASFunction> value;
    ParsedXMASFunctionExtension() {
    }
    ParsedXMASFunctionExtension(const std::shared_ptr<ParsedXMASFunction>& value) : value(value) {
    }
};

std::ostream &operator <<(std::ostream &out, const ParsedXMASFunction &c);

struct ParsedXMASRestrictedJoin: public XMASComponentExtension {
    int function = 0;
    ParsedXMASRestrictedJoin() {
    }
    ParsedXMASRestrictedJoin(int function) : function(function) {
    }
};


class PacketFunctionParseResult {
    bool success;
    int pos;
    bitpowder::lib::String errorMessage;
    std::shared_ptr<ParsedXMASFunction> retval;
public:
    PacketFunctionParseResult(int position, bitpowder::lib::String errorMessage)
        : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    PacketFunctionParseResult(std::shared_ptr<ParsedXMASFunction> &&retval)
        : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    std::shared_ptr<ParsedXMASFunction> result() {
        return retval;
    }
    bitpowder::lib::String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};

PacketFunctionParseResult ParsePacketFunction(const bitpowder::lib::String &str,
                                              bitpowder::lib::MemoryPool &mp);

namespace std {
inline std::ostream& operator<< (std::ostream& out, const Interval &n)
{
    n.print(out);
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const SymbolicPacketSet &n)
{
    n.print(out);
    return out;
}
}

/**
 * @brief Parse Reads a file from a specified filename and parses it using the json parser
 *
 * This function parses a file containing correct JSON for an xmas network
 * and returns a map containing the xmas components (indexed by name) and the json data.
 *
 * Remark that the lifetime of strings in the structure is limited to the lifetime of
 * the memory pool.
 *
 * @param filename the name of the file containing the json for the network.
 *
 * @param mp the memory pool
 * @return the pair of a map to all xmas components and the json data.
 */
std::pair<std::map<bitpowder::lib::String, XMASComponent *>,bitpowder::lib::JSONData>
parse_xmas_from_file(const std::string &filename, bitpowder::lib::MemoryPool &mp);

/**
 * @brief parse_network_json Reads a string containing a json spec of a network and parses it using the json parser
 *
 * This function parses a string containing correct JSON for an xmas network
 * and returns a map containing the xmas components (indexed by name) and the json data.
 *
 * Remark that the lifetime of strings in the structure is limited to the lifetime of
 * the memory pool.
 *
 * @param sstr The stringstream contain the json for the network.
 * @param mp the memory pool.
 * @return the pair of a map to all xmas components and the json data.
 */
std::pair<std::map<bitpowder::lib::String, XMASComponent *>,bitpowder::lib::JSONData>
parse_xmas_from_json(const std::string &str, bitpowder::lib::MemoryPool &mp);

/**
 * @brief parse_json_buffer Uses the parsed JSON for a network to return the xmas components.
 *
 * This function uses the parsed JSON for an xmas network to return a map containing the
 * xmas components indexed by name plus the json data.
 *
 * Remark that the lifetime of strings in the structure is limited to the lifetime of
 * the memory pool.
 *
 * @param parseResult the parse result from ParseJSON.
 *
 * @param mp the memory pool.
 * @return the pair of a map to all xmas components and the json data.
 */
std::pair<std::map<bitpowder::lib::String, XMASComponent *>,bitpowder::lib::JSONData>
generate_xmas_from_parse_result(bitpowder::lib::JSONParseResult &parseResult, bitpowder::lib::MemoryPool &mp);

#endif // PARSE_H
