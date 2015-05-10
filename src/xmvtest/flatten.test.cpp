#include "xmas.h"
#include "flatten.h"
#include "flatten-gates.h"

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
        EXPECT_FALSE(typeid(*init) == typeid(XMASSource) && dynamic_cast<XMASSource*>(init)->required_input);
        EXPECT_FALSE(typeid(*init) == typeid(XMASSink) && dynamic_cast<XMASSink*>(init)->required_output);

        EXPECT_FALSE(typeid(*target) == typeid(XMASInGate));
        EXPECT_FALSE(typeid(*target) == typeid(XMASOutGate));
        EXPECT_FALSE(typeid(*target) == typeid(XMASComposite));
        EXPECT_FALSE(typeid(*target) == typeid(XMASSource) && dynamic_cast<XMASSource*>(target)->required_input);
        EXPECT_FALSE(typeid(*target) == typeid(XMASSink) && dynamic_cast<XMASSink*>(target)->required_output);
    }

}

void checkValidAndFlat(const XMASNetwork& network) {
    for (auto x : network.getComponentMap()) {
        XMASComponent* c = x.second;
        EXPECT_TRUE(c->valid());
        checkGateAndCompositeFree(c);
    }
}

Input& composite_i(XMASComposite* c, const char* portName) {
    Input* i = c->findInputPort(portName);
    EXPECT_NE(i, nullptr);
    return *i;
}

Output& composite_o(XMASComposite* c, const char* portName) {
    Output* o = c->findOutputPort(portName);
    EXPECT_NE(o, nullptr);
    return *o;
}

class CreditCounterTest : public ::testing::Test
{
protected:
    XMASNetwork         creditCounter {"credit-counter"};

    CreditCounterTest()
    {
        auto in = creditCounter.insert<XMASSource>("in", true);
        auto out = creditCounter.insert<XMASSink>("out", true);

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
        auto in = bridge.insert<XMASSource>("in", true);
        auto out = bridge.insert<XMASSink>("out", true);

        connect(in->o, out->i);
    }
};

TEST_F(BridgeCompositeTest, EmptyFlatNetwork) {
    XMASNetwork test {"test"};

    auto bridge1 = test.insert<XMASComposite>("bridge", std::ref(bridge));

    connect(composite_o(bridge1, "out"), composite_i(bridge1, "in"));

    XMASNetwork flattened = flatten(test);

    auto fcs = flattened.getComponentMap();
    EXPECT_EQ(0, fcs.size());

    // quite pointless to check an empty network, but why not?
    checkValidAndFlat(flattened);
}

TEST_F(BridgeCompositeTest, SourceBridgeSink) {
    // create a simple test network containing a sink, source and a bridge composite
    XMASNetwork     test {"test"};

    auto source1 = test.insert<XMASSource>("source1");
    auto sink1 = test.insert<XMASSink>("sink1");
    auto bridge1 = test.insert<XMASComposite>("bridge1", std::ref(bridge));


    connect(source1->o, composite_i(bridge1, "in"));
    connect(composite_o(bridge1, "out"), sink1->i);


    // add another bridge, isolated from the other components and connect the
    // two gates to each other
    // after flattening, no traces of the bridge should be left!
    auto bridge2 = test.insert<XMASComposite>("bridge2", std::ref(bridge));
    connect(composite_o(bridge2, "out"), composite_i(bridge2, "in"));



    // now flatten this network
    XMASNetwork flattened = flatten(test);

    // and test the result
    auto fcs = flattened.getComponentMap();
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

    auto fcs = flattened.getComponentMap();
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

    connect(source1->o, composite_i(cc, "in"));
    connect(composite_o(cc, "out"), sink1->i);


    // now flatten this network
    XMASNetwork flattened = flatten(test);

    // and test the result
    auto fcs = flattened.getComponentMap();
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

    connect(source1->o, composite_i(cc1, "in"));
    connect(composite_o(cc1, "out"), composite_i(cc2, "in"));
    connect(composite_o(cc2, "out"), sink1->i);


    // now flatten this network
    XMASNetwork flattened = flatten(test);

    // and test the result
    auto fcs = flattened.getComponentMap();
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
