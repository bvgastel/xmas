#include "serialize_network.h"
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
    pr.startObject();

    pr.startProperty("VARS");
    pr.startArray();
    pr.endArray();
    pr.endProperty();

    pr.startProperty("PACKET_TYPE");
    pr.startObject();
    pr.endObject();
    pr.endProperty();

    pr.startProperty("COMPOSITE_OBJECTS");
    pr.startArray();
    pr.endArray();
    pr.endProperty();

    pr.startProperty("NETWORK");
    pr.startArray();

    for (auto c : network) {
        pr.startObject();

        NameComponentVisitor ncv;
        c->accept(ncv);

        pr.writeStringProperty("id", c->getName().stl());
        pr.writeStringProperty("type", ncv.result);

        auto outs = c->outputPorts();
        if (outs.begin() != outs.end()) {
            pr.startProperty("outs");
            pr.startArray();

            for (auto out : outs) {
                auto target = out->getTarget();

                pr.startObject();
                pr.writeStringProperty("id", target->getName().stl());

                int i = 0;
                for (auto in : target->inputPorts()) {
                    if (in == out->getTargetPort())
                        pr.writeNumberProperty("in_port", std::to_string(i));
                    i++;
                }

                pr.endObject();
            }

            pr.endArray();
            pr.endProperty();
        }

        pr.endObject();
    }

    pr.endArray();
    pr.endProperty();
    pr.endObject();
}


JsonPrinter::JsonPrinter(std::ostream& stream)
  : stream(&stream), ownStream(false)
{
}

JsonPrinter::JsonPrinter(std::ostream* stream)
  : stream(stream), ownStream(true)
{
}

JsonPrinter::~JsonPrinter()
{
    if (ownStream)
        delete(stream);
}

void JsonPrinter::startObject()  {
    if (!firstItem)
        *stream << ',' << std::endl << do_indent();
    *stream << '{';
    depth++;
    firstItem = true;
}

void JsonPrinter::endObject() {
    depth--;

    if (firstItem) {
        *stream << '}';
    } else {
        *stream << std::endl << do_indent() << '}';
    }

    firstItem = false;
}

void JsonPrinter::startArray() {
    if (!firstItem)
        *stream << ',';
    *stream << '[';
    depth++;
    firstItem = true;
}

void JsonPrinter::endArray() {
    depth--;
    *stream << ']';
    firstItem = false;
}

void JsonPrinter::startProperty(const std::string &name) {
    if (!firstItem)
        *stream << ',';
    *stream << std::endl << do_indent() <<  '"' << name << "\": ";

    firstItem = true;
}

void JsonPrinter::endProperty() {
    firstItem = false;
}

void JsonPrinter::writeNumber(const std::string& value) {
    *stream << value;
}

void JsonPrinter::writeString(const std::string& value) {
    *stream << '"' << value << '"';
}

void JsonPrinter::writeNumberProperty(const std::string& name, const std::string& value) {
    startProperty(name);
    writeNumber(value);
    endProperty();
}

void JsonPrinter::writeStringProperty(const std::string& name, const std::string& value) {
    startProperty(name);
    writeString(value);
    endProperty();
}
