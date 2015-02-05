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
        comp = new XMASFork("test_fork");
    }

    virtual void TearDown() {
        delete comp;
    }

    XMASComponent *comp;

};

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

