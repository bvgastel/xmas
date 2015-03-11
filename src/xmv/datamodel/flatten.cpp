#include "xmas.h"

using bitpowder::lib::String;

static void flattenInto(XMASNetwork& dst, const XMASNetwork& src, const String prefix);

class FlattenVisitor : public HierarchicalComponentVisitor
{
public:
    FlattenVisitor(XMASNetwork& network, String name) : network(network), name(name)
    {}

    void visit(XMASSink *) override         { network.insert<XMASSink>(name); }
    void visit(XMASSource *) override       { network.insert<XMASSource>(name); }
    void visit(XMASQueue *) override        { network.insert<XMASQueue>(name); }
    void visit(XMASFunction *) override     { network.insert<XMASFunction>(name); }
    void visit(XMASSwitch *) override       { network.insert<XMASSwitch>(name); }
    void visit(XMASFork *) override         { network.insert<XMASFork>(name); }
    void visit(XMASMerge *) override        { network.insert<XMASMerge>(name); }
    void visit(XMASJoin *) override         { network.insert<XMASJoin>(name); }
    void visit(XMASInGate *) override       { network.insert<XMASInGate>(name); }           // use a separate memory pool for hierarchical components
    void visit(XMASOutGate *) override      { network.insert<XMASOutGate>(name); }          // so they don't leave holes in the memory after collapsing?
    void visit(XMASComposite* c) override   {
        const XMASNetwork& subnetwork = c->getNetwork();
        const String prefix { name.stl() + "::" };
        flattenInto(network, subnetwork, prefix);
    }

private:
    XMASNetwork& network;
    const String name;
};

XMASNetwork flatten(const XMASNetwork& src)
{
    XMASNetwork result {src.getStdName() + "_flattened"};

    flattenInto(result, src, "");

    return std::move(result);
}

void flattenInto(XMASNetwork& dst, const XMASNetwork& src, const String prefix)
{
    // copy all components
    for (auto entry : src.getComponents()) {
        auto name = entry.first;
        auto c = entry.second;

        const String qualifiedName  {prefix.stl() + name.stl()};
        FlattenVisitor fv {dst, qualifiedName};
        c->accept(fv);
    }

    // TODO: connect channels
    // TODO: collapse gates/composite ports
}
