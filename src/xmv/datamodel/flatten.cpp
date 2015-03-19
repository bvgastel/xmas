#include "xmas.h"
#include "flatten.h"


using bitpowder::lib::String;
using bitpowder::lib::Exception;

struct Gates
{
    std::vector<XMASInGate*>    inGates;
    std::vector<XMASOutGate*>   outGates;
};

class XMASFlattenedComposite : public XMASComponent
{
public:
    Gates gates;
    std::vector<Port*> p;

    XMASFlattenedComposite(const String& name, Gates&& gates) : XMASComponent(name), gates(gates)
    {
        for (XMASInGate* i: gates.inGates)
            p.push_back(&i->i_ext);
        for (XMASOutGate* o: gates.outGates)
            p.push_back(&o->o_ext);
    }

    void accept(XMASComponentVisitor&) override
    {
        throw Exception("Unable to visit flattened composites.");
    }


    Port** beginPort(PortType type) override
    {
        return type == PortType::OUTPUT_PORT ? &p[gates.inGates.size()] : &p[0];
    }

    Port** endPort(PortType type) override
    {
        return type == PortType::INPUT_PORT ? &p[gates.inGates.size()] : &p[p.size()];
    }
};


static Gates flattenInto(XMASNetwork& dst, const XMASNetwork& src, const std::string prefix);

class FlattenVisitor : public HierarchicalComponentVisitor
{
public:
    FlattenVisitor(XMASNetwork& network, Gates& gates, String name) : network(network), gates(gates), name(name)
    {}

    void visit(XMASSink *) override         { result = network.insert<XMASSink>(name); }
    void visit(XMASSource *) override       { result = network.insert<XMASSource>(name); }
    void visit(XMASQueue *) override        { result = network.insert<XMASQueue>(name); }
    void visit(XMASFunction *) override     { result = network.insert<XMASFunction>(name); }
    void visit(XMASSwitch *) override       { result = network.insert<XMASSwitch>(name); }
    void visit(XMASFork *) override         { result = network.insert<XMASFork>(name); }
    void visit(XMASMerge *) override        { result = network.insert<XMASMerge>(name); }
    void visit(XMASJoin *) override         { result = network.insert<XMASJoin>(name); }
    void visit(XMASInGate *) override       { auto inGate = new XMASInGate(name); result = inGate; gates.inGates.push_back(inGate); }
    void visit(XMASOutGate *) override      { auto outGate = new XMASOutGate(name); result = outGate; gates.outGates.push_back(outGate); }
    void visit(XMASComposite* c) override   {
        const XMASNetwork& subnetwork = c->getNetwork();
        const std::string prefix { name.stl() + "::" };
        Gates gates = flattenInto(network, subnetwork, prefix);

        result = new XMASFlattenedComposite(name, std::move(gates));
    }

    XMASComponent* result = nullptr;
private:
    XMASNetwork& network;
    Gates& gates;
    const String name;
};

XMASNetwork flatten(const XMASNetwork& src)
{
    XMASNetwork result {src.getStdName() + "_flattened"};

    flattenInto(result, src, "::");

    return std::move(result);
}

int getPortNo(XMASComponent* comp, Port* port) {
    int index = 0;
    for (auto p : comp->ports()) {
        if (p == port)
            return index;
        ++index;
    }
    return -1;
}

Port* getPort(XMASComponent* comp, int index) {
    for (auto p : comp->ports()) {
        if (index == 0)
            return p;
        --index;
    }
    return nullptr;
}


Gates flattenInto(XMASNetwork& dst, const XMASNetwork& src, const std::string prefix)
{
    Gates gates;

    // a mapping between the original hierarchical component (src) and the flattened copy (dst)
    std::map<XMASComponent*, XMASComponent*> hierFlatMap;

    // step 1: copy all components and gates, composites are recursively flattened
    for (auto entry : src.getComponents()) {
        auto name = entry.first;
        auto c = entry.second;

        const std::string qualifiedName  {prefix + name.stl()};
        FlattenVisitor fv {dst, gates, qualifiedName};
        c->accept(fv);

        hierFlatMap[c] = fv.result;

        //std::cout << qualifiedName << std::endl;
    }


    // step 2: copy all channels; this implicitly connects composite channels to the subnetworks gates
    for (auto entry : src.getComponents()) {
        // consider this component as the initiator of a channel
        XMASComponent* hierInitComp = entry.second;
        XMASComponent* flatInitComp = hierFlatMap[hierInitComp];

        if (!flatInitComp)
            throw Exception("Flattened initiator component could not be found");

        // loop through all output ports
        for (Output* hierInitPort : hierInitComp->outputPorts()) {
            // find the target port & component in the hierarchical network
            Input* hierTargetPort = hierInitPort->getTargetPort();
            XMASComponent* hierTargetComp = hierInitPort->getTarget();

            // and the target component in the flattened network
            XMASComponent* flatTargetComp = hierFlatMap[hierTargetComp];
            if (!flatTargetComp)
                throw Exception("Flattened target component could not be found");


            // get the port indices
            int initPortNo = getPortNo(hierInitComp, hierInitPort);
            int targetPortNo = getPortNo(hierTargetComp, hierTargetPort);

            // and find the corresponding ports in the flattened network
            Output* flatInitPort = dynamic_cast<Output*>(getPort(flatInitComp, initPortNo));
            Input* flatTargetPort = dynamic_cast<Input*>(getPort(flatTargetComp, targetPortNo));

            if (!flatInitPort || !flatTargetPort)
                throw Exception("Could not find the ports in the flattened network.");

            // finally, connect!
            connect(*flatInitPort, *flatTargetPort);

            //std::cout << "Connecting " << flatInitPort->m_owner->getName() << "." << flatInitPort->getName()
            //          << " to " << flatTargetPort->m_owner->getName() << "." << flatTargetPort->getName() << std::endl;
        }

    }

    // step 3: collapse gates/composite ports
    for (auto c : src.componentsOfType<XMASComposite>()) {
        XMASFlattenedComposite* flatC = dynamic_cast<XMASFlattenedComposite*>(hierFlatMap[c]);
        if (!flatC)
            throw Exception("Composite not flattened correctly.");

        while (flatC->gates.inGates.size() > 0) {
            XMASInGate* gate = flatC->gates.inGates.back();
            flatC->gates.inGates.pop_back();

            // weld the two channels
            connect(*gate->i_ext.getInitiatorPort(), *gate->o.getTargetPort());

            // and delete the gate
            delete(gate);
        }
        while (flatC->gates.outGates.size() > 0) {
            XMASOutGate* gate = flatC->gates.outGates.back();
            flatC->gates.outGates.pop_back();

            // weld the two channels
            connect(*gate->i.getInitiatorPort(), *gate->o_ext.getTargetPort());

            // and delete the gate
            delete(gate);
        }

        // delete the temporary XMASFlattenedComposite
        delete(flatC);
    }


    // TODO: copy relevant extensions (e.g. messagespec, switching function) from src
    return gates;
}
