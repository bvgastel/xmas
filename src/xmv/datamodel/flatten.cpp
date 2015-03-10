#include "xmas.h"

using bitpowder::lib::String;

class CopyVisitor : public HierarchicalComponentVisitor
{
public:
    CopyVisitor(XMASNetwork& network, String name) : network(network), name(name)
    {}

    void visit(XMASSink *) override     { network.insert<XMASSink>(name); }
    void visit(XMASSource *) override   { network.insert<XMASSource>(name); }
    void visit(XMASQueue *) override    { network.insert<XMASQueue>(name); }
    void visit(XMASFunction *) override { network.insert<XMASFunction>(name); }
    void visit(XMASSwitch *) override   { network.insert<XMASSwitch>(name); }
    void visit(XMASFork *) override     { network.insert<XMASFork>(name); }
    void visit(XMASMerge *) override    { network.insert<XMASMerge>(name); }
    void visit(XMASJoin *) override     { network.insert<XMASJoin>(name); }
    void visit(XMASInGate *) override   {}
    void visit(XMASOutGate *) override   {}
    void visit(XMASComposite *) override   {}

private:
    XMASNetwork& network;
    const String name;
};

XMASNetwork flatten(XMASNetwork& in)
{
    XMASNetwork result {in.getStdName() + "_flattened"};

    // copy all components
    for (auto entry : in.getComponents()) {
        auto name = entry.first;
        auto c = entry.second;

        CopyVisitor cv {result, name};
        c->accept(cv);
    }

    // TODO: connect channels
    // TODO: collapse gates/composite ports

    return std::move(result);
}
