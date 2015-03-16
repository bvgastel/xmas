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

class TestJoinFunctionSpec : public ::testing::Test
{
protected:
    TestJoinFunctionSpec()
    {

    }
    virtual ~TestJoinFunctionSpec()
    {

    }

    virtual void SetUp()
    {
        m_join = new XMASJoin("join");
    }

    virtual void TearDown()
    {
        delete m_join;
    }

    const std::string spec_open = "(";
    const std::string spec_close = ")";
    const std::string spec_and = " && ";
    const std::string spec_or_in = " || ";
    const std::string spec_or = " or ";
    const std::string spec_end = ";";
    const std::string spec_delim = ", ";

    std::string port1_in = "1";
    std::string port1_out = "1";
    std::string port2_in = "2";
    std::string port2_out = "2";

    std::string comparison1_in = "data == 1";
    std::string comparison1_out = "data == 1";

    XMASJoin *m_join;

};

TEST_F(TestJoinFunctionSpec, test1_unrestricted_join)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_join->setUnrestrictedJoinExpression(comparison1_in, mp));
    result = m_join->getJoinExpression(mp);
    EXPECT_EQ(comparison1_out, result.stl());
}

TEST_F(TestJoinFunctionSpec, test2_restricted_join)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_join->setRestrictedJoinPort(port1_in, mp));
    result = m_join->getJoinExpression(mp);
    EXPECT_EQ(port1_out, result.stl());
}

} // namespace


// EOF





