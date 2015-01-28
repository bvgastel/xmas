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

#define DEBUG_EXCEPTIONS

#include "stringparse.h"
#include <functional>
#include <iterator>
#include <unistd.h>  // NOLINT
#include <gtest/gtest.h>

#include <map>
#include <string>
#include <ostream>
#include <iostream>

#include <math.h>


namespace tests {
using namespace bitpowder::lib;

TEST(StringParse, Accept) {
    EXPECT_TRUE(StringParser("Hello Hello").accept("Hello").span(Char<' '>()).accept("Hello").empty());
}

TEST(StringParse, Number) {
    int parsedInt = -1;
    EXPECT_TRUE(StringParser("number=0;something").accept("number=").parse(parsedInt).accept(";something").empty());
    EXPECT_EQ(parsedInt, 0);
    // try to parse a letter
    EXPECT_FALSE(StringParser("number=a").accept("number=").parse(parsedInt).empty());
    // on failure, the int is not modified
    EXPECT_EQ(parsedInt, 0);
}

TEST(StringParse, Construct) {
    int parsedInt = -1;
    int resultingObject = -1;
    EXPECT_TRUE(StringParser("number=10").accept("number=").parse(parsedInt).construct<int>([&]{ return parsedInt; }).empty().retreive(resultingObject));
    EXPECT_EQ(parsedInt, 10);
    EXPECT_EQ(resultingObject, 10);
}

TEST(StringParse, Put) {
    int parsedInt = -1;
    int resultingObject = -1;
    EXPECT_TRUE(StringParser("number=10").accept("number=").parse(parsedInt).put(parsedInt).empty().retreive(resultingObject));
    EXPECT_EQ(parsedInt, 10);
    EXPECT_EQ(resultingObject, 10);
}

TEST(StringParse, Apply) {
    int firstInt = -1;
    int secondInt = -1;
    int resultingObject = -1;
    EXPECT_TRUE(StringParser("2*3").parse(firstInt).put(firstInt).accept("*").parse(secondInt).apply([&](int && current){
        return current * secondInt;
    }).empty().retreive(resultingObject));
    EXPECT_EQ(firstInt, 2);
    EXPECT_EQ(secondInt, 3);
    EXPECT_EQ(resultingObject, 6);
}

TEST(StringParse, Opt) {
    int firstInt = -1;
    int secondInt = -1;
    int resultingObject = -1;
    EXPECT_TRUE(StringParser("2*3").parse().retreive(firstInt).opt([&](StringParse<int> & start){
                    return start().accept("*").parse(secondInt).apply([&](int && current){
                        return current * secondInt;
                    });
                }).empty().retreive(resultingObject));
    EXPECT_EQ(firstInt, 2);
    EXPECT_EQ(secondInt, 3);
    EXPECT_EQ(resultingObject, 6);
}

TEST(StringParse, Opt2) {
    int firstInt = -1;
    int secondInt = -1;
    int resultingObject = -1;
    EXPECT_TRUE(StringParser("2").parse(firstInt).put(firstInt).opt([&](StringParse<int> start){
                    return start().accept("*").parse(secondInt).apply([&](int current){
                        return current * secondInt;
                    });
                }).empty().retreive(resultingObject));
    EXPECT_EQ(firstInt, 2);
    EXPECT_EQ(secondInt, -1);
    EXPECT_EQ(resultingObject, 2);
}

TEST(StringParse, Opt3) {
    int firstInt = -1;
    int secondInt = -1;
    int resultingObject = -1;
    EXPECT_FALSE(StringParser("2*").parse(firstInt).put(firstInt).opt([&](StringParse<int> & start){
                    return start().accept("*").parse(secondInt).apply([&](int current){
                        return current * secondInt;
                    });
                }).empty().retreive(resultingObject));
    EXPECT_EQ(firstInt, 2);
    EXPECT_EQ(secondInt, -1);
    EXPECT_EQ(resultingObject, -1);
}

TEST(StringParse, Opt4) {
    auto c = [](const String &str) -> bool {
        int errorCode;
        int errorFile;
        return StringParser(str).accept("FAILED[").parse(errorCode).opt([&errorFile](StringParse<int> & cont) {
            return cont().accept(":").parse(errorFile);
        }).accept("]");
    };
    EXPECT_TRUE(c("FAILED[-1:1]"));
    EXPECT_TRUE(c("FAILED[-1]"));
    EXPECT_FALSE(c("FAILED[-1"));
    EXPECT_FALSE(c("FAILED[-1:lala]"));
}

StringParse<int> & constant(StringParse<int> & cont) {
    int parsedInt;
    return cont().parse(parsedInt).put(parsedInt);
}

StringParse<int> & expr(StringParse<int> & start);

StringParse<int> & haakjes(StringParse<int> & cont) {
    return cont().accept("(").perform(expr).accept(")");
}

StringParse<int> & primary(StringParse<int> & cont) {
    return cont().choose(haakjes, constant);
}

StringParse<int> & mulTail(StringParse<int> & cont) {
    int a;
    return cont()
            .accept("*")
            .performTo(primary, a)
            .apply([](int b, int &a) {
                return a*b;
            }, a)
            .opt(mulTail);
}

StringParse<int> & mul(StringParse<int> & cont) {
    return cont().perform(primary).opt(mulTail);
}

StringParse<int> & plusTail(StringParse<int> & cont) {
    int a;
    return cont()
            .accept("+")
            .performTo(mul, a)
            .apply([](int b, int & a) {
                return a+b;
            }, a)
            .opt(plusTail);
}

StringParse<int> & plus(StringParse<int> & cont) {
    return cont().perform(mul).opt(plusTail);
}

StringParse<int> & expr(StringParse<int> & start) {
    return plus(start);
}

TEST(StringParse, CalculatorOfPositiveValues) {
    int result = -1;
    EXPECT_TRUE(StringParser("2+1").opt(expr).empty().retreive(result));
    EXPECT_EQ(result, 3);
    EXPECT_TRUE(StringParser("3*3+2*2+1*1").opt(expr).empty().retreive(result));
    EXPECT_EQ(result, 14);
    EXPECT_TRUE(StringParser("(3*3)+(2*2)+(1*1)").opt(expr).empty().retreive(result));
    EXPECT_EQ(result, 14);
    EXPECT_TRUE(StringParser("3*(3+2)*(2+1)*1").opt(expr).empty().retreive(result));
    EXPECT_EQ(result, 45);
}

}
