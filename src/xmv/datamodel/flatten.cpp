#include "xmas.h"
#include "flatten.h"

using bitpowder::lib::String;

static void flattenInto(XMASNetwork& dst, const XMASNetwork& src, const std::string prefix);

class FlattenVisitor : public HierarchicalComponentVisitor
{
public:
    FlattenVisitor(XMASNetwork& network, String name) : network(network), name(name)
    {}

    void visit(XMASSink *) override         { result = network.insert<XMASSink>(name); }
    void visit(XMASSource *) override       { result = network.insert<XMASSource>(name); }
    void visit(XMASQueue *) override        { result = network.insert<XMASQueue>(name); }
    void visit(XMASFunction *) override     { result = network.insert<XMASFunction>(name); }
    void visit(XMASSwitch *) override       { result = network.insert<XMASSwitch>(name); }
    void visit(XMASFork *) override         { result = network.insert<XMASFork>(name); }
    void visit(XMASMerge *) override        { result = network.insert<XMASMerge>(name); }
    void visit(XMASJoin *) override         { result = network.insert<XMASJoin>(name); }
    void visit(XMASInGate *) override       { result = network.insert<XMASInGate>(name); }           // use a separate memory pool for hierarchical components
    void visit(XMASOutGate *) override      { result = network.insert<XMASOutGate>(name); }          // so they don't leave holes in the memory after collapsing?
    void visit(XMASComposite* c) override   {
        const XMASNetwork& subnetwork = c->getNetwork();
        const std::string prefix { name.stl() + "::" };
        flattenInto(network, subnetwork, prefix);
    }

    XMASComponent* result = nullptr;
private:
    XMASNetwork& network;
    const String name;
};

XMASNetwork flatten(const XMASNetwork& src)
{
    XMASNetwork result {src.getStdName() + "_flattened"};

    flattenInto(result, src, "::");

    return std::move(result);
}

void flattenInto(XMASNetwork& dst, const XMASNetwork& src, const std::string prefix)
{
    // a mapping between the original hierarchical component (src) and the flattened copy (dst)
    std::map<XMASComponent*, XMASComponent*> hierFlatMap;

    // copy all components
    for (auto entry : src.getComponents()) {
        auto name = entry.first;
        auto c = entry.second;

        const std::string qualifiedName  {prefix + name.stl()};
        FlattenVisitor fv {dst, qualifiedName};
        c->accept(fv);

        hierFlatMap[c] = fv.result;
    }

    // TODO: copy relevant extensions (e.g. messagespec, switching function) from src


    // connect channels
    for (auto entry : src.getComponents()) {
        // consider this component as the initiator of a channel
        XMASComponent* hierInitComp = entry.second;
        XMASComponent* flatInitComp = hierFlatMap[hierInitComp];

        // check if the flattened network still contains this component (i.e. it wasn't a composite)
        if (flatInitComp) {
            // loop through all output ports
            for (Output* hierInitPort : hierInitComp->outputPorts()) {
                const char* initPortName = hierInitPort->getName();

                // find the corresponding port in the flattened network
                Output* flatInitPort = flatInitComp->findOutputPort(initPortName);

                // find the target port & component in the hierarchical network
                Input* hierTargetPort = hierInitPort->getTargetPort();
                XMASComponent* hierTargetComp = hierInitPort->getTarget();
                const char* targetPortName = hierTargetPort->getName();

                // and in the flattened network
                XMASComponent* flatTargetComp = hierFlatMap[hierTargetComp];

                // also check the target still exists
                if (flatTargetComp) {
                    Input* flatTargetPort = flatTargetComp->findInputPort(targetPortName);

                    // now connect the ports in the flattened network
                    connect(*flatInitPort, *flatTargetPort);
                }
            }
        }
    }

    // TODO: collapse gates/composite ports
}
