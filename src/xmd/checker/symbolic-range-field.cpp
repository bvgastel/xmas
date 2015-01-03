#include "symbolic-range-field.h"

#include <sstream>
#include "lib/exception.h"

using namespace bitpowder::lib;

std::vector<std::shared_ptr<SymbolicPacketField>> SymbolicRangeField::getIntersection(const std::shared_ptr<SymbolicPacketField> &sa) const {
    //SymbolicRangeField *a = dynamic_cast<SymbolicRangeField*>(sa.get());
    SymbolicRangeField *a = (SymbolicRangeField*)sa.get();
    if (!a)
        return {};
    std::vector<std::shared_ptr<SymbolicPacketField>> retvals;
    if (!complement && !a->complement) {
        // e.g. {1..4} intersection {2..5} -> {2..4}
        if (std::max(min, a->min) < std::min(max, a->max))
            retvals.push_back(RANGE(std::max(min, a->min), std::min(max, a->max)));
    } else if (complement && a->complement) {
        // e.g. !{1..4} intersection ![2..5] -> !{1..5}
        if (std::min(min, a->min) < std::max(max, a->max))
            retvals.push_back(RANGE(std::min(min, a->min), std::max(max, a->max), true));
    } else if (!complement && a->complement) {
        // e.g. {1..6} intersection !{3..4} -> {1..2} {5..6}
        if (min < std::min(max, a->min))
            retvals.push_back(RANGE(min, std::min(max, a->min)));
        if (std::max(min, a->max) < max)
            retvals.push_back(RANGE(std::max(min, a->max), max));

    } else if (complement && !a->complement) {
        // e.g. !{3..4} intersection {1..6} -> {1..2} {5..6}
        if (a->min < std::min(a->max, min))
            retvals.push_back(RANGE(a->min, std::min(a->max, min)));

        if (std::max(a->min, max) < a->max)
            retvals.push_back(RANGE(std::max(a->min, max), a->max));
    }
    /*
    std::cout << "intersection between ";
    print(std::cout);
    std::cout << " and ";
    a->print(std::cout);
    std::cout << " -> " << retvals.size() << "=" << retvals << std::endl;
    */
    return retvals;
}

void SymbolicRangeField::getDifference(const std::shared_ptr<SymbolicPacketField> &sa, std::function<void (std::shared_ptr<SymbolicPacketField>&&)> &&f) const {
    //SymbolicRangeField *a = dynamic_cast<SymbolicRangeField*>(sa.get());
    SymbolicRangeField *a = (SymbolicRangeField*)sa.get();
    if (!a)
            return;
    if (!complement && !a->complement) {
        // e.g. {a,b} diff {b,c} -> {a}
        if (min < std::min(max, a->min))
            f(RANGE(min, std::min(max, a->min)));

        if (std::max(min, a->max) < max)
            f(RANGE(std::max(min, a->max), max));
    } else if (complement && a->complement) {
        // e.g. !{a,b} diff !{b,c} -> {c}
        if (a->min < std::min(a->max, min))
            f(RANGE(a->min, std::min(a->max, min)));

        if (std::max(a->min, max) < a->max)
            f(RANGE(std::max(a->min, max), a->max));
    } else if (!complement && a->complement) {
        // e.g. {a,b} diff !{b,c} -> {b}
        if (std::max(min, a->min) < std::min(max, a->max))
            f(RANGE(std::max(min, a->min), std::min(max, a->max)));
    } else if (complement && !a->complement) {
        // e.g. !{a,b} diff {b,c} -> !{a,b,c}
        // e.g. !{a,b} diff {d,e} -> !{a,b,d,e}
        // difference between !{0..1} and {3..3} -> !{0..3} and {2..2}
        // !{2..2} and ({1..1} {3..3}) should be !{1..3}
        if (std::min(min, a->min) < std::max(max, a->max))
            f(RANGE(std::min(min, a->min), std::max(max, a->max), true));
        if (max < a->min)
            f(RANGE(max, a->min));
        if (a->max < min)
            f(RANGE(a->max, min));
    }
    //std::cout << "difference between " << *this << " and " << a << " -> " << retval << std::endl;
}

bool SymbolicRangeField::hasValue(int value) const
{
    //return (min <= value && value < max && !complement) || ((value < min || max <= value) && complement);
    return (min <= value && value < max) == !complement;
}

bool SymbolicRangeField::contains(const std::shared_ptr<SymbolicPacketField> &sf) const {
    //SymbolicRangeField *f = dynamic_cast<SymbolicRangeField*>(sf.get());
    SymbolicRangeField *f = (SymbolicRangeField*)sf.get();
    if (!f)
            return nullptr;
    bool retval = false;
    if (!complement && f->complement) {
        // e.g. {1..2} does not contains !{3..5}, because the second argument could also contain x (e.g. value 6)
        // because you don't know the whole alphabet, this is unknown per definition
        retval = false;
    } else if (complement && !f->complement) {
        // e.g. !{1..3} contains {4..6}
        // if elements of the second argument are not found in the first argument, it is true
        retval = f->min > max || f->max <= min;
    } else if (complement && f->complement) {
        // e.g. !{1..3} contains !{0..4}
        // all elements of the first argument should be contained in the second argument
        retval = f->min <= min && max <= f->max;
    } else if (!complement && !f->complement) {
        // e.g. {1..3} contains {1..2}
        retval = min <= f->min && f->max <= max;
    }
    //print(std::cout);
    //std::cout << " contains " << f << " -> " << retval << std::endl;
    return retval;
}

std::shared_ptr<SymbolicPacketField> SymbolicRangeField::combine(const std::shared_ptr<SymbolicPacketField> &sf) const
{
    //SymbolicRangeField *f = dynamic_cast<SymbolicRangeField*>(sf.get());
    SymbolicRangeField *f = (SymbolicRangeField*)sf.get();
    if (!f)
            return nullptr;
    int retvalMin;
    int retvalMax;
    if (!complement && !f->complement) {
        retvalMin = std::min(min, f->min); // union
        retvalMax = std::max(max, f->max);
    } else if (complement && f->complement) {
        retvalMin = std::max(min, f->min); // intersection
        retvalMax = std::min(max, f->max);
    } else {
        throw Exception("combine a complement packet with a non-complement packet is not possible");
    }
    //std::cout << "combine of " << *this << std::endl;
    return RANGE(retvalMin,retvalMax, complement);
}

void SymbolicRangeField::print(std::ostream &out) const
{
    if (complement)
        out << "!";
    out << "{" << min << ".." << (max-1);
    //out << " " << hash();
    out << "}";
}

std::vector<std::shared_ptr<SymbolicPacketField> > SymbolicRangeField::negate() const
{
    return {std::make_shared<SymbolicRangeField>(min, max, !complement)};
}

void SymbolicRangeField::updateHash()
{
    // nop, intentionally left blank
}
