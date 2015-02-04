#include <gtest/gtest.h>

#include "xmas.h"
#include "cycle.h"
#include "symbolic.h"
#include "symbolic-enum-field.h"
//#include "symbolic-range-field.h"
#include "symbolic-interval-field.h"
#include "messagespec.h"
#include <map>
#include "lib/simplestring.h"
#include "parse.h"

namespace {

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
    ASSERT_EQ(ext->availablePackets.size(), 1U);
    EXPECT_EQ(ext->availablePackets[0], p);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASFunction, Valid) {
    XMASFunction c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASSource, SimpleIdentity) {
    XMASSource source("source");
    XMASFunction func("func");
    XMASSink sink("sink");

    attachFunction(&func, [](const std::vector<SymbolicPacket> &ps) -> std::vector<SymbolicPacket> {
        return ps;
    });

    connect(source.o, func.i);
    connect(func.o, sink.i);

    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};
    attach(&source.o, p);

    EXPECT_TRUE(source.valid());
    EXPECT_TRUE(func.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&source, &func, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(ext->availablePackets.size(), 1U);
    EXPECT_EQ(ext->availablePackets[0], p);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASSource, ConvertXtoY) {
    XMASSource source("source");
    XMASFunction func("func");
    XMASSink sink("sink");

    connect(source.o, func.i);
    connect(func.o, sink.i);

    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};
    attach(&source.o, p);

    attachFunction(&func, [](const std::vector<SymbolicPacket> &ps) -> std::vector<SymbolicPacket> {
        std::vector<SymbolicPacket> retval;
        for (auto &p : ps) {
            auto lookup = p.fields.find("first");
            retval.push_back({*lookup, NAMED_ENUM("second", "y")});
        }
        return retval;
    });

    EXPECT_TRUE(source.valid());
    EXPECT_TRUE(func.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&source, &func, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(ext->availablePackets.size(), 1U);
    SymbolicPacket expectedPacket = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "y")};
    EXPECT_EQ(ext->availablePackets[0], expectedPacket);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASSource, ConvertWithIfThenElse) {
    MemoryPool mp;

    XMASSource source("source");
    XMASFunction func("func");
    XMASSink sink("sink");

    connect(source.o, func.i);
    connect(func.o, sink.i);

    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x", "y")};
    attach(&source.o, p);

    auto result = ParsePacketFunction("if first in {r} then first := first, second := second with {_: x} else first := first, second := second with {_: y}", mp);
    EXPECT_TRUE(bool(result)) << result.error() << " on " << result.position();
    auto f = result.result();
    attachFunction(&func, [f](const std::vector<SymbolicPacket> &p) {
        return (*f)(p);
    });

    EXPECT_TRUE(source.valid());
    EXPECT_TRUE(func.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&source, &func, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(ext->availablePackets.size(), 2U);
    SymbolicPacket expectedPacket = {NAMED_ENUM("first", "g"), NAMED_ENUM("second", "y")};
    EXPECT_EQ(expectedPacket, ext->availablePackets[0]);
    expectedPacket = {NAMED_ENUM("first", "r"), NAMED_ENUM("second", "x")};
    EXPECT_EQ(expectedPacket, ext->availablePackets[1]);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASQueue, Valid) {
    XMASQueue c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASQueue, Simple) {
    XMASSource source("source");
    XMASQueue queue("queue");
    XMASSink sink("sink");

    connect(source.o, queue.i);
    connect(queue.o, sink.i);

    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};
    attach(&source.o, p);

    EXPECT_TRUE(source.valid());
    EXPECT_TRUE(queue.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&source, &queue, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(ext->availablePackets.size(), 1U);
    EXPECT_EQ(ext->availablePackets[0], p);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASJoin, Valid) {
    XMASJoin c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASJoin, Simple) {
    XMASSource sourceA("sourceA");
    XMASSource sourceB("sourceB");
    XMASJoin join("join");
    XMASSink sink("sink");

    connect(sourceA.o, join.a);
    connect(sourceB.o, join.b);
    connect(join.o, sink.i);

    SymbolicPacket inputA = {NAMED_ENUM("first", "x")};
    attach(&sourceA.o, inputA);

    SymbolicPacket inputB = {NAMED_ENUM("second", "y")};
    attach(&sourceB.o, inputB);

    EXPECT_TRUE(sourceA.valid());
    EXPECT_TRUE(sourceB.valid());
    EXPECT_TRUE(join.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&sourceA, &sourceB, &join, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(ext->availablePackets.size(), 1U);
    SymbolicPacket output = {*inputA.fields.find("first"), *inputB.fields.find("second")};
    EXPECT_EQ(ext->availablePackets[0], output);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASJoin, RestrictedJoin) {
    XMASSource sourceA("sourceA");
    XMASSource sourceB("sourceB");
    XMASJoin join("join");
    XMASSink sink("sink");

    connect(sourceA.o, join.a);
    connect(sourceB.o, join.b);
    connect(join.o, sink.i);

    SymbolicPacket inputA = {NAMED_ENUM("first", "x")};
    attach(&sourceA.o, inputA);

    SymbolicPacket inputB = {NAMED_ENUM("first", "y")};
    attach(&sourceB.o, inputB);

    attachFunction(&join, [](const SymbolicPacket &a, const SymbolicPacket &b) {
        return a;
    });

    EXPECT_TRUE(sourceA.valid());
    EXPECT_TRUE(sourceB.valid());
    EXPECT_TRUE(join.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&sourceA, &sourceB, &join, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);
    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(1U, ext->availablePackets.size());
    EXPECT_EQ(inputA, ext->availablePackets[0]);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASMerge, Valid) {
    XMASMerge c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASMerge, Simple) {
    XMASSource sourceA("sourceA");
    XMASSource sourceB("sourceB");
    XMASMerge merge("merge");
    XMASSink sink("sink");

    connect(sourceA.o, merge.a);
    connect(sourceB.o, merge.b);
    connect(merge.o, sink.i);

    SymbolicPacket inputA = {NAMED_ENUM("first", "x")};
    attach(&sourceA.o, inputA);

    SymbolicPacket inputB = {NAMED_ENUM("first", "y")};
    attach(&sourceB.o, inputB);

    EXPECT_TRUE(sourceA.valid());
    EXPECT_TRUE(sourceB.valid());
    EXPECT_TRUE(merge.valid());
    EXPECT_TRUE(sink.valid());

    std::set<XMASComponent*> allComponents = {&sourceA, &sourceB, &merge, &sink};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);

    SymbolicTypesExtension *mergeA = merge.a.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    std::cout << "mergeA: " << mergeA->availablePackets[0] << std::endl;

    SymbolicTypesExtension *mergeB = merge.b.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    std::cout << "mergeB: " << mergeB->availablePackets[0] << std::endl;

    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(ext->availablePackets.size(), 1U);
    SymbolicPacket output = {NAMED_ENUM("first", "x", "y")};
    std::cout << "sink: " << ext->availablePackets[0] << std::endl;
    EXPECT_EQ(output, ext->availablePackets[0]);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASSwitch, Valid) {
    XMASSwitch c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASSwitch, Simple) {
    XMASSource source("source");
    XMASSwitch sw("switch");
    XMASSink sinkA("sinkA");
    XMASSink sinkB("sinkB");

    connect(source.o, sw.i);
    connect(sw.a, sinkA.i);
    connect(sw.b, sinkB.i);

    SymbolicPacket input = {NAMED_ENUM("first", "REQ", "RSP"), NAMED_ENUM("second", "x", "y")};
    attach(&source.o, input);

    attachSwitchingFunction(&sw, {NAMED_ENUM("first", "REQ"), NAMED_ENUM("second", "x")});

    EXPECT_TRUE(source.valid());
    EXPECT_TRUE(sw.valid());
    EXPECT_TRUE(sinkA.valid());
    EXPECT_TRUE(sinkB.valid());

    std::set<XMASComponent*> allComponents = {&source, &sw, &sinkA, &sinkB};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);

    SymbolicTypesExtension *extA = sinkA.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    ASSERT_EQ(extA->availablePackets.size(), 1U);
    SymbolicPacket outputA = {NAMED_ENUM("first", "REQ"), NAMED_ENUM("second", "x")};
    EXPECT_EQ(extA->availablePackets[0], outputA);

    SymbolicTypesExtension *extB = sinkB.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    SymbolicPacket outputBone = {NAMED_ENUM("first", "REQ", "RSP"), NAMED_ENUM("second", "y")};
    SymbolicPacket outputBtwo = {NAMED_ENUM("first", "RSP"), NAMED_ENUM("second", "x", "y")};
    //SymbolicPacket outputBone = {NAMED_ENUM("first", "REQ", "RSP"), NAMED_ENUM("second", "y")};
    //SymbolicPacket outputBtwo = {NAMED_ENUM("first", "RSP"), NAMED_ENUM("second", "x", "y")};
    for (SymbolicPacket &p : extB->availablePackets) {
        EXPECT_TRUE((outputBone.contains(p) && p.contains(outputBone)) || (outputBtwo.contains(p) && p.contains(outputBtwo))) << p;
    }

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASFork, Valid) {
    XMASFork c("a");
    EXPECT_FALSE(c.valid());
}

TEST(XMASFork, Simple) {
    XMASSource source("source");
    XMASFork fork("fork");
    XMASSink sinkA("sinkA");
    XMASSink sinkB("sinkB");

    connect(source.o, fork.i);
    connect(fork.a, sinkA.i);
    connect(fork.b, sinkB.i);

    //SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};
    attach(&source.o, {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")});

    EXPECT_TRUE(source.valid());
    EXPECT_TRUE(fork.valid());
    EXPECT_TRUE(sinkA.valid());
    EXPECT_TRUE(sinkB.valid());

    std::set<XMASComponent*> allComponents = {&source, &fork, &sinkA, &sinkB};
    EXPECT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);

    SymbolicPacket p = {NAMED_ENUM("first", "r","g"), NAMED_ENUM("second", "x")};

    SymbolicTypesExtension *extA = sinkA.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(extA->availablePackets.size(), 1U);
    EXPECT_EQ(extA->availablePackets[0], p);

    SymbolicTypesExtension *extB = sinkB.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    EXPECT_EQ(extB->availablePackets.size(), 1U);
    EXPECT_EQ(extB->availablePackets[0], p);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}


TEST(ForkJoin, ObviousCombinatorialCycle) {
    // source -> fork -> join -> sink
    XMASSource source("source");
    XMASFork fork("fork");
    XMASJoin join("join");
    XMASSink sink("sink");

    connect(source.o, fork.i);
    connect(fork.a, join.a);
    connect(fork.b, join.b);
    connect(join.o, sink.i);

    std::set<XMASComponent*> allComponents = {&source, &fork, &join, &sink};
    EXPECT_TRUE(std::all_of(allComponents.begin(), allComponents.end(), std::mem_fun(&XMASComponent::valid)));
    EXPECT_TRUE(CombinatorialCycleDetector(allComponents));

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(ForkJoin, ObviousCombinatorialCycleAlt) {
    // source -> fork -> merge -> sink
    XMASSource source("source");
    XMASFork fork("fork");
    XMASMerge merge("merge");
    XMASSink sink("sink");

    connect(source.o, fork.i);
    connect(fork.a, merge.a);
    connect(fork.b, merge.b);
    connect(merge.o, sink.i);

    std::set<XMASComponent*> allComponents = {&source, &fork, &merge, &sink};
    EXPECT_TRUE(std::all_of(allComponents.begin(), allComponents.end(), std::mem_fun(&XMASComponent::valid)));
    EXPECT_TRUE(CombinatorialCycleDetector(allComponents));

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}


template <class T>
T *insert(std::map<std::string, XMASComponent*> &allComponents, const std::string &_name, int i) {
    std::string name = (std::string)(ToString(_name) + i);
    //std::cout << name << std::endl;
    T *comp = new T(name);
    allComponents.insert(std::make_pair(name, comp));
    return comp;
}

int leftDistance(int current, int dest, int total) {
    return (current - dest + total) % total;
}

int rightDistance(int current, int dest, int total) {
    return (dest - current + total) % total;
}

int sideDistance(int current, int dest, int total) {
    return std::min(leftDistance(current, dest, total),
                    rightDistance(current, dest, total));
}

int acrossDistance(int current, int dest, int total) {
    int across = (current+total/2) % total;
    return sideDistance(across, dest, total);
}

void createSpidergonNodes(std::map<std::string, XMASComponent*> &allComponents, int i, int total, MemoryPool &mp) {
    //int left = (i+total-1) % total;           // FIXME: unused variable warning
    //int right = (i+1) % total;                // FIXME: unused variable warning
    //int across = (i+total/2) % total;         // FIXME: unused variable warning

    XMASMerge *mergeLeftAndRight = insert<XMASMerge>(allComponents, "mergeLeftAndRight", i);

    XMASSource *source = insert<XMASSource>(allComponents, "source", i);

    XMASMerge *mergeAcrossAndSource = insert<XMASMerge>(allComponents, "mergeAcrossAndSource", i);
    connect(source->o, mergeAcrossAndSource->b);

    XMASMerge *mergeBoth = insert<XMASMerge>(allComponents, "mergeBoth", i);
    connect(mergeLeftAndRight->o, mergeBoth->a);
    connect(mergeAcrossAndSource->o, mergeBoth->b);

    XMASSwitch *switchLeftRight = insert<XMASSwitch>(allComponents, "switchLeftRight", i);
    {
        std::vector<SymbolicEnumField::Type> targets;
        for (int j = 0; j < total; ++j) {
            if (i != j && sideDistance(i, j, total) <= acrossDistance(i, j, total))
                targets.push_back(("dest"_S + j)(mp));
                //targets.push_back("dest"_S + j);
        }
        attachSwitchingFunction(switchLeftRight, {MAKE_NAMED_ENUM("first", targets)});
    }
    connect(mergeBoth->o, switchLeftRight->i);

    XMASSwitch *switchLeft = insert<XMASSwitch>(allComponents, "switchLeft", i);
    {
        std::vector<SymbolicEnumField::Type> targets;
        for (int j = 0; j < total; ++j) {
            if (i != j && leftDistance(i, j, total) <= rightDistance(i, j, total))
                targets.push_back(("dest"_S + j)(mp));
                //targets.push_back("dest"_S + j);
        }
        attachSwitchingFunction(switchLeft, {MAKE_NAMED_ENUM("first", targets)});
    }
    connect(switchLeftRight->a, switchLeft->i);

    XMASSwitch *switchThis = insert<XMASSwitch>(allComponents, "switchThis", i);
    attachSwitchingFunction(switchThis, {NAMED_ENUM("first", ("dest"_S + i)(mp))});
    //attachSwitchingFunction(switchThis, {NAMED_ENUM("first", "dest"_S + i)});
    connect(switchLeftRight->b, switchThis->i);

    XMASSink *sink = insert<XMASSink>(allComponents, "sink", i);
    connect(switchThis->a, sink->i);

    XMASQueue *incommingLeft = insert<XMASQueue>(allComponents, "incommingLeft", i);
    connect(incommingLeft->o, mergeLeftAndRight->a);
    XMASQueue *incommingRight = insert<XMASQueue>(allComponents, "incommingRight", i);
    connect(incommingRight->o, mergeLeftAndRight->b);
    XMASQueue *incommingAcross = insert<XMASQueue>(allComponents, "incommingAcross", i);
    connect(incommingAcross->o, mergeAcrossAndSource->a);

    std::vector<SymbolicEnumField::Type> targets;
    for (int j = 0; j < total; ++j)
        if (j != i)
            targets.push_back(("dest"_S + j)(mp));
            //targets.push_back("dest"_S + j);
    //attach(&source->o, {MAKE_NAMED_ENUM("first", targets), NAMED_ENUM("first", "src"_S + i)});
    attach(&source->o, {MAKE_NAMED_ENUM("first", targets)});

    // mergeLeftAndRight.a <-> switchLeft.b
    // mergeLeftAndRight.b <-> switchLeft.a
    // mergeAcrossAndSource.a <-> switchThis.b
}

void connectSpidergonNodes(std::map<std::string, XMASComponent*> &allComponents, int i, int total) {
    int left = (i+total-1) % total;
    int right = (i+1) % total;
    int across = (i+total/2) % total;

    connect(((XMASSwitch*)allComponents[(std::string)("switchLeft"_S + i)])->a, ((XMASQueue*)allComponents[(std::string)("incommingRight"_S + left)])->i);
    connect(((XMASSwitch*)allComponents[(std::string)("switchLeft"_S + i)])->b, ((XMASQueue*)allComponents[(std::string)("incommingLeft"_S + right)])->i);
    connect(((XMASSwitch*)allComponents[(std::string)("switchThis"_S + i)])->b, ((XMASQueue*)allComponents[(std::string)("incommingAcross"_S + across)])->i);
}

int spidergonSize = 16;

TEST(Spidergon, DISABLED_Enum) {
    MemoryPool mp;
    std::map<std::string, XMASComponent*> components;

    int total = spidergonSize;
    for (int i = 0; i < total; ++i) {
        createSpidergonNodes(components, i, total, mp);
    }
    for (int i = 0; i < total; ++i) {
        connectSpidergonNodes(components, i, total);
    }

    std::set<XMASComponent*> allComponents;
    for (auto it : components) {
        ASSERT_TRUE(it.second->valid());
        allComponents.insert(it.second);
    }

    ASSERT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);

    for (int i = 0; i < total; ++i) {
        //std::cout << "checking sink " << i << std::endl;
        XMASSink *sink = (XMASSink *)components[(std::string)("sink"_S + i)];
        SymbolicTypesExtension *ext = sink->i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
        auto spec = ENUM(("dest"_S + i)(mp));
        //auto spec = ENUM("dest"_S + i);
        for (auto ap : ext->availablePackets) {
            EXPECT_TRUE(spec->contains(ap.fields["dst"]));
            //std::cout << sink->getName() << ": " << ap << std::endl;
        }
    }
    std::cout << "network consisted of " << components.size() << " components" << std::endl;

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

void createSpidergonNodesInterval(std::map<std::string, XMASComponent*> &allComponents, int i, int total) {
    //int left = (i+total-1) % total;           // FIXME: unused variable warning
    //int right = (i+1) % total;                // FIXME: unused variable warning
    //int across = (i+total/2) % total;         // FIXME: unused variable warning


    XMASMerge *mergeLeftAndRight = insert<XMASMerge>(allComponents, "mergeLeftAndRight", i);

    XMASSource *source = insert<XMASSource>(allComponents, "source", i);

    XMASMerge *mergeAcrossAndSource = insert<XMASMerge>(allComponents, "mergeAcrossAndSource", i);
    connect(source->o, mergeAcrossAndSource->b);

    XMASMerge *mergeBoth = insert<XMASMerge>(allComponents, "mergeBoth", i);
    connect(mergeLeftAndRight->o, mergeBoth->a);
    connect(mergeAcrossAndSource->o, mergeBoth->b);

    int q = total/4;
    XMASSwitch *switchLeftRight = insert<XMASSwitch>(allComponents, "switchLeftRight", i);
    if (i - q < 0) {
        // FIXME: make it a complement
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i-q+total, total)});
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", 0, i)});
        //attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i, i-q+total, true)});
    } else
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i-q, i)});
    if (i + q >= total) {
        // FIXME: make it a complement
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i+1, total)});
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", 0, i+q-total)});
        //attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i+q-total, i, true)});
    } else
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i+1, i+1+q)});
    connect(mergeBoth->o, switchLeftRight->i);

    XMASSwitch *switchLeft = insert<XMASSwitch>(allComponents, "switchLeft", i);
    if (i - q < 0) {
        // FIXME: make it a complement
        attachSwitchingFunction(switchLeft, {NAMED_INTERVAL("dst", i-q+total, total)});
        attachSwitchingFunction(switchLeft, {NAMED_INTERVAL("dst", 0, i)});
        //attachSwitchingFunction(switchLeft, {NAMED_INTERVAL("dst", i, i-q+total, true)});
    } else
        attachSwitchingFunction(switchLeft, {NAMED_INTERVAL("dst", i-q, i)});
    connect(switchLeftRight->a, switchLeft->i);

    XMASSwitch *switchThis = insert<XMASSwitch>(allComponents, "switchThis", i);
    attachSwitchingFunction(switchThis, {NAMED_INTERVAL("dst", i,i+1)});
    connect(switchLeftRight->b, switchThis->i);

    XMASSink *sink = insert<XMASSink>(allComponents, "sink", i);
    connect(switchThis->a, sink->i);

    XMASQueue *incommingLeft = insert<XMASQueue>(allComponents, "incommingLeft", i);
    connect(incommingLeft->o, mergeLeftAndRight->a);
    XMASQueue *incommingRight = insert<XMASQueue>(allComponents, "incommingRight", i);
    connect(incommingRight->o, mergeLeftAndRight->b);
    XMASQueue *incommingAcross = insert<XMASQueue>(allComponents, "incommingAcross", i);
    connect(incommingAcross->o, mergeAcrossAndSource->a);

    // FIXME: make it a complement
    //attach(&source->o, {NAMED_INTERVAL("dst", i, i+1, true), NAMED_INTERVAL("dst", i, i+1)});
    //attach(&source->o, {NAMED_INTERVAL("dst", i, i+1, true), NAMED_ENUM("first", "src"_S + i)});
    attach(&source->o, {NAMED_INTERVAL("dst", 0, i)});
    attach(&source->o, {NAMED_INTERVAL("dst", i+1, total)});
    //attach(&source->o, {NAMED_INTERVAL("dst", i, i+1, true), NAMED_INTERVAL("dst", 0, total)});

    // mergeLeftAndRight.a <-> switchLeft.b
    // mergeLeftAndRight.b <-> switchLeft.a
    // mergeAcrossAndSource.a <-> switchThis.b
}

void connectSpidergonNodesInterval(std::map<std::string, XMASComponent*> &allComponents, int i, int total) {
    int left = (i+total-1) % total;
    int right = (i+1) % total;
    int across = (i+total/2) % total;

    connect(((XMASSwitch*)allComponents[(std::string)("switchLeft"_S + i)])->a, ((XMASQueue*)allComponents[(std::string)("incommingRight"_S + left)])->i);
    connect(((XMASSwitch*)allComponents[(std::string)("switchLeft"_S + i)])->b, ((XMASQueue*)allComponents[(std::string)("incommingLeft"_S + right)])->i);
    connect(((XMASSwitch*)allComponents[(std::string)("switchThis"_S + i)])->b, ((XMASQueue*)allComponents[(std::string)("incommingAcross"_S + across)])->i);
}

TEST(Spidergon, Range) {
    std::map<std::string, XMASComponent*> components;

    int total = spidergonSize;
    for (int i = 0; i < total; ++i) {
        createSpidergonNodesInterval(components, i, total);
    }
    for (int i = 0; i < total; ++i) {
        connectSpidergonNodesInterval(components, i, total);
    }

    std::set<XMASComponent*> allComponents;
    for (auto it : components) {
        ASSERT_TRUE(it.second->valid());
        allComponents.insert(it.second);
    }

    ASSERT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);

    for (int i = 0; i < total; ++i) {
        //std::cout << "checking sink " << i << std::endl;
        XMASSink *sink = (XMASSink *)components[(std::string)("sink"_S + i)];
        SymbolicTypesExtension *ext = sink->i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
        //SymbolicPacket spec = {NAMED_INTERVAL("dst", i, i+1)};
        auto spec = INTERVAL(i,i+1);
        for (auto& ap : ext->availablePackets) {
            //EXPECT_TRUE(spec.contains(ap));
            EXPECT_TRUE(spec->contains(ap.fields["dst"]));
            //std::cout << sink->getName() << ": " << ap << std::endl;
        }
    }
    std::cout << "network consisted of " << components.size() << " components" << std::endl;

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASSource, LoopWithFunction) {
    MemoryPool mp;

    XMASSource source("source");
    XMASMerge merge("merge");
    XMASFunction func("func");
    XMASSwitch sw("switch");
    XMASSink sink("sink");
    XMASQueue q("queue");

    connect(source.o, merge.a);
    connect(merge.o, q.i);
    connect(q.o, sw.i);
    connect(sw.a, sink.i);
    connect(sw.b, func.i);
    connect(func.o, merge.b);

    SymbolicPacket p = {NAMED_INTERVAL("x", 1, 2)};
    attach(&source.o, p);

    auto fResult = ParsePacketFunction("x := x + 1", mp);
    ASSERT_TRUE(bool(fResult)) << fResult.error() << " on " << fResult.position();
    auto f = fResult.result();
    attachFunction(&func, [f](const std::vector<SymbolicPacket> &p) {
        return (*f)(p);
    });

    auto swResult = ParsePacketExpression("x > 10000", mp);
    ASSERT_TRUE(bool(swResult)) << swResult.error() << " on " << swResult.position();
    auto& swFuncs = swResult.result().values;
    for (const auto& swFunc : swFuncs)
        attachSwitchingFunction(&sw, swFunc);

    std::set<XMASComponent*> allComponents = {&source, &merge, &sw, &sink, &func, &q};
    for (auto c : allComponents)
        ASSERT_TRUE(c->valid());

    ASSERT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);

    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    ASSERT_EQ(ext->availablePackets.size(), 1U);
    SymbolicPacket expectedPacket = {NAMED_INTERVAL("x", 10001, 10002)};
    EXPECT_EQ(expectedPacket, ext->availablePackets[0]);

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

TEST(XMASSource, LoopWithStateMachine) {
    MemoryPool mp;

    XMASSource state("state");
    XMASMerge initialStateMerge("initialStateMerge");
    XMASSource source("source");
    XMASJoin join("join");
    XMASFork fork("fork");
    XMASFunction funcStripState("state-strip");
    XMASFunction funcStripPacket("packet-strip");
    XMASSink sink("sink");
    XMASQueue q("queue");
    XMASFunction func("func");

    connect(source.o, join.a);
    connect(join.o, func.i);
    connect(func.o, fork.i);
    connect(fork.a, funcStripState.i);
    connect(funcStripState.o, sink.i);
    connect(fork.b, funcStripPacket.i);
    connect(funcStripPacket.o, q.i);
    connect(q.o, initialStateMerge.b);
    connect(state.o, initialStateMerge.a);
    connect(initialStateMerge.o, join.b);

    SymbolicPacket p = {NAMED_INTERVAL("packet", 5, 11)};
    attach(&source.o, p);

    p = {NAMED_INTERVAL("state", 5, 6)};
    attach(&state.o, p);

    auto fResult = ParsePacketFunction("if state >= 100 then packet := packet + state, state := 5 else packet := packet + state, state := state + 1", mp);
    ASSERT_TRUE(bool(fResult)) << fResult.error() << " on " << fResult.position();
    auto f = fResult.result();
    attachFunction(&func, [f](const std::vector<SymbolicPacket> &p) {
        return (*f)(p);
    });

    fResult = ParsePacketFunction("state := state", mp);
    ASSERT_TRUE(bool(fResult)) << fResult.error() << " on " << fResult.position();
    f = fResult.result();
    attachFunction(&funcStripPacket, [f](const std::vector<SymbolicPacket> &p) {
        return (*f)(p);
    });

    fResult = ParsePacketFunction("packet := packet", mp);
    ASSERT_TRUE(bool(fResult)) << fResult.error() << " on " << fResult.position();
    f = fResult.result();
    attachFunction(&funcStripState, [f](const std::vector<SymbolicPacket> &p) {
        return (*f)(p);
    });

    std::set<XMASComponent*> allComponents = {&state, &source, &join, &fork, &sink, &func, &q, &funcStripPacket, &funcStripState, &initialStateMerge};
    for (auto c : allComponents)
        ASSERT_TRUE(c->valid());

    ASSERT_FALSE(CombinatorialCycleDetector(allComponents));

    SymbolicTypes(allComponents);

    SymbolicTypesExtension *ext = sink.i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    ASSERT_EQ(ext->availablePackets.size(), 1U);
    SymbolicPacket expectedPacket = {NAMED_INTERVAL("packet", 10, 111)};
    EXPECT_EQ(expectedPacket, ext->availablePackets[0]);

    if (false)
        for (XMASComponent *c : allComponents) {
            for (Input *i : c->inputPorts()) {
                SymbolicTypesExtension *ext = i->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
                ext->simplify("output", true);
                std::cout << *i << ": ";
                for (auto &ap : ext->availablePackets) {
                    std::cout << ap << ", ";
                }
                std::cout << std::endl;
            }
        }

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);
}

}
