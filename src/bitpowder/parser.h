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

#ifndef PARSER_H
#define PARSER_H

#include "type_hash.h"
#include <iostream>
#include <type_traits>
#include <tuple>

/**
 * TODO:
 * - make the tokenizer read in extra data if needed (as in streaming parser)
 * - make the parser return if the tokenizer does not have enough data available to return the next token (remember all the states and choices made by the parser, so if new data is available, it is possible to continue)
 * - make the JSON parser more dynamic with user callbacks, allowing the user to influence which fields are remembered, and possible adjust values on the fly
 */

namespace bitpowder {
namespace lib {

class TokenBase {
public:
    TokenBase(FastTypeT type) : type(type), start(0), length(0) {
    }
    TokenBase(const TokenBase &rhs) : type(rhs.type), start(rhs.start), length(rhs.length) {
    }
    TokenBase& operator=(const TokenBase &rhs) {
        type = rhs.type;
        start = rhs.start;
        length = rhs.length;
        return *this;
    }
    FastTypeT type;
    int start;
    int length;
    virtual ~TokenBase() { }
    virtual void print(std::ostream& out) const = 0;
};

template <class T>
class Token : public TokenBase {
public:
    T value;
    Token() : TokenBase(FastType<Token<T>>::value), value() {
    }
    Token(const T& value) : TokenBase(FastType<Token<T>>::value), value(value) {
        //std::cout << "TokenConst with value " << value << " hash=" << type << std::endl;
    }
    Token(const Token& t) : TokenBase(t), value(t.value) {
        //std::cout << "TokenCopy with value " << value << " hash=" << type << std::endl;
    }
    Token(T&& value) : TokenBase(FastType<Token<T>>::value), value(std::move(value)) {
        //std::cout << "TokenMove with value " << value << " hash=" << type << std::endl;
    }
    ~Token() {
    }
    Token& operator=(const Token &rhs) {
        TokenBase::operator=(rhs);
        value = rhs.value;
        return *this;
    }

    virtual void print(std::ostream& out) const {
        out << "Token[" << value << "]";
    }
    operator T() const {
        return value;
    }
};

//#define PARSER_DEBUG
//#define PARSER_USE_DYNAMIC_CAST

template <class Lexer, int N>
class ParserState {
public:
    Lexer *lexer;
    TokenBase *current[N];

    void step() {
        for (int i = 0; i < N-1; ++i)
            current[i] = current[i+1];
        current[N-1] = lexer->next(); //lexer->end() ? nullptr : lexer->next();
        //std::cout << "token: " << current[N-1] << std::endl;
    }
    ParserState& operator=(const ParserState&c) = delete;
public:
    ParserState(Lexer& lexer,int error = 0) : lexer(&lexer), current() {
        for (auto&c : current) {
            c = this->lexer->next();//this->lexer->end() ? nullptr : this->lexer->next();
            //std::cout << "token: " << c << std::endl;
        }
    }
    ParserState(const ParserState&c) : lexer(c.lexer), current() {
        for (int i = 0; i < N; ++i)
            current[i] = c.current[i];
    }

    template <int I = 0>
    typename std::enable_if<I >= 0 && I < N, TokenBase*>::type getToken() {
        return current[I];
    }
};

template <class Lexer, int N, class T, class UserData>
class Parser {
public:
    typedef ParserState<Lexer, N> ParserStateType;

    public:
    ParserStateType *p;
    T result;

    int err; // 0 = no error
    int test;
    int acceptIndex;
    UserData userData;
public:
    typedef Parser& (*Cont)(Parser&, UserData userData);

    Parser(ParserStateType *ParserState, UserData userData, int test = 0, int acceptIndex = 0, const T&result = T()) : p(ParserState), result(result), err(0), test(test), acceptIndex(acceptIndex), userData(userData) {
    }
    ~Parser() {
    }
    Parser(Parser&& c) : p(c.p), result(c.result), err(c.err), test(c.test), acceptIndex(c.acceptIndex), userData(std::move(c.userData)) {
    }
    Parser(const Parser& c) : p(c.p), result(c.result), err(c.err), test(c.test), acceptIndex(c.acceptIndex), userData(c.userData) {
    }
    Parser& operator=(const Parser& c) {
        p = c.p;
        result = c.result;
        err = c.err;
        test = c.test;
        acceptIndex = c.acceptIndex;
        return *this;
    }

    // expect end
    Parser& end() {
        if (!err && p->current[0])
            err = -1;
        return *this;
    }

    // if there is an undefined error, set the error code to argument
    Parser& error(int ifErrorErrorIs) {
        if (err == -1)
            err = ifErrorErrorIs;
        return *this;
    }

    // indicate there is an error
    Parser& setError(int setErrorTo) {
        err = setErrorTo;
        return *this;
    }

    // fetch the current token and store it as the current value of the parser
    Parser& fetchAndStore() {
        if (!err && (!test || acceptIndex < N)) {
            TokenBase *token = p->current[acceptIndex];
            Token<T> *c = nullptr;
            if (token && FastType<Token<T>>::pointer_of_type(token))
                c = static_cast<Token<T>*>(token);
#ifdef PARSER_USE_DYNAMIC_CAST
            else if (token)
                c = dynamic_cast<Token<T>*>(token);
#endif
            if (c) {
                if (test) {
                    acceptIndex++;
                } else {
                    result = std::move(c->value);
                    p->step();
                }
            } else
                err = -1;
        }
        //std::cout << "set() resulted in ParserState(" << current[0] << ", " << error << ")" << std::endl;
        return *this;
    }

    // accept a token with value expectedToken
    template <class S>
    Parser& accept(const S& expectedToken) {
        if (!err && (!test || acceptIndex < N)) {
#ifdef PARSER_DEBUG
            std::cout << "accept(" << expectedToken << ") (";
            if (p->current[acceptIndex] != nullptr)
                p->current[acceptIndex]->print(std::cout);
            std::cout << ") resulted in" << std::endl;
#endif
            TokenBase *token = p->current[acceptIndex];
            Token<S> *c = nullptr;
            if (token && FastType<Token<S>>::pointer_of_type(token))
                c = static_cast<Token<S>*>(token);
#ifdef PARSER_USE_DYNAMIC_CAST
            else if (token)
                c = dynamic_cast<Token<S>*>(token);
#endif
            if (c && c->value == expectedToken) {
                if (test) {
                    acceptIndex++;
                } else
                    p->step();
            } else
                err = -1;
#ifdef PARSER_DEBUG
            std::cout << " ParserState(";
            if (acceptIndex < N)
                std::cout << p->current[acceptIndex];
            else
                std::cout << "lookahead reached: " << acceptIndex;
            std::cout << ", " << err << ")" << std::endl;
#endif
        }
        return *this;
    }

    // helper class
    template <class S>
    Parser& accept(const Token<S>& expectedToken) {
        return accept<S>(expectedToken.value);
    }

    // execute another rule
    Parser& perform1(Cont path) {
        return err || (test && acceptIndex >= N) ? *this : path(*this, userData);
    }

    // handy shortcut to accept a token of a certain type
    template <class TokenType>
    Parser& perform1(const Token<TokenType>&expectedValue) {
        return accept(expectedValue.value);
    }

    // execute a rule to incorporate a certain token in the current type
    template <class Type>
    Parser& perform1(int (*construct)(T&, const Token<Type>&, UserData userData)) {
        if (!err && (!test || acceptIndex < N)) {
            TokenBase *token = p->current[acceptIndex];
            Token<Type> *c = nullptr;
            if (token && FastType<Token<Type>>::pointer_of_type(token))
                c = static_cast<Token<Type>*>(token);
#ifdef PARSER_USE_DYNAMIC_CAST
            else if (token)
                c = dynamic_cast<Token<Type>*>(token);
#endif
#ifdef PARSER_DEBUG
            std::cout << "perform1() resulted in " << c << " ParserState(" << p->current[acceptIndex] << ", ";
            //std::cout << "convert " << p->current[acceptIndex] << " to Token<Number>: " << c << std::endl;
#endif
            if (c) {
                if (test) {
                    acceptIndex++;
                } else {
                    err = std::move(construct(result, *c, userData));
                    p->step();
                }
            } else
                err = -1;
#ifdef PARSER_DEBUG
            std::cout << err << ", result=" << result << ")" << std::endl;
#endif
        }
        return *this;
    }

    // perform can execute all kinds of actions
    template<typename Type>
    Parser& perform(Type&& path) {
        return err ? *this : perform1(std::forward<Type>(path));
    }

    // perform can execute all kinds of actions
    template<typename Type, typename... Args>
    Parser& perform(Type&& path, Args&&... path2) {
        return err ? *this : perform1(std::forward<Type>(path)).perform(std::forward<Args>(path2)...);
    }

    // execute the rule indicated in path and store the result of the rule in 'result'
    Parser& fetch(Cont&& path, T& result) {
        if (!err) {
            T a = std::move(this->result);
            perform1(std::forward<Cont>(path));
            if (!err)
                result = std::move(this->result);
            this->result = std::move(a);
        }
        return *this;
    }

    // store the token of type T in the variable 'result'
    Parser& fetch(T& result) {
        if (!err) {
            T a = std::move(this->result);
            fetchAndStore();
            if (!err)
                result = std::move(this->result);
            this->result = std::move(a);
        }
        return *this;
    }

    // store the token of type NewT in the variable 'result'
    template <class NewT>
    Parser& fetch(NewT& result) {
        if (!err) {
            Parser<Lexer, N, NewT, UserData> tParser(p, userData, test, acceptIndex);
            tParser.fetchAndStore();
            err = tParser.err;
            test = tParser.test;
            acceptIndex = tParser.acceptIndex;
            if (!err)
                result = std::move(tParser.result);
        }
        return *this;
    }

    // store the result of the rule indicated in 'result'
    template <class NewT, class Type>
    Parser& fetch(int (*construct)(NewT& retval, const Token<Type>&, UserData userData), NewT& result, const NewT& startResult = NewT()) {
        if (!err) {
            Parser<Lexer, N, NewT, UserData> tParser(p, userData, test, acceptIndex, startResult);
            tParser.perform(construct);
            err = tParser.err;
            test = tParser.test;
            acceptIndex = tParser.acceptIndex;
            if (!err)
                result = std::move(tParser.result);
        }
        return *this;
    }

    // store the result of the rule indicated in 'result'
    template <class NewT>
    Parser& fetch(Parser<Lexer, N, NewT, UserData>& (*path)(Parser<Lexer, N, NewT, UserData>& cont, UserData userData), NewT&result, const NewT& startResult = NewT()) {
        if (!err) {
            Parser<Lexer, N, NewT, UserData> tParser(p, userData, test, acceptIndex, startResult);
            tParser.perform(path);
            err = tParser.err;
            test = tParser.test;
            acceptIndex = tParser.acceptIndex;
            if (!err)
                result = std::move(tParser.result);
        }
        return *this;
    }

    // execute some rule, and process the result with a function
    template <typename... Args>
    Parser& process(Cont path, Args&&... args) {
        T value;
        return fetch(path, value).modify(std::forward<Args>(args)..., std::move(value));
    }

    // execute some rule, and process the result with a function
    template <class NewT, class Type, typename... Args>
    Parser& process(int (*construct)(NewT& retval, const Token<Type>&, UserData userData), Args&&... args) {
        if (!err) {
            Parser<Lexer, N, NewT, UserData> tParser(p, userData, test, acceptIndex);
            tParser.perform(construct);
            err = tParser.err;
            test = tParser.test;
            acceptIndex = tParser.acceptIndex;
            if (!err)
                modify(std::forward<Args>(args)..., std::move(tParser.result));
        }
        return *this;
    }

    // execute some rule, and process the result with a function
    template <class NewT, typename... Args>
    Parser& process(Parser<Lexer, N, NewT, UserData>& (*path)(Parser<Lexer, N, NewT, UserData>& cont, UserData userData), Args&&... args) {
        if (!err) {
            Parser<Lexer, N, NewT, UserData> tParser(p, userData, test, acceptIndex);
            tParser.perform(path);
            err = tParser.err;
            test = tParser.test;
            acceptIndex = tParser.acceptIndex;
            if (!err)
                modify(std::forward<Args>(args)..., std::move(tParser.result));
        }
        return *this;
    }

    // optionally repeat this rule
    template<typename... Args>
    Parser& repeat(Args&&... path) {
        if (err || test)
            return *this;
        while (!err && !test) {
            test++;
            int acceptIndex = this->acceptIndex;
            perform(std::forward<Args>(path)...);
            test--;
            this->acceptIndex = acceptIndex;
            if (!err)
                perform(std::forward<Args>(path)...);
        }
        err = 0;
        return *this;
    }

    // optionally repeat this rule
    template<typename... Args>
    Parser& repeatChoose(Args&&... path) {
        if (err || test)
            return *this;
        while (!err && !test) {
            test++;
            int acceptIndex = this->acceptIndex;
            choose(std::forward<Args>(path)...);
            test--;
            this->acceptIndex = acceptIndex;
            if (!err)
                choose(std::forward<Args>(path)...);
        }
        err = 0;
        return *this;
    }

    // optionally execute this rule
    template<typename... Args>
    Parser& opt(Args&&... path) {
        if (err || test)
            return *this;
        test++;
        int acceptIndex = this->acceptIndex;
        perform(std::forward<Args>(path)...);
        test--;
        this->acceptIndex = acceptIndex;
        if (!err) {
            perform(std::forward<Args>(path)...);
            return *this;
        }
        err = 0;
        return *this;
    }

    // optionally execute one of the mentioned rules
    template<typename... Args>
    Parser& optChoose(Args&&... path) {
        if (err || test)
            return *this;
        test++;
        int acceptIndex = this->acceptIndex;
        choose(std::forward<Args>(path)...);
        test--;
        this->acceptIndex = acceptIndex;
        if (!err) {
            choose(std::forward<Args>(path)...);
            return *this;
        }
        err = 0;
        return *this;
    }

    // choose one of the given options
    template <typename Type>
    Parser& choose(Type path) {
        return perform1(std::forward<Type>(path));
    }

    // choose one of the given options
    template<typename Type, typename... Args>
    Parser& choose(Type path, Args&&... path2) {
        if (err)
            return *this;
        test++;
        int acceptIndex = this->acceptIndex;
        perform1(path);
        test--;
        this->acceptIndex = acceptIndex;
        if (!err) {
            //std::cout << "choose succeeded, continue" << std::endl;
            return perform1(path);
        }
        //std::cout << "choose failed with error=" << err << std::endl;
        err = 0;
        return choose(std::forward<Args>(path2)...);
    }

    // retreive the current value
    Parser& retreive(T&result) {
        if (!err && !test) {
            result = std::move(this->result);
        }
        return *this;
    }

    // store a value in the parser
    Parser& store(T (*func)(UserData userData)) {
        if (!err && !test)
            this->result = std::move(func(userData));
        return *this;
    }

    // store this value in the parser
    Parser& store(T&& result) {
        if (!err && !test)
            this->result = std::move(result);
        return *this;
    }

    // store this value in the parser
    Parser& store(const T& result) {
        if (!err && !test)
            this->result = result;
        return *this;
    }

    template <typename Func, typename... Extra>
    Parser& apply(const Func& construct, Extra&&... extra) {
        if (!err && !test)
            result = std::move(construct(std::move(result), std::forward<Extra>(extra)...));
        return *this;
    }

    /*
    template <typename Func, typename... Extra>
    //C++14 typename std::enable_if<std::is_void<typename std::result_of<>::type>::value,Parser&>::type
    Parser& modify(const Func&construct, Extra&&... extra) {
        if (!error && !test)
            construct(result, std::forward<Extra>(extra)...);
        return *this;
    }
    */

    template <typename Func, typename... Extra>
    //C++14 typename std::enable_if<std::is_int<typename std::result_of<>::type>::value,Parser&>::type
    Parser& modify(const Func& construct, Extra&&... extra) {
        if (!err && !test)
            err = construct(result, std::forward<Extra>(extra)..., userData);
        return *this;
    }

    Parser& operator()() {
        return *this;
    }

    operator bool() const {
        return !err;
    }
    operator int() const {
        return err;
    }
};

}
}

namespace std {
std::ostream& operator<< (std::ostream& out, const bitpowder::lib::TokenBase *t);
}

#endif // PARSER_H
