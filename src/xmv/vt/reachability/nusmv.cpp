#include "nusmv.h"

#include <exception>
#include <ext/stdio_filebuf.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <sys/wait.h>

#include "../lib/zip.h"
#include "../symbolic-enum-field.h"

#include "reachable-private.h"
#include "util.h"

namespace {

class QueueVar;
class InputVar;

class QueueVarXMASComponentExtension : public XMASComponentExtension {
 public:
  QueueVar *qv;
};

class InputVarXMASComponentExtension : public XMASComponentExtension {
 public:
  InputVar *iv;
};

const std::vector<SymbolicEnumField::Type> &enum_type(Port *p) {
  assert(p);
  auto ext = p->getInitiatorPort()->getPortExtension<SymbolicTypesExtension>(false);

  assert(ext);

  auto &packets = ext->availablePackets;

  assert(packets.size() >= 1);

  // FIXME(snnw): looking only at first packet in packet expression
  auto &field = *packets[0].fields.begin();
  auto enum_field = dynamic_cast<SymbolicEnumField *>(field.second.get());
  return enum_field->values;
}

class NusmvOperatorPrinter : public OperatorPrinter {
  virtual void print(const InputOp *op, std::ostream &o);
  
  virtual void print(const BinOp *op, std::ostream& o) {
    assert(op);
    assert(op->rhs);
    if (op->op == BinOp::CONTAINED) {
      assert(op->lhs);
      o << "(" << op->lhs << " in " << op->rhs << ")";
    } else if (op->op == BinOp::EQUALS) {
      assert(op->lhs);
      o << "(" << op->lhs << " = " << op->rhs << ")";
    }
  }
  virtual void print(const ConstantEnumOp *op, std::ostream& o) {
    o << op->s;
  }
  virtual void print(const QueueOp *op, std::ostream& o);
  virtual void print(const ConstantOp *op, std::ostream& o) {
    //
    o << "{";
    // FIXME(snnw): the usual assumptions
    auto &field = *op->p.fields.begin();
    auto enum_field = dynamic_cast<SymbolicEnumField *>(field.second.get());
    bool first = true;
    for (auto &t : enum_field->values) {
      o << (first ? "" : ", ") << t;
      first = false;
    }
    o << "}";
  }
  virtual void print(const UniOp *op, std::ostream& o) {
    if (op->op == UniOp::NEGATE) {
      o << "!" << op->operand;
    }
  }

  virtual void print(const AssocOp *op, std::ostream& o) {
    std::string op_sign = "";
    if (op->op == AssocOp::LOGICAL_OR) {
      op_sign = " | ";
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

template <typename T>
std::string NumberToString(T Number) {
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}


class QueueVar {
 public:
  QueueVar(XMASQueue *queue) : q(queue) {}

  std::string decl() const {
    return name() + ": array 1.." + NumberToString(capacity()) + " of " +
           type() + ";";
  }

  std::string size_decl() const {
    return size_name() + ": 0.." + NumberToString(capacity()) + ";";
  }

  std::string at(size_t i) const {
    return name() + "[" + NumberToString(i) + "]";
  }

  std::string front() const {
    return at(capacity());
  }

  std::string size_name() const { return name() + "_n"; }
  bool constant() const { return !none(); }
  size_t capacity() const { return q->c; }

  XMASQueue *q;

 private:
  const std::string& name() const { return q->getName(); }

  std::string type() const {
    std::string res = "{";

    auto &values = enum_type(&q->o);

    bool first = true;

    if (none()) {
      res += "none";
      first = false;
    }

    for (auto &v : values) {
      res += (first ? "" : ", ") + v;
      first = false;
    }

    return res + "}";
  }

  bool none() const { return enum_type(&q->o).size() != 1; }
};

class InputVar {
 public:
  InputVar(XMASComponent *c, Port *p) : c_(c), p_(p) {}
  std::string const& name() {
    return c_->getName();
  }
  std::string decl() {
    return name() + ": " + type();
  }

  std::string type() const {
    std::string res = "{";
    auto &values = enum_type(p_);

    bool first = true;
    for (auto &v : values) {
      res += (first ? "" : ", ") + v;
      first = false;
    }

    return res + "}";
  }

 private:
  XMASComponent *c_;
  Port *p_;
};

void NusmvOperatorPrinter::print(const InputOp *op, std::ostream &o) {
  auto ext =
      op->c->getComponentExtension<InputVarXMASComponentExtension>(false);
  assert(ext);

  o << ext->iv->name();
}

std::vector<QueueVar> build_queue_vars(const std::set<XMASComponent *> &network) {
  std::vector<QueueVar> result;

  for (auto c : network) {
    auto q = dynamic_cast<XMASQueue *>(c);
    if (!q) continue;

    result.emplace_back(q);
  }

  auto it = result.begin();

  for (auto c : network) {
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
    const std::vector<GenericTransition> &trans, nusmv_flags flags) {
  std::vector<std::unique_ptr<InputVar>> result;

  for (auto &t : trans) {
    for (auto c : t.out()) {
      if (flags & nusmv_reverse) {
        auto join = dynamic_cast<XMASJoin *>(c.first);
        auto sink = dynamic_cast<XMASSink *>(c.first);

        if (join) add_ivar(&result, join, c.second);
        if (sink) add_ivar(&result, sink, &sink->i);
      } else {
        auto src = dynamic_cast<XMASSource *>(c.first );

        if (src) add_ivar(&result, src, &src->o);
      }
    }
  }

  return std::move(result);
}

std::string name(const GenericTransition& t) {
  return "T" + NumberToString(t.id());
}

std::string size_name(XMASQueue *q) {
  auto ext = q->getComponentExtension<QueueVarXMASComponentExtension>(false);
  assert(ext);

  return ext->qv->size_name();
}

size_t capacity(XMASQueue *q) {
  auto ext = q->getComponentExtension<QueueVarXMASComponentExtension>(false);
  assert(ext);

  return ext->qv->capacity();
}

size_t queue_size(const XMASState &state, XMASQueue *q) {
  return state.get_queue(q).size();
}

const SymbolicPacket &queue_at(const XMASState &state, XMASQueue *q, size_t i) {
  assert(state.get_queue(q)[i].size() == 1);
  return state.get_queue(q)[i][0];
}

void NusmvOperatorPrinter::print(const QueueOp *op, std::ostream &o) {
  auto ext =
      op->q->getComponentExtension<QueueVarXMASComponentExtension>(false);
  assert(ext);

  o << ext->qv->front();
}

} // namespace

void print_nusmv(std::ostream &out, const std::set<XMASComponent *> &network,
                 const XMASState &state, nusmv_flags flags) {
  auto trans = compute_transitions(network, flags & nusmv_reverse);

  int i = 0;
  for (auto &t : trans) {
    t.set_id(i++);
  }

  auto qvars = build_queue_vars(network);
  auto ivars = build_input_vars(trans, flags);

  NusmvOperatorPrinter printer;

  Operator::set_printer(&printer);

  out << "MODULE main" << std::endl;
  out << "VAR" << std::endl;

  // Queue declarations
  for (auto &qvar : qvars) {
    // FIXME(snnw): assuming one field and one SymbolicPacket as type
    out << "  " << qvar.decl() << std::endl;
    out << "  " << qvar.size_decl() << std::endl;
  }

  // out << "IVAR" << std::endl;
  // transition variable declaration
  out << "  t: {";
  bool first = true;
  for (auto &t : trans) {
    out << (first ? "" : ", ") << name(t);
    first = false;
  }
  out << "};" << std::endl;

  for (auto &ivar : ivars) {
    out << "  " << ivar->decl() << ";" << std::endl;
  }

  out << std::endl;

  for (auto &t : trans) {
    // transition is disabled when out-queue is not empty
    for (auto out_queue : t.out_queues()) {
      out << "INVAR t = " << name(t) << " -> " << size_name(out_queue)
          << " != 0;" << std::endl;
    }

    // transition is disabled when in-queue is full
    for (auto in_queue : t.in_queues()) {
      out << "INVAR t = " << name(t) << " -> " << size_name(in_queue)
          << " != " << capacity(in_queue) << ";" << std::endl;
    }

    // transition is disabled when condition is not satisfied
    for (auto &c : t.conditions()) {
      out << "INVAR t = " << name(t) << " -> " << c.op << ";" << std::endl;
    }

    out << std::endl;
  }

  out  << "ASSIGN" << std::endl;

  for (auto &qvar : qvars) {
    out << "  init(" << qvar.size_name()
        << ") := " << (flags & nusmv_reverse ? queue_size(state, qvar.q) : 0)
        << ";" << std::endl;
  }

  out << std::endl;

  for (auto &qvar : qvars) {
    out << "  next(" << qvar.size_name() << ") :=" << std::endl;
    out << "    case" << std::endl;

    for (auto &t : trans) {
      for (auto out_queue : t.out_queues()) {
        if (out_queue == qvar.q) {
          out << "      (t = " << name(t) << " & " << qvar.size_name()
              << " > 0): " << qvar.size_name() << " - 1;" << std::endl;
        }
      }

      for (auto in_queue : t.in_queues()) {
        if (in_queue == qvar.q) {
          out << "      (t = " << name(t) << " & " << qvar.size_name() << " < "
              << qvar.capacity() << "): " << qvar.size_name() << " + 1;"
              << std::endl;
        }
      }
    }

    out << "      TRUE: " << qvar.size_name() << ";" << std::endl;
    out << "    esac;" << std::endl << std::endl;
  }

  for (auto &qvar : qvars) {
    if (qvar.constant()) continue;

    for (int i = 1; i <= qvar.capacity(); ++i) {
      out << "  init(" << qvar.at(i) << ") := ";
      if (flags & nusmv_reverse &&
         qvar.capacity() - queue_size(state, qvar.q) <= i - 1) {
        auto op = std::make_shared<ConstantOp>();
        op->p = queue_at(state, qvar.q,
                         i - 1 - qvar.capacity() + queue_size(state, qvar.q));
        out << op;
      } else {
        out << "none";
      }

      out << ";" << std::endl;
      out << "  next(" << qvar.at(i) << ") :=" << std::endl;
      out << "    case" << std::endl;

      for (auto &t : trans) {
        for (auto out_queue : t.out_queues()) {
          if (out_queue != qvar.q) continue;

          out << "      t = " << name(t) << ": ";
          if (i > 1) {
            out << qvar.at(i - 1) << ";" << std::endl;
          } else {
            out << "none;" << std::endl;
          }
        }

        for (auto pair : zip(t.in_queues(), t.in_functions())) {
          if (pair.first != qvar.q) continue;

          out << "      (t = " << name(t) << " & " << qvar.size_name() << " = "
              << qvar.capacity() - i;
          for (auto &c : t.conditions()) {
            out << " & " << c.op;
          }

          out << "): " << pair.second.op << ";" << std::endl;
        }
      }

      out << "      TRUE: " << qvar.at(i) << ";" << std::endl;
      out << "    esac;" << std::endl << std::endl;
    }
  }

  if (flags & nusmv_reverse) {
    out << "CTLSPEC AG !(";
    bool first = true;
    for (auto &qvar : qvars) {
      out << (first ? "": " & ") << qvar.size_name() << " = 0";
      first = false;
    }
    out << ")" << std::endl;
  } else {
    out << "CTLSPEC AG !(";
    bool first = true;
    for (auto &qvar : qvars) {
      out << (first ? "": " & ") << qvar.size_name() << " = " << queue_size(state, qvar.q);
      first = false;
    }

    for (auto &qvar : qvars) {
      for (int i = 1; i <= qvar.capacity(); ++i) {
        if (qvar.capacity() - queue_size(state, qvar.q) <= i - 1) {
          auto op = std::make_shared<ConstantOp>();
          op->p = queue_at(state, qvar.q,
                           i - 1 - qvar.capacity() + queue_size(state, qvar.q));
          out << (first ? "": " & ") << qvar.at(i) << " in " << op;
        }
      }

      first = false;
    }
    out << ")" << std::endl;
  }

  for (auto c : network) {
    c->clearComponentExtension<InputVarXMASComponentExtension>();
  }
}

class MyException : std::exception {
 private:
  std::string message_;

 public:
  MyException(const std::string &message) : message_(message) {}
  virtual const char *what() const throw() { return message_.c_str(); }
};

bool isReachableNusmv(const std::set<XMASComponent *> &components,
                      const XMASState &state, nusmv_flags flags) {

  static auto not_reachable = std::regex("-- specification AG .* is true");
  static auto reachable = std::regex("-- specification AG .* is false");
  static auto num_states_regex = std::regex("  iteration .* states = (.*)");
  std::smatch num_states_match;

  struct popen2 nusmv_proc;
  popen2("time sh -c \"nuxmv -dynamic -AG -v 3 2>&1\"", &nusmv_proc);
  // popen2("tee | time nuxmv -dynamic -AG 2>/dev/null", &nusmv_proc);

  file_buffer<char> buf2{nusmv_proc.from_child, std::ios::in};
  std::istream in{&buf2};

  {
    file_buffer<char> buf{nusmv_proc.to_child, std::ios::out};
    std::ostream out{&buf};
    // print_nusmv(std::cout, components, state, flags);
    print_nusmv(out, components, state, flags);
  }

  bool is_reachable = false;
  bool is_not_reachable = false;
  std::string num_states = "";

  std::string line;
  while (std::getline(in, line)) {
    // std::cout << line << std::endl;
    if (std::regex_match(line, not_reachable)) is_not_reachable = true;
    if (std::regex_match(line, reachable)) is_reachable = true;
    if (std::regex_match(line, num_states_match, num_states_regex))
      num_states = num_states_match[1].str();
  }

  std::cerr << "number of states: " << num_states << std::endl;

  if (is_not_reachable && is_reachable) {
    throw MyException(
        "state both reachable and not reachable, according to nuxmv");
  } else if (!is_not_reachable && !is_reachable) {
    throw MyException("could not determine if state is reachable");
  } else {
    return is_reachable;
  }
}
