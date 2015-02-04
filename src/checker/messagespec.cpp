#include "messagespec.h"
#include "lib/simplestring.h"
#include "symbolic.h"
#include <sstream>
#include <unordered_set>
#ifdef CONCURRENCY
#include "lib/dispatch.h"
#include "lib/lock.h"
#endif

typedef std::pair<Output*,SymbolicPacket> VisitedNode;
//typedef Output* Visited;

typedef std::unordered_set<VisitedNode, std::hash<VisitedNode>, std::equal_to<VisitedNode>, MemoryPool::Allocator<VisitedNode>> Visited;

namespace std {
template<>
    struct hash<VisitedNode>
    {
        typedef VisitedNode argument_type;
        typedef std::size_t value_type;

        value_type operator()(argument_type const& s) const
        {
            value_type const h1 = (value_type)s.first;
            value_type const h2 = std::hash<SymbolicPacket>()(s.second);
            return h1 ^ (h2 << 1);
        }
    };
}

MessageSpec::Ref ConstructFormula(Output *o, Visited &visited, const std::vector<SymbolicPacket> &packets);

class MessageSpecForward : public XMASComponentVisitor {
public:
    MessageSpec::Ref expr;
    Visited &visited;
    const std::vector<SymbolicPacket> &packets;

    MessageSpecForward(Visited &visited, const std::vector<SymbolicPacket> &packets) : expr(), visited(visited), packets(packets) {
    }

    virtual void visit(XMASSink *c) {
        expr = S(c, packets);
    }

    virtual void visit(XMASSource *c) {
        for (Output *o : c->outputPorts()) {
            expr = ConstructFormula(o, visited, packets);
        }
    }

    virtual void visit(XMASQueue *c) {
        for (Output *o : c->outputPorts()) {
            expr  = ConstructFormula(o, visited, packets);
        }
    }

    virtual void visit(XMASFunction *c) {
        SymbolicFunctionExtension<std::vector<SymbolicPacket> (const std::vector<SymbolicPacket>&)> *ext = c->getComponentExtension<SymbolicFunctionExtension<std::vector<SymbolicPacket> (const std::vector<SymbolicPacket>&)>>();
        if (ext != nullptr || (bool)ext->function) {
        /*
            std::vector<SymbolicPacket> newPackets;
            for (auto &packet : packets) {
                auto result = ext->function(packet);
                newPackets.insert(newPackets.end(), result.begin(), result.end());
            }
            */
            auto newPackets = ext->function(packets);
            expr = ConstructFormula(&c->o, visited, newPackets);
        }
    }

    virtual void visit(XMASSwitch *c) {
        {
            // output a
            SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
            std::vector<SymbolicPacket> outPackets;
            for (auto &funcPacket : ext->availablePackets) {
                for (auto &packet : packets) {
                    auto result = packet.getIntersection(funcPacket);
                    outPackets.insert(outPackets.end(), result.begin(), result.end());
                }
            }
            expr = ConstructFormula(&c->a, visited, outPackets);
        }
        {
            // output b
            SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
            std::vector<SymbolicPacket> outPackets;
            for (auto &packet : packets)
                outPackets.push_back(packet);
            for (auto &funcPacket : ext->availablePackets) {
                std::vector<SymbolicPacket> newRetval;
                for (auto &r : outPackets) {
                    r.getDifference(funcPacket, [&newRetval,&funcPacket,this](SymbolicPacket &&p) {
                        //std::cout << "difference between " << r << " and " << funcPacket << " -> " << p << std::endl;
                        newRetval.emplace_back(std::move(p));
                        //retval.push_back(p);
                    });
                }
                outPackets = std::move(newRetval);
            }

            MessageSpec::Ref newRetval = ConstructFormula(&c->b, visited, outPackets);
            if (newRetval && expr) {
                //retval = "("_S + retval + ") OR (" + newRetval + ")";
                expr = expr | newRetval; //ToString(retval) + " OR " + newRetval;
            } else if (!expr) {
                expr = newRetval;
            }
        }
    }

    virtual void visit(XMASFork *c) {
        for (Output *o : c->outputPorts()) {
            if (expr) {
                expr = expr & ConstructFormula(o, visited, packets);
            } else {
                expr = ConstructFormula(o, visited, packets);;
            }
        }
    }

    virtual void visit(XMASMerge *c) {
        for (Output *o : c->outputPorts())
            expr = ConstructFormula(o, visited, packets);
    }

    virtual void visit(XMASJoin *c) {
        // FIXME: adjusted packets
        for (Output *o : c->outputPorts())
            expr  = ConstructFormula(o, visited, packets);
    }
};

void add(Output *out, const std::vector<SymbolicPacket> &packets, Visited &visited) {
    for (const SymbolicPacket &p : packets) {
        //std::cerr << "visting " << *out << " (" << out << ") with packet " << p << std::endl;
        visited.insert(std::make_pair(out, p));
        //visited.insert(out);
    }
}

void remove(Output *out, const std::vector<SymbolicPacket> &packets, Visited &visited) {
    for (const SymbolicPacket &p : packets) {
       // std::cerr << "unvisting " << *out << " (" << out << ") with packet " << p << std::endl;
        visited.erase(std::make_pair(out, p));
        //visited.erase(out);
    }
}

MessageSpec::Ref ConstructFormula(Output *out, Visited &visited, const std::vector<SymbolicPacket> &packets) {
    if (packets.empty())
        return nullptr;
    //std::cerr << *out << ":";
    //for (const SymbolicPacket &packet : packets)
    //    std::cerr << packet << ",";
    //std::cerr << std::endl;
    if (std::any_of(packets.begin(), packets.end(), [out, &visited](const SymbolicPacket &p) {
                    //std::cerr << "find (" << out << "," << p << ") in visited(" << visited.size() << ")" << std::endl;
                    return visited.find(std::make_pair(out, p)) != visited.end();
                    //return visited.find(output) != visited.end();
    })) {
        std::string pString;
        for (const SymbolicPacket &p : packets) {
            std::ostringstream o;
            o << p;
            pString = pString + o.str();
        }
        return MessageSpec::make_shared<MessageSpecError>((std::string)("LOOP@" + out->getComponent()->getName() + "=" + pString));
    }
    add(out, packets, visited);
    MessageSpecForward ms(visited, packets);
    out->getTarget()->accept(ms);
    remove(out, packets, visited);
    return ms.expr;
}
#ifndef CONCURRENCY
class Work {

};
#endif

struct MessageSpecWorkerItem : public Work {
    concurrent_count refcount;
    XMASSource *source;
    std::tuple<std::vector<SymbolicPacket>, MessageSpec::Ref> &info;
    std::atomic<long> &problems;
    MessageSpecWorkerItem(XMASSource *source, std::tuple<std::vector<SymbolicPacket>, MessageSpec::Ref> &info, std::atomic<long> &problems) : source(source), info(info), problems(problems) {
    }
    void execute() {
        MemoryPool mp;
        Visited visited(256, std::hash<VisitedNode>(), std::equal_to<VisitedNode>(), mp.allocator<VisitedNode>());

        auto result = ConstructFormula(&source->o, visited, std::get<0>(info));
        auto &spec = std::get<1>(info);
        bool correct = *spec == result;
        //std::cout << *item->source << " with spec " << *spec << " -> " << *result << ": " <<  correct << std::endl;
        if (!correct) {
            problems++;
            std::cout << *source << " with spec " << *spec << " -> " << *result << ": " <<  correct << std::endl;
        }
    }
    void aborted(Exception& e) {
    }
};

bool CheckMessageSpec(std::set<XMASComponent *> allComponents) {
    std::atomic<long> problems(0);
#ifdef CONCURRENCY
    WorkQueue allItems;
#endif
    for (XMASComponent *c : allComponents) {
        XMASSource *s = dynamic_cast<XMASSource*>(c);
        if (!s)
            continue;
        MessageSpecExtension *ext = s->o.getPortExtension<MessageSpecExtension>(false);
        if (ext)
            for (auto &info : ext->specs) {
                auto w = new MessageSpecWorkerItem(s, info, problems);
#ifdef CONCURRENCY
                allItems.push_back(w);
#else
                w->execute();
#endif
            }
    }
#ifdef CONCURRENCY
    parallel(allItems);
#endif
    std::cout << "found " << problems << " issues with the specification" << std::endl;
    return problems == 0;
}


std::ostream &operator <<(std::ostream &out, const MessageSpec &c)
{
    if (&c == nullptr) {
        out << "(null)";
    } else
        c.print(out);
    return out;
}


void MessageSpecEnd::print(std::ostream &out) const
{
    out << "(";
    out << *endpoint << " as ";
    for (const SymbolicPacket &p : packets)
        out << p;
    out << ")";
}

bool MessageSpecEnd::operator==(const MessageSpec::Ref &b) const
{
    auto other = FastType<MessageSpecEnd>::cast(b.get());
    if (!other)
        return false;
    return endpoint == other->endpoint && packets == other->packets;
}

void MessageSpecEndLookup::print(std::ostream &out) const
{
    out << "(";
    if (endpoint) {
        out << *endpoint;
    } else {
        out << "(lookup not performed/not found)";
    }
    out << " as ";
    for (const SymbolicPacket &p : packets)
        out << p;
    out << ")";
}

bool MessageSpecEndLookup::operator==(const MessageSpec::Ref &b) const
{
    auto other = FastType<MessageSpecEndLookup>::cast(b.get());
    if (!other)
        return false;
    return endpoint == other->endpoint && packets == other->packets;
}

void MessageSpecEndLookup::lookup(std::map<String, XMASComponent *> mapping) {
    // FIXME: if not found, throw error
    endpoint = mapping[endpointName];
}

void MessageSpecOr::print(std::ostream &out) const
{
    out << *a << " OR " << *b;
}

bool MessageSpecOr::operator==(const MessageSpec::Ref &other) const
{
    auto o = FastType<MessageSpecOr>::cast(other.get());
    if (!o)
        return false;
    if (*a == o->a)
        return *b == o->b;
    return *a == o->b && *b == o->a;
}

void MessageSpecOr::lookup(std::map<String, XMASComponent *> mapping)
{
    a->lookup(mapping);
    b->lookup(mapping);
}


void MessageSpecError::print(std::ostream &out) const
{
    out << "ERROR: " << description;
}

bool MessageSpecError::operator==(const MessageSpec::Ref &o) const
{
    auto other = FastType<MessageSpecError>::cast(o.get());
    if (!other)
        return false;
    return description == other->description;
}


void MessageSpecAnd::print(std::ostream &out) const
{
    out << *a << " AND " << *b;
}

bool MessageSpecAnd::operator==(const MessageSpec::Ref &other) const
{
    auto o = FastType<MessageSpecAnd>::cast(other.get());
    if (!o)
        return false;
    if (*a == o->a)
        return *b == o->b;
    return *a == o->b && *b == o->a;
}

void MessageSpecAnd::lookup(std::map<String, XMASComponent *> mapping) {
    a->lookup(mapping);
    b->lookup(mapping);
}


void attachMessageSpec(Output *port, const std::vector<SymbolicPacket> &a, MessageSpec::Ref spec) {
    if (spec) {
        MessageSpecExtension *info = port->getPortExtension<MessageSpecExtension>();
        info->addMessageSpec(a, spec);
    }

    attachPackets(port, a);
}

void ClearMessageSpec(std::set<XMASComponent *> allComponents)
{
    for (XMASComponent *c : allComponents) {
        for (Output *p : c->outputPorts())
            p->clearPortExtension<MessageSpecExtension>();
    }
}
