#ifndef COMPONENTWALKER_H
#define COMPONENTWALKER_H

#include "xmas.h"

class ComponentWalker : public XMASComponentVisitor
{
public:
    ComponentWalker();
    ~ComponentWalker();

    virtual void visit(XMASSink *);
    virtual void visit(XMASSource *);
    virtual void visit(XMASQueue *);
    virtual void visit(XMASFunction *);
    virtual void visit(XMASSwitch *);
    virtual void visit(XMASFork *);
    virtual void visit(XMASMerge *);
    virtual void visit(XMASJoin *);

private:
};

#endif // COMPONENTWALKER_H
