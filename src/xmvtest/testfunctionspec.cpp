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

#include "xmas.h"
#include "symbolic.h"
#include "symbolic-enum-field.h"
#include "simplestring.h"
#include "messagespec.h"

namespace
{

class TestFunctionSpec : public ::testing::Test
{
protected:
    TestFunctionSpec()
    {

    }
    virtual ~TestFunctionSpec()
    {

    }

    virtual void SetUp()
    {
        m_function = new XMASFunction("function");
    }

    virtual void TearDown()
    {
        delete m_function;
    }

    const std::string spec_open = "(";
    const std::string spec_close = ")";
    const std::string spec_and = " && ";
    const std::string spec_or_in = " || ";
    const std::string spec_or = " or ";
    const std::string spec_end = ";";
    const std::string spec_delim = "\r\n";

    std::string assgn1_in = "data := 1";
    std::string assgn1_out = "";

    std::string assgn2_in = "return p_dx == 0 && p_dy == 0;";       // from 2x2_xy_mesh
    std::string assgn2_out = "";

    std::string assgn3_in = "value := value - 10";   // from func
    std::string assgn3_out = "";

    XMASFunction *m_function;

};

TEST_F(TestFunctionSpec, test1_assignment1)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_function->setFunctionExpression(assgn1_in, mp));
    result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(assgn1_out, result.stl());
}

TEST_F(TestFunctionSpec, test2_assignment2)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_function->setFunctionExpression(assgn2_in, mp));
    result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(assgn2_out, result.stl());
}

TEST_F(TestFunctionSpec, test3_assignment3)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_function->setFunctionExpression(assgn3_in, mp));
    result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(assgn3_out, result.stl());
}

} // namespace


// EOF

