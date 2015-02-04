#include <gtest/gtest.h>
#include "xmas.h"

namespace {

class XCompFlatTest : public ::testing::Test {
protected:
    XCompFlatTest() {

    }
    virtual ~XCompFlatTest() {

    }

    virtual void SetUp() {
        comp = new XMASFork("test_fork");
    }

    virtual void TearDown() {
        delete comp;
    }

    XMASComponent *comp;

};

TEST_F(XCompFlatTest, NameEqTest) {
    EXPECT_EQ(comp->getName(), "test_fork");
}

TEST_F(XCompFlatTest, SomeThingElse) {
    EXPECT_NE(comp->getName(), "test_not_a_fork");
}

TEST(XMASSink, Valid) {
    XMASSink c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASSource, Valid) {
    XMASSource c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASSource, Simple) {
    XMASSource source("source");
    XMASSink sink("sink");

    connect(source.o, sink.i);
    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};

    attach(&source.o, p);

    EXPECT_TRUE(source.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&source, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    ASSERT_EQ(ext->availablePackets.size(), 1);
    EXPECT_EQ(ext->availablePackets[0], p);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}



} // namespace

// EOF
