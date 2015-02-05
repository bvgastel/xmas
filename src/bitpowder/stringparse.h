/**
Copyright 2013-2015 Bernard van Gastel, bvgastel@bitpowder.com.
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

/**
      if (line.parse().split(Char<','>(),
                           nop,
                           span(space).toNumber(day)
                           .span(space).toNumber(month)
                           .accept("GMT").store(remainer)))
        // arrows in c++: GMT => 0; hh:mm => unsigned int (offset), deze integegreren in resultaat
        Parse(line).or(epxr, stmt, [optionally] lambda to integrate current result with new result).store(&expression)

                hoe met conditionals om te gaan, "S1;S2" kunnen parsen maar ook "S1", optioneel dus. Mogelijk
                een extra optie om te parsen met een combine functie
                Parse(line).something.opt(S2, [](Expression a, Expression b) { return Or(a, b) })

                linenumbers/posities onthouden voor elke operatie

                mooie error meldingen (misschien onError())

        struct {
            bool error;
            int position;

        };

    ParseResult<ResultClass>
    .result([]() {
        return TM::Cache(minutes, hours);
    })
*/

/**
  * TODO:
  * - add error messages (with positions in the text)
  * - split tokenizer and parsing (so the whole document does not need to be in memory!)
  * - accept(Token) should be possible, together with accept(object, object, object)
  * - make sure there is a streaming interface available
  * - make it easy to creating some sort of LL<n> parser
  */

#ifndef STRINGPARSE_H
#define STRINGPARSE_H

#include "common.h"
#include "simplestring.h"

namespace bitpowder {
namespace lib {

template <class T = int>
class StringParse
{
    String left;
    T result;
    int error; // 0 = no error

public:
    typedef StringParse & (*Cont)(StringParse &);

    StringParse(const String& input, const T &result = T(), int error = 0) : left(input), result(result), error(0) {
        unused(error);
        //std::cout << "construct StringParse(" << left.size() << ")" << std::endl;
    }
    StringParse(const StringParse &copy) : left(copy.left), result(copy.result), error(copy.error) {
        //std::cout << "copy construct StringParse(" << left.size() << ")" << std::endl;
    }

    ~StringParse() {
        //std::cout << "destruct StringParse(" << left << ", " << error << ")" << std::endl;
    }

    const String & remaining() const {
        return left;
    }

    StringParse & accept(const String &str, int setErrorTo = 1) {
        if (!error) {
            if (left.startsWith(str))
                left = left.substring(str.length());
            else
                error = setErrorTo;
        }
        //std::cout << "accept() resulted in StringParse(" << left << ", " << error << ")" << std::endl;
        return *this;
    }

    StringParse & accept(char c, int setErrorTo = 1) {
        if (!error) {
            if (left.size() > 0 && left.pointer()[0] == c)
                left = left.substring(1);
            else
                error = setErrorTo;
        }
        //std::cout << "accept() resulted in StringParse(" << left << ", " << error << ")" << std::endl;
        return *this;
    }

    StringParse & nonEmpty(int setErrorTo = 1) {
        if (!error && left.empty())
            error = setErrorTo;
        //std::cout << "nonEmpty() resulted in StringParse(" << left.length() << ", " << error << ") " << left.empty() << std::endl;
        return *this;
    }

    StringParse & empty(int setErrorTo = 1) {
        if (!error && !left.empty())
            error = setErrorTo;
        //std::cout << "empty() resulted in StringParse(" << left << ", " << error << ")" << std::endl;
        return *this;
    }

    template <class C>
    StringParse & span(const C & c) {
        if (!error)
            left.doSpan(c);
        return *this;
    }

    template <class Split>
    StringParse & span(String &prefix, const Split &splitOn) {
        if (!error)
            prefix = left.doSpan<Split>(splitOn);
        return *this;
    }

    template <class C>
    StringParse & rspan(const C & c) {
        if (!error)
            left.doRSpan(c);
        return *this;
    }

    StringParse & search(const String &text, int setErrorTo = 1) {
        if (!error) {
            String remainder;
            String prefix = left.split(text, remainder);
            //std::cout << "search: prefix=" << prefix.length() << " original=" << left.length() << std::endl;
            if (prefix == left) {
                // not found
                error = setErrorTo;
                //std::cout << "search: setErrorTo=" << setErrorTo << std::endl;
            } else {
                left = remainder;
            }
        }
        return *this;
    }

    StringParse & searchIgnoreCase(const String &text, int setErrorTo = 1) {
        if (!error) {
            String remainder;
            String prefix = left.splitIgnoreCase(text, remainder);
            //std::cout << "search: prefix=" << prefix.length() << " original=" << left.length() << std::endl;
            if (prefix == left) {
                // not found
                error = setErrorTo;
                //std::cout << "search: setErrorTo=" << setErrorTo << std::endl;
            } else {
                left = remainder;
            }
        }
        return *this;
    }

    template<typename... Args>
    StringParse & search(const String &text, Args... path2) {
        if (error)
            return *this;
        StringParse retval(*this);
        retval.search(path2...);
        _first(retval, [text](StringParse &cont) -> StringParse & { return cont().search(text); });
        return *this;
    }

    template<typename... Args>
    StringParse & searchIgnoreCase(const String &text, Args... path2) {
        if (error)
            return *this;
        StringParse retval(*this);
        retval.searchIgnoreCase(path2...);
        _first(retval, [text](StringParse &cont) -> StringParse & { return cont().searchIgnoreCase(text); });
        return *this;
    }

    StringParse & rsearch(const String &text, int setErrorTo = 1) {
        if (!error) {
            String remainder;
            String prefix = left.rsplit(text, remainder);
            if (prefix == left) {
                // not found
                error = setErrorTo;
            } else {
                left = remainder;
            }
        }
        return *this;
    }

    template <class Split>
    StringParse & splitOn(String &prefix, const Split &splitOn, char &delim) {
        if (!error)
            prefix = left.doSplitOn<Split>(splitOn, &delim);
        return *this;
    }

    template <class Split>
    StringParse & splitOn(String &prefix, const Split &splitOn) {
        if (!error)
            prefix = left.doSplitOn<Split>(splitOn);
        return *this;
    }

    template <class Split>
    StringParse & splitOn(const Split &splitOn, char &delim) {
        if (!error)
            left.doSplitOn<Split>(splitOn, &delim);
        return *this;
    }

    template <class Split>
    StringParse & splitOn(const Split &splitOn) {
        if (!error)
            left.doSplitOn<Split>(splitOn);
        return *this;
    }

    StringParse & split(String &prefix, const String &splitOn) {
        if (!error)
            prefix = left.doSplit(splitOn);
        return *this;
    }

    StringParse & splitIgnoreCase(String &prefix, const String &splitOn) {
        if (!error)
            prefix = left.doSplitIgnoreCase(splitOn);
        return *this;
    }

    template <class Split>
    StringParse & rsplitOn(String &prefix, const Split &splitOn, char *delim = nullptr) {
        if (!error)
            prefix = left.doRSplitOn<Split>(splitOn, delim);
        return *this;
    }

    StringParse & rsplit(String &prefix, const String &splitOn) {
        if (!error)
            prefix = left.doRSplit(splitOn);
        return *this;
    }

    StringParse & rsplitIgnoreCase(String &prefix, const String &splitOn) {
        if (!error)
            prefix = left.doRSplitIgnoreCase(splitOn);
        return *this;
    }

    template <class N>
    StringParse & parse(N &n, int setErrorTo = 1) {
        String remainder;
        N retval = left.toNumber<N>(remainder);
        if (remainder.size() == left.size()) {
            error = setErrorTo;
        } else {
            n = retval;
            left = remainder;
        }
        return *this;
    }

    StringParse& parse() {
        T tmp;
        return parse(tmp).put(tmp);
    }

    template <class T2>
    StringParse<T2> constructUsingResult(std::function<T2 (const T &result)> construct) {
        return StringParse<T2>(left, std::move(construct(result)), error);
    }

    template <class T2>
    StringParse<T2> construct(std::function<T2 ()> construct) {
        return StringParse<T2>(left, std::move(construct()), error);
    }

    /*
    StringParse & apply(std::function<T (T & result)> construct) {
        if (!error)
            result = std::move(construct(std::move(result)));
        return *this;
    }
    */
    //T (*construct)(T &&, Extra&... extra)
    template <typename Func, typename... Extra>
    StringParse & apply(const Func &construct, Extra&... extra) {
        if (!error)
            result = std::move(construct(std::move(result), extra...));
        return *this;
    }

    //void (*construct)(T &, Extra&... extra)
    template <typename Func, class... Extra>
    StringParse & modify(const Func &construct, Extra&... extra) {
        if (!error)
            construct(result, extra...);
        return *this;
    }

    StringParse & put(const T &result) {
        if (!error)
            this->result = result;
        return *this;
    }

    StringParse & retreive(T &result) {
        if (!error)
            result = this->result;
        return *this;
    }

    StringParse & perform(Cont path) {
        if (!error)
            path(*this);
        return *this;
    }

    StringParse & perform(std::function<StringParse & (StringParse & cont)> path) {
        if (!error)
            path(*this);
        return *this;
    }

    StringParse & performTo(Cont path, T &result) {
        if (!error) {
            T a = std::move(this->result);
            perform(path);
            if (!error)
                result = std::move(this->result);
            this->result = std::move(a);
        }
        return *this;
    }

    template <class T2>
    StringParse & performOn(Cont path, T &result) {
        if (!error) {
            StringParse<T2> retval(left, T2(), error);
            retval.perform(path);
            left = retval.left;
            error = retval.error;
            result = std::move(retval.result);
        }
        return *this;
    }

    template <typename Func, typename... Extra>
    StringParse & opt(const Func & path, Extra&... extra) {
        if (!error) {
            String left = this->left;
            T result = this->result;
            path(*this, extra...);
            if (error) {
                this->left = left;
                this->result = std::move(result);
                error = 0;
            }
        }
        return *this;
    }

    StringParse & choose(Cont path) {
        perform(path);
        return *this;
    }


    template<typename... Args>
    StringParse & choose(Cont path, Args... path2) {
        if (error)
            return *this;
        String left = this->left;
        T result = this->result;
        perform(path);
        if (!error)
            return *this;
        this->left = left;
        this->result = std::move(result);
        error = 0;
        choose(path2...);
        return *this;
    }
    /*
    template<typename... Args>
    StringParse & choose(Cont path, Args... path2) {
        if (error)
            return *this;
        StringParse retval(*this);
        retval.perform(path);
        if (!retval.error) {
            *this = retval;
            return *this;
        }
        choose(path2...);
        return *this;
    }
    */

    StringParse & _first(const StringParse &retval2, std::function<StringParse & (StringParse & cont)> path) {
        if (error)
            return *this;
        StringParse retval(*this);
        retval.perform(path);
        if (retval.error && retval2.error) {
            error = retval.error;
        } else if (retval2.error || retval.left.size() > retval2.left.size()) {
            left = retval.left;
            result = retval.result;
        } else {
            left = retval2.left;
            result = retval2.result;
        }
        return *this;
    }

    StringParse & first(Cont path) {
        perform(path);
        return *this;
    }
    template<typename... Args>
    StringParse & first(Cont path, Args... path2) {
        if (error)
            return std::move(*this);
        StringParse retval2(*this);
        retval2.first(path2...);
        return (*this)._first(retval2, path);
    }

    // TODO: split
    // - versie die functie heeft wat die er mee moet doen; return value wordt altijd weggegooit zodat niet per ongeluk LL(n heel groot) parsers geschreven kunnen worden
    // - versie die het opslaat in een string
    // TODO: remainder(String &remainder)
    // - store remainder

    StringParse & remainder(StringParse &remainder) {
        if (!error)
            remainder = *this;
        return *this;
    }

    StringParse & remainder(String &remainder) {
        if (!error)
            remainder = left;
        return *this;
    }

    StringParse & ensure(int extraError) {
        if (!error)
            error = extraError;
        return *this;
    }

    StringParse & operator()() {
        return *this;
    }

    operator bool() const {
        return !error;
    }
};

template <class T = int>
StringParse<T> StringParser(const String &input, const T &result = T()) {
    return StringParse<T>(input, result);
}

}
}

#endif // STRINGPARSE_H
