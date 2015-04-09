#include "cycle.h"
#include "memorypool.h"

#include "feedback-interface.h"

constexpr auto CYCLE_CHECKER = "cycle-checker";

class CombinatorialCycleDependencies : public XMASComponentVisitor {
public:
    Port *p;
    std::vector<Port*> retval;

    CombinatorialCycleDependencies(Port *p) : p(p), retval() {
    }

    static Port *trdy(Port &p) {
        return p.getTargetPort();
    }
    static Port *irdy(Port &p) {
        return p.getInitiatorPort();
    }

    virtual void visit(XMASSink *c) {
        bitpowder::lib::unused(c);
    }

    virtual void visit(XMASSource *c) {
        bitpowder::lib::unused(c);
    }

    virtual void visit(XMASQueue *c) {
        bitpowder::lib::unused(c);
    }

    virtual void visit(XMASFunction *c) {
        if (trdy(c->i) == p) { // i.trdy = o.trdy
            retval = {trdy(c->o)};
        } else if (irdy(c->o) == p) { // o.irdy = i.irdy
            retval = {irdy(c->i)};
        } else
            throw bitpowder::lib::Exception("Function, unknown signal dependency port");
    }

    virtual void visit(XMASSwitch *c) {
        if (trdy(c->i) == p) { // i.trdy = (a.irdy and a.trdy) or (b.irdy and b.trdy)
            retval = {irdy(c->a), trdy(c->a), irdy(c->b), trdy(c->b)};
        } else if (irdy(c->a) == p || irdy(c->b) == p) { // {a,b}.irdy = i.irdy
            retval = {irdy(c->i)};
        } else
            throw bitpowder::lib::Exception("Switch, unknown signal dependency port");
    }

    virtual void visit(XMASFork *c) {
        if (trdy(c->i) == p) { // i.trdy = a.trdy and b.trdy
            retval = {trdy(c->a), trdy(c->b)};
        } else if (irdy(c->a) == p) { // a.irdy = i.irdy and b.trdy
            retval = {irdy(c->i), trdy(c->b)};
        } else if (irdy(c->b) == p) { // b.irdy = i.irdy and a.trdy
            retval = {irdy(c->i), trdy(c->a)};
        } else
            throw bitpowder::lib::Exception("Fork, unknown signal dependency port");
    }

    virtual void visit(XMASMerge *c) {
        if (irdy(c->o) == p) { // o.irdy = a.irdy or b.irdy
            retval = {irdy(c->a), irdy(c->b)};
        } else if (trdy(c->a) == p) { // a.trdy = o.trdy and a.irdy
            retval = {trdy(c->o), irdy(c->a)};
        } else if (trdy(c->b) == p) { // b.trdy = o.trdy and b.irdy
            retval = {trdy(c->o), irdy(c->b)};
        } else
            throw bitpowder::lib::Exception("Merge, unknown signal dependency port");
    }

    virtual void visit(XMASJoin *c) {
        if (irdy(c->o) == p) { // o.irdy = a.irdy and b.irdy
            retval = {irdy(c->a), irdy(c->b)};
        } else if (trdy(c->a) == p) { // a.trdy = o.trdy and b.irdy
            retval = {trdy(c->o), irdy(c->b)};
        } else if (trdy(c->b) == p) { // b.trdy = o.trdy and a.irdy
            retval = {trdy(c->o), irdy(c->a)};
        } else
            throw bitpowder::lib::Exception("Join, unknown signal dependency port");
    }
};

// if this extension is associated with an input, it is for the irdy signals
// if this extension is associated with an output, it is for the trdy signals
class CombinatorialCyclePortExtension : public PortExtension {
public:
    bool checking;
    bool checked;
    CombinatorialCyclePortExtension() : checking(false), checked(false) {
    }
};

bool checkSignal(Port *p, int depth) {
    CombinatorialCyclePortExtension *ext = p->getPortExtension<CombinatorialCyclePortExtension>();
    //std::cout << "checkSignal: depth=" << depth << " " << *p->getComponent() << "." << p->getName() << "." << (trdy ? "trdy" : "irdy") << " -> checking=" << ext->checking << " checked=" << ext->checked << std::endl;
    // if part of current search, report cycle
    if (ext->checking)
        return true;
    // if already checked this, stop
    if (ext->checked)
        return false;
    ext->checking = true;

    CombinatorialCycleDependencies dependenciesVisitor(p);
    p->getComponent()->accept(dependenciesVisitor);
    for (Port *next : dependenciesVisitor.retval) {
        if (checkSignal(next, depth+1))
            return true;
    }

    ext->checking = false;
    ext->checked = true;
    return false;
}

bool CombinatorialCycleDetector(XMASComponent *c) {
    try {
        for (Port *p : c->ports()) {
            //std::cout << " --- irdy" << std::endl;
            if (checkSignal(CombinatorialCycleDependencies::irdy(*p), 0))
                return true;
            //std::cout << " --- trdy" << std::endl;
            if (checkSignal(CombinatorialCycleDependencies::trdy(*p), 0))
                return true;
        }
    } catch (bitpowder::lib::Exception &e) {
        std::cout << "exception: " << e << std::endl;
    }
    return false;
}

bool CombinatorialCycleDetector(std::set<XMASComponent *> allComponents) {
    // default no cycle
    bool cycle = false;

    feedback_message(CYCLE_CHECKER, FeedbackSeverity::Info, "Starting cycle checker");

    // check for each component if there is a cycle
    for (XMASComponent *c : allComponents)
        cycle = cycle || CombinatorialCycleDetector(c);
    // clean up memory allocated for the combi cycle checker
    for (XMASComponent *c : allComponents)
        for (Port *p : c->ports())
            p->clearPortExtension<CombinatorialCyclePortExtension>();
    // return result

    feedback_message(CYCLE_CHECKER, FeedbackSeverity::Info, "Finished!");

    return cycle;
}
