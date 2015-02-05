#include <gtest/gtest.h>

#include "symbolic.h"
#include "symbolic-enum-field.h"
#include "cycle.h"
#include "messagespec.h"
#include "xmas.h"


namespace {

class CycleCheckerTest : public ::testing::Test {
protected:
    CycleCheckerTest() {

    }
    virtual ~CycleCheckerTest() {

    }

    virtual void SetUp() {
        m_source = new XMASSource("source");
        m_sink = new XMASSink("sink");
        m_fork = new XMASFork("fork");
        m_function = new XMASFunction("function");
        m_join = new XMASJoin("join");
        m_merge = new XMASMerge("merge");
        m_queue = new XMASQueue("queue");
    }

    virtual void TearDown() {
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

TEST_F(CycleCheckerTest, Simple) {
    connect(m_source->o, m_sink->i);

    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};
    attach(&m_source->o, p);

    std::set<XMASComponent*> allComponents = {m_source, m_sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

} // namespace

// EOF

