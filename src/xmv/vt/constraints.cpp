#include "constraints.h"

#include "extension.h"
#include <set>
#include "memorypool.h"
#include <sstream>


// TODO: What to do with Z3? It is a theorem prover. License not compatible with GPLv3
//#include <z3++.h>
//using namespace z3;

class ConstraintForward : public XMASComponentVisitor {
public:
    Input *input;
    const ConstraintPacket &packet;
    std::function<void(Input *, ConstraintPacket &)> *callback;

    ConstraintForward(Input *input, ConstraintPacket &packet, std::function<void(Input *, ConstraintPacket &)> *callback) : input(input), packet(packet), callback(callback) {
    }

    virtual void visit(XMASSink *c) {
        bitpowder::lib::unused(c);
    }

    virtual void visit(XMASSource *c) {
        bitpowder::lib::unused(c);
    }

    virtual void visit(XMASQueue *c) {
        attachPackets(c->getOutputPorts(), {packet}, callback);
    }

    virtual void visit(XMASFunction *c) {
        attachPackets(c->getOutputPorts(), {packet}, callback);
    }

    virtual void visit(XMASSwitch *c) {
        {
            // output a
            ConstraintSwitchingFunctionExtension *ext = getComponentInfo<ConstraintSwitchingFunctionExtension>(c);
            std::vector<ConstraintPacket> retval;
            for (auto &funcPacket : ext->availablePackets)
                retval.push_back(packet.getIntersection(funcPacket));
            attachPackets(&c->a, std::move(retval), callback);
        }
        {
            // output b
            ConstraintSwitchingFunctionExtension *ext = getComponentInfo<ConstraintSwitchingFunctionExtension>(c);
            std::vector<ConstraintPacket> retval;
            for (auto &funcPacket : ext->availablePackets) {
                std::vector<ConstraintPacket> result = packet.getDifference(funcPacket);
                retval.insert(retval.end(), result.begin(), result.end());
            }
            attachPackets(&c->b, std::move(retval), callback);
        }
    }

    virtual void visit(XMASFork *c) {
        // just propagate, type stays the same
        attachPackets(c->getOutputPorts(), {packet}, callback);
    }

    virtual void visit(XMASMerge *c) {
        // just propagate, type stays the same
        attachPackets(c->getOutputPorts(), {packet}, callback);
    }

    virtual void visit(XMASJoin *c) {
        // store both ports seperatly in the join, and combine this new packet with available packets from the other ports
        ConstraintFunctionExtension<ConstraintPacket (const ConstraintPacket&, const ConstraintPacket&)> *ext = getComponentInfo<ConstraintFunctionExtension<ConstraintPacket (const ConstraintPacket&, const ConstraintPacket&)>>(c, false);
        if (input == &c->a) {
            ConstraintTypesExtension *extB = getPortInfo<ConstraintTypesExtension>(&c->b);
            for (auto &packetB : extB->availablePackets) {
                if (ext) {
                    ConstraintPacket output = ext->function(packet, packetB);
                    attachPackets(c->getOutputPorts(), {output}, callback);
                } else {
                    std::vector<ConstraintPacketField> fields;
                    std::copy(packet.fields.begin(), packet.fields.end(), std::back_inserter(fields));
                    std::copy(packetB.fields.begin(), packetB.fields.end(), std::back_inserter(fields));
                    attachPackets(c->getOutputPorts(), {fields}, callback);
                }
            }
        } else {
            ConstraintTypesExtension *extA = getPortInfo<ConstraintTypesExtension>(&c->a);
            for (auto &packetA : extA->availablePackets) {
                if (ext) {
                    ConstraintPacket output = ext->function(packetA, packet);
                    attachPackets(c->getOutputPorts(), {output}, callback);
                } else {
                    std::vector<ConstraintPacketField> fields;
                    std::copy(packetA.fields.begin(), packetA.fields.end(), std::back_inserter(fields));
                    std::copy(packet.fields.begin(), packet.fields.end(), std::back_inserter(fields));
                    attachPackets(c->getOutputPorts(), {fields}, callback);
                }
            }
        }
    }
};

/*
void propagate(Input *input, ConstraintPacket &packet) {
    packet.propagated = true;

    XMASComponent *c = input->getComponent();
    ConstraintTypesExtension *ext = getPortInfo<ConstraintTypesExtension>(input);
    std::ostringstream o;
    o << *c;
    ext->simplify(o.str());

    std::function<void(Input *, ConstraintPacket &)> callback(propagate);
    ConstraintForward sf(input, packet, &callback);
    c->accept(sf);
}
*/

void attach(Input *port, ConstraintPacket &&a, std::function<void(Input *, ConstraintPacket &)> *callback) {
    if (!a.valid())
        return;
    ConstraintTypesExtension *ext = getPortInfo<ConstraintTypesExtension>(port->getTargetPort());
    ConstraintPacket *newPacket = ext->addConstraintPacket(std::move(a));
    if (newPacket && callback != nullptr) {
        (*callback)(port->getTargetPort(), *newPacket);
    }
}


void attachPackets(Input *port, std::vector<ConstraintPacket> &&a, std::function<void(Input *, ConstraintPacket &)> *callback) {
    for (auto &p : a)
        attach(port, std::move(p), callback);
}

void attachPackets(const std::vector<Input *> &ports, const std::vector<ConstraintPacket> &p, std::function<void(Input *, ConstraintPacket &)> *callback) {
    for (auto &port : ports)
        attachPackets(port, std::vector<ConstraintPacket>(p), callback);
}

void attachPackets(Output *port, std::vector<ConstraintPacket> &&a, std::function<void(Input *, ConstraintPacket &)> *callback) {
    attachPackets(port->getTargetPort(), std::move(a), callback);
}

void attach(Output *port, ConstraintPacket &&a, std::function<void(Input *, ConstraintPacket &)> *callback) {
    attach(port->getTargetPort(), std::move(a), callback);
}

void attachPackets(const std::vector<Output *> &ports, const std::vector<ConstraintPacket> &p, std::function<void(Input *, ConstraintPacket &)> *callback) {
    for (auto &port : ports)
        attachPackets(port, std::vector<ConstraintPacket>(p), callback);
}


void attachSwitchingFunction(XMASSwitch *c, ConstraintPacket &&a)
{
    ConstraintSwitchingFunctionExtension *ext = getComponentInfo<ConstraintSwitchingFunctionExtension>(c);
    ext->addConstraintPacket(std::move(a));
}

/*
void bootstrapPropagate(XMASComponent *c) {
    for (Input *input : c->getInputPorts()) {
        ConstraintTypesExtension *ext = getPortInfo<ConstraintTypesExtension>(input);
        std::ostringstream o;
        o << *c;
        ext->simplify(o.str());
        for (ConstraintPacket &packet : ext->availablePackets) {
            if (packet.propagated)
                continue;
            packet.propagated = true;

            std::function<void(Input *, ConstraintPacket &)> callback(propagate);
            ConstraintForward sf(input, packet, &callback);
            c->accept(sf);
        }
    }
}

void ConstraintTypesRecursive(std::set<XMASComponent *> allComponents) {
    for (XMASComponent *c : allComponents)
        bootstrapPropagate(c);
}
*/

bool iterativePropagateConstraints(XMASComponent *c) {
    bool somethingHappend = false;
    for (Input *input : c->getInputPorts()) {
        ConstraintTypesExtension *ext = getPortInfo<ConstraintTypesExtension>(input);
        std::ostringstream o;
        o << *c;
        ext->simplify(o.str());
        for (ConstraintPacket &packet : ext->availablePackets) {
            if (packet.propagated)
                continue;
            packet.propagated = true;

            ConstraintForward sf(input, packet, nullptr);
            c->accept(sf);

            somethingHappend = true;
        }
    }
    return somethingHappend;
}

void ConstraintTypes(std::set<XMASComponent *> allComponents) {
    MemoryPool mp;
    bool somethingHappend;
    do {
        somethingHappend = false;
        for (XMASComponent *c : allComponents) {
            somethingHappend |= iterativePropagateConstraints(c);
        }
    } while (somethingHappend);
    mp.clear();
}

std::ostream &operator <<(std::ostream &out, const ConstraintPacketField &c) {
    out << "{";
    out << c.constraint;
    out << "}";
    return out;
}

std::ostream &operator <<(std::ostream &out, const ConstraintPacket &c) {
    out << "[";
    for (auto it = c.fields.begin(); it != c.fields.end(); ++it) {
        if (it != c.fields.begin())
            out << " ";
        out << *it;
    }
    out << "]";
    return out;
}

void printConstraintTypeInfo(std::ostream &out, Port *input)
{
    out << *input << ": ";
    ConstraintTypesExtension *ext = getPortInfo<ConstraintTypesExtension>(input);
    if (ext) {
        for (ConstraintPacket &p : ext->availablePackets) {
            out << p << ", ";
        }
    }
}

std::string simplify(const std::string &left_constraint, const std::string &right_constraint, bool combine = false) {
    context c;
    expr e(c);

    func_decl p = c.function("p", 0, nullptr, c.int_sort());

    expr Rinst = c.int_const("R");
    func_decl R = Rinst.decl();
    expr Binst = c.int_const("B");
    func_decl B = Binst.decl();
    expr Ginst = c.int_const("G");
    func_decl G = Ginst.decl();

    expr Xinst = c.int_const("X");
    func_decl X = Xinst.decl();
    expr Yinst = c.int_const("Y");
    func_decl Y = Yinst.decl();
    expr Zinst = c.int_const("Z");
    func_decl Z = Zinst.decl();

    Z3_func_decl func_decls[] = {p, R, B, G, X, Y, Z};
    Z3_symbol func_decl_names[] = {p.name(), R.name(), B.name(), G.name(), X.name(), Y.name(), Z.name()};

    std::string left_constraint_wrap = "(assert " + left_constraint + ")";
    Z3_ast left_ast_c = Z3_parse_smtlib2_string(c, left_constraint_wrap.c_str(), 0, 0, 0, 7, func_decl_names, func_decls);
    expr left = to_expr(c, left_ast_c);

    std::string right_constraint_wrap = "(assert " + right_constraint + ")";
    Z3_ast right_ast_c = Z3_parse_smtlib2_string(c, right_constraint_wrap.c_str(), 0, 0, 0, 7, func_decl_names, func_decls);
    expr right = to_expr(c, right_ast_c);

    goal g(c);
    //g.add(p() == R() || p() == B() || p() == X() || p() == Y());

    Z3_ast argsRGB[3] = { Rinst, Binst, Ginst };
    Z3_ast rRGB = Z3_mk_distinct(c, 3, argsRGB);
    g.add(to_expr(c, rRGB));

    Z3_ast argsXYZ[3] = { Xinst, Yinst, Zinst };
    Z3_ast rXYZ = Z3_mk_distinct(c, 3, argsXYZ);
    g.add(to_expr(c, rXYZ));

    if (combine) {
        g.add(left || right);
    } else {
        g.add(left);
        g.add(right);
    }

    tactic simplify(c, "ctx-solver-simplify");
    tactic propagate_ineqs(c, "propagate-ineqs");
    expr result = (propagate_ineqs & simplify).apply(g)[0].as_expr();


    expr spareResult = result;
    if (result.is_app()) {
        func_decl rf = result.decl();
        if (rf.name().str() == "and") {
            spareResult = c.bool_val(true);
            for (int i = 0; i < result.num_args(); ++i) {
                expr arg = result.arg(i);
                func_decl rf = arg.decl();
                if (rf.name().str() != "distinct") {
                    spareResult = spareResult && arg;
                }
            }
            spareResult = spareResult.simplify();
        }
    }

    std::stringstream ss;
    ss << spareResult;
    return ss.str();
}

ConstraintPacketField ConstraintPacketField::getIntersection(const ConstraintPacketField &a) const {
    ConstraintPacketField retval;
    /*
    if (!complement && !a.complement) {
        // e.g. {a,b} intersection {b,c} -> {b}
        set_intersection(values.begin(), values.end(),
                         a.values.begin(), a.values.end(),
                         std::back_inserter(retval.values));
    } else if (complement && a.complement) {
        // e.g. !{a,b} intersection !{b,c} -> !{a,b,c}
        set_union(values.begin(), values.end(),
                  a.values.begin(), a.values.end(),
                  std::back_inserter(retval.values));
        retval.complement = true;
    } else if (!complement && a.complement) {
        // e.g. {a,b} intersection !{b,c} -> {a}
        set_difference(values.begin(), values.end(),
                       a.values.begin(), a.values.end(),
                       std::back_inserter(retval.values));
    } else if (complement && !a.complement) {
        // e.g. !{a,b} intersection {b,c} -> {c}
        set_difference(a.values.begin(), a.values.end(),
                       values.begin(), values.end(),
                       std::back_inserter(retval.values));
    }
    std::cout << "intersection between " << *this << " and " << a << " -> " << retval << std::endl;
    */

    retval.constraint = simplify(constraint, a.constraint);
    std::cout << retval.constraint << std::endl;
    //char const* spec2 = Z3_ast_to_string(ctx, b);
    //std::cout << "parsed from '" << constraint << "': " << (left && right).simplify() << std::endl;
    return retval;
}

ConstraintPacketField ConstraintPacketField::getDifference(const ConstraintPacketField &a) const {
    ConstraintPacketField retval;
    /*
    if (!complement && !a.complement) {
        // e.g. {a,b} diff {b,c} -> {a}
        set_difference(values.begin(), values.end(),
                       a.values.begin(), a.values.end(),
                       std::back_inserter(retval.values));
    } else if (complement && a.complement) {
        // e.g. !{a,b} diff !{b,c} -> {c}
        set_difference(a.values.begin(), a.values.end(),
                       values.begin(), values.end(),
                       std::back_inserter(retval.values));
    } else if (!complement && a.complement) {
        // e.g. {a,b} diff !{b,c} -> {b}
        set_intersection(values.begin(), values.end(),
                       a.values.begin(), a.values.end(),
                       std::back_inserter(retval.values));
    } else if (complement && !a.complement) {
        // e.g. !{a,b} diff {b,c} -> !{a,b,c}
        set_union(values.begin(), values.end(),
                  a.values.begin(), a.values.end(),
                  std::back_inserter(retval.values));
        retval.complement = true;
    }
    std::cout << "difference between " << *this << " and " << a << " -> " << retval << std::endl;
    */
    retval.constraint = simplify(constraint, "(not " + a.constraint + ")");
    return retval;
}

bool ConstraintPacketField::contains(const ConstraintPacketField &f) const {
    // FIXME: checks if the one implies the other
    return constraint == f.constraint;
}

ConstraintPacketField ConstraintPacketField::combine(const ConstraintPacketField &f) const
{
    ConstraintPacketField retval;
    std::cout << "combine of " << *this << std::endl;
    retval.constraint = simplify(constraint, f.constraint, true);
    return retval;
}

ConstraintPacket ConstraintPacket::getIntersection(const ConstraintPacket &a) const {
    if (a.fields.size() > fields.size())
        throw Exception("can not switch on fields that are possible non-existent");
    ConstraintPacket retval;
    for (unsigned int i = 0; i < fields.size(); ++i)
        retval.fields.push_back(i < a.fields.size() ? fields[i].getIntersection(a.fields[i]) : fields[i]);
    return retval;
}

std::vector<ConstraintPacket> ConstraintPacket::getDifference(const ConstraintPacket &a) const {
    if (a.fields.size() > fields.size())
        throw Exception("can not switch on fields that are possible non-existent");
    std::vector<ConstraintPacket> retval;
    // e.g. switch, input = [{a,b},{c,d}], switch function is [{a},{c}], result of _second_ output is [{b}, {c,d}] AND [{a,b},{d}]
    for (unsigned int i = 0; i < fields.size(); ++i) {
        if (i < a.fields.size()) {
            ConstraintPacketField diff = fields[i].getDifference(a.fields[i]);
            std::cout << "difference result: " << diff << std::endl;
            if (!diff.empty()) {
                ConstraintPacket packet(*this);
                packet.fields[i] = std::move(diff);
                retval.push_back(packet);
                //std::cout << "difference result -> " << packet << std::endl;
            }
        }
    }
    return retval;
}

bool ConstraintPacket::operator==(const ConstraintPacket &p) const {
    if (fields.size() != p.fields.size())
        return false;
    for (unsigned int i = 0; i < fields.size(); ++i)
        if (!(fields[i] == p.fields[i]))
            return false;
    return true;
}

bool ConstraintPacket::contains(const ConstraintPacket &p) const {
    if (fields.size() != p.fields.size())
        return false;
    for (unsigned int i = 0; i < fields.size(); ++i)
        if (!fields[i].contains(p.fields[i]))
            return false;
    return true;
}

bool ConstraintPacket::isCombinable(const ConstraintPacket &p) const {
    // e.g. for two fields (field == p.field && secondField.isCombinable(p.secondField)) || (secondField == p.secondField && field.isCombinable(secondField));
    if (fields.size() != p.fields.size())
        return false;
    bool oneFieldCombinable = false;
    for (unsigned int i = 0; i < fields.size(); ++i) {
        if (!(fields[i] == p.fields[i]) && (oneFieldCombinable || !(oneFieldCombinable = fields[i].isCombinable(p.fields[i]))))
            return false;
    }
    return true;
}

ConstraintPacket ConstraintPacket::combine(const ConstraintPacket &p) const {
    // e.g. for two fields: return (field == p.field) ? {field, secondField.combine(p.secondField)} : {field.combine(p.field), secondField};
    ConstraintPacket retval(*this);
    for (unsigned int i = 0; i < fields.size(); ++i) {
        if (!(retval.fields[i] == p.fields[i])) {
            // combine
            retval.fields[i] = retval.fields[i].combine(p.fields[i]);
            break;
        }
    }
    return retval;
}


ConstraintPacket* ConstraintTypesExtension::addConstraintPacket(const ConstraintPacket &&p) {
    if (std::find(availablePackets.begin(), availablePackets.end(), p) == availablePackets.end()) {
        availablePackets.emplace_back(std::move(p));
        return &availablePackets.back();
    }
    return nullptr;
}

void ConstraintTypesExtension::simplify(const std::string &desc) {
    // simple version is to check if a Constraint packet is contained in another packet
    // e.g. [{r,b} {x,y}] [r x] -> [{r,b} {x,y}]
    // complexity: O(n^2)
    for (auto it = availablePackets.begin(); it != availablePackets.end();) {
        bool erase = false;
        for (auto it2 = availablePackets.begin(); it2 != availablePackets.end(); ++it2) {
            if (it != it2 && (*it2).contains(*it)) {
                std::cout << "erasing " << *it << " from " << desc << std::endl;
                erase = true;
                break;
            }
        }
        if (erase)
            it = availablePackets.erase(it);
        else
            ++it;
    }

    // extensive version is to combine to fields together if other fields are the same:
    // e.g. [{r} {x}] + [{b} {x}] -> [{r,b} {x}]
    // complexity: O(1/2 n^2)
    for (auto it = availablePackets.begin(); it != availablePackets.end();) {
        ConstraintPacket p;
        bool combine = false;
        for (auto it2 = it+1; it2 != availablePackets.end(); ++it2) {
            if ((*it).isCombinable(*it2)) {
                p = (*it).combine(*it2);
                std::cout << "combining " << *it << " with " << *it2 << " as " << p << " in " << desc << std::endl;
                it2 = availablePackets.erase(it2);
                combine = true;
                break;
            }
        }
        if (combine) {
            *it = p;
        } else
            ++it;
    }

    // depending on the level of aggressive over estimation, one can enable only strict combinations;
    // that is a combination where all the fields are the same except for one, which is combined.
    // another way is to aggressively combine [{r} {x}] + [{b} {y}] = [{r,b} {x,y}]

    // FIXME: if a field contains the whole alphabet of that field, just mark that field as don't care
    // e.g. ([!{a,b} x] or [{a,b} x]) -> [!{} x]

    // FIXME: if part of a Constraint packet is contained in another packet, remove it from current one
    // e.g. [{r,b} x] [r {x,y}] -> [{r,b} x] [r y]

    // note: probably more efficient to only check not-propgated Constraint packets
    // also keep the correct propagated information
}


void ConstraintSwitchingFunctionExtension::addConstraintPacket(const ConstraintPacket &p) {
    availablePackets.insert(p);
}
