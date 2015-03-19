#include "xmas.h"
#include "flatten.h"

#include <string>
#include "gtest/gtest.h"


/**
 * @brief Checks if this component and all components connected to it
 * are not gates or composites. During flattening, gates and composites
 * are created but not added to the network. This test checks if some
 * components in the network incorrectly still connect to gates/composites.
 * @param comp
 */
void checkGateAndCompositeFree(XMASComponent* comp) {

    for (auto p : comp->ports()) {
        XMASComponent* init = p->getInitiator();
        XMASComponent* target = p->getTarget();

        // check both ends of the channel (also checks comp itself!)
        EXPECT_FALSE(typeid(*init) == typeid(XMASInGate));
        EXPECT_FALSE(typeid(*init) == typeid(XMASOutGate));
        EXPECT_FALSE(typeid(*init) == typeid(XMASComposite));

        EXPECT_FALSE(typeid(*target) == typeid(XMASInGate));
        EXPECT_FALSE(typeid(*target) == typeid(XMASOutGate));
        EXPECT_FALSE(typeid(*target) == typeid(XMASComposite));
    }

}

void checkValidAndFlat(const XMASNetwork& network) {
    for (auto x : network.getComponents()) {
        XMASComponent* c = x.second;
        EXPECT_TRUE(c->valid());
        checkGateAndCompositeFree(c);
    }
}



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

/*
 * A simple composite containing just an in and an out gate
 * This composite essentially bridges two channels
 */
class BridgeCompositeTest : public ::testing::Test
{
protected:
    XMASNetwork     bridge {"bridge"};

    BridgeCompositeTest()
    {
        auto in = bridge.insert<XMASInGate>("in");
        auto out = bridge.insert<XMASOutGate>("out");

        connect(in->o, out->i);
    }
};



TEST_F(BridgeCompositeTest, SourceBridgeSink) {
    // create a simple test network containing a sink, source and a bridge composite
    XMASNetwork     test {"test"};

    auto source1 = test.insert<XMASSource>("source1");
    auto sink1 = test.insert<XMASSink>("sink1");
    auto bridge1 = test.insert<XMASComposite>("bridge1", std::ref(bridge));

    connect(source1->o, bridge1->getInputs()[0]);
    connect(bridge1->getOutputs()[0], sink1->i);


    // now flatten this network
    XMASNetwork flattened = flatten(test);

    // and test the result
    auto fcs = flattened.getComponents();
    EXPECT_EQ(2, fcs.size());
    EXPECT_TRUE(fcs.find("::source1") != fcs.end());
    EXPECT_TRUE(fcs.find("::sink1") != fcs.end());

    // the gates should have been collapsed!
    EXPECT_TRUE(fcs.find("::bridge1::in") == fcs.end());
    EXPECT_TRUE(fcs.find("::bridge1::out") == fcs.end());

    // check if the components are valid
    checkValidAndFlat(flattened);
}

TEST_F(CreditCounterTest, AlreadyFlat) {
    XMASNetwork     test {"test"};

    auto source1 = test.insert<XMASSource>("source1");
    auto func1 = test.insert<XMASFunction>("func1");
    auto queue1 = test.insert<XMASQueue>("queue1");
    auto sink1 = test.insert<XMASSink>("sink1");

    connect(source1->o, func1->i);
    connect(func1->o, queue1->i);
    connect(queue1->o, sink1->i);

    XMASNetwork flattened = flatten(test);

    auto fcs = flattened.getComponents();
    EXPECT_EQ(4, fcs.size());
    EXPECT_TRUE(fcs.find("::source1") != fcs.end());
    EXPECT_TRUE(fcs.find("::sink1") != fcs.end());
    EXPECT_TRUE(fcs.find("::func1") != fcs.end());
    EXPECT_TRUE(fcs.find("::queue1") != fcs.end());

    // check if the components are valid
    checkValidAndFlat(flattened);

}

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
    checkValidAndFlat(flattened);

}

TEST_F(CreditCounterTest, ConsecutiveCC) {
    // create a simple test network containing a source, two consecutive credit counters and a sink
    // this tests checks the flattener when directly connecting two composites to each other

    XMASNetwork     test {"test"};

    auto source1 = test.insert<XMASSource>("source1");
    auto sink1 = test.insert<XMASSink>("sink1");
    auto cc1 = test.insert<XMASComposite>("cc1", std::ref(creditCounter));
    auto cc2 = test.insert<XMASComposite>("cc2", std::ref(creditCounter));

    connect(source1->o, cc1->getInputs()[0]);
    connect(cc1->getOutputs()[0], cc2->getInputs()[0]);
    connect(cc2->getOutputs()[0], sink1->i);


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
    EXPECT_TRUE(fcs.find("::cc2::source") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc2::fork") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc2::queue") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc2::join") != fcs.end());
    EXPECT_TRUE(fcs.find("::cc2::sink") != fcs.end());

    // the gates should have been collapsed!
    EXPECT_TRUE(fcs.find("::cc1::in") == fcs.end());
    EXPECT_TRUE(fcs.find("::cc1::out") == fcs.end());
    EXPECT_TRUE(fcs.find("::cc2::in") == fcs.end());
    EXPECT_TRUE(fcs.find("::cc2::out") == fcs.end());

    // check if the components are valid
    checkValidAndFlat(flattened);

}
