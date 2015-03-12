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

TEST_F(TestDataModel, ConnectedValid)
{
    connect(m_source->o, m_function->i);
    connect(m_function->o, m_queue->i);
    connect(m_queue->o, m_sink->i);
    EXPECT_TRUE(m_source->valid());
    EXPECT_TRUE(m_function->valid());
    EXPECT_TRUE(m_sink->valid());
}

TEST_F(TestDataModel, setSourceSpecInValid)
{
    m_source->setSourceExpression(std::string("abc == 20"));
    EXPECT_EQ("", m_source->getSourceExpression());

}

TEST_F(TestDataModel, setSourceSpecValid)
{
    connect(m_source->o, m_sink->i);
    ASSERT_TRUE(m_source->valid());
    ASSERT_TRUE(m_sink->valid());

    ASSERT_NO_THROW(m_source->setSourceExpression(std::string("abc == 20")));

    EXPECT_TRUE(m_source->valid());
    EXPECT_TRUE(m_sink->valid());

    bitpowder::lib::String result = m_source->getSourceExpression();
    EXPECT_EQ("abc >= 20 && abc < 21", result.stl());
}

TEST_F(TestDataModel, SimpleSymbolics)
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
