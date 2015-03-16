#include "symbolic-interval-field.h"

#include <sstream>
#include "exception.h"

using namespace bitpowder::lib;

std::vector<std::shared_ptr<SymbolicPacketField>> SymbolicIntervalField::getIntersection(const std::shared_ptr<SymbolicPacketField> &sa) const {
    SymbolicIntervalField *a = FastType<SymbolicIntervalField>::cast(sa.get());
    if (!a)
        return {};
    std::vector<std::shared_ptr<SymbolicPacketField>> retvals;
    // e.g. {1..4} intersection {2..5} -> {2..4}
    if (std::max(min, a->min) < std::min(max, a->max))
       retvals.push_back(INTERVAL(std::max(min, a->min), std::min(max, a->max)));
#ifdef DEBUG
    std::cout << "intersection between ";
    print(std::cout);
    std::cout << " and ";
    a->print(std::cout);
    std::cout << " -> " << retvals.size() << "=" << retvals << std::endl;
#endif
    return retvals;
}

void SymbolicIntervalField::getDifference(const std::shared_ptr<SymbolicPacketField> &sa, std::function<void (std::shared_ptr<SymbolicPacketField>&&)> &&f) const {
    SymbolicIntervalField *a = FastType<SymbolicIntervalField>::cast(sa.get());
    if (!a)
            return;
    // e.g. {a,b} diff {b,c} -> {a}
    if (min < std::min(max, a->min))
        f(INTERVAL(min, std::min(max, a->min)));

    if (std::max(min, a->max) < max)
        f(INTERVAL(std::max(min, a->max), max));

#ifdef DEBUG
    std::cout << "difference between " << *this << " and " << *a << std::endl;
#endif
}

bool SymbolicIntervalField::hasValue(interval_type value) const
{
    return (min <= value && value < max);
}

bool SymbolicIntervalField::contains(const std::shared_ptr<SymbolicPacketField> &sf) const {
    SymbolicIntervalField *f = FastType<SymbolicIntervalField>::cast(sf.get());
    if (!f)
            return nullptr;
    // e.g. {1..3} contains {1..2}
    bool retval = min <= f->min && f->max <= max;
#ifdef DEBUG
    print(std::cout);
    std::cout << " contains " << f << " -> " << retval << std::endl;
#endif
    return retval;
}

std::shared_ptr<SymbolicPacketField> SymbolicIntervalField::combine(const std::shared_ptr<SymbolicPacketField> &sf) const
{
    SymbolicIntervalField *f = FastType<SymbolicIntervalField>::cast(sf.get());
    if (!f)
            return nullptr;
    interval_type retvalMin = std::min(min, f->min); // union
    interval_type retvalMax = std::max(max, f->max);
#ifdef DEBUG
    std::cout << "combine of " << *this << std::endl;
#endif
    return INTERVAL(retvalMin, retvalMax);
}

void SymbolicIntervalField::print(std::ostream &out) const
{
    if (min == max-1) {
        out << min;
    } else {
        out << "[" << min << ".." << (max-1);
        //out << " " << hash();
        out << "]";
    }
}

void SymbolicIntervalField::updateHash()
{
    // nop, intentionally left blank
}

std::vector<std::shared_ptr<SymbolicPacketField>> SymbolicIntervalField::negate() const
{
    std::vector<std::shared_ptr<SymbolicPacketField>> retval;
    if (min > 0)
        retval.push_back(std::make_shared<SymbolicIntervalField>(0, min));
    if (max < std::numeric_limits<interval_type>::max())
        retval.push_back(std::make_shared<SymbolicIntervalField>(max, std::numeric_limits<interval_type>::max()));
    return retval;
}

void SymbolicIntervalField::printOldCSyntax(std::ostream& out, std::map<String, int>& enumMap, const String& key) const
{
    bitpowder::lib::unused(enumMap);
    out << key << " >= " << min << " && " << key << " < " << max;
}

std::ostream &operator <<(std::ostream &out, const SymbolicIntervalField &c)
{
    out << "[" << c.min << ".." << (c.max-1) << "]";
    return out;
}
