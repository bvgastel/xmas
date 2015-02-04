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

#include "parser_json.h"

namespace bitpowder {
namespace lib {

struct Operator {
    char op;

    Operator(char op = 0) : op(op) {
    }

    bool operator==(const Operator &b) const {
        return op == b.op;
    }
};

}
}

namespace bitpowder {
namespace lib {

class JSONLexer {
    typedef StringParse<int> StringParser;
    String original;
    //StringParser c;
    String c;
    Token<String> stringToken;
    Token<int> numberToken;
    Token<Operator> opToken;
    Token<Exception> exceptionToken;

    void whitespace() {
        const static auto space = C(' ') + C('\n') + C('\t') + C('\r');
        StringParser(c).span(space).remainder(c);
    }
public:
    static const Token<Operator> COMMA;
    static const Token<Operator> ARRAY_OPEN;
    static const Token<Operator> ARRAY_CLOSE;
    static const Token<Operator> OBJECT_OPEN;
    static const Token<Operator> OBJECT_FIELD;
    static const Token<Operator> OBJECT_CLOSE;
    static const Token<Operator> NULL_OBJECT;

    JSONLexer(const String &str) : original(str), c(str), stringToken(), numberToken(), opToken(), exceptionToken() {
        //std::cerr << "hash_type of Token<int> = " << numberToken.type << std::endl;
        //std::cerr << "expecting hash of " << CachingTypeHash<Token<int>>::hash_code << std::endl;
    }
    TokenBase *next() {
        whitespace();
        const char *currentPtr = c.pointer();
        TokenBase *retval = recognise();
        if (retval != nullptr) {
            retval->start = currentPtr - original.pointer();
            retval->length = c.pointer() - currentPtr;
        }
        return retval;
    }
    TokenBase *recognise() {
        if (c.length() == 0)
            return nullptr;
        if (StringParser(c).accept("null"_S).remainder(c)) {
            opToken.value = NULL_OBJECT;
            return &opToken;
        }
        if (StringParser(c).accept(COMMA.value.op).remainder(c)) {
            opToken = COMMA;
            return &opToken;
        }
        if (StringParser(c).accept(ARRAY_OPEN.value.op).remainder(c)) {
            opToken = ARRAY_OPEN;
            return &opToken;
        }
        if (StringParser(c).accept(ARRAY_CLOSE.value.op).remainder(c)) {
            opToken = ARRAY_CLOSE;
            return &opToken;
        }
        if (StringParser(c).accept(OBJECT_OPEN.value.op).remainder(c)) {
            opToken = OBJECT_OPEN;
            return &opToken;
        }
        if (StringParser(c).accept(OBJECT_FIELD.value.op).remainder(c)) {
            opToken = OBJECT_FIELD;
            return &opToken;
        }
        if (StringParser(c).accept(OBJECT_CLOSE.value.op).remainder(c)) {
            opToken = OBJECT_CLOSE;
            return &opToken;
        }
        if (StringParser(c).accept('"').splitOn(stringToken.value, C('"')).remainder(c)) {
            return &stringToken;
        }
        if (StringParser(c).parse(numberToken.value).remainder(c))
            return &numberToken;
        std::cerr << "remaining: " << c << std::endl;
        exceptionToken.value = Exception("could not tokenize remaining value");
        return &exceptionToken;
    }
    String getInputString(TokenBase *token) {
        return token ? original.substring(token->start, token->length) : "(none)";
    }
    String remaining() {
        return c;
    }
};

const Token<Operator> JSONLexer::NULL_OBJECT = Operator('n');
const Token<Operator> JSONLexer::COMMA = Operator(',');
const Token<Operator> JSONLexer::ARRAY_OPEN = Operator('[');
const Token<Operator> JSONLexer::ARRAY_CLOSE = Operator(']');
const Token<Operator> JSONLexer::OBJECT_OPEN = Operator('{');
const Token<Operator> JSONLexer::OBJECT_CLOSE = Operator('}');
const Token<Operator> JSONLexer::OBJECT_FIELD = Operator(':');


template <class Lexer = JSONLexer>
struct JSONParserState {
    typedef JSONData Data;
    typedef MemoryPool &UserData;
    typedef Parser<Lexer, 1, Data, UserData> & PS;
    static const int ERROR_IN_CONSTANT = 10;

    static int convertRawString(String &retval, const Token<String> &t, UserData userData) {
        unused(userData);
        retval = t.value;
        return 0;
    }

    static PS nullConstant(PS cont, UserData userData) {
        unused(userData);
        return cont().accept(JSONLexer::NULL_OBJECT).store(JSONData());
    }

    static int convertNumber(JSONData &retval, const Token<int> &t, UserData userData) {
        unused(userData);
        retval = JSONData(t.value);
        return 0;
    }

    static int convertString(JSONData &retval, const Token<String> &t, UserData userData) {
        unused(userData);
        retval = JSONData(t.value);
        return 0;
    }

    static PS primary(PS cont, UserData userData) {
        unused(userData);
        return cont().choose(nullConstant, convertNumber, convertString);
    }

    static int applyField(JSONData& v, String& key, JSONData&& value, UserData userData) {
        unused(userData);
        v.asObject().insert(std::make_pair(key, std::move(value)));
        return 0;
    }

    static PS objectField(PS cont, UserData userData) {
        unused(userData);
        String key;
        JSONData value;
        return cont()
                .fetch(convertRawString, key).error(-2)
                .accept(JSONLexer::OBJECT_FIELD).error(-3)
                .fetch(expr, value).error(-4)
                .modify(applyField, key, std::move(value)).error(-5);
    }
    static PS objectOp(PS cont, UserData userData) {
        unused(userData);
        return cont().store([](UserData userData) -> JSONData {
            return JSONData::AllocateMap(userData);
        })
        .accept(JSONLexer::OBJECT_OPEN).error(-6)
        .opt(objectField).error(-7)
        .repeat(JSONLexer::COMMA, objectField)
        .accept(JSONLexer::OBJECT_CLOSE).error(-8);
    }
    static int applyEntry(JSONData& v, JSONData&& value, UserData userData) {
        unused(userData);
        v.asVector().push_back(std::move(value));
        return 0;
    }
    static PS arrayEntry(PS cont, UserData userData) {
        unused(userData);
        return cont()
                .process(expr, applyEntry);
    }
    static PS arrayOp(PS cont, UserData userData) {
        unused(userData);
        return cont().store([](UserData userData) -> JSONData {
            return JSONData::AllocateVector(userData);
        })
        .accept(JSONLexer::ARRAY_OPEN)
        .opt(arrayEntry)
        .repeat(JSONLexer::COMMA, arrayEntry)
        .accept(JSONLexer::ARRAY_CLOSE).error(-9);
    }
    static PS expr(PS cont, UserData userData) {
        unused(userData);
        return cont().choose(primary, objectOp, arrayOp);
    }
    static PS topLevelExpr(PS cont, UserData userData) {
        unused(userData);
        return cont().perform(expr).end();
    }
};

JSONParseResult ParseJSON(const String &str, MemoryPool &mp)
{
    JSONLexer lexer(str);
    auto p = ParserState<JSONLexer, 1>(lexer);
    JSONData result;
    int retval = Parser<JSONLexer, 1, JSONData, MemoryPool&>(&p, mp).perform(JSONParserState<JSONLexer>::topLevelExpr).retreive(result);
    if (retval != 0) {
        std::cout << "parsing error in " << lexer.remaining() << " code=" << retval << " part='" << lexer.getInputString(p.getToken()) << "' result=";
        result.print(std::cout);
        std::cout << std::endl;
        TokenBase *token = p.getToken();
        return {token ? token->start : 0, lexer.getInputString(token)};
    }
    return std::move(result);
}

void JSONData::clear() {
    type = JSONNull;
}

JSONData& JSONData::operator=(const JSONData &b) {
    //std::cerr << "JSONData assignment" << std::endl;
    clear();
    type = b.type;
    if (type == JSONString)
        str = b.str;
    else if (type == JSONNumber)
        number = b.number;
    else if (type == JSONArray) {
        array = new (b.array->get_allocator().mp) Vector(*b.array);
    } else if (type == JSONObject)
        object = new (b.object->get_allocator().mp) Map(*b.object);
    return *this;
}

bool JSONData::operator==(const JSONData &b) const {
    if (type == b.type) {
        if (type == JSONNull)
            return true;
        if (type == JSONString)
            return str == b.str;
        if (type == JSONNumber)
            return number == b.number;
        if (type == JSONArray)
            return *array == *b.array;
        if (type == JSONObject)
            return *object == *b.object;
    }
    return false;
}

JSONData& JSONData::operator=(JSONData &&b) {
    //std::cerr << "JSONData move assignment" << std::endl;
    clear();
    type = b.type;
    if (type == JSONString)
        str = b.str;
    else if (type == JSONNumber)
        number = b.number;
    else if (type == JSONArray) {
        array = b.array;
        b.array = nullptr; b.type = JSONNull;
    } else if (type == JSONObject) {
        object = b.object;
        b.object = nullptr; b.type = JSONNull;
    }
    return *this;
}

JSONData::JSONData(const JSONData &b) noexcept : type(b.type) {
    //if (type != JSONNull)
    //    std::cerr << "JSONData copy" << std::endl;
    if (type == JSONString)
        str = b.str;
    else if (type == JSONNumber)
        number = b.number;
    else if (type == JSONArray) {
        array = new (b.array->get_allocator().mp) Vector(*b.array);
    } else if (type == JSONObject)
        object = new (b.object->get_allocator().mp) Map(*b.object);
}

JSONData::JSONData(JSONData &&b) noexcept : type(b.type) {
    //std::cerr << "JSONData move" << std::endl;
    if (type == JSONString)
        str = b.str;
    else if (type == JSONNumber) {
        number = b.number;
    } else if (type == JSONArray) {
        array = b.array;
        b.array = nullptr; b.type = JSONNull;
    } else if (type == JSONObject) {
        object = b.object;
        b.object = nullptr; b.type = JSONNull;
    }
}

void JSONData::print(std::ostream &out) const {
    //out << "(json " << type << std::endl;
    if (type == JSONNull)
        out << "null";
    if (type == JSONNumber)
        out << number;
    if (type == JSONString)
        out << '"' << str << '"';
    if (type == JSONArray) {
        out << "[";
        bool first = true;
        for (auto &it : *array) {
            if (!first)
                out << ",";
            it.print(out);
            first = false;
        }
        out << "]";
    }
    if (type == JSONObject) {
        out << "{";
        bool first = true;
        for (auto &it : *object) {
            if (!first)
                out << ",";
            out << "\"" << it.first << "\":";
            it.second.print(out);
            first = false;
        }
        out << "}";
    }
    //out << ")";
}

JSONData::iterator JSONData::begin() {
    return {isVector() ? array : nullptr, 0};
}

JSONData::iterator JSONData::end() {
    return isVector() ? iterator(array, array->size()) : iterator(nullptr, 0);
}


}
}

namespace std {
inline std::ostream& operator<< (std::ostream& out, const bitpowder::lib::Operator &op)
{
    out << "(operator " << op.op << ")";
    return out;
}

ostream &operator<<(ostream &out, bitpowder::lib::JSONParseResult &parseResult)
{
    out << "[result pos=" << parseResult.position() << " error=" << parseResult.error() << " result=" << parseResult.result() << "]";
    return out;
}

}
