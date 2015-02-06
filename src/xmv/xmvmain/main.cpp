#include <iostream>
#include <tuple>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
#include <chrono>

#include "xmas.h"
#include "cycle.h"
#include "symbolic.h"
#include "symbolic-enum-field.h"
//#include "symbolic-range-field.h"
#include "symbolic-interval-field.h"
#include "messagespec.h"
#include "parse.h"
#include "export.h"

#ifndef __MINGW32__
#include <sys/resource.h>
#endif

using namespace std;

/**
  * Features:
  * - combinatorial cycle detection
  * - symbolic type simulator
  *     - dynamic number of fields in a packet
  *     - dynamic number of fields in packets in a channels
  *     - not needed to represent the set of all possible values on a wire
  *     - both unrestricted as restricted join (restricted using a special function on the join)
  *     - arbritrary xMAS functions, using lambda functions in C++11
  * (- MessageSpec validator)
  *
  * - using a unit test framework for all kinds of small tests
  */

/**
 * TODO:
 * - catch exceptions that are thrown because of unsuspected malformed networks
 * - symbolic representation of xMAS functions
 */

/**
 * Optimalisation possibilities:
 * - eliminate unneeded copies (using move copy constructors?)
 * - eliminate strings (by putting them in a large static stringpool, and using pointers to those strings, elimating string comparison, because strings with the same value have the same pointer)
 * - using bitvector representation of sets, one can model intersection fast using a bitwise AND, if the same can be done for the other operations, we can reduce the strength of the algorithm and be faster
 * - using pointers to SymbolicPackets instead of SymbolicPackets themselfs, as it is cheaper to propagate a pointer in the network as opposed to containers (as the operation done most often is propagation)
*/
bool checkNetwork(std::set<XMASComponent*> allComponents) {
    auto begin = std::chrono::high_resolution_clock::now();
    auto start = begin;

    // STEP 1: check if the topology is well formed
    // check that each pointer is reflexive
    bool valid = true;
    for (XMASComponent *c : allComponents) {
        bool cValid = c->valid();
        //std::cout << *c << " = " << cValid << std::endl;
        valid = valid & cValid;
    }

    auto current = std::chrono::high_resolution_clock::now();
    std::cout << "syntactic check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    std::cout << "syntactic correct: \t\t" << (valid ? "OK" : "ERROR") << std::endl;
    if (!valid)
        goto error;

    // absence of combinatorial cycles
    {
        bool cycle = CombinatorialCycleDetector(allComponents);

        current = std::chrono::high_resolution_clock::now();
        std::cout << "combi cycle check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
        start = current;

        std::cout << "no combinatorial cycles: \t" << (!cycle ? "OK" : "ERROR") << std::endl;
        if (cycle)
            goto error;
    }

    // STEP 2: calculate frames of all the channels
    SymbolicTypes(allComponents);

    current = std::chrono::high_resolution_clock::now();
    std::cout << "type inference in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    if (false) {
        std::cout << "symbolic packet types calculated:" << std::endl;
        for (XMASComponent *c : allComponents) {
            for (Input *input : c->inputPorts()) {
                printSymbolicTypeInfo(std::cout, input);
                std::cout << std::endl;
            }
        }
    }

    ClearSymbolicTypes(allComponents);

    // STEP 3: check message specification

    return true;

error:
    ClearSymbolicTypes(allComponents);
    return false;
}

//#define ANY {{}, true}

template <class T, class Name>
T *insert(std::map<std::string, XMASComponent*> &allComponents, const Name &_name) {
    std::string name = (std::string)_name;
    //std::cout << name << std::endl;
    if (allComponents.find(name) != allComponents.end()) {
        std::cout << "component already exists: " << name << std::endl;
        throw bitpowder::lib::Exception(42, __FILE__, __LINE__);
    }
    T *comp = new T(name);
    allComponents.insert(std::make_pair((std::string)comp->getName(), comp));
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

void createSpidergonNodesRange(std::map<std::string, XMASComponent*> &allComponents, int i, int total) {
    //int left = (i+total-1) % total;           // FIXME: unused variable warning
    //int right = (i+1) % total;                // FIXME: unused variable warning
    //int across = (i+total/2) % total;         // FIXME: unused variable warning

    XMASMerge *mergeLeftAndRight = insert<XMASMerge>(allComponents, "mergeLeftAndRight"_S + i);

    XMASSource *source = insert<XMASSource>(allComponents, "source"_S + i);

    XMASMerge *mergeAcrossAndSource = insert<XMASMerge>(allComponents, "mergeAcrossAndSource"_S + i);
    connect(source->o, mergeAcrossAndSource->b);

    XMASMerge *mergeBoth = insert<XMASMerge>(allComponents, "mergeBoth"_S + i);
    connect(mergeLeftAndRight->o, mergeBoth->a);
    connect(mergeAcrossAndSource->o, mergeBoth->b);

    int q = total/4;
    XMASSwitch *switchLeftRight = insert<XMASSwitch>(allComponents, "switchLeftRight"_S + i);
    if (i - q < 0) {
        // FIXME: make it a complement
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i-q+total, total)});
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", 0, i)});
        //attachSwitchingFunction(switchLeftRight, {INTERVAL(i, i-q+total, true)});
    } else {
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i-q, i)});
    }
    if (i + q >= total) {
        // FIXME: make it a complement
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i+1, total)});
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", 0, i+1+q-total)});
        //attachSwitchingFunction(switchLeftRight, {INTERVAL(i+1+q-total, i, true)});
    } else {
        attachSwitchingFunction(switchLeftRight, {NAMED_INTERVAL("dst", i+1, i+1+q)});
    }
    connect(mergeBoth->o, switchLeftRight->i);

    XMASSwitch *switchLeft = insert<XMASSwitch>(allComponents, "switchLeft"_S + i);
    if (i - q < 0) {
        // FIXME: make it a complement
        attachSwitchingFunction(switchLeft, {NAMED_INTERVAL("dst", i-q+total, total)});
        attachSwitchingFunction(switchLeft, {NAMED_INTERVAL("dst", 0, i)});
        //attachSwitchingFunction(switchLeft, {RANGE(i, i-q+total, true)});
    } else {
        attachSwitchingFunction(switchLeft, {NAMED_INTERVAL("dst", i-q, i)});
    }
    connect(switchLeftRight->a, switchLeft->i);

    XMASSwitch *switchThis = insert<XMASSwitch>(allComponents, "switchThis"_S + i);
    attachSwitchingFunction(switchThis, {NAMED_INTERVAL("dst", i,i+1)});
    connect(switchLeftRight->b, switchThis->i);

    XMASSink *sink = insert<XMASSink>(allComponents, "sink"_S + i);
    connect(switchThis->a, sink->i);

    XMASQueue *incommingLeft = insert<XMASQueue>(allComponents, "incommingLeft"_S + i);
    connect(incommingLeft->o, mergeLeftAndRight->a);
    XMASQueue *incommingRight = insert<XMASQueue>(allComponents, "incommingRight"_S + i);
    connect(incommingRight->o, mergeLeftAndRight->b);
    XMASQueue *incommingAcross = insert<XMASQueue>(allComponents, "incommingAcross"_S + i);
    connect(incommingAcross->o, mergeAcrossAndSource->a);
}

void connectSpidergonNodesRange(std::map<std::string, XMASComponent*> &allComponents, int i, int total) {
    int left = (i+total-1) % total;
    int right = (i+1) % total;
    int across = (i+total/2) % total;

    connect(((XMASSwitch*)allComponents[(std::string)("switchLeft"_S + i)])->a, ((XMASQueue*)allComponents[(std::string)("incommingRight"_S + left)])->i);
    connect(((XMASSwitch*)allComponents[(std::string)("switchLeft"_S + i)])->b, ((XMASQueue*)allComponents[(std::string)("incommingLeft"_S + right)])->i);
    connect(((XMASSwitch*)allComponents[(std::string)("switchThis"_S + i)])->b, ((XMASQueue*)allComponents[(std::string)("incommingAcross"_S + across)])->i);
}


void specSpidergon(std::map<std::string, XMASComponent*> &allComponents, int i, int total) {
    XMASSource *source = dynamic_cast<XMASSource*>(allComponents[(std::string)("source"_S + i)]);
    for (int j = 0; j < total; ++j) {
        if (j != i) {
            XMASSink *sink = dynamic_cast<XMASSink*>(allComponents[(std::string)("sink"_S + j)]);
            std::vector<SymbolicPacket> packets = {{NAMED_INTERVAL("dst", j, j+1), NAMED_INTERVAL("src", i, i+1)}};
            attachMessageSpec(&source->o, packets, S(sink, packets));
        }
    }
}

// from: http://stackoverflow.com/questions/364985/algorithm-for-finding-the-smallest-power-of-two-thats-greater-or-equal-to-a-giv
/// Round up to next higher power of 2 (return x if it's already a power
/// of 2).
inline int
pow2roundup (int x)
{
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}

void SpidergonTest(int size, bool showSinks, bool showAll) {
    auto begin = std::chrono::high_resolution_clock::now();
    auto start = begin;

    std::map<std::string, XMASComponent*> components;

    int total = pow2roundup(size);
    std::cout << "Spidergon with " << total << " nodes" << std::endl;
    for (int i = 0; i < total; ++i)
        createSpidergonNodesRange(components, i, total);
    for (int i = 0; i < total; ++i)
        connectSpidergonNodesRange(components, i, total);
    for (int i = 0; i < total; ++i)
        specSpidergon(components, i, total);

    auto current = std::chrono::high_resolution_clock::now();
    std::cout << "generated mesh in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    std::set<XMASComponent*> allComponents;
    for (auto &it : components) {
        checkAssert(it.second->valid());
        if (it.second)
            allComponents.insert(it.second);
    }

    current = std::chrono::high_resolution_clock::now();
    std::cout << "syntactic check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    checkAssert(!CombinatorialCycleDetector(allComponents));

    current = std::chrono::high_resolution_clock::now();
    std::cout << "combi cycle check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    SymbolicTypes(allComponents);

    current = std::chrono::high_resolution_clock::now();
    std::cout << "type inference in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    if (showSinks) {
        for (int i = 0; i < total; ++i) {
            XMASSink *sink = (XMASSink *)components[(std::string)("sink"_S + i)];
            SymbolicTypesExtension *ext = sink->i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
            auto spec = INTERVAL(i,i+1);
            for (auto &ap : ext->availablePackets) {
                std::cout << sink->getName() << ": " << ap << std::endl;
                checkAssert(spec->contains(ap.fields["dst"]));
            }
        }
    }

    // output types
    if (showAll) {
        for (XMASComponent *c : allComponents) {
            for (Input *i : c->inputPorts()) {
                SymbolicTypesExtension *ext = i->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
                std::cout << *i << ": ";
                for (auto &ap : ext->availablePackets) {
                    std::cout << ap << ", ";
                }
                std::cout << std::endl;
            }
        }
    }

    start = std::chrono::high_resolution_clock::now();

    CheckMessageSpec(allComponents);

    current = std::chrono::high_resolution_clock::now();
    std::cout << "message spec check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    ClearSymbolicTypes(allComponents);

    for (auto it : components) {
        delete it.second;
    }
    std::cout << "network consisted of " << components.size() << "/" << allComponents.size() << " components" << std::endl;
}

Output& doMerge(std::map<std::string, XMASComponent*> &allComponents, Output& a, Output& b) {
    XMASMerge *merge = insert<XMASMerge>(allComponents, "merge_"_S + a.getComponent()->getName() + "." + a.getName() + "_" + b.getComponent()->getName() + "." + b.getName());
    connect(a, merge->a);
    connect(b, merge->b);
    return merge->o;
}

void createMeshNodes(std::map<std::string, XMASComponent*> &allComponents,
                     int x, int y, int width, int height,
                     bitpowder::lib::MemoryPool &mp) {
/*
    // every node is both a master and a slave
    XMASSink *sink = insert<XMASSink>(allComponents, "out_local"_S + x + "_" + y);

    XMASSource *source = insert<XMASSource>(allComponents, "in_local"_S + x + "_" + y);
    XMASMerge *merge = insert<XMASMerge>(allComponents, "in_locals"_S + x + "_" + y);
    connect(source->o, merge->a);
    */
    XMASQueue *slaveFuncQueue = nullptr;
    Output* in = nullptr;
    if (x < width/2) {
        XMASSource *source = insert<XMASSource>(allComponents, "in_local"_S + x + "_" + y);
        in = &source->o;
    } else {
        slaveFuncQueue = insert<XMASQueue>(allComponents, "slaveFuncQueue"_S + x + "_" + y);
        in = &slaveFuncQueue->o;
    }

    // X PART
    // west
    if (x != 0) {
        XMASQueue *incomming = insert<XMASQueue>(allComponents, "in_west"_S + x + "_" + y);
        in = &doMerge(allComponents, *in, incomming->o);
    }

    // east
    if (x < width-1) {
        XMASQueue *incomming = insert<XMASQueue>(allComponents, "in_east"_S + x + "_" + y);
        in = &doMerge(allComponents, *in, incomming->o);
    }

    XMASSwitch *switchCorrectX = insert<XMASSwitch>(allComponents, "switchCorrectX"_S + x + "_" + y);
    connect(*in, switchCorrectX->i);
    attachSwitchingFunction(switchCorrectX, {NAMED_INTERVAL("x", x, x+1)});

    if (x > 0 && x < width-1) {
        XMASSwitch *switchEast = insert<XMASSwitch>(allComponents, "switchEast"_S + x + "_" + y);
        connect(switchCorrectX->b, switchEast->i);
        attachSwitchingFunction(switchEast, {NAMED_INTERVAL("x", x, width)});

        XMASQueue *outEast = insert<XMASQueue>(allComponents, "out_east"_S + x + "_" + y);
        connect(switchEast->a, outEast->i);
        XMASQueue *outWest = insert<XMASQueue>(allComponents, "out_west"_S + x + "_" + y);
        connect(switchEast->b, outWest->i);
    } else {
        XMASQueue *out = insert<XMASQueue>(allComponents, (x == 0 ? "out_east"_S : "out_west"_S) + x + "_" + y);
        connect(switchCorrectX->b, out->i);
    }

    // Y PART
    in = &switchCorrectX->a;

    // north
    if (y != 0) {
        XMASQueue *incomming = insert<XMASQueue>(allComponents, "in_north"_S + x + "_" + y);
        in = &doMerge(allComponents, *in, incomming->o);
    }

    // south
    if (y < height-1) {
        XMASQueue *incomming = insert<XMASQueue>(allComponents, "in_south"_S + x + "_" + y);
        in = &doMerge(allComponents, *in, incomming->o);
    }

    // local
    XMASSwitch *switchCorrectXY = insert<XMASSwitch>(allComponents, "switchCorrectXY"_S + x + "_" + y);
    connect(*in, switchCorrectXY->i);
    attachSwitchingFunction(switchCorrectXY, {NAMED_INTERVAL("y", y, y+1)});

    if (y > 0 && y < height-1) {
        XMASSwitch *switchSouth = insert<XMASSwitch>(allComponents, "switchSouth"_S + x + "_" + y);
        connect(switchCorrectXY->b, switchSouth->i);
        attachSwitchingFunction(switchSouth, {NAMED_INTERVAL("y", y, height)});

        XMASQueue *outSouth = insert<XMASQueue>(allComponents, "out_south"_S + x + "_" + y);
        connect(switchSouth->a, outSouth->i);
        XMASQueue *outNorth = insert<XMASQueue>(allComponents, "out_north"_S + x + "_" + y);
        connect(switchSouth->b, outNorth->i);
    } else {
        XMASQueue *out = insert<XMASQueue>(allComponents, (y == 0 ? "out_south"_S : "out_north"_S) + x + "_" + y);
        connect(switchCorrectXY->b, out->i);
    }

    /*
    // every node is both a master and a slave
    XMASSwitch *switchSlave = insert<XMASSwitch>(allComponents, "switchSlave"_S + x + "_" + y);
    connect(switchCorrectXY->a, switchSlave->i);

    attachSwitchingFunction(switchSlave, {NAMED_ENUM("type", "req")});
    connect(switchSlave->b, sink->i);

    XMASFunction *slaveFunc = insert<XMASFunction>(allComponents, "slaveFunc"_S + x + "_" + y);
    connect(switchSlave->a, slaveFunc->i);

    static auto result = ParsePacketFunction("type := type with {req: rsp}, x := src_x, y := src_y, dst_x := x, dst_y := y", Interval::all());
    //static auto result = ParsePacketFunction("type := type with {req: rsp}, x := src_x, y := src_y", Interval::all());
    auto func = result.result();
    attachFunction(slaveFunc, [func](const SymbolicPacket &p) {
        return (*func)(p);
    });

    XMASQueue *slaveFuncQueue = insert<XMASQueue>(allComponents, "slaveFuncQueue"_S + x + "_" + y);
    connect(slaveFunc->o, slaveFuncQueue->i);
    connect(slaveFuncQueue->o, merge->b);
    */
    if (x < width/2) {
        XMASSink *sink = insert<XMASSink>(allComponents, "out_local"_S + x + "_" + y);
        connect(switchCorrectXY->a, sink->i);
    } else {
        XMASFunction *slaveFunc = insert<XMASFunction>(allComponents, "slaveFunc"_S + x + "_" + y);
        connect(switchCorrectXY->a, slaveFunc->i);
        auto result = ParsePacketFunction("type := type with {req: rsp}, x := src_x, y := src_y, dst_x := x, dst_y := y, payload := payload", mp);
        //static auto result = ParsePacketFunction("type := type with {req: rsp}, x := src_x, y := src_y, payload := payload", Interval::all());
        auto func = result.result();
        attachFunction(slaveFunc, [func](const std::vector<SymbolicPacket> &p) {
            return (*func)(p);
        });
        connect(slaveFunc->o, slaveFuncQueue->i);
    }
}

void connectMeshNodes(std::map<std::string, XMASComponent*> &allComponents, int x, int y, int width, int height) {
    if (x > 0)
        connect(((XMASQueue*)allComponents[(std::string)("out_east"_S + (x-1) + "_" + y)])->o, ((XMASQueue*)allComponents[(std::string)("in_west"_S + x + "_" + y)])->i);
    if (x < width-1)
        connect(((XMASQueue*)allComponents[(std::string)("out_west"_S + (x+1) + "_" + y)])->o, ((XMASQueue*)allComponents[(std::string)("in_east"_S + x + "_" + y)])->i);
    if (y > 0)
        connect(((XMASQueue*)allComponents[(std::string)("out_south"_S + x + "_" + (y-1))])->o, ((XMASQueue*)allComponents[(std::string)("in_north"_S + x + "_" + y)])->i);
    if (y < height-1)
        connect(((XMASQueue*)allComponents[(std::string)("out_north"_S + x + "_" + (y+1))])->o, ((XMASQueue*)allComponents[(std::string)("in_south"_S + x + "_" + y)])->i);
}


void specMesh(std::map<std::string, XMASComponent*> &allComponents,
              int x, int y, int width, int height,
              bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::unused(height, mp);
    if (x >= width/2)
        return;
    XMASSource *source = dynamic_cast<XMASSource*>(allComponents[(std::string)("in_local"_S + x + "_" + y)]);
    XMASSink *sink = dynamic_cast<XMASSink*>(allComponents[(std::string)("out_local"_S + x + "_" + y)]);
    for (int dstx = width/2; dstx < width; ++dstx) {
        for (int dsty = 0; dsty < width; ++dsty) {
            if (x != dstx || y != dsty) {
                std::vector<SymbolicPacket> packetIn = {{NAMED_ENUM("type", "req"), NAMED_INTERVAL("x", dstx, dstx+1), NAMED_INTERVAL("y", dsty, dsty+1), NAMED_INTERVAL("src_x", x, x+1), NAMED_INTERVAL("src_y", y, y+1), NAMED_INTERVAL("payload", 0, 4294967296)}};
                std::vector<SymbolicPacket> packetOut = {{NAMED_ENUM("type", "rsp"), NAMED_INTERVAL("x", x, x+1), NAMED_INTERVAL("y", y, y+1), NAMED_INTERVAL("dst_x", dstx, dstx+1), NAMED_INTERVAL("dst_y", dsty, dsty+1), NAMED_INTERVAL("payload", 0, 4294967296)}};
                //std::vector<SymbolicPacket> packetOut = {{NAMED_ENUM("type", "rsp"), NAMED_INTERVAL("x", x, x+1), NAMED_INTERVAL("y", y, y+1)}};
                attachMessageSpec(&source->o, packetIn, S(sink, packetOut));
                /*
                String types = ("x = "_S + dstx + " && y = " + dsty + " -> (out_local"_S + dstx + "_" + dsty + ", x = " + dstx + " && y = " + dsty + ")")(mp);
                auto result = ParseSourceExpression(types, mp);
                if (result) {
                    //std::cout << "parsing " << types << ": " << result.result() << std::endl;
                    for (auto &packet : result.result().spec) {
                        attachMessageSpec(&source->o, std::get<0>(packet).values, std::get<1>(packet));
                    }
                } else {
                    std::cerr << "parsing " << types << std::endl;
                    std::cerr << "error parsing at position " << result.position() << " is " << result.error() << std::endl;
                    exit(-1);
                }
                */
            }
        }
    }
}

void MeshTest(int size, bool showSinks, bool showAll) {
    auto begin = std::chrono::high_resolution_clock::now();
    auto start = begin;

    bitpowder::lib::StaticMemoryPool<256> mp;
    std::map<std::string, XMASComponent*> components;

    int width = size;
    int height = size;
    std::cout << "checking mesh of size " << width << "x" << height << std::endl;

    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            createMeshNodes(components, x, y, width, height, mp);
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            connectMeshNodes(components, x, y, width, height);
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            specMesh(components, x, y, width, height, mp);

    auto current = std::chrono::high_resolution_clock::now();
    std::cout << "generated mesh in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    std::set<XMASComponent*> allComponents;
    for (auto &it : components) {
        //std::cout << "checking " << *it.second << std::endl;
        checkAssert(it.second->valid());
        if (it.second)
            allComponents.insert(it.second);
    }

    current = std::chrono::high_resolution_clock::now();
    std::cout << "syntactic check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    checkAssert(!CombinatorialCycleDetector(allComponents));

    current = std::chrono::high_resolution_clock::now();
    std::cout << "combi cycle check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    SymbolicTypes(allComponents);

    current = std::chrono::high_resolution_clock::now();
    std::cout << "type inference in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    if (showSinks)
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y) {
                XMASSink *sink = (XMASSink *)components[(std::string)("out_local"_S + x + "_" + y)];
                if (sink) {
                    std::cout << *sink << std::endl;
                    SymbolicTypesExtension *ext = sink->i.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
                    for (auto &ap : ext->availablePackets) {
                        std::cout << sink->getName() << ": " << ap << std::endl;
                    }
                }
            }

    // output types
    if (showAll)
        for (XMASComponent *c : allComponents) {
            for (Input *i : c->inputPorts()) {
                SymbolicTypesExtension *ext = i->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
                ext->simplify("check", true);
                std::cout << *i << ": ";
                for (auto &ap : ext->availablePackets) {
                    std::cout << ap << ", ";
                }
                std::cout << std::endl;
            }
        }

    start = std::chrono::high_resolution_clock::now();

    CheckMessageSpec(allComponents);

    current = std::chrono::high_resolution_clock::now();
    std::cout << "message spec check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << "ms" << std::endl;
    start = current;

    ClearSymbolicTypes(allComponents);
    ClearMessageSpec(allComponents);

    std::cout << "network consisted of " << components.size() << "/" << allComponents.size() << " components" << std::endl;
    for (auto it : components) {
        delete it.second;
    }
}

void TestFile(const std::string &filename, bool showAll) {
    bitpowder::lib::MemoryPool mp;

    auto begin = std::chrono::high_resolution_clock::now();
    auto start = begin;

    auto parse = Parse(filename, mp);
    auto& components = parse.first;

    auto current = std::chrono::high_resolution_clock::now();
    std::cout << "parsed JSON file in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << " ms" << std::endl;
    start = current;

    std::set<XMASComponent*> allComponents;
    for (auto &it : components) {
        //std::cout << "checking " << it.first << std::endl;
        checkAssert(it.second->valid());
        if (it.second)
            allComponents.insert(it.second);
    }

    current = std::chrono::high_resolution_clock::now();

    std::cout << "syntactic check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << " ms" << std::endl;
    start = current;

    checkAssert(!CombinatorialCycleDetector(allComponents));

    current = std::chrono::high_resolution_clock::now();
    std::cout << "combi cycle check in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << " ms" << std::endl;
    start = current;

    SymbolicTypes(allComponents);

    current = std::chrono::high_resolution_clock::now();
    std::cout << "type inference in \t" << std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count() << " ms" << std::endl;
    start = current;

    // output types
    if (showAll)
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

    std::cout << "network consisted of " << components.size() << "/" << allComponents.size() << " components" << std::endl;
}

int main(int argc, char* argv[]) {
/*
    XMASQueue q("test");
    for (Port* p : q.ports())
        std::cout << *p << std::endl;
    return 1;
    */
    try {
        if (argc >= 3 && strcmp("-json", argv[1]) == 0) {
            TestFile(argv[2], true);
        } else if (argc >= 3 && strcmp("-mesh", argv[1]) == 0) {
            MeshTest(atoi(argv[2]), false, false);
        } else if (argc >= 3 && strcmp("-spidergon", argv[1]) == 0) {
            SpidergonTest(atoi(argv[2]), false, false);
        } else {
            std::cerr << "Usage:" << std::endl;
            std::cerr << "- " << argv[0] << " -json [json-file]" << std::endl;
            std::cerr << "- " << argv[0] << " -mesh [size]" << std::endl;
            std::cerr << "- " << argv[0] << " -spidergon [size]" << std::endl;
        }
    } catch (bitpowder::lib::Exception &e) {
        std::cerr << e << std::endl;
    }
#ifndef __MINGW32__
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
#ifdef __APPLE__
    std::cout << usage.ru_maxrss << " bytes of memory used at max" << std::endl;
    std::cout << (usage.ru_maxrss/1024/1024) << " megabytes of memory used at max" << std::endl;
#else
    std::cout << usage.ru_maxrss << " kilobytes of memory used at max" << std::endl;
    std::cout << (usage.ru_maxrss/1024) << " megabytes of memory used at max" << std::endl;
#endif
#endif
    return 0;
}

