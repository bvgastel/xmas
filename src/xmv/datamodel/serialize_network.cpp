#include "serialize_network.h"
#include "jsonprinter.h"
#include "xmas.h"

class NameComponentVisitor : public XMASComponentVisitor
{
public:
    virtual void visit(XMASSink *)      { result = "sink"; }
    virtual void visit(XMASSource *)    { result = "source"; }
    virtual void visit(XMASQueue *)     { result = "queue"; }
    virtual void visit(XMASFunction *)  { result = "function"; }
    virtual void visit(XMASSwitch *)    { result = "xswitch"; }
    virtual void visit(XMASFork *)      { result = "xfork"; }
    virtual void visit(XMASMerge *)     { result = "merge"; }
    virtual void visit(XMASJoin *)      { result = "join"; }

    const char* result = nullptr;
};

void serialize_network(std::ostream& os, std::set<XMASComponent*>& network) {
    JsonPrinter pr {os};

    pr << json_startobj <<
          jsonprop("VARS", json_startarray) << json_endarray << json_endprop <<
          jsonprop("PACKET_TYPE", json_startobj) << json_endobj << json_endprop <<
          jsonprop("COMPOSITE_OBJECTS", json_startarray) << json_endarray << json_endprop <<
          jsonprop("NETWORK", json_startarray);


    for (auto c : network) {
        pr.startObject();

        NameComponentVisitor ncv;
        c->accept(ncv);

        pr << jsonprop("id", c->getName().stl().c_str()) <<
              jsonprop("type", ncv.result);

        auto outs = c->outputPorts();
        if (outs.begin() != outs.end()) {
            pr << jsonprop("outs", json_startarray);

            for (auto out : outs) {
                auto target = out->getTarget();

                pr << json_startobj << jsonprop("id", target->getName().stl().c_str());

                int i = 0;
                for (auto in : target->inputPorts()) {
                    if (in == out->getTargetPort())
                        pr << jsonprop("in_port", i);

                    i++;
                }

                pr << json_endobj;
            }

            pr << json_endarray << json_endprop;
        }

        pr << json_endobj;
    }

    pr << json_endarray << json_endprop << json_endobj;
}
