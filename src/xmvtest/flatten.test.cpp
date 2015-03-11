#include "xmas.h"
#include "flatten.h"

#include <string>
#include "gtest/gtest.h"

class CreditCounterTest : public ::testing::Test
{
protected:
    XMASNetwork         creditCounter {"credit-counter"};

    CreditCounterTest()
    {
        auto in = creditCounter.insert<XMASInGate>("in");
        auto out = creditCounter.insert<XMASOutGate>("out");

        auto source = creditCounter.insert<XMASSource>("source");
        auto fork = creditCounter.insert<XMASFork>("fork");
        auto queue = creditCounter.insert<XMASQueue>("queue");
        auto join = creditCounter.insert<XMASJoin>("join");
        auto sink = creditCounter.insert<XMASSink>("sink");

        queue->c = 12;

        connect(in->o, join->a);
        connect(source->o, fork->i);
        connect(fork->a, queue->i);
        connect(fork->b, out->i);
        connect(queue->o, join->b);
        connect(join->o, sink->i);
    }
};

TEST_F(CreditCounterTest, SourceCCSink) {
    // create a simple test network containing a sink, source and a credit counter composite
    XMASNetwork     test {"test"};

    auto source1 = test.insert<XMASSource>("source1");
    auto sink1 = test.insert<XMASSink>("sink1");
    auto cc = test.insert<XMASComposite>("cc1", std::ref(creditCounter));

    connect(source1->o, cc->getInputs()[0]);
    connect(cc->getOutputs()[0], sink1->i);


    // now flatten this network
    XMASNetwork flattened = flatten(test);

    // and test the result
    auto fcs = flattened.getComponents();
    EXPECT_TRUE(fcs.find("::source1") != fcs.end());
    EXPECT_TRUE(fcs.find("::sink1") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc1::source") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc1::fork") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc1::queue") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc1::join") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc1::sink") != fcs.end());

    // the gates should have been collapsed!
    EXPECT_TRUE(fcs.find("::cc1::in") == fcs.end());
    EXPECT_TRUE(fcs.find("::cc1::out") == fcs.end());

    // check if the components are valid
    for (auto x : flattened.getComponents()) {
        XMASComponent* c = x.second;
        EXPECT_TRUE(c->valid());
        std::cout << x.first << std::endl;
    }

}
