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
        ExportFieldVisitor(std::ostringstream& out) : out(out) {
        }

        virtual void nextField() {
            out << " && ";
        }

        virtual void visit(const String& field, const SymbolicIntervalField *f) {
            if (f->getMin() == f->getMax()-1) {
                out << field << " == " << f->getMin();
            } else {
                out << field << " >= " << f->getMin() << " && " << field << " < " << f->getMax();
            }
        }

        virtual void visit(const String& field, const SymbolicEnumField *f) {
            out << field << " in {";
            bool first = true;
            for (const String& v : f->values) {
                if (!first)
                    out << ", ";
                out << v;
            }
            out << "}";
        }

        virtual void visit(const String& field, const SymbolicAnyField *f) {
            bitpowder::lib::unused(field, f);
            out << "true";
        }
    };


    class ExportToOldCFieldVisitor : public SymbolicFieldVisitor {
    public:
    std::ostringstream& out;
    std::map<bitpowder::lib::String,int>& enumMap;

    ExportToOldCFieldVisitor(std::ostringstream& out, std::map<String,int>& enumMap) : out(out), enumMap(enumMap) {
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
    bitpowder::lib::MemoryPool& mp;
    ExportVisitor(JSONData::Map& json) : json(json), mp(*json.get_allocator().mp) {
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

    virtual String exportSourceExpression(XMASSource* source, MemoryPool& mp) {
        return Export(source, mp);
    }

    virtual void visit(XMASSource *c) {
        json["type"] = "source"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->o, outs);
        json["outs"] = std::move(outs);

        String types = exportSourceExpression(c, mp);
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

    virtual String exportFunctionExpression(XMASFunction* c, MemoryPool& mp) {
        ParsedXMASFunctionExtension *ext = c->getComponentExtension<ParsedXMASFunctionExtension>(false);
        std::ostringstream tmp;
        tmp << *ext->value;
        return String(tmp.str())(mp);
    }

    virtual void visit(XMASFunction *c) {
        json["type"] = "function"_S;

        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->o, outs);
        json["outs"] = std::move(outs);

        String function = exportFunctionExpression(c, mp);
        JSONData::Vector fields = JSONData::AllocateVector(mp);
        JSONData::Map function_wrapper = JSONData::AllocateMap(mp);
        function_wrapper["function"] = function;
        fields.push_back(std::move(function_wrapper));
        json["fields"] = std::move(fields);
    }

    virtual String exportSwitchExpression(XMASSwitch* c, MemoryPool& mp) {
        return Export(c, mp);
    }

    virtual void visit(XMASSwitch *c) {
        json["type"] = "xswitch"_S;
        JSONData::Vector outs = JSONData::AllocateVector(mp);
        writeOut(c->a, outs);
        writeOut(c->b, outs);
        json["outs"] = std::move(outs);

        String function = exportSwitchExpression(c, mp);
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



class ExportOldCVisitor : public ExportVisitor {
    std::map<String,int>& enumMap;
public:
    ExportOldCVisitor(JSONData::Map& json, std::map<String,int>& enumMap) : ExportVisitor(json), enumMap(enumMap) {
    }

    virtual String exportSourceExpression(XMASSource* c, MemoryPool& mp) {
        return ExportOldCStyle(c, enumMap, mp);
    }

    virtual String exportSwitchExpression(XMASSwitch* c, MemoryPool& mp) {
        return ExportOldCStyle(c, enumMap, mp);
    }

    virtual String exportFunctionExpression(XMASFunction *c, MemoryPool &mp) {
        ParsedXMASFunctionExtension *ext = c->getComponentExtension<ParsedXMASFunctionExtension>(false);
        std::ostringstream tmp;
        ext->value->printOldCSyntax(tmp, enumMap);
        return String(tmp.str())(mp);
    }
};

String ExportOldCStyle(std::set<XMASComponent *> allComponents, const JSONData& globals, MemoryPool& returnMemoryPool) {
    MemoryPool mp;

    std::map<String,int> enumMap;
    JSONData::Vector network = JSONData::AllocateVector(mp);
    for (XMASComponent* component : allComponents) {
        JSONData::Map jsonComponent = JSONData::AllocateMap(mp);
        jsonComponent["id"] = JSONData(component->getName());

        ExportOldCVisitor visitor(jsonComponent, enumMap);
        component->accept(visitor);

        network.push_back(std::move(jsonComponent));
    }

    JSONData::Map root = globals.asObject(); //JSONData::AllocateMap(mp);
    root["NETWORK"] = network;

        std::ostringstream buffer;
        buffer << root;
        return String(buffer.str())(returnMemoryPool);
     }


    String ExportOldCStyle(XMASSource* c, std::map<String, int>& enumMap, MemoryPool& mp)
    {
        std::ostringstream buffer;
        ExportToOldCFieldVisitor visitor(buffer, enumMap);
        bool first = true;
        SymbolicTypesExtension *ext = c->o.getPortExtension<SymbolicTypesExtension>();
        for (const SymbolicPacket& packet : ext->availablePackets) {
            if (!first)
                buffer << " || ";
            buffer << "(";
            packet.accept(visitor);
            buffer << ")";
            first = false;
        }
        return ("{p in PacketDomain | "_S + buffer.str() + "}")(mp);
    }

    String ExportOldCStyle(XMASSwitch* c, std::map<String, int>& enumMap, MemoryPool& mp)
    {
        std::ostringstream buffer;
        ExportToOldCFieldVisitor visitor(buffer, enumMap);
        bool first = true;
        SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
        for (const SymbolicPacket& packet : ext->availablePackets) {
            if (first) {
                first = false;
            } else {
                buffer << " || ";
            }
            buffer << "(";
            packet.accept(visitor);
            buffer << ")";
        }
        return ("return "_S + buffer.str() + ";")(mp);
    }


    String ExportOldCStyleClause(const SymbolicPacket& packet, std::map<String, int>& enumMap, MemoryPool& mp)
    {
        std::ostringstream buffer;
        ExportToOldCFieldVisitor visitor(buffer, enumMap);
        packet.accept(visitor);
        return String(buffer.str())(mp);
    }


    String ExportClause(const SymbolicPacket& packet, MemoryPool& mp)
    {
        std::ostringstream buffer;
        ExportFieldVisitor visitor(buffer);
        packet.accept(visitor);
        return String(buffer.str())(mp);
    }


    String Export(XMASSource* source, MemoryPool& mp)
    {
        std::ostringstream buffer;
        ExportFieldVisitor visitor(buffer);
        SymbolicTypesExtension *ext = source->o.getPortExtension<SymbolicTypesExtension>();
        bool first = true;
        bool multiple = ext->availablePackets.size() > 1;
        for (const SymbolicPacket& packet : ext->availablePackets) {
            if (!first)
                buffer << " or ";
            if (multiple)
                buffer << "(";
            packet.accept(visitor);
            if (multiple)
                buffer << ")";
            first = false;
        }
        return String(buffer.str())(mp);
    }


    String Export(XMASSwitch* c, MemoryPool& mp)
    {
        std::ostringstream buffer;
        ExportFieldVisitor visitor(buffer);
        SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
        bool first = true;
        bool multiple = ext->availablePackets.size() > 1;
        for (const SymbolicPacket& packet : ext->availablePackets) {
            if (!first)
                buffer << " or ";
            if (multiple)
                buffer << "(";
            packet.accept(visitor);
            if (multiple)
                buffer << ")";
            first = false;
        }
        return String(buffer.str())(mp);
    }

    String Export(std::set<XMASComponent*> allComponents, const JSONData& globals, bitpowder::lib::MemoryPool& returnMemoryPool)
    {
        MemoryPool mp;
        JSONData::Vector network = JSONData::AllocateVector(mp);
        for (XMASComponent* component : allComponents) {
            JSONData::Map jsonComponent = JSONData::AllocateMap(mp);
            jsonComponent["id"] = JSONData(component->getName());

            ExportVisitor visitor(jsonComponent);
            component->accept(visitor);

            network.push_back(std::move(jsonComponent));
        }

        JSONData::Map root = globals.asObject();
        root["NETWORK"] = network;

        std::ostringstream buffer;
        buffer << root;
        return String(buffer.str())(returnMemoryPool);
    }

