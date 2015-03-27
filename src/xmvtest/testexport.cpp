/*********************************************************************
  *
  * Copyright (C) Guus Bonnema, 2015
  *
  * This file is part of the xmas-design tool.
  *
  * The xmas-design tool is free software: you can redistribute it
  * and/or modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation, either version 3 of
  * the License, or (at your option) any later version.
  *
  * The xmas-design tool is distributed in the hope that it will be
  * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
  * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with the xmas-design tool.  If not, see
  * <http://www.gnu.org/licenses/>.
  *
  **********************************************************************/

#include <gtest/gtest.h>

#include "export.h"
#include "parse.h"
#include "simplestring.h"
#include "xmas.h"

namespace
{

class TestExport : public ::testing::Test
{
protected:
    TestExport()
    {

    }
    virtual ~TestExport()
    {

    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    const bitpowder::lib::String json_input_1 = "{"
                                        "\"COMPOSITE_OBJECTS\":[],"
                                        "\"NETWORK\":["
                                          "{\"id\":\"src0\",\"type\":\"source\",\"outs\":[{\"id\":\"q1\",\"in_port\":0}],\"pos\":{\"x\":100,\"y\":100,\"orientation\":0,\"scale\":100},\"fields\":[{\"init_types\":\"value < 65384\"}]},"
                                          "{\"id\":\"q1\",\"type\":\"queue\",\"outs\":[{\"id\":\"q2\",\"in_port\": 0}],\"pos\":{\"x\":210,\"y\":210,\"orientation\":90,\"scale\":200},\"fields\":[{\"size\": 2}]},"
                                          "{\"id\":\"q2\",\"type\":\"queue\",\"outs\":[{\"id\":\"sink3\",\"in_port\": 0}],\"pos\":{\"x\":310,\"y\":310,\"orientation\":0,\"scale\":100},\"fields\":[{\"size\": 5}]},"
                                          "{\"id\":\"sink3\",\"type\":\"sink\",\"pos\": {\"x\":410,\"y\":410,\"orientation\":0,\"scale\":100}}"
                                          "],"
                                          "\"PACKET_TYPE\":{\"val \": 2},"
                                          "\"VARS\":[]"
                                      "}";

    const bitpowder::lib::String json_input_2 = "{"
                                        "\"COMPOSITE_OBJECTS\":[],"
                                        "\"NETWORK\":["
                                          "{\"id\":\"src0\",\"type\":\"source\",\"outs\":[{}],\"pos\":{\"x\":100,\"y\":100,\"orientation\":0,\"scale\":100},\"fields\":[{\"init_types\":\"value < 65384\"}]},"
                                          "],"
                                          "\"PACKET_TYPE\":{\"val \": 2},"
                                          "\"VARS\":[]"
                                      "}";

};

std::set<XMASComponent *> convertToSet(std::map<bitpowder::lib::String, XMASComponent *> compMap) {
    std::set<XMASComponent *> componentSet;
    XMASComponent *comp;
    for (auto entry : compMap) {
        comp = entry.second;
        componentSet.insert(comp);
    }
    return componentSet;
}

TEST_F(TestExport, export_json_string_1)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::JSONData globals = bitpowder::lib::JSONData::AllocateMap(mp);

    std::map<bitpowder::lib::String, XMASComponent *> componentMap;
    bitpowder::lib::String json = json_input_1(mp);
    std::tie(componentMap, globals) = parse_xmas_from_json(json.stl(), mp);

    std::set<XMASComponent *> componentSet = convertToSet(componentMap);
    bitpowder::lib::String result = ::Export(componentSet, globals, mp);

    // Init types and packet types have random memory var names
    EXPECT_EQ(json, result);
}


TEST_F(TestExport, export_json_string_2)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::JSONData globals = bitpowder::lib::JSONData::AllocateMap(mp);

    std::map<bitpowder::lib::String, XMASComponent *> componentMap;
    bitpowder::lib::String json = json_input_2(mp);
    std::tie(componentMap, globals) = parse_xmas_from_json(json.stl(), mp);

    std::set<XMASComponent *> componentSet = convertToSet(componentMap);
    bitpowder::lib::String result = ::Export(componentSet, globals, mp);

    // Init types and packet types have random memory var names
    EXPECT_EQ(json, result);
}

} // namespace


// EOF


