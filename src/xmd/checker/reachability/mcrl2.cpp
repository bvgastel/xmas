
#include "mcrl2.h"

#include <sstream>
#include <vector>

#include "../lib/zip.h"
#include "../symbolic-enum-field.h"

#include "reachable-private.h"

namespace {

const std::vector<SymbolicEnumField::Type> &enum_type(Port *p) {
  assert(p);
  auto ext = p->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>(false);

  assert(ext);

  auto &packets = ext->availablePackets;

  // FIXME(snnw): looking only at first packet in packet expression
  auto &field = *packets[0].fields.begin();
  auto enum_field = dynamic_cast<SymbolicEnumField *>(field.second.get());
  return enum_field->values;
}

template <typename T>
std::string NumberToString(T Number) {
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

class InputVar {
 public:
  InputVar(XMASComponent *c, Port *p) : c_(c), p_(p) {
    i_ = i++;
  }

  std::string name() const {
    return "m" + NumberToString(i);
  }

  Port *port() const {
    return p_;
  }

 private:
  XMASComponent *c_;
  Port *p_;
  int i_;
  static int i;
};

int InputVar::i = 0;

class QueueVar {
 public:
  QueueVar(XMASQueue *q_) : q(q_) {}

  XMASQueue *q;
};

class QueueVarXMASComponentExtension : public XMASComponentExtension {
 public:
  QueueVar *qv;
};

class InputVarXMASComponentExtension : public XMASComponentExtension {
 public:
  InputVar *iv;
};

const InputVar &iv(XMASComponent *c) {
  assert(c->getComponentExtension<InputVarXMASComponentExtension>(false));
  return *c->getComponentExtension<InputVarXMASComponentExtension>(false)->iv;
}

const QueueVar &qv(XMASQueue *q) {
  return *q->getComponentExtension<QueueVarXMASComponentExtension>(false)->qv;
}

std::string name(const QueueVar &q) {
  return q.q->getName();
}

std::string type(const QueueVar &q) {
  return "T" + name(q);
}

std::string size(const QueueVar &q) {
  return name(q) + "n";
}


class Mcrl2OperatorPrinter : public OperatorPrinter {
  virtual void print(const InputOp *op, std::ostream &o) {
    auto types = enum_type(iv(op->c).port());
    if (types.size() == 1) {
      o << types[0];
    } else {
      o << iv(op->c).name();
    }
  }
  
  virtual void print(const BinOp *op, std::ostream& o) {
    assert(op);
    assert(op->rhs);

    if (op->op == BinOp::CONTAINED) {
      assert(op->lhs);
      o << "(" << op->lhs << " in " << op->rhs << ")";
    } else if (op->op == BinOp::EQUALS) {
      assert(op->lhs);
      o << "(" << op->lhs << " == " << op->rhs << ")";
    }
  }

  virtual void print(const QueueOp *op, std::ostream& o) {
    o << name(qv(op->q)) << "(0)";
  }

  virtual void print(const ConstantEnumOp *op, std::ostream& o) {
    o << op->s;
  }
  virtual void print(const ConstantOp *op, std::ostream& o) {
    // FIXME(snnw): the usual assumptions
    auto &field = *op->p.fields.begin();
    auto enum_field = dynamic_cast<SymbolicEnumField *>(field.second.get());
    o << enum_field->values[0];
  }

  virtual void print(const UniOp *op, std::ostream& o) {
    if (op->op == UniOp::NEGATE) {
      o << "!" << op->operand;
    }
  }

  virtual void print(const AssocOp *op, std::ostream& o) {
    std::string op_sign = "";
    if (op->op == AssocOp::LOGICAL_OR) {
      op_sign = " || ";
    }

    o << "(";

    bool first = true;
    for (auto &operand : op->operands) {
      o << (first ? "" : op_sign) << operand;
      first = false;
    }

    o << ")";
  }
};

std::vector<QueueVar> build_queue_vars(const std::set<XMASComponent *> &components) {
  std::vector<QueueVar> result;

  for (auto c : components) {
    auto q = dynamic_cast<XMASQueue *>(c);
    if (!q) continue;

    result.emplace_back(q);
  }

  auto it = result.begin();

  for (auto c : components) {
    auto q = dynamic_cast<XMASQueue *>(c);
    if (!q) continue;

    auto ext = q->getComponentExtension<QueueVarXMASComponentExtension>();
    ext->qv = &*it;

    ++it;
  }

  return std::move(result);
}

void add_ivar(std::vector<std::unique_ptr<InputVar>> *ivars, XMASComponent *c,
              Port *p) {
  auto ext = c->getComponentExtension<InputVarXMASComponentExtension>();

  if (!ext->iv) {
    ivars->push_back(std::unique_ptr<InputVar>(new InputVar(c, p)));
    ext->iv = ivars->back().get();
  }
}

std::vector<std::unique_ptr<InputVar>> build_input_vars(
    const std::vector<GenericTransition> &trans) {
  std::vector<std::unique_ptr<InputVar>> result;

  for (auto &t : trans) {
    for (auto c : t.out()) {
      auto join = dynamic_cast<XMASJoin *>(c.first);
      auto sink = dynamic_cast<XMASSink *>(c.first);

      if (join) add_ivar(&result, join, c.second);
      if (sink) add_ivar(&result, sink, &sink->i);
    }
  }

  return result;
}

std::unordered_set<std::string> all_types(const std::set<XMASComponent *>
  &components) {
  std::unordered_set<std::string> types;
  for (auto c : components) {
    for (auto p : c->ports()) {
      for (auto &s : enum_type(p)) {
        types.insert(s);
      }
    }
  }

  return types;
}

}

void print_mcrl2(std::ostream &out, const std::set<XMASComponent *> &components,
                 const XMASState &state) {
  auto types = all_types(components);
  auto qvars = build_queue_vars(components);

  Mcrl2OperatorPrinter printer;
  Operator::set_printer(&printer);

  out << "sort Msg = struct ";

  bool first = true;
  for (const auto &t : types) {
    out << (first ? "" : " | ") << t;
    first = false;
  }

  out << ";" << std::endl << std::endl;

  for (const auto &q : qvars) {
    out << "sort " << type(q) << " = Int -> Msg;" << std::endl;
  }

  out << std::endl;

  out << "map" << std::endl;

  for (const auto &q : qvars) {
    out << "  give_" << name(q) << ": Msg # " << type(q) << " # Int -> "
        << type(q) << ";" << std::endl;

    out << "  take_" << name(q) << ": " << type(q) << " -> " << type(q) << ";"
        << std::endl;

    out << "  shift_" << name(q) << ": " << type(q) << " -> " << type(q) << ";"
        << std::endl;
  }

  out << std::endl << "var" << std::endl;
  out << "  n : Int;" << std::endl;
  out << "  m : Msg;" << std::endl;

  for (const auto &q : qvars) {
    out << "  " << name(q) << " : " << type(q) << ";" << std::endl;
  }

  out << std::endl << "eqn" << std::endl;

  for (const auto &q : qvars) {
    out << "  give_" << name(q) << "(m, " << name(q) << ", n) = " << name(q)
        << "[n -> m];" << std::endl;

    out << "  take_" << name(q) << "(" << name(q) << ") = shift_" << name(q)
        << "(" << name(q) << ");" << std::endl;

    out << "  shift_" << name(q) << "(" << name(q) << ")(n) = " << name(q)
        << "(n+1);" << std::endl;

    out << "  shift_" << name(q) << "(" << name(q) << ")[n -> m] = " << name(q)
        << "[n+1 -> m];" << std::endl;
  }

  out << std::endl << "act" << std::endl;

  auto transitions = compute_transitions(components, true);

  auto ivars = build_input_vars(transitions);

  int i = 0;
  for (auto &t : transitions) {
    out << "  T" << i;
    t.set_id(i++);

    bool first = true;
    for (const auto &o : t.out()) {
      if (enum_type(o.second).size() <= 1) continue;

      out << (first ? ": " : " # ") << "Msg";
      first = false;
    }
    out << ";" << std::endl;
  }

  out << std::endl;

  out << "proc XMAS(";

  first = true;
  for (const auto &q : qvars) {
    out << (first ? "" : ", ") << name(q) << " : " << type(q)
        << ", " << size(q) << " : Int";
    first = false;
  }
  out << ") = " << std::endl;

  first = true;
  for (const auto &t : transitions) {
    out << "  " << (first ? "  " : "+ ") << "(";
    bool first2 = true;
    for (auto q : t.in_queues()) {
      out << (first2 ? "" : " && ") << size(qv(q)) << " != " << q->c;
      first2 = false;
    }

    for (auto q : t.out_queues()) {
      out << (first2 ? "" : " && ") << size(qv(q)) << " != " << 0;
      first2 = false;
    }

    for (auto c : t.conditions()) {
      out << (first2 ? "" : " && ") << c.op;
      first2 = false;
    }

    out << ") -> ";

    first2 = true;
    for (auto pair : t.out()) {
      if (enum_type(pair.second).size() <= 1) continue;

      out << (first2 ? "sum " : ", ") << iv(pair.first).name() << ": Msg";

      first2 = false;
    }

    if (!first2) out << ". ";

    out << "( T" << t.id();

    first2 = true;
    for (auto pair : t.out()) {
      if (enum_type(pair.second).size() <= 1) continue;

      out << (first2 ? "(" : ", ") << iv(pair.first).name();

      first2 = false;
    }
    if (!first2) out << ")";

    out << " . XMAS(";
    first2 = true;
    for (const auto &qvar : qvars) {
      auto inqs = zip(t.in_queues(), t.in_functions());
      auto outqs = t.out_queues();

      // TODO: efficiency
      auto inq_it = std::find_if(
          inqs.begin(), inqs.end(),
          [&](const std::pair<XMASQueue *, const SymbolicFunction &> &pair) {
            return pair.first == qvar.q;
          });

      if (inq_it != inqs.end()) {
        out << (first2 ? "" : ", ") << "give_" << name(qvar) << "("
            << (*inq_it).second.op << ", " << name(qvar) << ", " << size(qvar)
            << "), " << size(qvar) << "+1";
      } else if (std::find_if(outqs.begin(), outqs.end(), [&](XMASQueue *q) {
                   return q == qvar.q;
                 }) != outqs.end()) {
        out << (first2 ? "" : ", ") << "take_" << name(qvar) << "("
            << name(qvar) << "), " << size(qvar) << "-1";
      } else {
        // nothing happens...
        out << (first2 ? "" : ", ") << name(qvar) << ", " << size(qvar);
      }
      first2 = false;
    }

    out << ")";

    out << " )";
    out << std::endl << std::endl;
    first = false;
  }
  out << ";" << std::endl;

  out << "init XMAS(";
  first = true;
  for (auto &qvar : qvars) {
    auto &qstate = state.get_queue(qvar.q);
    out << (first ? "" : ",\n          ") << "(lambda n : Int . "
        << *types.begin() << ")";

    int i = 0;
    for (auto &packet : qstate) {
      // FIXME: that's a lot for one line
      out << "[" << i << "->"
          << dynamic_cast<SymbolicEnumField *>(
                 packet[0].fields.begin()->second.get())->values[0] << "]";
    }

    out << ", " << qstate.size();
    first = false;
  }

  out << ");";

  out << std::endl << std::endl << std::endl;

  out << "mu X(";

  first = true;
  for (const auto &qvar : qvars) {
    out << (first ? "" : ", ") << size(qvar) << ":Int = " << state.get_queue(qvar.q).size();
    first = false;
  }

  out << ")" << std::endl << " . (" << std::endl;
  out << "      ";
  first = true;
  for (const auto &qvar : qvars) {
    out << (first ? "(" : " && ") << "val(" << size(qvar) << " == 0)";
    first = false;
  }
  out << ")" << std::endl << "   || (";
  first = true;
  for (const auto &qvar : qvars) {
    out << (first ? "(" : " || ") << "val(" << size(qvar) << " != 0)";
    first = false;
  }
  out << ")" << std::endl << "      && (" << std::endl;

  first = true;
  for (const auto &t : transitions) {
    out << "      " << (first ? "   " : "|| ");
 
    out << "(<";
    bool first2 = true;
    for (auto pair : t.out()) {
      if (enum_type(pair.second).size() <= 1) continue;

      out << (first2 ? "exists " : ", ") << iv(pair.first).name() << ": Msg";
      first2 = false;
    }

    if (!first2) out << " . ";

    out << "T" << t.id();
    first2 = true;
    for (auto pair : t.out()) {
      if (enum_type(pair.second).size() <= 1) continue;

      out << (first2 ? "(" : ",") << iv(pair.first).name();
      first2 = false;
    }
    if (!first2) out << ")";

    out << "> X(";
    first2 = true;
    for (const auto &qvar : qvars) {
      auto inqs = zip(t.in_queues(), t.in_functions());
      auto outqs = t.out_queues();

      // TODO: efficiency
      auto inq_it = std::find_if(
          inqs.begin(), inqs.end(),
          [&](const std::pair<XMASQueue *, const SymbolicFunction &> &pair) {
            return pair.first == qvar.q;
          });

      if (inq_it != inqs.end()) {
        out << (first2 ? "" : ", ") << size(qvar) << "+1";
      } else if (std::find_if(outqs.begin(), outqs.end(), [&](XMASQueue *q) {
                   return q == qvar.q;
                 }) != outqs.end()) {
        out << (first2 ? "" : ", ") << size(qvar) << "-1";
      } else {
        // nothing happens...
        out << (first2 ? "" : ", ") << size(qvar);
      }
      first2 = false;
    }



    out << "))" << std::endl;
    first = false;
  }
  out << "   ))" << std::endl;

  out << ")" << std::endl;

  for (auto c : components) {
    c->clearComponentExtension<InputVarXMASComponentExtension>();
  }
}
