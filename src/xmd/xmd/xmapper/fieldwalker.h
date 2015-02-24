#ifndef FIELDWALKER_H
#define FIELDWALKER_H

#include <sstream>

#include "simplestring.h"
#include "symbolic.h"
#include "symbolic-visitor.h"
#include "symbolic-interval-field.h"
#include "symbolic-enum-field.h"


class FieldWalker : public SymbolicFieldVisitor {
public:
    std::ostringstream& out;
    std::map<bitpowder::lib::String,int>& enumMap;

    FieldWalker(std::ostringstream& out, std::map<bitpowder::lib::String,int>& enumMap)
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
        for (const bitpowder::lib::String& v : f->values) {
            if (!first)
                out << " || ";
            auto it = enumMap.find(v);
            if (it == enumMap.end())
                std::tie(it, std::ignore) = enumMap.insert(std::make_pair(v, enumMap.size()));
            out << "p_" << field << " == " << it->second;
            first = false;
        }
    }

    virtual void visit(const bitpowder::lib::String& field, const SymbolicAnyField *f) {
        unused(field, f);
    }
};


#endif // FIELDWALKER_H
