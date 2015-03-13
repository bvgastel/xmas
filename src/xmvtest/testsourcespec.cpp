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

class TestSourceSpec : public ::testing::Test
{
protected:
    TestSourceSpec()
    {

    }
    virtual ~TestSourceSpec()
    {

    }

    virtual void SetUp()
    {
        m_source = new XMASSource("source");
        m_sink = new XMASSink("sink");
        m_fork = new XMASFork("fork");
        m_function = new XMASFunction("function");
        m_join = new XMASJoin("join");
        m_merge = new XMASMerge("merge");
        m_queue = new XMASQueue("queue");
    }

    virtual void TearDown()
    {
        delete m_sink;
        delete m_source;
        delete m_fork;
        delete m_function;
        delete m_join;
        delete m_merge;
        delete m_queue;
    }

    XMASSink *m_sink;
    XMASSource *m_source;
    XMASFork *m_fork;
    XMASFunction *m_function;
    XMASJoin *m_join;
    XMASMerge *m_merge;
    XMASQueue *m_queue;

};

TEST_F(TestSourceSpec, inValid_not_connected)
{
    bitpowder::lib::MemoryPool mp;
    m_source->setSourceExpression(std::string("abc == 20"), mp);
    EXPECT_EQ("", m_source->getSourceExpression());

}

TEST_F(TestSourceSpec, valid_connected)
{
    bitpowder::lib::MemoryPool mp;

    connect(m_source->o, m_sink->i);
    ASSERT_TRUE(m_source->valid());
    ASSERT_TRUE(m_sink->valid());

    /* Test the spec is retrievable as stored */
    ASSERT_NO_THROW(m_source->setSourceExpression(std::string("abc == 20"), mp));
    bitpowder::lib::String result = m_source->getSourceExpression();
    EXPECT_EQ("abc >= 20 && abc < 21", result.stl());
}

TEST_F(TestSourceSpec, replace_spec)
{
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* connect port for valid spec attachment */
    connect(m_source->o, m_sink->i);
    ASSERT_TRUE(m_source->valid());
    ASSERT_TRUE(m_sink->valid());

    /* set specification to one string */
    ASSERT_NO_THROW(m_source->setSourceExpression(std::string("abc == 20"), mp));
    result = m_source->getSourceExpression();
    EXPECT_EQ("abc >= 20 && abc < 21", result.stl());

    /* overwrite specification with a larger string */
    ASSERT_NO_THROW(m_source->setSourceExpression(std::string("abc == 20 && def == 40"), mp));
    result = m_source->getSourceExpression();
    EXPECT_EQ("abc >= 20 && abc < 21 && def >= 40 && def < 41", result.stl());

    /* overwrite specification with a smaller string */
    ASSERT_NO_THROW(m_source->setSourceExpression(std::string("def == 40"), mp));
    result = m_source->getSourceExpression();
    EXPECT_EQ("def >= 40 && def < 41", result.stl());

}

TEST_F(TestSourceSpec, SimpleSymbolics)
{
    connect(m_source->o, m_sink->i);
    EXPECT_TRUE(m_source->valid());
    EXPECT_TRUE(m_sink->valid());

    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};

    attach(&m_source->o, p);

    EXPECT_TRUE(m_source->valid());
    EXPECT_TRUE(m_sink->valid());

    std::set<XMASComponent*> allComponents = {m_source, m_sink};

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = m_sink->i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    ASSERT_EQ(ext->availablePackets.size(), 1U);
    EXPECT_EQ(ext->availablePackets[0], p);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

} // namespace


// EOF
