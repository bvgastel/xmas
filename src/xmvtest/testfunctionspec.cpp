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

    std::string spec_abc_in = "data := i.data + 1\r\nkind := 1";
    std::string spec_abc_out = "";
    std::string spec_def_in = "def := 4 ";
    std::string spec_def_out = "ret_def = 4;";
    std::string spec_abc_def_in = spec_abc_in + spec_delim + spec_def_in;
    std::string spec_abc_def_out =
            spec_abc_out + spec_delim + spec_def_out;

    XMASFunction *m_function;

};

TEST_F(TestFunctionSpec, test1_assignment)
{
    bitpowder::lib::MemoryPool mp;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_function->setFunctionExpression(spec_abc_in, mp));
    bitpowder::lib::String result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(spec_abc_out, result.stl());
}

TEST_F(TestFunctionSpec, test2_replacement)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* set specification to one string */
    ASSERT_NO_THROW(m_function->setFunctionExpression(spec_abc_in, mp));
    result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(spec_abc_out, result.stl());

    /* overwrite specification with a larger string */
    ASSERT_NO_THROW(m_function->setFunctionExpression(spec_abc_def_in, mp));
    result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(spec_abc_def_out, result.stl());

}

TEST_F(TestFunctionSpec, test3_replace_short)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* set specification to a larger string */
    ASSERT_NO_THROW(m_function->setFunctionExpression(spec_abc_def_in, mp));
    result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(spec_abc_def_out, result.stl());

    /* overwrite specification with a smaller string */
    ASSERT_NO_THROW(m_function->setFunctionExpression(spec_def_in, mp));
    result = m_function->getFunctionExpression(mp);
    EXPECT_EQ(spec_def_out, result.stl());

}


} // namespace


// EOF

