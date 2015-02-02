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
    EXPECT_EQ(comp->getName(), "test_comp");
}

} // namespace


// EOF
