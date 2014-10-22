#ifndef PARSE_H
#define PARSE_H

#include <map>
#include "lib/memorypool.h"
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
    String errorMessage;
    SymbolicPacketSet retval;
public:
    PacketExpressionParseResult(int position, String errorMessage) : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    PacketExpressionParseResult(SymbolicPacketSet &&retval) : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    SymbolicPacketSet& result() {
        return retval;
    }
    String error() {
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
    String errorMessage;
    SpecSet retval;
public:
    SourceExpressionParseResult(int position, String errorMessage) : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    SourceExpressionParseResult(SpecSet &&retval) : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    SpecSet& result() {
        return retval;
    }
    String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};
PacketExpressionParseResult ParsePacketExpression(const String &str, MemoryPool &memoryPool);
SourceExpressionParseResult ParseSourceExpression(const String &str, MemoryPool &memoryPool);

class ParsedXMASExpression {
public:
    virtual ~ParsedXMASExpression() {}
    virtual std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const = 0;
};

class ParsedXMASFunction {
public:
    virtual ~ParsedXMASFunction() {}

    bool hasCondition = false;
    std::vector<SymbolicPacket> conditions;
    std::shared_ptr<ParsedXMASFunction> next; // if condition was not met

    std::map<String, std::shared_ptr<ParsedXMASExpression>> fields; // function to apply for each field

    std::vector<SymbolicPacket> operator()(const std::vector<SymbolicPacket> &packet) const;
};

class PacketFunctionParseResult {
    bool success;
    int pos;
    String errorMessage;
    std::shared_ptr<ParsedXMASFunction> retval;
public:
    PacketFunctionParseResult(int position, String errorMessage) : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    PacketFunctionParseResult(std::shared_ptr<ParsedXMASFunction> &&retval) : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    std::shared_ptr<ParsedXMASFunction> result() {
        return retval;
    }
    String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};

PacketFunctionParseResult ParsePacketFunction(const String &str, MemoryPool &mp);

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

// correct lifetime of strings in the structure is limited to the lifetime of the MemoryPool
std::map<bitpowder::lib::String, XMASComponent *> Parse(const std::string &filename, MemoryPool &mp);

#endif // PARSE_H
