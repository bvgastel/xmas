#include "symbolic.h"

#include "extension.h"
#include <set>
#include "memorypool.h"
#include <sstream>
#include <sys/time.h>
#include <functional>
#ifdef CONCURRENCY
#include "dispatch.h"
#include "lock.h"
#endif
#include "shared_object.h"
#include "symbolic-visitor.h"

class SymbolicForward : public XMASComponentVisitor {
public:
    Input *input;
    const SymbolicPacket &packet;

    SymbolicForward(Input *input, SymbolicPacket &packet) : input(input), packet(packet) {
    }

    virtual void visit(XMASSink *c) {
        bitpowder::lib::unused(c);
    }

    virtual void visit(XMASSource *c) {
        bitpowder::lib::unused(c);
    }

    virtual void visit(XMASQueue *c) {
        // FIXME: make the symbolic packet also a shared_ptr, so the map does not need to be copied over each time
        for (auto p : c->outputPorts())
            attachPackets(p, {packet});
    }

    virtual void visit(XMASFunction *c) {
        SymbolicFunctionExtension<std::vector<SymbolicPacket> (const std::vector<SymbolicPacket>&)> *ext = c->getComponentExtension<SymbolicFunctionExtension<std::vector<SymbolicPacket> (const std::vector<SymbolicPacket>&)>>(false);
        if (ext != nullptr || (bool)ext->function) {
            std::vector<SymbolicPacket> packets;
            packets.push_back(packet);
            auto result = ext->function(packets);
            for (auto p : c->outputPorts())
                attachPackets(p, result);
        }
    }

    virtual void visit(XMASSwitch *c) {
        {
            // output a
            SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
            //std::cout << "executing XMASSwitch with " << ext->availablePackets.size() << " sets" << std::endl;
            std::vector<SymbolicPacket> retval;
            for (auto &funcPacket : ext->availablePackets) {
                auto result = packet.getIntersection(funcPacket);
                retval.insert(retval.end(), result.begin(), result.end());
            }
            attachPackets(&c->a, std::move(retval));
        }
        {
            // output b
            SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
            std::vector<SymbolicPacket> retval;
            retval.push_back(packet);
            for (auto &funcPacket : ext->availablePackets) {
                std::vector<SymbolicPacket> newRetval;
                for (auto &r : retval) {
                    r.getDifference(funcPacket, [&newRetval,&funcPacket,this](SymbolicPacket &&p) {
                        //std::cout << "difference between " << packet << " and " << funcPacket << " -> " << p << std::endl;
                        newRetval.emplace_back(std::move(p));
                        //retval.push_back(p);
                    });
                }
                retval = std::move(newRetval);
            }
            attachPackets(&c->b, std::move(retval));
        }
    }

    virtual void visit(XMASFork *c) {
        // just propagate, type stays the same
        for (auto p : c->outputPorts())
            attachPackets(p, {packet});
    }

    virtual void visit(XMASMerge *c) {
        //std::cout << "propagate with merge: " << packet << std::endl;
        // just propagate, type stays the same
        for (auto p : c->outputPorts())
            attachPackets(p, {packet});
    }

    virtual void visit(XMASJoin *c) {
        // store both ports seperatly in the join, and combine this new packet with available packets from the other ports
        SymbolicFunctionExtension<SymbolicPacket (const SymbolicPacket&, const SymbolicPacket&)> *ext = c->getComponentExtension<SymbolicFunctionExtension<SymbolicPacket (const SymbolicPacket&, const SymbolicPacket&)>>(false);
        if (input == &c->a) {
            SymbolicTypesExtension *extB = c->b.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
            for (auto &packetB : extB->availablePackets) {
                if (ext) {
                    SymbolicPacket output = ext->function(packet, packetB);
                    for (auto p : c->outputPorts())
                        attachPackets(p, {output});
                } else {
                    using namespace std::placeholders;
                    std::map<SymbolicPacket::key_type,std::shared_ptr<SymbolicPacketField>> fields;
                    for (auto &value : packet.fields)
                        fields.insert(value);
                    for (auto &value : packetB.fields)
                        fields.insert(value);
                    for (auto p : c->outputPorts())
                        attachPackets(p, {fields});
                }
            }
        } else {
            SymbolicTypesExtension *extA = c->a.getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
            for (auto &packetA : extA->availablePackets) {
                if (ext) {
                    SymbolicPacket output = ext->function(packetA, packet);
                    for (auto p : c->outputPorts())
                        attachPackets(p, {output});
                } else {
                    std::map<SymbolicPacket::key_type,std::shared_ptr<SymbolicPacketField>> fields;
                    for (auto &value : packetA.fields)
                        fields.insert(value);
                    for (auto &value : packet.fields)
                        fields.insert(value);
                    for (auto p : c->outputPorts())
                        attachPackets(p, {fields});
                }
            }
        }
    }
};

/*
void propagate(Input *input, SymbolicPacket &packet) {
    packet.propagated = true;

    XMASComponent *c = input->getComponent();
    SymbolicTypesExtension *ext = getPortInfo<SymbolicTypesExtension>(input);
    std::ostringstream o;
    o << *c;
    ext->simplify(o.str());

    std::function<void(Input *, SymbolicPacket &)> callback(propagate);
    SymbolicForward sf(input, packet, &callback);
    c->accept(sf);
}
*/

void attach(Input *port, SymbolicPacket &&a) {
    //std::cout << "attach(Input): " << a << " valid=" << a.valid() << " port=" << *port << std::endl;
    if (!a.valid())
        return;
    if (!port)
        throw std::exception();
    SymbolicTypesExtension *ext = port->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    ext->addSymbolicPacket(std::move(a));
}

void removeTypes(Port *port) {
    port->getInitiatorPort()->clearPortExtension<SymbolicTypesExtension>();
}


void attachPackets(Input *port, std::vector<SymbolicPacket> &&a) {
    for (auto &p : a)
        attach(port, std::move(p));
}
void attachPackets(Output *port, const std::vector<SymbolicPacket> &a) {
    for (auto &p : a)
        attach(port, SymbolicPacket(p));
}

void attachPackets(const std::vector<Input *> &ports, const std::vector<SymbolicPacket> &p) {
    for (auto &port : ports)
        attachPackets(port, std::vector<SymbolicPacket>(p));
}

void attachPackets(Output *port, std::vector<SymbolicPacket> &&a) {
    attachPackets(port->getTargetPort(), std::move(a));
}

void attach(Output *port, SymbolicPacket &&a) {
    attach(port->getTargetPort(), std::move(a));
}

void attachPackets(const std::vector<Output *> &ports, const std::vector<SymbolicPacket> &p) {
    for (auto &port : ports)
        attachPackets(port, std::vector<SymbolicPacket>(p));
}


void attachSwitchingFunction(XMASSwitch *c, SymbolicPacket &&a)
{
    SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
    if (a.valid()) {
        //std::cout << "attaching switching function " << a << " to switch " << *c << std::endl;
        ext->addSymbolicPacket(std::move(a));
    }
}



void attachSwitchingFunction(XMASSwitch *c, const SymbolicPacket &a)
{
    SymbolicSwitchingFunctionExtension *ext = c->getComponentExtension<SymbolicSwitchingFunctionExtension>();
    if (a.valid()) {
        //std::cout << "attaching switching function " << a << " to switch " << *c << " with hash " << a.hash() << std::endl;
        ext->addSymbolicPacket(a);
    }
}

struct TypeWorkerItem {
    bitpowder::lib::concurrent_count refcount;
    bitpowder::lib::shared_object<TypeWorkerItem> next;
    XMASComponent *c;
    std::vector<std::pair<Input*, SymbolicTypesExtension*>> inputs;
    TypeWorker *worker;
    TypeWorkerItem(XMASComponent* c, TypeWorker *worker) : next(nullptr), c(c), inputs(), worker(worker) {
        for (Input *input : c->inputPorts()) {
            SymbolicTypesExtension *ext = input->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
            // to bootstrap
            std::swap(ext->availablePackets, ext->candidates);
            ext->availablePacketsHashes.clear();
            // remember me
            ext->workerItem = this;
            // ...
            inputs.push_back(std::make_pair(input,ext));
        }
    }
};

#ifdef CONCURRENCY
class TypeWorker : public DispatchQueueFor<shared_object<TypeWorkerItem>> {
    concurrent_count refcount;
    SpinLock lock;
    TypeWorker::DispatchObjectQueue nextRound;
    Semaphore sem;
public:
    TypeWorker(int max = std::thread::hardware_concurrency()) : DispatchQueueFor<shared_object<TypeWorkerItem>>("type-worker", max), lock(), nextRound(), sem(0) {
    }

    virtual void dispatchQueue(DispatchObjectQueue &queue, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) {
        for (const auto &it : queue)
            refcount++;
        DispatchQueueFor::dispatchQueue(queue, priority);
    }

    void executeOn(shared_object<TypeWorkerItem> item) {
        bool somethingHappend = false;
        //std::cout << "execute on " << item->c << std::endl;
        //fprintf(stderr, "execute\n");
        try {
            for (auto& i : item->inputs) {
                Input *input = i.first;
                SymbolicTypesExtension *ext = i.second;
                auto l = ext->lock.locker();
                std::vector<SymbolicPacket> candidates = std::move(ext->candidates);
                l.unlock();

                bool insertedSomething = false;
                for (SymbolicPacket &p : candidates) {
                    if (//ext->availablePacketsHashes.find(p.hash()) == ext->availablePacketsHashes.end() ||
                     std::find_if(ext->availablePackets.begin(), ext->availablePackets.end(), [&p](const SymbolicPacket &a) {
                                     return a == p || a.contains(p);
                }) == ext->availablePackets.end()) {
                        ext->availablePacketsHashes.insert(p.hash());
                        ext->availablePackets.push_back(std::move(p));
                        insertedSomething = true;
                    }
                }

                if (!insertedSomething)
                    continue;

                ext->simplify("", false);
                for (SymbolicPacket &packet : ext->availablePackets) {
                    if (packet.propagated)
                        continue;
                    packet.propagated = true;

                    SymbolicForward sf(input, packet);
                    item->c->accept(sf);

                    somethingHappend = true;
                }
            }
        } catch (Exception e) {
            //std::cerr << "offending component: " << *c << std::endl;
            std::cerr << e << std::endl;
            throw e;
        }

        auto l = lock.locker();
        // this way the longest running item is executed first
        nextRound.push_front(item);
        if (somethingHappend) {
            dispatchQueue(nextRound);
        }
        --refcount;
    }

    void abortedOn(shared_object<TypeWorkerItem> item, Exception &e) {
        --refcount;
    }

    void dispatchSelf(Dispatcher<DispatchOperation*> *t, Dispatch::Priority priority) {
        refcount++;
        DispatchQueueFor::dispatchSelf(t, priority);
    }

    void execute() {
        DispatchQueueFor::execute();
        if (--refcount == 0) {
            nextRound.clear();
            sem.signal();
        }
    }

    void wait() {
        sem.wait();
    }
};
#endif

bool iterativePropagate(XMASComponent *c) {
    bool somethingHappend = false;
    try {
        for (Input *input : c->inputPorts()) {
            SymbolicTypesExtension *ext = input->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
            if (std::all_of(ext->availablePackets.begin(), ext->availablePackets.end(), [](SymbolicPacket &packet){
                            return packet.propagated;
        }))
                continue;
            //std::ostringstream o;
            //o << *c;
            ext->simplify(/*o.str()*/"", false);
            for (SymbolicPacket &packet : ext->availablePackets) {
                if (packet.propagated)
                    continue;
                packet.propagated = true;

                SymbolicForward sf(input->getTargetPort(), packet);
                c->accept(sf);

                somethingHappend = true;
            }
        }
    } catch (bitpowder::lib::Exception e) {
        std::cerr << "offending component: " << *c << std::endl;
        std::cerr << e << std::endl;
        throw e;
    }
    return somethingHappend;
}

/*
void loop(XMASComponent *c, std::vector<XMASComponent *> &sortedComponents, std::set<XMASComponent *> &allComponents) {
    if (allComponents.find(c) == allComponents.end())
        return;
    allComponents.erase(c);
    sortedComponents.push_back(c);
    for (Port* i : c->ports(PortType::OUTPUT)) {
        loop(i->getComponent(), sortedComponents, allComponents);
    }
}
*/

void SymbolicTypes(std::set<XMASComponent *> allComponents) {
    /*
    // first sort so sources will be first to propagate
    struct timeval t;
    gettimeofday(&t, NULL);
    std::vector<XMASComponent *> sortedComponents;

    std::set<XMASComponent *> copy = allComponents;
    for (XMASComponent *c : allComponents) {
        loop(c, sortedComponents, copy);
    }
    struct timeval t2;
    gettimeofday(&t2, NULL);
    timersub(&t2,&t,&t);
    std::cout << t.tv_sec << "." << t.tv_usec << " duration" << std::endl;
*/
#ifdef CONCURRENCY
    if (std::thread::hardware_concurrency() <= 8) {
#endif
        bool somethingHappend;
        do {
            //std::cout << "starting round" << std::endl;
            somethingHappend = false;
            for (XMASComponent *c : allComponents) {
                somethingHappend |= iterativePropagate(c);
            }
        } while (somethingHappend);
#ifdef CONCURRENCY
    } else {
        // should be static/global, otherwise the main thread continues while threads are still running
        static TypeWorker worker;
        GlobalDispatcher::ensure(1, 1);

        TypeWorker::DispatchObjectQueue allItems;
        for (XMASComponent *c : allComponents) {
            TypeWorkerItem* work = new TypeWorkerItem(c, &worker);
            allItems.push_back(work);
        }
        //std::cout << "starting round" << std::endl;
        worker.dispatchQueue(allItems);
        worker.wait();
    }
#endif

    for (XMASComponent *c : allComponents) {
        for (Port *input : c->ports(PortType::INPUT_PORT)) {
            SymbolicTypesExtension *ext = input->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
            std::ostringstream o;
            o << *input;
            ext->simplify(o.str(), true);
        }
    }
}

void ClearSymbolicTypes(std::set<XMASComponent *> allComponents) {
    for (XMASComponent *c : allComponents) {
        for (Port *p : c->ports())
            p->clearPortExtension<SymbolicTypesExtension>();
        c->clearComponentExtension<SymbolicSwitchingFunctionExtension>();
    }
}

std::ostream &operator <<(std::ostream &out, const SymbolicPacket &c) {
    out << "{";
    for (auto it = c.fields.begin(); it != c.fields.end(); ++it) {
        if (it != c.fields.begin())
            out << " ";
        out << (*it).first << "=" << *(*it).second;
    }
    //out << " " << c._hash;
    out << "}";
    return out;
}

bool operator<(const SymbolicPacket &lhs, const SymbolicPacket &rhs) {
    return std::tie(lhs._hash, lhs.fields) < std::tie(rhs._hash, rhs.fields);
}

std::ostream &operator <<(std::ostream &out, const SymbolicPacketField &c) {
    c.print(out);
    return out;
}

void printSymbolicTypeInfo(std::ostream &out, Port *input)
{
    out << *input << ": ";
    SymbolicTypesExtension *ext = input->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>();
    if (ext) {
        for (SymbolicPacket &p : ext->availablePackets) {
            out << p << ", ";
        }
    }
}

void SymbolicPacket::setHash()
{
    _hash = 0;
    for (auto &f : fields) {
        _hash <<= 4;
        _hash += f.second->hash();
    }
}

std::vector<SymbolicPacket> SymbolicPacket::getIntersection(const SymbolicPacket &a) const {
    //std::cout << "getIntersection of " << *this << " with " << a << std::endl;
    if (std::any_of(a.fields.begin(), a.fields.end(), [this](const decltype(fields)::value_type& it) {
                    return fields.find(it.first) == fields.end();
})) {
        std::cerr << "getIntersection of " << *this << " with " << a << std::endl;
        throw bitpowder::lib::Exception("can not switch on fields that are possible non-existent");
    }
    std::vector<SymbolicPacket> retvals;
    retvals.push_back(SymbolicPacket());
    for (auto &it : fields) {
        if (retvals.empty())
            break;
        std::vector<std::shared_ptr<SymbolicPacketField>> newFields;
        auto lookup = a.fields.find(it.first);
        if (lookup != a.fields.end()) {
            newFields = it.second->getIntersection(lookup->second);
        } else
            newFields = {it.second};
        std::vector<SymbolicPacket> newRetvals;
        for (auto &retval : retvals) {
            //std::cout << "getIntersection: adjusting " << retval << std::endl;
            for (auto &newField : newFields) {
                if (!newField->valid())
                    continue;
                SymbolicPacket p(retval);
                p.fields.insert(std::make_pair(it.first, newField));
                if (p.valid())
                    newRetvals.push_back(p);
                //std::cout << "getIntersection: " << p << std::endl;
            }
        }
        retvals = std::move(newRetvals);
    }
    for (auto &sp : retvals)
        sp.setHash();
    return retvals;
}

void SymbolicPacket::getDifference(const SymbolicPacket &a, std::function<void(SymbolicPacket&&)> &&f) const {
    if (std::any_of(a.fields.begin(), a.fields.end(), [this](const decltype(fields)::value_type& it) {
                    return fields.find(it.first) == fields.end();
})) {
        std::cerr << "getDifference of " << *this << " with " << a << std::endl;
        throw bitpowder::lib::Exception("can not switch on fields that are possible non-existent");
    }
    // e.g. switch, input = [{a,b},{c,d}], switch function is [{a},{c}], result of _second_ output is [{b}, {c,d}] AND [{a,b},{d}]
    for (auto &it : a.fields) {
        auto &key = it.first;
        //std::cout << "difference for key " << key << std::endl;
        auto lookup = fields.find(key);
        lookup->second->getDifference(it.second, [this,&key,&f](std::shared_ptr<SymbolicPacketField> &&d) {
            SymbolicPacket packet(*this);
            packet.fields[key] = std::move(d);
            packet.setHash();
            if (packet.valid())
                f(std::move(packet));
            //std::cout << "difference result -> " << packet << std::endl;
        });
    }
}

std::vector<SymbolicPacket> SymbolicPacket::negate() const
{
    std::vector<SymbolicPacket> retval;
    retval.push_back(SymbolicPacket());
    for (auto& field : fields) {
        auto result = field.second->negate();

        auto retvalBefore = std::move(retval);
        for (auto& r : result) {
            for (auto& b : retvalBefore) {
                SymbolicPacket p(b);
                p.fields.insert(std::make_pair(field.first, r));
                retval.push_back(p);
            }
        }
    }
    return retval;
}

bool SymbolicPacket::operator==(const SymbolicPacket &p) const {
    //std::cout << *this << " == " << p << "?" << std::endl;
    if (this == &p)
        return true;
    if (_hash != p._hash || fields.size() != p.fields.size()) {
        return false;
    }
    return std::equal(fields.begin(), fields.end(), p.fields.begin());
}

template<class InputIt1, class InputIt2>
bool pair_contains(InputIt1 first1, InputIt1 last1,
                   InputIt2 first2)
{
    for (; first1 != last1; ++first1, ++first2) {
        auto& f1 = *first1;
        auto& f2 = *first2;
        if (f1.first != f2.first || !f1.second->contains(f2.second)) {
            return false;
        }
    }
    return true;
}

bool SymbolicPacket::contains(const SymbolicPacket &p) const {
    if (fields.size() != p.fields.size())
        return false;
    return pair_contains(fields.begin(), fields.end(), p.fields.begin());
}

template<class InputIt1, class InputIt2>
bool first_equal(InputIt1 first1, InputIt1 last1,
                 InputIt2 first2)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (first1->first != first2->first) {
            return false;
        }
    }
    return true;
}

bool SymbolicPacket::isCombinable(const SymbolicPacket &p) const {
    //std::cout << "isCombinable: " << *this << " and " << p << ": ";
    // e.g. for two fields (field == p.field && secondField.isCombinable(p.secondField)) || (secondField == p.secondField && field.isCombinable(secondField));
    if (fields.size() != p.fields.size()) {
        //std::cout << "other size in isCombinable" << std::endl;
        return false;
    }
    // should contain the same named fields
    if (!first_equal(fields.begin(), fields.end(), p.fields.begin())) {
        //std::cout << "other fields" << std::endl;
        return false;
    }
    bool oneFieldCombinable = false;
    auto it = fields.begin();
    auto it2 = p.fields.begin();
    for (; it != fields.end(); ++it, ++it2) {
        auto& thisField = it->second;
        auto& otherField = it2->second;
        if (*thisField != *otherField && (oneFieldCombinable || !(oneFieldCombinable = thisField->isCombinable(otherField)))) {
            //std::cout << "two fields are not equal: " << *it.second << " and " << *otherField << std::endl;
            return false;
        }
    }
    //std::cout << "ok" << std::endl;
    return true;
}

SymbolicPacket SymbolicPacket::combine(const SymbolicPacket &p) const {
    // e.g. for two fields: return (field == p.field) ? {field, secondField.combine(p.secondField)} : {field.combine(p.field), secondField};
    SymbolicPacket retval(*this);
    auto it = retval.fields.begin();
    auto it2 = p.fields.begin();
    for (; it != retval.fields.end(); ++it, ++it2) {
        auto& thisField = it->second;
        auto& otherField = it2->second;
        if (!(*thisField == *otherField)) {
            // combine
            thisField = thisField->combine(otherField);
            break;
        }
    }
    retval.setHash();
    return retval;
}

void SymbolicPacket::updateHash()
{
    for (auto &v : fields) {
        v.second->updateHash();
    }
    setHash();
}

void SymbolicPacket::accept(SymbolicFieldVisitor &visitor) const
{
    bool first = true;
    for (auto& field : fields) {
        if (!first)
            visitor.nextField();
        field.second->accept(field.first, visitor);
        first = false;
    }
}

void SymbolicPacket::printOldCSyntax(std::ostream& out, std::map<bitpowder::lib::String, int>& enumMap) const
{
    bool first = true;
    for (auto& field : fields) {
        if (!first)
            out << " && ";
        field.second->printOldCSyntax(out, enumMap, field.first);
        first = false;
    }
}


void SymbolicTypesExtension::addSymbolicPacket(SymbolicPacket &&p) {
    if (workerItem) {
        //std::cout << "multithreaded addSymbolicPacket" << std::endl;
#ifdef CONCURRENCY
        auto l = lock.locker();
#endif
        candidates.push_back(std::move(p));
        return;
    }
    //std::cout << "singlethreaded addSymbolicPacket" << std::endl;
    //std::cout << "trying to insert packet with hash " << p.hash() << ": " << p << std::endl;
    if (/*availablePacketsHashes.find(p.hash()) == availablePacketsHashes.end() ||*/ std::find_if(availablePackets.begin(), availablePackets.end(), [&p](const SymbolicPacket &a) {
                                                                                                  return a == p || a.contains(p);
}) == availablePackets.end()) {
        availablePacketsHashes.insert(p.hash());
        availablePackets.push_back(std::move(p));
    }
    //std::cout << "already in here, ignoring" << std::endl;
}

void SymbolicTypesExtension::simplify(const std::string &desc, bool full) {
    bitpowder::lib::unused(desc);
    //std::cout << "simplify " << desc << std::endl;
    // simple version is to check if a symbolic packet is contained in another packet
    // e.g. [{r,b} {x,y}] [r x] -> [{r,b} {x,y}]
    // complexity: O(n^2)
    //if (full)
    for (auto it = availablePackets.begin(); it != availablePackets.end();) {
        bool erase = false;
        //std::cout << "check if " << *it << " is contained in some other packet" << std::endl;
        for (auto it2 = availablePackets.begin(); it2 != availablePackets.end(); ++it2) {
            if (it != it2 && (*it2).contains(*it)) {
                //std::cout << "erasing " << *it << " from " << desc << " because it is contained in " << *it2 << std::endl;
                erase = true;
                break;
            }
        }
        if (erase) {
            it = availablePackets.erase(it);
        }
        else
            ++it;
    }

    // extensive version is to combine to fields together if other fields are the same:
    // e.g. [{r} {x}] + [{b} {x}] -> [{r,b} {x}]
    // complexity: O(1/2 n^2)
    bool combinedSomething;
    do {
        combinedSomething = false;
        for (auto it = availablePackets.begin(); it != availablePackets.end();) {
            SymbolicPacket p;
            bool combine = false;
            for (auto it2 = it+1; it2 != availablePackets.end(); ++it2) {
                //if (full) std::cout << "check combining " << *it << " with " << *it2 << " in " << desc << std::endl;
                if ((*it).isCombinable(*it2)) {
                    p = (*it).combine(*it2);
                    //if (full) std::cout << "combining " << *it << " with " << *it2 << " as " << p << " in " << desc << std::endl;
                    it2 = availablePackets.erase(it2);
                    combinedSomething = combine = true;
                    break;
                }
            }
            if (combine) {
                *it = p;
            } else
                ++it;
        }
    } while (combinedSomething);
    // depending on the level of aggressive over estimation, one can enable only strict combinations;
    // that is a combination where all the fields are the same except for one, which is combined.
    // another way is to aggressively combine [{r} {x}] + [{b} {y}] = [{r,b} {x,y}]

    // FIXME: if a field contains the whole alphabet of that field, just mark that field as don't care
    // e.g. ([!{a,b} x] or [{a,b} x]) -> [!{} x]

    // if part of a symbolic packet is contained in another packet, remove it from current one
    // e.g. [{r,b} x] [r {x,y}] -> [{r,b} x] [r y]
    // complexity: O(1/2 n^2)
    if (full && false)
        for (auto it = availablePackets.begin(); it != availablePackets.end(); ++it) {
            for (auto it2 = it+1; it2 != availablePackets.end(); ++it2) {
                if (std::all_of(it2->fields.begin(), it2->fields.end(), [it](const decltype(it->fields)::value_type& i) {
                                return it->fields.find(i.first) != it->fields.end();
            })) {
                    std::vector<SymbolicPacket> intersections = (*it).getIntersection(*it2);
                    for (auto &intersection : intersections) {
                        if (intersection.valid()) {
                            bool replaced = false;
                            // FIXME: by using std::shared_ptr as wrapper around SymbolicPacket, we can avoid having these copy costs
                            auto copy = *it2;
                            copy.getDifference(intersection, [&it2,&replaced,this](SymbolicPacket &&p){
                                if (!replaced) {
                                    *it2 = std::move(p);
                                    replaced = true;
                                    return;
                                }
                                availablePackets.push_back(std::move(p));
                            });
                        }
                    }
                }
            }
        }

    // note: probably more efficient to only check not-propgated symbolic packets
    // also keep the correct propagated information
}


void SymbolicSwitchingFunctionExtension::addSymbolicPacket(const SymbolicPacket &p) {
    availablePackets.insert(p);
}


bool operator==(const std::shared_ptr<SymbolicPacketField> &lhs, const std::shared_ptr<SymbolicPacketField> &rhs)
{
    return *lhs == *rhs;
}



// ANY PACKET
std::vector<std::shared_ptr<SymbolicPacketField> > SymbolicAnyField::getIntersection(const std::shared_ptr<SymbolicPacketField> &a) const {
    return {a};
}

void SymbolicAnyField::getDifference(const std::shared_ptr<SymbolicPacketField> &a, std::function<void (std::shared_ptr<SymbolicPacketField> &&)> &&f) const
{
    bitpowder::lib::unused(a, f);
}

bool SymbolicAnyField::operator==(const SymbolicPacketField &sb) const
{
    const SymbolicAnyField *b = bitpowder::lib::FastType<SymbolicAnyField>::cast(&sb);
    return b;
}

bool SymbolicAnyField::valid() const
{
    return true;
}

bool SymbolicAnyField::empty() const
{
    return false;
}

bool SymbolicAnyField::contains(const std::shared_ptr<SymbolicPacketField> &f) const
{
    bitpowder::lib::unused(f);
    return false;
}

bool SymbolicAnyField::isCombinable(const std::shared_ptr<SymbolicPacketField> &f) const
{
    bitpowder::lib::unused(f);
    return false;
}

std::shared_ptr<SymbolicPacketField> SymbolicAnyField::combine(const std::shared_ptr<SymbolicPacketField> &f) const
{
    bitpowder::lib::unused(f);
    return nullptr;
}

void SymbolicAnyField::print(std::ostream &out) const
{
    out << "*";
}

int SymbolicAnyField::hash() const
{
    return 1;
}

void SymbolicAnyField::updateHash()
{
    // nop, intentionally left blank
}

std::vector<std::shared_ptr<SymbolicPacketField>> SymbolicAnyField::negate() const
{
    return {};
}

void SymbolicAnyField::accept(const bitpowder::lib::String &field, SymbolicFieldVisitor &v) {
    v.visit(field, this);
}

void SymbolicAnyField::printOldCSyntax(std::ostream& out,
                                       std::map<bitpowder::lib::String, int>& enumMap,
                                       const bitpowder::lib::String& key) const
{
    unused(enumMap, key);
    out << "true";
}
