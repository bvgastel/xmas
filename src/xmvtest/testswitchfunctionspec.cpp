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

class TestSwitchFunctionSpec : public ::testing::Test
{
protected:
    TestSwitchFunctionSpec()
    {

    }
    virtual ~TestSwitchFunctionSpec()
    {

    }

    virtual void SetUp()
    {
        m_switch = new XMASSwitch("switch");
    }

    virtual void TearDown()
    {
        delete m_switch;
    }

    const std::string spec_open = "(";
    const std::string spec_close = ")";
    const std::string spec_and = " && ";
    const std::string spec_or_in = " || ";
    const std::string spec_or = " or ";
    const std::string spec_end = ";";
    const std::string spec_delim = "\r\n";

    std::string comparison1_in = "data == 1";
    std::string comparison1_out = "data == 1";

    std::string comparison2_in = "src % 2 == 0";
    std::string comparison2_out = "src % 2 == 0";

    XMASSwitch *m_switch;

};

TEST_F(TestSwitchFunctionSpec, test1_assignment1)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_switch->setSwitchExpression(comparison1_in, mp));
    result = m_switch->getSwitchExpression(mp);
    EXPECT_EQ(comparison1_out, result.stl());
}

TEST_F(TestSwitchFunctionSpec, test2_assignment2)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* Test the spec is retrievable as stored */
    EXPECT_NO_THROW(m_switch->setSwitchExpression(comparison2_in, mp));
    result = m_switch->getSwitchExpression(mp);
    EXPECT_EQ(comparison2_out, result.stl());
}

} // namespace


// EOF



