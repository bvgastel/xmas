#define DEBUG_EXCEPTIONS

#include "parser_json.h"

#include <functional>
#include <iterator>
#include <unistd.h>  // NOLINT
#include <gtest/gtest.h>

#include <map>
#include <string>
#include <ostream>
#include <iostream>

#include <math.h>

using namespace bitpowder::lib;
using namespace std;

namespace tests {

/*
int ParseJSONExpression(const String &str, JSONData &result, MemoryPool &mp) {
    //std::cout << "parsing: " << str << std::endl;
    JSONLexer lexer(str);
    ParserState<JSONLexer, 1> p = ParserState<JSONLexer, 1>(lexer);
    int retval = Parser<JSONLexer, 1, JSONData, MemoryPool&>(&p, mp).perform(JSONParserState<JSONLexer>::topLevelExpr).expectEnd(-2).retreive(retval.result());
    if (retval != 0) {
        std::cout << "parsing error in " << str << " code=" << retval << " part='" << lexer.getInputString(p.getToken()) << "' result=";
        result.print(std::cout);
        std::cout << std::endl;
    }
    return retval;
}
*/

TEST(Parser, JSONParserState) {
    MemoryPool mp;
    /*
    JSONData t = JSONData(JSONData::Vector(mp.allocator<JSONData>()), mp);
    t.asVector().push_back(retval.result());
    t.asVector().push_back(JSONData(1));
    t.print(std::cout);
    return;
    */

    auto retval = ParseJSON("null", mp);
    ASSERT_TRUE(retval) << retval.error() << " at " << retval.position();
    EXPECT_EQ(JSONData(), retval.result());

    retval = ParseJSON("1", mp);
    ASSERT_TRUE(retval) << retval.error() << " at " << retval.position();
    EXPECT_EQ(JSONData(1), retval.result());

    retval = ParseJSON("\"foobar\"", mp);
    EXPECT_TRUE(retval);
    EXPECT_EQ(JSONData("foobar"), retval.result());

    mp.clear();
    retval = ParseJSON("[]", mp);
    EXPECT_TRUE(retval);
    EXPECT_EQ(JSONData(JSONData::AllocateVector(mp), mp), retval.result());

    retval = ParseJSON("{}", mp);
    EXPECT_TRUE(retval);
    EXPECT_EQ(JSONData(JSONData::AllocateMap(mp), mp), retval.result());

    /*
    {
        retval = ParseJSON("[1]", mp); EXPECT_TRUE(retval);
        JSONData expected(JSONData::Vector({1}, mp->allocator<JSONData>()), mp);
        EXPECT_EQ(expected, retval.result());
    }
    {
        retval = ParseJSON("[1,2,3]", result); EXPECT_TRUE(retval);
        JSONData expected(JSONData::Vector({1,2,3}), mp);
        EXPECT_EQ(expected, retval.result());
    }
    */

    {
        retval = ParseJSON("{\"someNumber\":1}", mp);
        EXPECT_TRUE(retval);
        JSONData::Map expectedMap = JSONData::AllocateMap(mp);
        expectedMap["someNumber"] = 1;
        JSONData expectedObject(expectedMap, mp);
        EXPECT_EQ(expectedObject, retval.result());
    }

    {
        retval = ParseJSON("{\"someNumber\":1,\"foo\":\"bar\"}", mp);
        EXPECT_TRUE(retval);
        JSONData::Map expectedMap = JSONData::AllocateMap(mp);
        expectedMap["someNumber"] = 1;
        expectedMap["foo"] = "bar"_S;
        JSONData expectedObject(expectedMap, mp);
        EXPECT_EQ(expectedObject, retval.result());
    }

    {
        retval = ParseJSON("{\"someNumber\":1,\"foo\":[\"bar\"]}", mp);
        EXPECT_TRUE(retval);
        JSONData::Map expectedMap = JSONData::AllocateMap(mp);
        expectedMap["someNumber"] = 1;

        JSONData::Vector expectedVector = JSONData::AllocateVector(mp);
        expectedVector.push_back("bar"_S);
        expectedMap["foo"] = JSONData(expectedVector, mp);
        JSONData expectedObject(expectedMap, mp);
        EXPECT_EQ(expectedObject, retval.result());
    }

    std::cout << "sizeof(String)" << sizeof(String) << std::endl;
    //StaticMemoryPool<1024> smp;
    for (int i = 0; i < 1000; i++)
    {
        mp.clear();
        String str = "[{\"id\":2345,\"name\":\"Rookbier\",\"beer_ids\":[356]},{\"id\":2347,\"name\":\"Winterbier\",\"beer_ids\":[198,175,262,402]},{\"id\":2253,\"name\":\"Vlaams Rood\",\"beer_ids\":[64,93,247,94]},{\"id\":2254,\"name\":\"Weizen Dubbelbock\",\"beer_ids\":[43,361]},{\"id\":2257,\"name\":\"Alcohol vrij\",\"beer_ids\":[24,25,299,334]},{\"id\":2258,\"name\":\"Alt\",\"beer_ids\":[]},{\"id\":2259,\"name\":\"American Amber Red\",\"beer_ids\":[353,90]}]";
        //String str = "[[2345,\"Rookbier\",[356]],[2347,\"Winterbier\",[198,175,262,402]],[2253,\"Vlaams Rood\",[64,93,247,94]],[2254,\"Weizen Dubbelbock\",[43,361]],[2257,\"Alcohol vrij\",[24,25,299,334]],[2258,\"Alt\",[]],[2259,\"American Amber Red\",[353,90]]]";
        //retval = ParseJSON(str, &mp); EXPECT_TRUE(retval);
        auto retval = ParseJSON(str, mp);
        assert(retval.result()[0]["id"] == JSONData(2345));
        //std::cout << "memorypool available: " << mp.available() << std::endl;
        //result.print(std::cout);
        //std::cout << std::endl;
    }
    /*
    String str = "[{\"id\":2345,\"name\":\"Rookbier\",\"beer_ids\":[356]},{\"id\":2347,\"name\":\"Winterbier\",\"beer_ids\":[198,175,262,402]},{\"id\":2253,\"name\":\"Vlaams Rood\",\"beer_ids\":[64,93,247,94]},{\"id\":2254,\"name\":\"Weizen Dubbelbock\",\"beer_ids\":[43,361]},{\"id\":2257,\"name\":\"Alcohol vrij\",\"beer_ids\":[24,25,299,334]},{\"id\":2258,\"name\":\"Alt\",\"beer_ids\":[]},{\"id\":2259,\"name\":\"American Amber Red\",\"beer_ids\":[353,90]}]";
    JSONLexer lexer(str);
    while (!lexer.end()) {
        std::cout << "token: " << lexer.getInputString(lexer.next()) << std::endl;
    }
    */

    EXPECT_FALSE(ParseJSON("something", mp));
}

}

