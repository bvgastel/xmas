#ifndef SYMBOLIC_FUNCTION_H
#define SYMBOLIC_FUNCTION_H

#include <cassert>
#include <ostream>
#include <unordered_map>

#include "state.h"

#include "symbolic-enum-field.h"

class BinOp;
class QueueOp;
class InputOp;
class ConstantEnumOp;
class ConstantOp;
class UniOp;
class AssocOp;

struct OperatorPrinter {
  virtual void print(const InputOp *, std::ostream&) = 0;
  virtual void print(const BinOp *, std::ostream&) = 0;
  virtual void print(const QueueOp *, std::ostream&) = 0;
  virtual void print(const ConstantEnumOp *, std::ostream&) = 0;
  virtual void print(const ConstantOp*, std::ostream&) = 0;
  virtual void print(const UniOp *, std::ostream&) = 0;
  virtual void print(const AssocOp *, std::ostream&) = 0;
};

struct Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream& o) const = 0;

  void accept(std::ostream& o) const {
    assert(p_);
    accept(p_, o);
  }

  static void set_printer(OperatorPrinter *p) {
    Operator::p_ = p;
  }

 private:
  static OperatorPrinter *p_;
};

namespace std {
static ostream &operator<<(ostream &o, Operator *op) {
  op->accept(o);
  return o;
}
}

struct XMASQueue;

struct InputOp : Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream& o) const {
    p->print(this, o);
  }

  XMASComponent *c;
};

struct QueueOp : Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream &o) const {
    p->print(this, o);
  }
  enum Ops { END_OF_QUEUE };
  QueueOp(Ops o) : op(o) {}
  Ops op;
  XMASQueue *q;
};


struct ConstantOp : Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream &o) const {
    p->print(this, o);
  }
  SymbolicPacket p;
};

struct ConstantEnumOp : Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream &o) const {
    p->print(this, o);
  }
  std::string s;
};

struct UniOp : Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream &o) const {
    p->print(this, o);
  }
  enum Ops {
    NEGATE
  };
  UniOp(Ops o) : op(o) {}
  Ops op;
  std::shared_ptr<Operator> operand;
};

struct BinOp : Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream &o) const {
    p->print(this, o);
  }
  enum Ops { EQUALS, CONTAINED };
  BinOp(Ops o) : op(o) {}
  Ops op;
  std::shared_ptr<Operator> lhs;
  std::shared_ptr<Operator> rhs;
};

struct AssocOp : Operator {
 public:
  virtual void accept(OperatorPrinter *p, std::ostream &o) const {
    p->print(this, o);
  }
  enum Ops {
    LOGICAL_OR
  };
  AssocOp(Ops o) : op(o) {}
  Ops op;
  std::vector<std::shared_ptr<Operator>> operands;
};



using Assignment = std::unordered_map<XMASComponent *, SymbolicPacket>;

// TODO(snnw):
// |SymbolicFunction| is meant to fully replace the std::function in
// |SymbolicFunctionExtension|.  Long term, |f_| should disappear in favour of
// an 'execution' Visitor for the operators.  The parser then only has to
// generate an |Operator| tree.
struct SymbolicFunction {
  void valid() {
    assert(op);
    assert(packet_function);
  }
  
  SymbolicPacket operator()(const Assignment &ass) const {
    return packet_function(ass);
  }

  // TODO: implement op->execute() and use that instead.
  // std::vector<SymbolicPacket> execute(const SymbolicPacket &p) const;

  void set_operator(std::shared_ptr<Operator> ops) {
    op = std::move(ops);
  }

  // TODO(snnw): pull |packet_function| out of |SymbolicFunction|
  std::function<SymbolicPacket(const Assignment &)> packet_function;

  std::shared_ptr<Operator> op;
};

class RealSymbolicFunctionExtension : public XMASComponentExtension {
 public:
  SymbolicFunction function;
  RealSymbolicFunctionExtension() {}
  RealSymbolicFunctionExtension(SymbolicFunction f) : function(f) {}
};

class RealSymbolicInverseFunctionExtension : public XMASComponentExtension {
 public:
  SymbolicFunction function;
  RealSymbolicInverseFunctionExtension() {}
  RealSymbolicInverseFunctionExtension(SymbolicFunction f) : function(f) {}
};

struct SymbolicCondition {
  void valid() {
    assert(op);
    assert(packet_function);
  }
  bool operator()(const Assignment &ass) const {
    return packet_function(ass);
  }

  // void set_std_function(std::function<bool(const SymbolicPacket &)> f) {
  //   f_ = std::move(f);
  // }

  void set_operator(std::shared_ptr<Operator> ops) {
    op = std::move(ops);
  }

  std::function<bool(const Assignment &)> packet_function;

  std::shared_ptr<Operator> op;
 private:
  std::function<bool(const SymbolicPacket &)> f_;
};

template <typename InputIterator>
bool contains(InputIterator start, InputIterator end, SymbolicPacket p) {
  return std::find_if(start, end, [&p](const SymbolicPacket &a) {
           return a == p || a.contains(p);
         }) != end;
}

template<typename InputIterator>
SymbolicCondition sym_contained_in(InputIterator start, InputIterator end, SymbolicFunction f) {
  assert(f.op);
  assert(f.packet_function);
  
  SymbolicCondition cond;
  cond.packet_function = [=](const Assignment &ass) {
    return contains(start, end, f.packet_function(ass));
  };

  auto op = std::make_shared<AssocOp>(AssocOp::LOGICAL_OR);

  for (auto p = start; p != end; ++p) {
    for (auto &field : p->fields) {
      // FIXME: the usual assumptions
      auto enum_field = dynamic_cast<SymbolicEnumField*>(field.second.get());
      for (auto &value : enum_field->values) {
        auto eq = std::make_shared<BinOp>(BinOp::EQUALS);
        auto constant_enum = std::make_shared<ConstantEnumOp>();
        constant_enum->s = value;
        eq->lhs = f.op;
        eq->rhs = constant_enum;
        op->operands.push_back(eq);
      }
    }
  }

  cond.set_operator(std::move(op));

  return cond;
}

SymbolicCondition sym_negate(SymbolicCondition f);

SymbolicCondition sym_equals(SymbolicFunction a, SymbolicFunction b);

SymbolicFunction sym_end_of_queue(XMASQueue *q);

SymbolicFunction sym_input_from(XMASComponent *);

static
void attachSymbolicInverseFunction(XMASFunction *c, SymbolicFunction f) {
  auto *ext = c->getComponentExtension<RealSymbolicInverseFunctionExtension>();
  ext->function = std::move(f);
}


#endif