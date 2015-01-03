#ifndef SYMBOLICVISITOR_H
#define SYMBOLICVISITOR_H

#include "symbolic.h"

class SymbolicIntervalField;
class SymbolicEnumField;
class SymbolicAnyField;

class SymbolicFieldVisitor {
public:
    virtual void nextField() = 0;
    virtual void visit(const String& field, const SymbolicIntervalField *) = 0;
    virtual void visit(const String& field, const SymbolicEnumField *) = 0;
    virtual void visit(const String& field, const SymbolicAnyField *) = 0;
};

#endif // SYMBOLICVISITOR_H
