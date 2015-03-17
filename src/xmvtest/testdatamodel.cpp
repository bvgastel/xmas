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

class TestDataModel : public ::testing::Test
{
protected:
    TestDataModel()
    {

    }
    virtual ~TestDataModel()
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

TEST_F(TestDataModel, NameEqTest)
{
    EXPECT_EQ(m_sink->getName(), "sink");
    EXPECT_EQ(m_source->getName(), "source");
    EXPECT_EQ(m_fork->getName(), "fork");
    EXPECT_EQ(m_function->getName(), "function");
    EXPECT_EQ(m_join->getName(), "join");
    EXPECT_EQ(m_merge->getName(), "merge");
    EXPECT_EQ(m_queue->getName(), "queue");
}

TEST_F(TestDataModel, NameNeTest)
{
    EXPECT_NE(m_sink->getName(), "sink1");
    EXPECT_NE(m_source->getName(), "source1");
    EXPECT_NE(m_fork->getName(), "fork1");
    EXPECT_NE(m_function->getName(), "function1");
    EXPECT_NE(m_join->getName(), "join1");
    EXPECT_NE(m_merge->getName(), "merge1");
    EXPECT_NE(m_queue->getName(), "queue1");
}

TEST_F(TestDataModel, NotConnectedInValid)
{
    EXPECT_FALSE(m_sink->valid());
    EXPECT_FALSE(m_source->valid());
    EXPECT_FALSE(m_fork->valid());
    EXPECT_FALSE(m_function->valid());
    EXPECT_FALSE(m_join->valid());
    EXPECT_FALSE(m_merge->valid());
    EXPECT_FALSE(m_queue->valid());
}

TEST_F(TestDataModel, DanglingConnect) {
    connect(m_source->o, m_function->i);
    connect(m_source->o, m_function->i);    // connect twice: no problem
    connect(m_function->o, m_fork->i);
    connect(m_source->o, m_queue->i);       // now m_function is dangling
    connect(m_queue->o, m_merge->a);
    EXPECT_TRUE(m_source->valid());
    EXPECT_TRUE(m_queue->valid());
    EXPECT_FALSE(m_function->valid());
}

TEST_F(TestDataModel, ConnectedValid)
{
    connect(m_source->o, m_function->i);
    connect(m_function->o, m_queue->i);
    connect(m_queue->o, m_sink->i);
    EXPECT_TRUE(m_source->valid());
    EXPECT_TRUE(m_function->valid());
    EXPECT_TRUE(m_sink->valid());
}


} // namespace


// EOF
