#include "deadlock.h"
#include "lib/shared_object.h"

#define USE_REFCOUNT

struct BIFormula {
    virtual ~BIFormula() {
    }
#ifdef USE_REFCOUNT
    int refcount;
    typedef bitpowder::lib::shared_object<BIFormula> Ref;
    BIFormula() : refcount(0) {
    }
    template <class T, class... Args>
    static Ref make_shared(Args &&... args) {
        return Ref(new T(std::forward<Args>(args)...));
    }
#else
    typedef std::shared_ptr<BIFormula> Ref;
    template <class T, class... Args>
    static Ref make_shared(Args &&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
#endif
};

struct BIFormulaNot : public BIFormula {
    BIFormula::Ref operand;
    BIFormulaNot(const BIFormula::Ref& operand) : operand(operand) {
    }
};

struct BIFormulaAnd : public BIFormula {
    BIFormula::Ref lhs;
    BIFormula::Ref rhs;
    BIFormulaAnd(const BIFormula::Ref& lhs, const BIFormula::Ref& rhs) : lhs(lhs), rhs(rhs) {
    }
};

struct BIFormulaOr : public BIFormula {
    BIFormula::Ref lhs;
    BIFormula::Ref rhs;

    BIFormulaOr(const BIFormula::Ref& lhs, const BIFormula::Ref& rhs) : lhs(lhs), rhs(rhs) {
    }
};

struct BIFormulaBlock : public BIFormula {
    XMASQueue* queue;
};

struct BIFormulaIdle : public BIFormula {
    XMASQueue* queue;
};

struct BIFormulaTrue : public BIFormula {
};

struct BIFormulaFalse : public BIFormula {
};

inline BIFormula::Ref operator!(BIFormula::Ref a) {
    return new BIFormulaNot(a);
}

inline BIFormula::Ref operator|(BIFormula::Ref a, BIFormula::Ref b) {
    return new BIFormulaOr(a, b);
}

inline BIFormula::Ref operator&(BIFormula::Ref a, BIFormula::Ref b) {
    return new BIFormulaAnd(a, b);
}

BIFormula::Ref Block(Output& output);
BIFormula::Ref Idle(Input& output);

class BlockVisitor : public XMASComponentVisitor {
public:
    Port& p;
    BIFormula::Ref retval;

    BlockVisitor(Port& p) : p(p), retval(nullptr) {
    }

    virtual void visit(XMASSink *) {
    }

    virtual void visit(XMASSource *) {
    }

    virtual void visit(XMASQueue *) {

    }

    virtual void visit(XMASFunction *) {

    }

    virtual void visit(XMASSwitch *) {

    }

    virtual void visit(XMASFork *) {

    }

    virtual void visit(XMASMerge *) {

    }

    virtual void visit(XMASJoin *join) {
        retval = Block(join->o) | !Idle(&join->a == &p ? join->b : join->a);
    }
};

BIFormula::Ref Idle(Input& output) {
    return nullptr;
}

BIFormula::Ref Block(Output& output) {
    BlockVisitor visitor(output);
    output.getTarget()->accept(visitor);
    return visitor.retval;
}

bool DeadlockChecker(std::set<XMASComponent *> allComponents) {
    // default no cycle
    bool deadlock = false;
    // check for each component if there is a cycle
    for (XMASComponent *c : allComponents) {
        XMASQueue* queue = dynamic_cast<XMASQueue*>(c);
        if (queue) {
            /*BIFormula* formula = */Block(queue->o); //& !Idle(queue->o));	// FIXME: unused-variable warning
        }
    }
    // return result
    return deadlock;
}
