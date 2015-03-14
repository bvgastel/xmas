#include "export.h"

#include "parser_json.h"
#include "symbolic.h"
#include "symbolic-interval-field.h"
#include "symbolic-enum-field.h"
#include "parse.h"

#include <sstream>

using namespace bitpowder::lib;

class ExportFieldVisitor : public SymbolicFieldVisitor {
public:
    std::ostringstream& out;
    std::map<bitpowder::lib::String,int>& enumMap;
    ExportFieldVisitor(std::ostringstream& out, std::map<bitpowder::lib::String,int>& enumMap)
        : out(out), enumMap(enumMap) {
    }

    virtual void nextField() {
        out << " && ";
    }

    virtual void visit(const bitpowder::lib::String& field, const SymbolicIntervalField *f) {
        if (f->getMin() == f->getMax()-1) {
            out << "p_" << field << " == " << f->getMin();
        } else {
            out << "p_" << field << " >= " << f->getMin() << " && p_" << field << " < " << f->getMax();
        }
    }

    virtual void visit(const bitpowder::lib::String& field, const SymbolicEnumField *f) {
        bool first = true;
        out << "(";
        for (const bitpowder::lib::String& v : f->values) {
            if (!first)
                out << " || ";
            auto it = enumMap.find(v);
            if (it == enumMap.end())
                std::tie(it, std::ignore) = enumMap.insert(std::make_pair(v, enumMap.size()));
            out << "p_" << field << " == " << it->second;
            first = false;
        }
        if (first) {
            out << "false";
        }
        out << ")";
    }

    virtual void visit(const bitpowder::lib::String& field, const SymbolicAnyField *f) {
        unused(field, f);
    }
};

class ExportVisitor : public XMASComponentVisitor {
public:
    bitpowder::lib::JSONData::Map& json;
    std::map<bitpowder::lib::String,int>& enumMap;
    bitpowder::lib::MemoryPool& mp;
    ExportVisitor(bitpowder::lib::JSONData::Map& json,
                  std::map<bitpowder::lib::String,int>& enumMap)
        : json(json), enumMap(enumMap), mp(*json.get_allocator().mp) {
    }

    void writeOut(Output &output, bitpowder::lib::JSONData::Vector& to) {
        bitpowder::lib::JSONData::Map channel = JSONData::AllocateMap(mp);
        channel["id"] = bitpowder::lib::String(output.getTarget()->getName());
        int index = 0;
        for (Input* input : output.getTarget()->inputPorts()) {
            if (input->getInitiatorPort() == &output)
                break;
            ++index;
        }
        channel["in_port"] = index;
        to.push_back(std::move(channel));
    }

    virtual void visit(XMASSink *c) {
        bitpowder::lib::unused(c);
        json["type"] = "sink"_S;
    }

    virtual void visit(XMASSource *c) {
        json["type"] = "source"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->o, outs);
        json["outs"] = std::move(outs);

        std::ostringstream tmp;
        ExportFieldVisitor visitor(tmp, enumMap);
        bool first = true;
        SymbolicTypesExtension *ext = c->o.getPortExtension<SymbolicTypesExtension>();
        for (const SymbolicPacket& packet : ext->availablePackets) {
            if (!first)
                tmp << " || ";
            tmp << "(";
            packet.accept(visitor);
            tmp << ")";
            first = false;
        }
        String types = ("{p in PacketDomain | "_S + tmp.str() + "}")(mp);

        JSONData::Vector fields = JSONData::AllocateVector(mp);
        JSONData::Map init_types_wrapper = JSONData::AllocateMap(mp);
        init_types_wrapper["init_types"] = types;
        fields.push_back(std::move(init_types_wrapper));
        json["fields"] = std::move(fields);
    }

    virtual void visit(XMASQueue *c) {
        json["type"] = "queue"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->o, outs);
        json["outs"] = std::move(outs);

        JSONData::Vector fields = JSONData::AllocateVector(mp);
        JSONData::Map size_wrapper = JSONData::AllocateMap(mp);
        size_wrapper["size"] = c->c;
        fields.push_back(std::move(size_wrapper));
        json["fields"] = std::move(fields);
    }

    virtual void visit(XMASFunction *c) {
        json["type"] = "function"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->o, outs);
        json["outs"] = std::move(outs);

        ParsedXMASFunctionExtension *ext = c->getComponentExtension<ParsedXMASFunctionExtension>(false);
        std::ostringstream tmp;
        //tmp << *ext->value;
        ext->value->printOldCSyntax(tmp, enumMap);
        String function = String(tmp.str())(mp);

        JSONData::Vector fields = JSONData::AllocateVector(mp);
        JSONData::Map function_wrapper = JSONData::AllocateMap(mp);
        function_wrapper["function"] = function;
        fields.push_back(std::move(function_wrapper));
        json["fields"] = std::move(fields);
    }

    virtual void visit(XMASSwitch *c) {
        json["type"] = "xswitch"_S;
        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->a, outs);
        writeOut(c->b, outs);
        json["outs"] = std::move(outs);

        std::ostringstream tmp;
        ExportFieldVisitor visitor(tmp, enumMap);
        bool first = true;
        SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
        for (const SymbolicPacket& packet : ext->availablePackets) {
            if (first) {
                first = false;
            } else {
                tmp << " || ";
            }
            tmp << "(";
            packet.accept(visitor);
            tmp << ")";
        }
        String function = ("return "_S + tmp.str() + ";")(mp);

        JSONData::Vector fields = JSONData::AllocateVector(mp);
        JSONData::Map function_wrapper = JSONData::AllocateMap(mp);
        function_wrapper["function"] = function;
        fields.push_back(std::move(function_wrapper));
        json["fields"] = std::move(fields);
    }

    virtual void visit(XMASFork *c) {
        json["type"] = "xfork"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->a, outs);
        writeOut(c->b, outs);
        json["outs"] = std::move(outs);
    }

    virtual void visit(XMASMerge *c) {
        json["type"] = "merge"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->o, outs);
        json["outs"] = std::move(outs);
    }

    virtual void visit(XMASJoin *c) {
        json["type"] = "join"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->o, outs);
        json["outs"] = std::move(outs);

        ParsedXMASRestrictedJoin* restricted = c->getComponentExtension<ParsedXMASRestrictedJoin>(false);
        if (restricted) {
            JSONData::Vector fields = JSONData::AllocateVector(mp);
            JSONData::Map function_wrapper = JSONData::AllocateMap(mp);
            function_wrapper["function"] = restricted->function;
            fields.push_back(std::move(function_wrapper));
            json["fields"] = std::move(fields);
        }
    }
};



void Export(std::ostream &out, std::set<XMASComponent *> allComponents, const JSONData& globals) {
    MemoryPool mp;

    std::map<String,int> enumMap;
    JSONData::Vector network = JSONData::AllocateVector(mp);
    for (XMASComponent* component : allComponents) {
        JSONData::Map jsonComponent = JSONData::AllocateMap(mp);
        jsonComponent["id"] = JSONData(component->getName());

        ExportVisitor visitor(jsonComponent, enumMap);
        component->accept(visitor);

        network.push_back(std::move(jsonComponent));
    }

    JSONData::Map root = globals.asObject(); //JSONData::AllocateMap(mp);
    root["NETWORK"] = network;

    out << root;
    std::flush(out);
}

void Export(std::set<XMASComponent *> allComponents, const JSONData& globals)
{
    Export(std::cout, allComponents, globals);
}

