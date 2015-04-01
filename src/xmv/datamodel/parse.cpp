#include "parse.h"

#include <sys/stat.h>
#include <fcntl.h>

#include "simplestring.h"
#include "canvascomponentextension.h"
#include "symbolic-enum.h"

using namespace bitpowder::lib;

class PacketExpressionLexer {
    String original;
    String c;
    Token<String> variable;
    Token<SymbolicInterval> intervalToken;
    Token<char> opToken;
    Token<SymbolicIntervalField::interval_type> constantToken;
    Token<Exception> exceptionToken;
    void whitespace() {
        static auto space = C(' ') + C('\n') + C('\t');
        StringParser(c).span(space).remainder(c);
    }
public:
    static const Token<char> INLINE_MUL;
    static const Token<char> INLINE_DIV;
    static const Token<char> INLINE_MOD;
    static const Token<char> INLINE_LESS_EQ;
    static const Token<char> INLINE_GREATER_EQ;
    static const Token<char> NOT_OP;
    static const Token<char> AND_OP;
    static const Token<char> OR_OP;
    static const Token<char> OPEN;
    static const Token<char> CLOSE;
    static const Token<char> SET_OPEN;
    static const Token<char> SET_CLOSE;
    static const Token<char> COMMA;
    static const Token<char> NOT_EQUAL;
    static const Token<char> EQUAL;
    static const Token<char> IN;
    static const Token<char> TO;
    static const Token<char> QUESTION;
    static const Token<char> COLON;
    PacketExpressionLexer(const String &str) : original(str), c(str), variable(), intervalToken(), opToken(), constantToken(), exceptionToken() {
    }
    TokenBase *next() {
        whitespace();
        const char *currentPtr = c.pointer();
        TokenBase *retval = recognise();
        if (retval != nullptr) {
            //std::cout << "token: " << retval << " type_hash=" << retval->type << std::endl;
            retval->start = currentPtr - original.pointer();
            retval->length = c.pointer() - currentPtr;
        }
        return retval;
    }
    TokenBase *recognise() {
        if (c.length() == 0)
            return nullptr;
        if (StringParser(c).accept("mod").remainder(c) || StringParser(c).accept("%").remainder(c)) {
            opToken = INLINE_MOD;
            return &opToken;
        }
        if (StringParser(c).accept("in").remainder(c) || StringParser(c).accept("==").remainder(c)) {
            opToken = EQUAL;
            return &opToken;
        }
        if (StringParser(c).accept("notin").remainder(c) || StringParser(c).accept("!=").remainder(c)) {
            opToken = NOT_EQUAL;
            return &opToken;
        }
        if (StringParser(c).accept("->").remainder(c)) {
            opToken = TO;
            return &opToken;
        }
        if (StringParser(c).accept("<=").remainder(c)) {
            opToken = INLINE_LESS_EQ;
            return &opToken;
        }
        if (StringParser(c).accept(">=").remainder(c)) {
            opToken = INLINE_GREATER_EQ;
            return &opToken;
        }
        if (StringParser(c).accept("&&").remainder(c) || StringParser(c).accept("and").remainder(c)) {
            opToken = AND_OP;
            return &opToken;
        }
        if (StringParser(c).accept("||").remainder(c) || StringParser(c).accept("or").remainder(c)) {
            opToken = OR_OP;
            return &opToken;
        }
        if (c[0] == INLINE_MUL || c[0] == INLINE_DIV ||
                c[0] == '+' || c[0] == '-' ||
                c[0] == '<' || c[0] == '>' ||
                c[0] == OPEN || c[0] == CLOSE || c[0] == COMMA || c[0] == EQUAL ||
                c[0] == SET_OPEN || c[0] == SET_CLOSE || c[0] == NOT_OP || c[0] == QUESTION ||
                c[0] == COLON) {
            opToken.value = c[0];
            c = c.substring(1);
            //std::cout << "token op: " << opToken.value << std::endl;
            return &opToken;
        }
        SymbolicIntervalField::interval_type minValue;
        SymbolicIntervalField::interval_type maxValue;
        if (StringParser(c).accept("[").parse(minValue).accept("..").parse(maxValue).accept("]").remainder(c)) {
            intervalToken.value = {minValue, maxValue+1};
            //std::cout << "token interval: " << intervalToken << std::endl;
            return &intervalToken;
        }

        if (StringParser(c).parse(constantToken.value).remainder(c)) {
            return &constantToken;
        }
        static auto variableName = Alpha() + C('_') + Num();
        if (StringParser(c).span(variable.value, variableName).remainder(c) && variable.value.size() > 0) {
            return &variable;
        }
        exceptionToken.value = Exception("could not tokenize remaining value");
        return &exceptionToken;
    }
    String getInputString(TokenBase *token) {
        return token ? original.substring(token->start, token->length) : "(none)";
    }
    String remaining() const {
        return c;
    }
};

const Token<char> PacketExpressionLexer::INLINE_MUL = '*';
const Token<char> PacketExpressionLexer::INLINE_DIV = '/';
const Token<char> PacketExpressionLexer::INLINE_MOD = 'm';
const Token<char> PacketExpressionLexer::INLINE_LESS_EQ = 'l';
const Token<char> PacketExpressionLexer::INLINE_GREATER_EQ = 'g';
const Token<char> PacketExpressionLexer::NOT_OP = '!';
const Token<char> PacketExpressionLexer::AND_OP = '&';
const Token<char> PacketExpressionLexer::OR_OP = '|';
const Token<char> PacketExpressionLexer::OPEN = '(';
const Token<char> PacketExpressionLexer::CLOSE = ')';
const Token<char> PacketExpressionLexer::SET_OPEN = '{';
const Token<char> PacketExpressionLexer::SET_CLOSE = '}';
const Token<char> PacketExpressionLexer::COMMA = ',';
const Token<char> PacketExpressionLexer::NOT_EQUAL = '~';
const Token<char> PacketExpressionLexer::EQUAL = '=';
const Token<char> PacketExpressionLexer::IN = 'i';
const Token<char> PacketExpressionLexer::TO = 't';
const Token<char> PacketExpressionLexer::QUESTION = '?';
const Token<char> PacketExpressionLexer::COLON = ':';

template <class Lexer>
struct PacketExpression {
    typedef MemoryPool& UserData;
    typedef SymbolicIntervalField::interval_type Constant;
    typedef Parser<Lexer, 1, Constant, UserData> & PInt;
    typedef Parser<Lexer, 1, SymbolicInterval, UserData> & PInterval;
    typedef Parser<Lexer, 1, Enum, UserData> & PEnum;
    //typedef Parser<Lexer, 1, SymbolicPacketRestriction, UserData> & PRestriction;
    //typedef Parser<Lexer, 1, SymbolicPacket, UserData> & PPacketDefinition;
    typedef Parser<Lexer, 1, SymbolicPacketSet, UserData> & PPacketExpr;
    typedef Parser<Lexer, 1, MessageSpec::Ref, UserData> & PSpecExpr;
    typedef Parser<Lexer, 1, SpecSet, UserData> & PSourceExpr;
    static const int ERROR_IN_CONSTANT = 10;
    static const int ERROR_IN_NEGATIVE_CONSTANT = 11;
    static const int ERROR_IN_PERCENTAGE = 12;
    static const int ERROR_IN_HAAKJES_BEGIN = 15;
    static const int ERROR_IN_HAAKJES_END = 16;
    static const int ERROR_IN_MUL = 23;
    static const int ERROR_IN_DIV = 24;
    static const int ERROR_IN_PLUS = 25;
    static const int ERROR_IN_MIN = 26;
    static const int ERROR_IN_MOD = 24;

    static int intervalInterval(SymbolicInterval &retval, const Token<SymbolicInterval> &t, UserData userData) {
        bitpowder::lib::unused(userData);
        retval = t.value;
        return 0;
    }

    static int constant(Constant &retval, const Token<Constant> &t, UserData userData) {
        bitpowder::lib::unused(userData);
        retval = t.value;
        return 0;
    }

    static int variable(SymbolicPacketSet &retval, const Token<String> &t, UserData userData) {
        //Interval openVariable = Interval::all();		// FIXME: unused-but-set-variable warning
        //SymbolicIntervalField field = {openVariable.min, openVariable.max};
        bitpowder::lib::unused(userData);
        retval.values.push_back({std::make_pair(t.value, std::make_shared<SymbolicAnyField>())});
        return 0;
    }

    static int constantToInterval(SymbolicInterval &retval, const Token<Constant> &t, UserData userData) {
        //std::cout << "Interval: ";
        //t.value.print(std::cout);
        //std::cout << std::endl;
        bitpowder::lib::unused(userData);
        retval = {t.value, t.value+1};
        return 0;
    }

    static PInterval interval(PInterval cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().choose(intervalInterval, constantToInterval);
    }

    static PPacketExpr haakjes(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont()
                .perform(PacketExpressionLexer::OPEN, expr, PacketExpressionLexer::CLOSE)
                .error(ERROR_IN_HAAKJES_END);
    }

    static PPacketExpr negate(PPacketExpr cont, UserData) {
        return cont().accept(PacketExpressionLexer::NOT_OP).perform(primary).modify([](SymbolicPacketSet &value, UserData &userData) {
            bitpowder::lib::unused(userData);
            value.negate();
            return 0;
        });
    }

    static PPacketExpr primary(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().choose(variable, haakjes, negate).modify([](SymbolicPacketSet &a, UserData) {
            bitpowder::lib::unused(a);
            ////std::cout << "returning from primary " << a << std::endl;
            return 0;
        });;
    }

    static PPacketExpr equalToInterval(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().process(interval, [](SymbolicPacketSet &retval, const SymbolicInterval& b, UserData userData) {
            bitpowder::lib::unused(userData);
            auto interval = std::make_shared<SymbolicIntervalField>(b.min, b.max);
            for (SymbolicPacket &packet : retval.values) {
                for (typename decltype(packet.fields)::iterator field = packet.fields.begin(); field != packet.fields.end(); ++field) {
                    std::vector<std::shared_ptr<SymbolicPacketField>> newFields = field->second->getIntersection(interval);
                    if (newFields.size() > 1)
                        throw Exception("can not handle non-continious intersection yet in the parser");
                    field->second = newFields[0];
                }
            }
            return 0;
        });
    }

    static PEnum enumEntry(PEnum cont, UserData userData) {
        String value;
        bitpowder::lib::unused(userData);
        return cont().fetch(value).modify([](Enum &e, const String &value, UserData userData) {
            bitpowder::lib::unused(userData);
            e.values.push_back(value);
            return 0;
        }, value).opt(PacketExpressionLexer::COMMA, enumEntry);
    }

    static PEnum enumDefinition(PEnum cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::SET_OPEN).opt(enumEntry).accept(PacketExpressionLexer::SET_CLOSE);
    }

    static PPacketExpr equalToEnum(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().process(enumDefinition, [](SymbolicPacketSet &retval, const Enum& b, UserData userData) {
            bitpowder::lib::unused(userData);
            auto interval = std::make_shared<SymbolicEnumField>(b.values);
            for (SymbolicPacket &packet : retval.values) {
                for (typename decltype(packet.fields)::iterator field = packet.fields.begin(); field != packet.fields.end(); ++field) {
                    std::vector<std::shared_ptr<SymbolicPacketField>> newFields = field->second->getIntersection(interval);
                    if (newFields.size() > 1)
                        throw Exception("can not handle non-continious intersection yet in the parser");
                    field->second = newFields[0];
                }
            }
            return 0;
        });
    }

    static PPacketExpr notEqualExprTail(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::NOT_EQUAL).choose(equalToInterval, equalToEnum).modify([](SymbolicPacketSet &value, UserData userData) {
            bitpowder::lib::unused(userData);
            value.negate();
            return 0;
        });
    }
    static PPacketExpr equalExprTail(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::EQUAL).choose(equalToInterval, equalToEnum);
    }
    static PPacketExpr equalExpr(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(primary).optChoose(equalExprTail, notEqualExprTail).modify([](SymbolicPacketSet &a, UserData) {
            bitpowder::lib::unused(a);
            //std::cout << "returning from equalExpr " << a << std::endl;
            return 0;
        });;
    }

   static PInt integerHaakjes(PInt cont, UserData userData) {
       bitpowder::lib::unused(userData);
       return cont()
                .perform(PacketExpressionLexer::OPEN, constantExpr, PacketExpressionLexer::CLOSE)
                .error(ERROR_IN_HAAKJES_END);
    }

    static PInt integerPrimary(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().choose(constant, integerHaakjes);
    }

    static PInt mulOp(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::INLINE_MUL).error(ERROR_IN_MUL).process(integerPrimary, [](Constant & a, const Constant & b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = a*b;
            return 0;
        });
    }

    static PInt divOp(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::INLINE_DIV).error(ERROR_IN_DIV).process(integerPrimary, [](Constant & a, Constant && b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = a / b;
            return 0;
        });
    }

    static PInt modOp(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::INLINE_MOD).error(ERROR_IN_MOD).process(integerPrimary, [](Constant & a, Constant && b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = a % b;
            return 0;
        });
    }

    // multiplication =  pow [multiplicationTail]
    static PInt multiplication(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(integerPrimary).repeatChoose(mulOp, divOp, modOp);
    }

    static PInt plusOp(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept('+').error(ERROR_IN_PLUS).process(multiplication, [](Constant &a, Constant &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = a + b;
            return 0;
        });
    }

    static PInt minOp(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept('-').error(ERROR_IN_MIN).process(multiplication, [](Constant &a, Constant &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = a - b;
            return 0;
        });
    }

    // addition = multiplication [additionTail]
    static PInt addition(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(multiplication).repeatChoose(plusOp, minOp);
    }

    static PInt constantExpr(PInt cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(addition);
    }

    static PPacketExpr lessOp(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept('<').process(constantExpr, [](SymbolicPacketSet &a, Constant b, UserData userData) {
            bitpowder::lib::unused(userData);
            a.lessAs(b);
            return 0;
        });
    }

    static PPacketExpr greaterOp(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept('>').process(constantExpr, [](SymbolicPacketSet &a, Constant b, UserData userData) {
            bitpowder::lib::unused(userData);
            a.greaterAs(b);
            return 0;
        });
    }

    static PPacketExpr lessEqOp(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::INLINE_LESS_EQ).process(constantExpr, [](SymbolicPacketSet &a, Constant b, UserData userData) {
            bitpowder::lib::unused(userData);
            a.lessEqualAs(b);
            return 0;
        });
    }

    static PPacketExpr greaterEqOp(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::INLINE_GREATER_EQ).process(constantExpr, [](SymbolicPacketSet &a, Constant b, UserData userData) {
            bitpowder::lib::unused(userData);
            a.greaterEqualAs(b);
            return 0;
        });
    }

    static PPacketExpr compare(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(equalExpr).optChoose(lessOp, lessEqOp, greaterOp, greaterEqOp).modify([](SymbolicPacketSet &a, UserData) {
            bitpowder::lib::unused(a);
            //std::cout << "returning from compare " << a << std::endl;
            return 0;
        });
    }

    static PPacketExpr ifThenElseTail(PPacketExpr cont, UserData userData) {
        SymbolicPacketSet a;
        SymbolicPacketSet b;
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::QUESTION).fetch(compare, a).accept(PacketExpressionLexer::COLON).fetch(compare, b).modify([](SymbolicPacketSet &retval, SymbolicPacketSet &&a, SymbolicPacketSet &&b, UserData) {
            SymbolicPacketSet value = retval;
            SymbolicPacketSet valueNegated = value;
            valueNegated.negate();
            retval = performOr( performAnd(value, a),
                                performAnd(valueNegated, b));
            //std::cout << "returning from ifThenElse: (" << value << " and " << a << ") or (" << valueNegated << " and " << b << ")" << std::endl;
            //std::cout << "returning from ifThenElse: (" << performAnd(value, a) << ") or (" << performAnd(valueNegated, b) << ")" << std::endl;
            //std::cout << "returning from ifThenElse " << retval << std::endl;
            return 0;
        }, std::move(a), std::move(b));
    }
    static PPacketExpr ifThenElse(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(compare).opt(ifThenElseTail);
    }


    static SymbolicPacketSet performAnd(const SymbolicPacketSet &a, const SymbolicPacketSet &b) {
        SymbolicPacketSet retval;
        for (const SymbolicPacket &packetA : a.values) {
            for (const SymbolicPacket &packetB : b.values) {
                SymbolicPacket current;
                // first process field present in first or both
                for (typename decltype(packetA.fields)::const_iterator fieldA = packetA.fields.cbegin(); fieldA != packetA.fields.cend(); ++fieldA) {
                    //std::cout << "andOp; processing field name " << fieldA->first << std::endl;
                    typename decltype(packetB.fields)::const_iterator fieldB = packetB.fields.find(fieldA->first);
                    if (fieldB != packetB.fields.cend()) {
                        // if present in both, take intersection
                        std::vector<std::shared_ptr<SymbolicPacketField>> newFields = fieldA->second->getIntersection(fieldB->second);
                        if (newFields.size() > 1)
                            throw Exception("can not handle non-continious intersection yet in the parser (andOp), or possible typing error (mixing field of different types)");
                        if (newFields.size() == 0)
                            goto next;
                        //std::cout << "andOp; intersection: " << *newFields[0] << std::endl;
                        current.fields[fieldA->first] = newFields[0];
                    } else {
                        // copy if not present
                        current.fields[fieldA->first] = fieldA->second;
                    }
                }
                // secondly process fields only present in second
                for (typename decltype(packetB.fields)::const_iterator fieldB = packetB.fields.cbegin(); fieldB != packetB.fields.cend(); ++fieldB) {
                    //std::cout << "andOp; processing field name " << fieldB->first << std::endl;
                    typename decltype(packetA.fields)::const_iterator fieldA = packetA.fields.find(fieldB->first);
                    if (fieldA == packetA.fields.cend()) {
                        current.fields[fieldB->first] = fieldB->second;
                    }
                }
                retval.values.push_back(current);
next:
                ;
            }
        }
        return retval;
    }

    static SymbolicPacketSet performOr(const SymbolicPacketSet &a, const SymbolicPacketSet &b) {
        SymbolicPacketSet retval;
        for (auto &value : a.values)
            retval.values.push_back(value);
        for (auto &value : b.values)
            retval.values.push_back(value);
        return retval;
    }

    static PPacketExpr andOp(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::AND_OP).process(ifThenElse, [](SymbolicPacketSet &a, SymbolicPacketSet &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = performAnd(a, b);
            return 0;
        }).modify([](SymbolicPacketSet &a, UserData) {
            bitpowder::lib::unused(a);
            //std::cout << "returning from andOp" << a << std::endl;
            return 0;
        });
    }

    static PPacketExpr orOp(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::OR_OP).process(ifThenElse, [](SymbolicPacketSet &a, SymbolicPacketSet &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = performOr(a, b);
            return 0;
        }).modify([](SymbolicPacketSet &a, UserData) {
            bitpowder::lib::unused(a);
            //std::cout << "returning from orOp" << a << std::endl;
            return 0;
        });
    }

    static PPacketExpr logical(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(ifThenElse).repeatChoose(andOp, orOp).modify([](SymbolicPacketSet &a, UserData) {
            bitpowder::lib::unused(a);
            //std::cout << "returning from logical " << a << std::endl;
            return 0;
        });
    }

    static PPacketExpr expr(PPacketExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(logical).modify([](SymbolicPacketSet &a, UserData) {
            bitpowder::lib::unused(a);
            //std::cout << "returning from expr " << a << std::endl;
            return 0;
        });
    }

    static PSpecExpr specExprOr(PSpecExpr cont, UserData userData) {
        String value;
        bitpowder::lib::unused(userData);
        return cont().perform(PacketExpressionLexer::OR_OP, PacketExpressionLexer::OPEN).fetch(value).accept(PacketExpressionLexer::COMMA)
                .process(expr, [](MessageSpec::Ref &a, String endpoint, SymbolicPacketSet &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = a | S(endpoint, b.values);
            return 0;
        }, value).accept(PacketExpressionLexer::CLOSE);
    }

    static PSpecExpr specExprAnd(PSpecExpr cont, UserData userData) {
        String value;
        bitpowder::lib::unused(userData);
        return cont().perform(PacketExpressionLexer::AND_OP, PacketExpressionLexer::OPEN).fetch(value).
                accept(PacketExpressionLexer::COMMA).process(expr, [](MessageSpec::Ref &a, String endpoint, SymbolicPacketSet &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = a & S(endpoint, b.values);
            return 0;
        }, value).accept(PacketExpressionLexer::CLOSE);
    }

    static PSpecExpr specExpr(PSpecExpr cont, UserData userData) {
        String value;
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::OPEN).fetch(value).accept(PacketExpressionLexer::COMMA).
                process(expr, [](MessageSpec::Ref &a, String endpoint, SymbolicPacketSet &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = S(endpoint, b.values);
            return 0;
        }, value).accept(PacketExpressionLexer::CLOSE).repeatChoose(specExprAnd, specExprOr);
    }

    static PSourceExpr sourceSpecTail(PSourceExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::TO).process(specExpr, [](SpecSet &a, MessageSpec::Ref &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a.add(std::move(b));
            return 0;
        });
    }

    static PSourceExpr sourceSpec(PSourceExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().process(expr, [](SpecSet &a, SymbolicPacketSet &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a.add(std::move(b));
            return 0;
        }).opt(sourceSpecTail);
    }

    static PSourceExpr sourceExpr(PSourceExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(sourceSpec).repeat(PacketExpressionLexer::COMMA, sourceSpec);
    }
};

PacketExpressionParseResult ParsePacketExpression(const String &str, MemoryPool& memoryPool)
{
    PacketExpressionLexer lexer(str);
    auto p = ParserState<PacketExpressionLexer, 1>(lexer);
    SymbolicPacketSet result;
    int retval = Parser<PacketExpressionLexer, 1, SymbolicPacketSet, PacketExpression<PacketExpressionLexer>::UserData>(&p, memoryPool).perform(PacketExpression<PacketExpressionLexer>::expr).end().retreive(result);
    if (retval != 0) {
        TokenBase *token = p.getToken();
        std::cout << "token: " << token << std::endl;
        std::cout << "token representing: " << lexer.getInputString(token) << std::endl;
        std::cout << "remaining: " << lexer.remaining() << std::endl;
        return {token ? token->start : 0, lexer.getInputString(token)};
    }
    result.updateHash();
    //std::cout << "result of parse is " << result << std::endl;
    return std::move(result);
}

SourceExpressionParseResult ParseSourceExpression(const String &str, MemoryPool& memoryPool)
{
    PacketExpressionLexer lexer(str);
    auto p = ParserState<PacketExpressionLexer, 1>(lexer);
    SpecSet result;
    int retval = Parser<PacketExpressionLexer, 1, SpecSet, PacketExpression<PacketExpressionLexer>::UserData>(&p, memoryPool)
            .perform(PacketExpression<PacketExpressionLexer>::sourceExpr)
            .end()
            .retreive(result);
    if (retval != 0) {
        TokenBase *token = p.getToken();
        std::cout << "token: " << token << std::endl;
        std::cout << "token representing: " << lexer.getInputString(token) << std::endl;
        std::cout << "remaining: " << lexer.remaining() << std::endl;
        return {token ? token->start : 0, lexer.getInputString(token)};
    }
    result.updateHash();
    //std::cout << "result of parse is " << result << std::endl;
    return std::move(result);
}

class PacketFunctionLexer {
    String original;
    String c;
    Token<String> variable;
    Token<SymbolicInterval> intervalToken;
    Token<char> opToken;
    Token<SymbolicIntervalField::interval_type> constantToken;
    Token<Exception> exceptionToken;
    void whitespace() {
        static auto space = C(' ') + C('\n') + C('\t');
        StringParser(c).span(space).remainder(c);
    }
    bool sawIf = false;
    bool sawThen = false;
public:
    static const Token<char> MUL;
    static const Token<char> DIV;
    static const Token<char> PLUS;
    static const Token<char> MIN;
    static const Token<char> ASSIGNMENT_OP;
    static const Token<char> COMMA_OP;
    static const Token<char> WITH_OP;
    static const Token<char> SET_OPEN;
    static const Token<char> SET_CLOSE;
    static const Token<char> OPEN;
    static const Token<char> CLOSE;
    static const Token<char> SET_FIELD;

    static const Token<char> IF_OP;
    static const Token<char> THEN_OP;
    static const Token<char> ELSE_OP;

    PacketFunctionLexer(const String &str) : original(str), c(str), variable(), intervalToken(), opToken(), constantToken(), exceptionToken() {
    }
    TokenBase *next() {
        whitespace();
        const char *currentPtr = c.pointer();
        TokenBase *retval = recognise();
        if (retval != nullptr) {
            //std::cout << "token: " << retval << " type_hash=" << retval->type << std::endl;
            retval->start = currentPtr - original.pointer();
            retval->length = c.pointer() - currentPtr;
        }
        return retval;
    }
    TokenBase *recognise() {
        if (sawThen) {
            sawThen = false;
            opToken = THEN_OP;
            return &opToken;
        }
        if (c.length() == 0)
            return nullptr;
        if (sawIf) {
            StringParser(c).split(variable.value, "then").remainder(c);
            sawIf = false;
            sawThen = true;
            return &variable;
        }
        if (StringParser(c).accept(":=").remainder(c)) {
            opToken = ASSIGNMENT_OP;
            return &opToken;
        }
        if (StringParser(c).accept("if").remainder(c)) {
            opToken = IF_OP;
            sawIf = true;
            return &opToken;
        }
        if (StringParser(c).accept("then").remainder(c)) {
            opToken = THEN_OP;
            return &opToken;
        }
        if (StringParser(c).accept("else").remainder(c)) {
            opToken = ELSE_OP;
            return &opToken;
        }
        if (StringParser(c).accept("with").remainder(c)) {
            opToken = WITH_OP;
            return &opToken;
        }
        if (c[0] == MUL || c[0] == DIV || c[0] == PLUS || c[0] == MIN || c[0] == COMMA_OP || c[0] == SET_OPEN || c[0] == SET_CLOSE || c[0] == SET_FIELD || c[0] == OPEN || c[0] == CLOSE) {
            opToken.value = c[0];
            c = c.substring(1);
            //std::cout << "token op: " << opToken.value << std::endl;
            return &opToken;
        }
        SymbolicIntervalField::interval_type minValue;
        SymbolicIntervalField::interval_type maxValue;
        if (StringParser(c).accept("[").parse(minValue).accept("..").parse(maxValue).accept("]").remainder(c)) {
            intervalToken.value = {minValue, maxValue+1};
            //std::cout << "token interval: " << intervalToken << std::endl;
            return &intervalToken;
        }
        if (StringParser(c).parse(constantToken.value).remainder(c)) {
            return &constantToken;
        }
        static auto variableName = Alpha() + C('_') + Num();
        if (StringParser(c).span(variable.value, variableName).remainder(c) && variable.value.size() > 0) {
            return &variable;
        }
        exceptionToken.value = Exception("could not tokenize remaining value");
        return &exceptionToken;
    }
    String getInputString(TokenBase *token) {
        return token ? original.substring(token->start, token->length) : "(none)";
    }
    String remaining() const {
        return c;
    }
};

const Token<char> PacketFunctionLexer::MUL = '*';
const Token<char> PacketFunctionLexer::DIV = '/';
const Token<char> PacketFunctionLexer::PLUS = '+';
const Token<char> PacketFunctionLexer::MIN = '-';
const Token<char> PacketFunctionLexer::ASSIGNMENT_OP = '=';
const Token<char> PacketFunctionLexer::COMMA_OP = ',';
const Token<char> PacketFunctionLexer::WITH_OP = 'w';
const Token<char> PacketFunctionLexer::SET_OPEN = '{';
const Token<char> PacketFunctionLexer::SET_CLOSE = '}';
const Token<char> PacketFunctionLexer::OPEN = '(';
const Token<char> PacketFunctionLexer::CLOSE = ')';
const Token<char> PacketFunctionLexer::SET_FIELD = ':';
const Token<char> PacketFunctionLexer::IF_OP = 'i';
const Token<char> PacketFunctionLexer::THEN_OP = 't';
const Token<char> PacketFunctionLexer::ELSE_OP = 'e';

struct ParsedXMASExpressionFieldAccess : public ParsedXMASExpression {
    String fieldname;
    ParsedXMASExpressionFieldAccess(const String &fieldname) : ParsedXMASExpression(), fieldname(fieldname) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        auto it = packet.fields.find(fieldname);
        if (it == packet.fields.end())
            throw Exception("Non existant field access in xMAS function expression");
        return {it->second};
    }
    virtual void print(std::ostream &out) const {
        out << fieldname;
    }
    virtual void printOldCSyntax(std::ostream &out, std::map<String,int>& enumMap) const {
        bitpowder::lib::unused(enumMap);
        out << "p_" << fieldname;
    }
};

struct ParsedXMASExpressionAddition : public ParsedXMASExpression {
    std::shared_ptr<ParsedXMASExpression> a;
    std::shared_ptr<ParsedXMASExpression> b;
    ParsedXMASExpressionAddition(const std::shared_ptr<ParsedXMASExpression> &a, const std::shared_ptr<ParsedXMASExpression> &b) : ParsedXMASExpression(), a(a), b(b) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        std::vector<std::shared_ptr<SymbolicPacketField>> retval;
        auto retvalA = (*a)(packet);
        auto retvalB = (*b)(packet);
        for (auto& rA : retvalA) {
            for (auto& rB : retvalB) {
                SymbolicIntervalField *intervalA = FastType<SymbolicIntervalField>::cast(rA.get());
                if (!intervalA)
                    throw Exception("expected interval type, is not an interval type");
                SymbolicIntervalField *intervalB = FastType<SymbolicIntervalField>::cast(rB.get());
                if (!intervalB)
                    throw Exception("expected interval type, is not an interval type");
                SymbolicIntervalField field(*intervalA);
                field.add(*intervalB);
                retval.push_back(std::make_shared<SymbolicIntervalField>(field));
            }
        }
        return retval;
    }
    virtual void print(std::ostream &out) const {
        out << "(" << *a << ") + (" << *b << ")";
    }
    virtual void printOldCSyntax(std::ostream &out, std::map<String,int>& enumMap) const {
        bitpowder::lib::unused(enumMap);
        out << "(" << *a << ") + (" << *b << ")";
    }
};

struct ParsedXMASExpressionMinus : public ParsedXMASExpression {
    std::shared_ptr<ParsedXMASExpression> a;
    std::shared_ptr<ParsedXMASExpression> b;
    ParsedXMASExpressionMinus(const std::shared_ptr<ParsedXMASExpression> &a, const std::shared_ptr<ParsedXMASExpression> &b) : ParsedXMASExpression(), a(a), b(b) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        std::vector<std::shared_ptr<SymbolicPacketField>> retval;
        auto retvalA = (*a)(packet);
        auto retvalB = (*b)(packet);
        for (auto& rA : retvalA) {
            for (auto& rB : retvalB) {
                SymbolicIntervalField *intervalA = FastType<SymbolicIntervalField>::cast(rA.get());
                if (!intervalA)
                    throw Exception("expected interval type, is not an interval type");
                SymbolicIntervalField *intervalB = FastType<SymbolicIntervalField>::cast(rB.get());
                if (!intervalB)
                    throw Exception("expected interval type, is not an interval type");
                SymbolicIntervalField field(*intervalA);
                field.minus(*intervalB);
                retval.push_back(std::make_shared<SymbolicIntervalField>(field));
            }
        }
        return retval;
    }
    virtual void print(std::ostream &out) const {
        out << "(" << *a << ") - (" << *b << ")";
    }
    virtual void printOldCSyntax(std::ostream &out, std::map<String,int>& enumMap) const {
        bitpowder::lib::unused(enumMap);
        out << "(" << *a << ") - (" << *b << ")";
    }
};

struct ParsedXMASExpressionMul : public ParsedXMASExpression {
    std::shared_ptr<ParsedXMASExpression> a;
    std::shared_ptr<ParsedXMASExpression> b;
    ParsedXMASExpressionMul(const std::shared_ptr<ParsedXMASExpression> &a, const std::shared_ptr<ParsedXMASExpression> &b) : ParsedXMASExpression(), a(a), b(b) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        bitpowder::lib::unused(packet);
        throw Exception("symbolic semantics of interval multiplication not defined");
    }
    virtual void print(std::ostream &out) const {
        out << "(" << *a << ") * (" << *b << ")";
    }
    virtual void printOldCSyntax(std::ostream &out, std::map<String,int>& enumMap) const {
        bitpowder::lib::unused(enumMap);
        out << "(" << *a << ") * (" << *b << ")";
    }
};

struct ParsedXMASExpressionDiv : public ParsedXMASExpression {
    std::shared_ptr<ParsedXMASExpression> a;
    std::shared_ptr<ParsedXMASExpression> b;
    ParsedXMASExpressionDiv(const std::shared_ptr<ParsedXMASExpression> &a, const std::shared_ptr<ParsedXMASExpression> &b) : ParsedXMASExpression(), a(a), b(b) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        bitpowder::lib::unused(packet);
        throw Exception("symbolic semantics of interval division not defined");
    }
    virtual void print(std::ostream &out) const {
        out << "(" << *a << ") / (" << *b << ")";
    }
    virtual void printOldCSyntax(std::ostream &out, std::map<String,int>& enumMap) const {
        bitpowder::lib::unused(enumMap);
        out << "(" << *a << ") / (" << *b << ")";
    }
};

struct ParsedXMASExpressionSubstituion : public ParsedXMASExpression {
    std::shared_ptr<ParsedXMASExpression> value;
    std::map<SymbolicEnumField::Type, SymbolicEnumField::Type> map;
    ParsedXMASExpressionSubstituion(const std::shared_ptr<ParsedXMASExpression> &value, std::map<SymbolicEnumField::Type,SymbolicEnumField::Type> &&map) : ParsedXMASExpression(), value(value), map(std::move(map)) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        std::vector<std::shared_ptr<SymbolicPacketField>> retval;
        auto result = (*value)(packet);
        for (auto& r : result) {
            SymbolicEnumField *enumeration = dynamic_cast<SymbolicEnumField*>(r.get());
            if (!enumeration)
                throw Exception("expected enum type");
            auto other = map.find("_");
            SymbolicEnumField field;
            for (auto& v : enumeration->values) {
                auto it = map.find(v);
                if (it != map.end()) {
                    if (std::find(field.values.begin(), field.values.end(), it->second) == field.values.end())
                        field.values.push_back(it->second);
                } else if (other != map.end()) {
                    if (std::find(field.values.begin(), field.values.end(), other->second) == field.values.end())
                        field.values.push_back(other->second);
                } else {
                    std::cerr << "key: " << v << std::endl;
                    throw Exception("No substituion defined for key");
                }
            }
            field.updateHash();
            retval.push_back(std::make_shared<SymbolicEnumField>(field));
        }
        return retval;
    }

    virtual void print(std::ostream &out) const {
        out << *value;
        out << " with {";
        bool first = true;
        SymbolicEnumField::Type any;
        for (const auto& e : map) {
            if (e.first != "_") {
                if (!first)
                    out << ", ";
                out << e.first << ": " << e.second;
                first = false;
            } else {
                any = e.second;
            }
        }
        if (!any.empty()) {
            if (!first)
                out << ", ";
            out << "_: " << any;
        }
        out << "}";
    }

    virtual void printOldCSyntax(std::ostream &out, std::map<String,int>& enumMap) const {
        out << "(";
        //bool first = true;		// FIXME: unused-but-set-variable warning
        for (const auto& e : map) {
            if (e.first == "_"_S)
                continue;
            out << "p_" << *value << " == ";
            auto it = enumMap.find(e.first);
            if (it == enumMap.end())
                std::tie(it, std::ignore) = enumMap.insert(std::make_pair(e.first, enumMap.size()));
            out << it->second;
            out << " ? ";
            it = enumMap.find(e.second);
            if (it == enumMap.end())
                std::tie(it, std::ignore) = enumMap.insert(std::make_pair(e.second, enumMap.size()));
            out << it->second;
            out << " : ";
            //first = false;		// FIXME: unused-but-set-variable warning
        }
        auto other = map.find("_");
        if (other == map.end()) {
            std::cerr << "for exporting C-syntax of a substitution, the other field ('_') is required." << std::endl;
            throw Exception("for exporting C-syntax of a substitution, the other field ('_') is required.");
        }
        auto it = enumMap.find(other->second);
        if (it == enumMap.end())
            std::tie(it, std::ignore) = enumMap.insert(std::make_pair(other->second, enumMap.size()));
        out << it->second;
        out << ")";
    }
};

/*
struct ParsedXMASExpressionSwitch : public ParsedXMASExpression {
    std::vector<std::pair<SymbolicPacketSet, std::shared_ptr<ParsedXMASExpression>>> sw;
    ParsedXMASExpressionSwitch(std::vector<std::pair<SymbolicPacketSet, std::shared_ptr<ParsedXMASExpression>>> &&sw) : ParsedXMASExpression(), sw(std::move(sw)) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        return {};
    }
};
*/

struct ParsedXMASExpressionConstant : public ParsedXMASExpression {
    std::shared_ptr<SymbolicPacketField> value;
    ParsedXMASExpressionConstant(const std::shared_ptr<SymbolicPacketField> &value) : ParsedXMASExpression(), value(value) {
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> operator()(const SymbolicPacket &packet) const {
        bitpowder::lib::unused(packet);
        return {value};
    }
    virtual void print(std::ostream &out) const {
        out << *value;
    }
    virtual void printOldCSyntax(std::ostream &out, std::map<String,int>& enumMap) const {
        SymbolicIntervalField* interval = FastType<SymbolicIntervalField>::cast(value.get());
        if (interval && interval->getMin() == interval->getMax()-1) {
            out << interval->getMin();
            return;
        }
        SymbolicEnumField* e = FastType<SymbolicEnumField>::cast(value.get());
        if (e && e->values.size() == 1) {
            auto it = enumMap.find(e->values[0]);
            if (it == enumMap.end())
                std::tie(it, std::ignore) = enumMap.insert(std::make_pair(e->values[0], enumMap.size()));
            out << it->second;
            return;
        }
        throw Exception("unknown field type or not in correct format");
    }
};

template <class Lexer>
struct PacketFunction {
    typedef SymbolicIntervalField::interval_type Constant;
    typedef std::shared_ptr<ParsedXMASFunction> Func;
    typedef std::shared_ptr<ParsedXMASExpression> Expr;
    typedef std::map<SymbolicEnumField::Type,SymbolicEnumField::Type> Map;
    typedef std::vector<std::pair<SymbolicPacketSet, std::shared_ptr<ParsedXMASExpression>>> Switch;
    //typedef Interval& UserData;
    typedef MemoryPool& UserData;
    typedef Parser<Lexer, 1, Func, UserData> & PFuncExpr;
    typedef Parser<Lexer, 1, Expr, UserData> & PExprExpr;
    typedef Parser<Lexer, 1, Map, UserData> & PMap;
    typedef Parser<Lexer, 1, Switch, UserData> & PSwitch;

    static int variable(Expr &value, const Token<String> &token, UserData userData) {
        bitpowder::lib::unused(userData);
        value = std::make_shared<ParsedXMASExpressionFieldAccess>(token.value);
        return 0;
    }

    static int constant(Expr &value, const Token<Constant> &token, UserData userData) {
        bitpowder::lib::unused(userData);
        value = std::make_shared<ParsedXMASExpressionConstant>(std::make_shared<SymbolicIntervalField>(token.value, token.value+1));
        return 0;
    }

    static PExprExpr haakjes(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(PacketFunctionLexer::OPEN, valueExpr, PacketFunctionLexer::CLOSE);
    }

    static PExprExpr primary(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().choose(variable, constant, haakjes);
    }

    static PExprExpr mulOp(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::INLINE_MUL).process(primary, [](Expr& a, Expr&& b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = std::make_shared<ParsedXMASExpressionMul>(a, b);
            return 0;
        });
    }

    static PExprExpr divOp(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketExpressionLexer::INLINE_DIV).process(primary, [](Expr& a, Expr&& b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = std::make_shared<ParsedXMASExpressionDiv>(a, b);
            return 0;
        });
    }

    static PExprExpr multiplication(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(primary).repeatChoose(mulOp, divOp);
    }

    static PExprExpr plusOp(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketFunctionLexer::PLUS).process(multiplication, [](Expr &a, Expr &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = std::make_shared<ParsedXMASExpressionAddition>(a, b);
            return 0;
        });
    }

    static PExprExpr minOp(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().accept(PacketFunctionLexer::MIN).process(multiplication, [](Expr &a, Expr &&b, UserData userData) {
            bitpowder::lib::unused(userData);
            a = std::make_shared<ParsedXMASExpressionMinus>(a, b);
            return 0;
        });
    }

    static PExprExpr additionTail(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().choose(divOp, mulOp, plusOp, minOp).opt(additionTail);
    }

    static PMap substitutionFields(PMap cont, UserData userData) {
        String key;
        String value;
        bitpowder::lib::unused(userData);
        return cont().fetch(key).accept(PacketFunctionLexer::SET_FIELD).fetch(value).modify([](Map &map, String &key, const String &value, UserData userData) {
            bitpowder::lib::unused(userData);
            map[key] = value;
            return 0;
        }, key, value).opt(PacketFunctionLexer::COMMA_OP, substitutionFields);
    }

    static PExprExpr substitutionTail(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(PacketFunctionLexer::WITH_OP, PacketFunctionLexer::SET_OPEN).process(substitutionFields, [](Expr &value, Map &&map, UserData userData) {
            bitpowder::lib::unused(userData);
            value = std::make_shared<ParsedXMASExpressionSubstituion>(value, std::move(map));
            return 0;
        }).perform(PacketFunctionLexer::SET_CLOSE);
    }

/*
    static PSwitch switchFields(PSwitch cont, UserData userData) {
        String key;
        String value;
        return cont().fetch(key).accept(PacketFunctionLexer::SET_FIELD).fetch(value).modify([](Switch &map, String &key, const String &value, UserData userData) {
            // FIXME: implement
            //map[key] = value;
            return 0;
        }, key, value).opt(PacketFunctionLexer::COMMA_OP, switchFields);
    }

    static PExprExpr switchTail(PExprExpr cont, UserData userData) {
        return cont().perform(PacketFunctionLexer::SET_OPEN).process(switchFields, [](Expr &value, Switch &&sw, UserData userData) {
            value = std::make_shared<ParsedXMASExpressionSwitch>(std::move(sw));
            return 0;
        }).perform(PacketFunctionLexer::SET_CLOSE);
    }

*/
    static PExprExpr valueExprTail(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().choose(additionTail, substitutionTail/*, switchTail*/);
    }

    static PExprExpr valueExpr(PExprExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().perform(primary).repeat(valueExprTail);
    }

    static PFuncExpr fieldDefinitions(PFuncExpr cont, UserData userData) {
        String variable;
        bitpowder::lib::unused(userData);
        return cont().fetch(variable).accept(PacketFunctionLexer::ASSIGNMENT_OP).process(valueExpr, [](Func &func, String &variable, const Expr &expr, UserData userData) {
            bitpowder::lib::unused(userData);
            func->fields[variable] = expr;
            return 0;
        }, variable).opt(PacketFunctionLexer::COMMA_OP, fieldDefinitions);
    }

    static PFuncExpr ifThenElse(PFuncExpr cont, UserData userData) {
        Func thenPart;
        String condition;
        bitpowder::lib::unused(userData);
        return cont().accept(PacketFunctionLexer::IF_OP).fetch(condition).accept(PacketFunctionLexer::THEN_OP).
                perform(fieldDefinitions).modify([](Func &func, String &condition, UserData userData) {
            func->hasCondition = true;
            auto result = ParsePacketExpression(condition, userData);
            if (!result)
                return -1;
            func->conditions = std::move(result.result().values);
            return 0;
        }, condition).retreive(thenPart).accept(PacketFunctionLexer::ELSE_OP).store(std::make_shared<ParsedXMASFunction>()).
                    perform1(exprTail).modify([](Func &func, Func &thenPart, UserData userData) {
            bitpowder::lib::unused(userData);
            thenPart->next = func;
            func = thenPart;
            return 0;
        }, thenPart);
    }

    static PFuncExpr exprTail(PFuncExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().choose(ifThenElse, fieldDefinitions);
    }

    static PFuncExpr expr(PFuncExpr cont, UserData userData) {
        bitpowder::lib::unused(userData);
        return cont().store(std::make_shared<ParsedXMASFunction>()).choose(exprTail).modify([](Func &a, UserData) {
            bitpowder::lib::unused(a);
            //std::cout << "returning from expr " << a << std::endl;
            return 0;
        });
    }
};

PacketFunctionParseResult ParsePacketFunction(const String &str, MemoryPool &mp)
{
    PacketFunctionLexer lexer(str);
    auto p = ParserState<PacketFunctionLexer, 1>(lexer);
    PacketFunction<PacketFunctionLexer>::Func result;
    int retval = Parser<PacketFunctionLexer, 1, PacketFunction<PacketFunctionLexer>::Func, MemoryPool&>(&p, mp)
                .perform(PacketFunction<PacketFunctionLexer>::expr)
                .end()
                .retreive(result);
    if (retval != 0) {
        TokenBase *token = p.getToken();
        std::cout << "token: " << token << std::endl;
        std::cout << "token representing: " << lexer.getInputString(token) << std::endl;
        std::cout << "remaining: " << lexer.remaining() << std::endl;
        return {token ? token->start : 0, lexer.getInputString(token)};
    }
    //std::cout << "result of parse is " << result << std::endl;
    return std::move(result);
}


template <class T>
T *insert(MemoryPool& mp, std::map<String, XMASComponent*>& allComponents, const String& name) {
    //std::cout << name << std::endl;
    if (allComponents.find(name) != allComponents.end())
        throw Exception(42, __FILE__, __LINE__);
    T *comp = new(mp, &destroy<XMASComponent>) T(name);
    allComponents.insert(std::make_pair(comp->getName(), comp));
    return comp;
}

template <class T, typename... Args>
T *insert(MemoryPool& mp, std::map<String, XMASComponent*>& allComponents, const String& name, Args... args) {
    if (allComponents.find(name) != allComponents.end())
        throw Exception(42, __FILE__, __LINE__);
    T *comp = new(mp, &destroy<XMASComponent>) T(name, args...);
    allComponents.insert(std::make_pair(comp->getName(), comp));
    return comp;
}

std::pair<std::map<bitpowder::lib::String, XMASComponent *>,JSONData> parse_xmas_from_json(const std::string &str, MemoryPool &mp)
{
    String current(str);
    auto parseResult = ParseJSON(current, mp);
    if (!parseResult) {
        std::cerr << "error in parsing json string stream : " << parseResult.error() << " at " << parseResult.position() << std::endl;
        return std::make_pair(std::map<String, XMASComponent *>(), JSONData());
    }
    return generate_xmas_from_parse_result(parseResult, mp, {});
}

std::pair<std::map<bitpowder::lib::String, XMASComponent *>,JSONData> parse_xmas_from_file(const std::string &filename, MemoryPool &mp)
{
    struct stat st;
    if (stat(filename.c_str(), &st)) {
        std::cerr << "error: " << strerror(errno) << std::endl;
        return std::make_pair(std::map<String, XMASComponent *>(), JSONData());
    }
#ifdef __MINGW32__
    int fd = open(filename.c_str(), O_RDONLY | O_BINARY);
#else
    int fd = open(filename.c_str(), O_RDONLY);
#endif
    if (!fd) {
        std::cerr << "error: " << strerror(errno) << std::endl;
        return std::make_pair(std::map<String, XMASComponent *>(), JSONData());
    }
    char *buffer = (char*)mp.alloc(st.st_size);
    int size = read(fd, buffer, st.st_size);
    if (size != st.st_size) {
        std::cerr << "wrong number of bytes read: " << size << " instead of " << st.st_size << std::endl;
        abort();
    }
    close(fd);

    String current(buffer, size);
    auto parseResult = ParseJSON(current, mp);
    if (!parseResult) {
        std::cerr << "error in parsing json file " << filename << ": " << parseResult.error() << " at " << parseResult.position() << std::endl;
        return std::make_pair(std::map<String, XMASComponent *>(), JSONData());
    }
    return generate_xmas_from_parse_result(parseResult, mp, {});
}

/**
 * @brief generate_xmas_from_parse_result Generates a map with XMASComponent pointers from the component's name
 *
 * This function uses the parseResult from a previous parse function using ParseJSON to generate a map
 * containing the XMASComponent instances indexed by name.
 *
 * @param parseResult The result of calling ParseJSON()
 * @param mp A reference to the memory pool.
 * @return The map of components indexed by name
 */
std::pair<std::map<bitpowder::lib::String, XMASComponent *>,JSONData> generate_xmas_from_parse_result(
        JSONParseResult &parseResult, MemoryPool &mp, const std::map<std::string, std::unique_ptr<XMASNetwork>> &networks) {
    std::map<String, XMASComponent *> retval;

    //std::cout << retval.result() << std::endl;
    JSONData json = parseResult.result();

    // pass one: make all components
    for (auto &jsonComponent : json["NETWORK"]) {
        String name = jsonComponent["id"];
        //std::cout << name << ": " << jsonComponent << std::endl;
        HashedString type = jsonComponent["type"];
        if (type == "source"_HS) {
            insert<XMASSource>(mp, retval, name);
        } else if (type == "sink"_HS) {
            insert<XMASSink>(mp, retval, name);
        } else if (type == "queue"_HS) {
            insert<XMASQueue>(mp, retval, name);
        } else if (type == "function"_HS) {
            insert<XMASFunction>(mp, retval, name);
        } else if (type == "xswitch"_HS) {
            insert<XMASSwitch>(mp, retval, name);
        } else if (type == "merge"_HS) {
            insert<XMASMerge>(mp, retval, name);
        } else if (type == "xfork"_HS) {
            insert<XMASFork>(mp, retval, name);
        } else if (type == "join"_HS) {
            insert<XMASJoin>(mp, retval, name);
        } else if (type == "composite"_HS) {
            String networkName = jsonComponent["subnetwork"];
            auto network_it = networks.find(networkName.stl());
            if (network_it != networks.end()) {
                XMASNetwork* network = network_it->second.get();
                insert<XMASComposite>(mp, retval, name, std::ref(*network));
            } else {
                throw Exception("Required composite network not loaded");
            }
        } else {
            std::cerr << type << std::endl;
            throw Exception("Unsupported component type");
        }
    }

    // pass two: connect all components, parse input types
    for (auto &jsonComponent : json["NETWORK"]) {
        String name = jsonComponent["id"];
        XMASComponent *component = retval[name];
        if (!component) {
            throw Exception((std::string)("inconsistent JSON file, component '" + name + "' missing"));
        }
        std::vector<Output*> outs;
        auto its = component->outputPorts();
        outs.insert(outs.begin(), its.begin(), its.end());

        for (auto &jsonConnection : jsonComponent["outs"]) {
            if (outs.empty())
                throw Exception("too many outputs specified for this component");
            Output *output = outs.front();

            XMASComponent *connectWith = retval[jsonConnection["id"].asString()];
            if (!connectWith)
                throw Exception("non-existent component referenced");
            std::vector<Input*> ins;
            auto its = connectWith->inputPorts();
            ins.insert(ins.begin(), its.begin(), its.end());

            int index = (int)jsonConnection["in_port"];
            if (index < 0 || static_cast<size_t>(index) >= ins.size())		// FIXME: sign-compare warning
                throw Exception("non existent input specified");

            Input *input = ins[index];
            connect(*output, *input);

            outs.erase(outs.begin());
        }

        // read canvas data
        JSONData posData = jsonComponent["pos"];
        if (!posData.isNull()) {
            try {
                CanvasComponentExtension* cce = new CanvasComponentExtension {
                    posData["x"].asNumber(),
                    posData["y"].asNumber(),
                    posData["orientation"].asNumber(),
                    posData["scale"].asNumber() * 0.01f      // NOTE: Parser only supports integral numbers, use ints for now and convert to float
                };
                component->addExtension(cce);
            } catch (Exception e) {
                std::cerr << "Invalid canvas data:" << std::endl << jsonComponent << std::endl;
                std::cerr << e << std::endl;
                std::cerr << "Skipping parse of pos data from json." << std::endl;
                // Removed exit(-1): frustrates xmd / user interface
                throw e;
            }
        }

        XMASSink *sink = dynamic_cast<XMASSink*>(component);
        if (sink) {
            try {
                JSONData external = jsonComponent["external"];          // boolean not supported by JSONData! use 0/1
                sink->external = external.isNumber() && (external.asNumber() > 0);
            } catch (Exception e) {
                std::cerr << jsonComponent << std::endl;
                std::cerr << e << std::endl;
                std::cerr << "Skipping parse of Sink external from json." << std::endl;
                // Removed exit(-1): frustrates xmd / user interface
                throw e;
            }
        }
        XMASSource *src = dynamic_cast<XMASSource*>(component);
        if (src) {
            try {
                JSONData external = jsonComponent["external"];          // boolean not supported by JSONData! use 0/1
                src->external = external.isNumber() && (external.asNumber() > 0);

                String types = jsonComponent["fields"][0]["init_types"];
                auto result = ParseSourceExpression(types, mp);
                if (result) {
#ifdef DEBUG_PARSING
                    std::cout << "parsing " << types << ": " << result.result() << std::endl;
#endif
                    for (auto &packet : result.result().spec) {
                        attachMessageSpec(outs[0], std::get<0>(packet).values, std::get<1>(packet));
                    }
                } else {
                    std::cerr << "parsing " << types << std::endl;
                    std::cerr << "error parsing at position " << result.position() << " is " << result.error() << std::endl;
                    std::cerr << "Skipping parse of Source expression (init_type) from json." << std::endl;
                    std::cerr << "In source file " << __FILE__ << " and line number " << __LINE__ << std::endl;
                    // Removed exit(-1): frustrates xmd / user interface
                }
            } catch (Exception e) {
                std::cerr << jsonComponent << std::endl;
                std::cerr << e << std::endl;
                throw e;    // Removed exit(-1): frustrates xmd / user interface
            }
        }
        XMASSwitch *sw = dynamic_cast<XMASSwitch*>(component);
        if (sw) {
            try {
                String types = jsonComponent["fields"][0]["function"];
                auto result = ParsePacketExpression(types, mp);
                if (result) {
#ifdef DEBUG_PARSING
                    std::cout << "parsing " << types << ": " << result.result() << std::endl;
#endif
                    //SymbolicPacket packet = {NAMED_INTERVAL("test", result.result().min, result.result().max)};
                    for (auto &packet : result.result().values) {
                        attachSwitchingFunction(sw, packet);
                    }
                } else {
                    std::cerr << "parsing " << types << std::endl;
                    std::cerr << "error parsing at position " << result.position() << " is " << result.error() << std::endl;
                    std::cerr << "Skipping parse of Switch expression (fields) from json." << std::endl;
                    // Removed exit(-1): frustrates xmd / user interface
                }
            } catch (Exception e) {
                std::cerr << jsonComponent << std::endl;
                std::cerr << e << std::endl;
                throw e;    // Removed exit(-1): frustrates xmd / user interface
            }
        }
        XMASFunction *f = dynamic_cast<XMASFunction*>(component);
        if (f) {
            try {
                String types = jsonComponent["fields"][0]["function"];
                auto result = ParsePacketFunction(types, mp);
                if (result) {
#ifdef DEBUG_PARSING
                    std::cout << "parsing " << types << ": " << result.result() << std::endl;
#endif
                    auto func = result.result();
                    f->addExtension(new ParsedXMASFunctionExtension(func));
                    attachFunction(f, [func](const std::vector<SymbolicPacket> &p) {
                        return (*func)(p);
                    });
                } else {
                    std::cerr << "parsing " << types << std::endl;
                    std::cerr << "error parsing at position " << result.position() << " is " << result.error() << std::endl;
                    std::cerr << "Skipping parse of Function expression (fields) from json." << std::endl;
                    // Removed exit(-1): frustrates xmd / user interface
                }
            } catch (Exception e) {
                std::cerr << jsonComponent << std::endl;
                std::cerr << e << std::endl;
                throw e;    // Removed exit(-1): frustrates xmd / user interface
            }
        }
        XMASQueue *q = dynamic_cast<XMASQueue*>(component);
        if (q) {
            try {
                JSONData size = jsonComponent["fields"][0]["size"];
                if (size.isNumber()) {
                    q->c = size.asNumber();
                } else {
                    q->c = size.asString().toNumber<int>();
                }
            } catch (Exception e) {
            }
        }
        XMASJoin *j = dynamic_cast<XMASJoin*>(component);
        if (j) {
            try {
                int port;
                JSONData restricted = jsonComponent["fields"][0]["function"];
                if (restricted.isNumber()) {
                    port = restricted.asNumber();
                } else {
                    port = restricted.asString().toNumber<int>();
                }
                auto restrictedJoin = new ParsedXMASRestrictedJoin(port);
                j->addExtension(restrictedJoin);
            } catch (Exception e) {
            }
        }
    }
    JSONData::Map globals = JSONData::AllocateMap(mp);
    globals["PACKET_TYPE"] = json["PACKET_TYPE"];
    globals["VARS"] = json["VARS"];
    globals["COMPOSITE_OBJECTS"] = json["COMPOSITE_OBJECTS"];
    return std::make_pair(retval, globals);
}
