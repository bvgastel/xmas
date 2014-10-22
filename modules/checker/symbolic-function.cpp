
#include "symbolic-function.h"

template <class T>
void valid(const T &f) {
  assert(f.op);
  assert(f.packet_function);
}

SymbolicCondition sym_negate(SymbolicCondition f) {
  valid(f);
  
  SymbolicCondition cond;
  cond.packet_function = [=](const Assignment &ass) {
    return !f.packet_function(ass);
  };

  auto op = std::make_shared<UniOp>(UniOp::NEGATE);
  cond.set_operator(op);

  op->operand = f.op;

  return std::move(cond);
}

SymbolicCondition sym_equals(SymbolicFunction a, SymbolicFunction b) {
  valid(a);
  valid(b);

  SymbolicCondition cond;
  cond.packet_function = [=](const Assignment &ass) {
    return a.packet_function(ass) == b.packet_function(ass);
  };

  auto op = std::make_shared<BinOp>(BinOp::EQUALS);
  cond.set_operator(op);

  op->lhs = a.op;
  op->rhs = b.op;

  return std::move(cond);
}

SymbolicFunction sym_input_from(XMASComponent *c) {
  SymbolicFunction f;
  f.packet_function = [=](const Assignment &ass) { return ass.at(c); };

  auto op = std::make_shared<InputOp>();
  f.set_operator(op);
  
  op->c = c;

  return std::move(f);
}

SymbolicFunction sym_end_of_queue(XMASQueue *q) {
  SymbolicFunction f;
  f.packet_function = [=](const Assignment &ass) { return ass.at(q); };

  auto op = std::make_shared<QueueOp>(QueueOp::END_OF_QUEUE);
  f.set_operator(op);

  op->q = q;

  return std::move(f);
}

OperatorPrinter *Operator::p_ = nullptr;