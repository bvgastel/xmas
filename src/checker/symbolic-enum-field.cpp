#include "symbolic-enum-field.h"

#include <sstream>
#include "lib/exception.h"

using namespace bitpowder::lib;

void SymbolicEnumField::updateHash() {
    _hash = values.size();
    for (auto &v : values) {
        _hash <<= 2;
        _hash ^= String(v).hash();
    }
}

std::vector<std::shared_ptr<SymbolicPacketField>> SymbolicEnumField::getIntersection(const std::shared_ptr<SymbolicPacketField> &sa) const {
    SymbolicEnumField *a = FastType<SymbolicEnumField>::cast(sa.get());
    if (!a)
            return {};
    std::shared_ptr<SymbolicEnumField> retval(new SymbolicEnumField());
    if (!complement && !a->complement) {
        // e.g. {a,b} intersection {b,c} -> {b}
        set_intersection(values.begin(), values.end(),
                         a->values.begin(), a->values.end(),
                         std::inserter(retval->values, retval->values.begin()));
    } else if (complement && a->complement) {
        // e.g. !{a,b} intersection !{b,c} -> !{a,b,c}
        set_union(values.begin(), values.end(),
                  a->values.begin(), a->values.end(),
                  std::inserter(retval->values, retval->values.begin()));
        retval->complement = true;
    } else if (!complement && a->complement) {
        // e.g. {a,b} intersection !{b,c} -> {a}
        set_difference(values.begin(), values.end(),
                       a->values.begin(), a->values.end(),
                       std::inserter(retval->values, retval->values.begin()));
    } else if (complement && !a->complement) {
        // e.g. !{a,b} intersection {b,c} -> {c}
        set_difference(a->values.begin(), a->values.end(),
                       values.begin(), values.end(),
                       std::inserter(retval->values, retval->values.begin()));
    }
    retval->updateHash();
    //std::cout << "intersection between " << *this << " and " << a << " -> " << retval << std::endl;
    return {retval};
}

void SymbolicEnumField::getDifference(const std::shared_ptr<SymbolicPacketField> &sa, std::function<void (std::shared_ptr<SymbolicPacketField>&&)> &&f) const {
    SymbolicEnumField *a = FastType<SymbolicEnumField>::cast(sa.get());
    if (!a)
        return;
    std::shared_ptr<SymbolicEnumField> retval(new SymbolicEnumField());
    if (!complement && !a->complement) {
        // e.g. {a,b} diff {b,c} -> {a}
        set_difference(values.begin(), values.end(),
                       a->values.begin(), a->values.end(),
                       std::inserter(retval->values, retval->values.begin()));
    } else if (complement && a->complement) {
        // e.g. !{a,b} diff !{b,c} -> {c}
        set_difference(a->values.begin(), a->values.end(),
                       values.begin(), values.end(),
                       std::inserter(retval->values, retval->values.begin()));
    } else if (!complement && a->complement) {
        // e.g. {a,b} diff !{b,c} -> {b}
        set_intersection(values.begin(), values.end(),
                       a->values.begin(), a->values.end(),
                       std::inserter(retval->values, retval->values.begin()));
    } else if (complement && !a->complement) {
        // e.g. !{a,b} diff {b,c} -> !{a,b,c}
        set_union(values.begin(), values.end(),
                  a->values.begin(), a->values.end(),
                  std::inserter(retval->values, retval->values.begin()));
        ((SymbolicEnumField*)retval.get())->complement = true;
    }
    retval->updateHash();
    //std::cout << "difference between " << *this << " and " << a << " -> " << retval << std::endl;
    f(std::move(retval));
}

bool SymbolicEnumField::hasValue(const SymbolicEnumField::Type &value) const
{
    // FIXME: this (contains) could be faster for large sets, by using the sorting of the values, so iterate one, and iterate the other if needed. Make sure parse.cpp also sort all the elements after inserting nodes
    // Also, if the remaining values in the lookup field are smaller as the items to be looked up, cancel the contains
    if (complement)
        return std::find(values.begin(), values.end(), value) == values.end();
    return std::find(values.begin(), values.end(), value) != values.end();
}

bool SymbolicEnumField::contains(const std::shared_ptr<SymbolicPacketField> &sf) const {
    SymbolicEnumField *f = FastType<SymbolicEnumField>::cast(sf.get());
    if (!f)
            return nullptr;
    bool retval = false;
    if (!complement && f->complement) {
        // e.g. {a,b} does not contains !{c,d,e}, because the second argument could also contain x (e.g. value f)
        // because you don't know the whole alphabet, this is unknown per definition
        retval = false;
    } else if (complement && !f->complement) {
        // e.g. !{a,b} contains {c,d}
        // if elements of the second argument are not found in the first argument, it is true
        retval = std::all_of(f->values.begin(), f->values.end(), [this](const Type &e) {
            return hasValue(e); //std::find(values.begin(), values.end(), e) == values.end();
        });
    } else if (complement && f->complement) {
        // e.g. !{a,b} contains !{a,b,c}
        // all elements of the first argument should be contained in the second argument
        retval = std::all_of(values.begin(), values.end(), [&f](const Type &e) {
            return !f->hasValue(e); //std::find(f.values.begin(), f.values.end(), e) != f.values.end();
        });
    } else if (!complement && !f->complement) {
        // e.g. {a,b} contains {a}
        retval = std::all_of(f->values.begin(), f->values.end(), [this](const Type &e) {
            return hasValue(e); //std::find(values.begin(), values.end(), e) != values.end();
        });
    }
    /*
    if (f.complement) {
        retval = std::all_of(f.values.begin(), f.values.end(), [this](const std::string &e) {
            return hasNotValue(e);
        });
    } else {
        retval = std::all_of(f.values.begin(), f.values.end(), [this](const std::string &e) {
            return hasValue(e);
        });
    }
    */
    //std::cout << *this << " contains " << f << " -> " << retval << std::endl;
    return retval;
}

std::shared_ptr<SymbolicPacketField> SymbolicEnumField::combine(const std::shared_ptr<SymbolicPacketField> &sf) const
{
    SymbolicEnumField *f = FastType<SymbolicEnumField>::cast(sf.get());
    if (!f)
            return nullptr;
        std::shared_ptr<SymbolicEnumField> retval(new SymbolicEnumField());
    if (!complement && !f->complement) {
        set_union(values.begin(), values.end(),
                  f->values.begin(), f->values.end(),
                  std::inserter(retval->values, retval->values.begin()));
    } else if (complement && f->complement) {
        set_intersection(values.begin(), values.end(),
                  f->values.begin(), f->values.end(),
                  std::inserter(retval->values, retval->values.begin()));
    } else {
        throw Exception("combine a complement packet with a non-complement packet is not possible");
    }
    //std::cout << "combine of ";
    //print(std::cout);
    //std::cout << std::endl;
    retval->updateHash();
    return retval;
}

void SymbolicEnumField::print(std::ostream &out) const
{
    if (complement)
        out << "!";
    out << "{";
    bool first = true;
    for (const auto &str : values) {
        if (!first)
            out << ",";
        out << str;
        first = false;
    }
    //out << " " << _hash;
    out << "}";
}

std::vector<std::shared_ptr<SymbolicPacketField>> SymbolicEnumField::negate() const
{
    return {std::make_shared<SymbolicEnumField>(values, !complement)};
}

void SymbolicEnumField::printOldCSyntax(std::ostream& out, std::map<String, int>& enumMap, const String& key) const
{
    if (complement)
        out << "!";
    out << "(";
        bool first = true;
    for (const auto &str : values) {
        if (!first)
            out << " || ";
        auto it = enumMap.find(str);
        if (it == enumMap.end())
            std::tie(it, std::ignore) = enumMap.insert(std::make_pair(str, enumMap.size()));
        out << key << " == " << it->second;
        first = false;
    }

    out << ")";
}

