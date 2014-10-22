
#include "reachable.h"
#include "reachable-private.h"

#include <unordered_set>

#include "../state.h"
#include "../symbolic-enum-field.h"

#include "transition.h"

namespace {

XMASComponent *backwards(XMASComponent *t, size_t i) {
  return t->inputPorts().begin()[i]->getInitiatorPort()->getComponent();
}


const std::vector<SymbolicPacket> &port_types(Output *port) {
  auto *ext = port->getPortExtension<SymbolicTypesExtension>(false);

  // symbolic types must have been computed first.
  assert(ext);

#ifndef NDEBUG
  // make sure we are only dealing with packets containing enums
  for (auto &packet : ext->availablePackets) {
    for (auto &named_field : packet.fields) {
      auto *a = FastType<SymbolicEnumField>::cast(named_field.second.get());
      assert(a);
    }
  }
#endif

  return ext->availablePackets;
}

const std::vector<SymbolicPacket> &port_types(Input *port) {
  // Symbolic types are only connected to an output port

  return port_types(port->getInitiatorPort());
}

const std::unordered_set<int> &port_types_hashes(Output *port) {
  auto *ext = port->getPortExtension<SymbolicTypesExtension>(false);

  // symbolic types must have been computed first.
  assert(ext);

  return ext->availablePacketsHashes;
}

const std::unordered_set<int> &port_types_hashes(Input *port) {
  return port_types_hashes(port->getInitiatorPort());
}


const std::vector<SymbolicPacket> &input_types(XMASComponent *t, size_t i) {
  return port_types(t->inputPorts().begin()[i]);
}

const std::unordered_set<int> &input_types_hashes(XMASComponent *t, size_t i) {
  return port_types_hashes(t->inputPorts().begin()[i]);
}

XMASComponent *forwards(XMASComponent *t, size_t i) {
  return t->outputPorts().begin()[i]->getTargetPort()->getComponent();
}

const std::vector<SymbolicPacket> &output_types(XMASComponent *t, size_t i) {
  return port_types(t->outputPorts().begin()[i]);
}

const std::unordered_set<int> &output_types_hashes(XMASComponent *t, size_t i) {
  return port_types_hashes(t->outputPorts().begin()[i]);
}

using TransitionPacketList =
    std::unordered_map<GenericTransition *, SymbolicFunction>;
using TPL = TransitionPacketList;

// convention: hang them on the initiator port
class TransitionContainer : public PortExtension {
 public:
  TransitionPacketList transitions;
};

class VisitedExtension : public XMASComponentExtension {
 public:
  bool visited = false;
};

bool visited(XMASComponent *c) {
  auto ext = c->getComponentExtension<VisitedExtension>();
  return ext->visited;
}

void set_visited(XMASComponent *c) {
  auto ext = c->getComponentExtension<VisitedExtension>();
  ext->visited = true;
}

class ComputeTransitions : public XMASComponentVisitor {
 public:
  ComputeTransitions(const std::vector<XMASComponent *> &components,
                     bool backwards)
      : backwards_(backwards) {
    for (auto c : components) {
      c->clearComponentExtension<VisitedExtension>();

      for (auto p : c->ports()) {
        p->clearPortExtension<TransitionContainer>();
      }
    }

    for (auto c : components) {
      c->accept(*this);
    }
  }

  void visit(XMASSource *c) {
    if (backwards_) return;

    auto t = new_transition();
    t->add_out(c, &c->o, &output_types(c, 0));

    SymbolicFunction f = sym_input_from(c);

    add(t, f, &c->o);
  }

  void visit(XMASSink *c) {
    if (!backwards_) return;

    auto t = new_transition();
    t->add_out(c, &c->i, &input_types(c, 0));

    SymbolicFunction f = sym_input_from(c);

    add(t, f, &c->i);
  }

  void visit(XMASQueue *c) {
    Port *p_out = &c->o;
    Port *p_in = &c->i;
    if (backwards_) {
      p_out = &c->i;
      p_in = &c->o;
    }

    if (!visited(c)) {
      // first visit
      SymbolicFunction f = sym_end_of_queue(c);

      auto t = new_transition();

      t->add_out_queue(c);
      t->add_condition(sym_contained_in(input_types(c, 0).begin(),
                                        input_types(c, 0).end(), f));

      add(t, f, p_out);
      set_visited(c);
    } else {
      // second visit
      assert(get_transitions(p_in)->size() != 0);

      for (auto &pair : *get_transitions(p_in)) {
        pair.first->add_in(c, pair.second);
      }
    }
  }

  void visit(XMASFunction *c) {
    auto sym_function = get_sym_function(c);

    assert(sym_function.op);

    Port *p_out = &c->o;
    Port *p_in = &c->i;
    if (backwards_) {
      p_out = &c->i;
      p_in = &c->o;
    }

    // Use |function| to set the |packet_function| of |sym_function|.  When |
    // |function| disappears, we should use |sym_function.execute()|.
    auto function = get_function(c);

    for (auto &pair : *get_transitions(p_in)) {
      auto packet = pair.second;

      sym_function.packet_function = [=](const Assignment &s) {
        return function(packet(s));
      };

      add(pair.first, sym_function, p_out);
    }
  }

  void visit(XMASJoin *c) {
    bool take_a = does_join_take_a(c);

    if (!backwards_) {
      // Generate a Cartesian product of the incoming transitions.

      // Make a copy of both transition lists, because iterators to the
      // originals will be invalidated.
      auto as = *get_transitions(&c->a);
      auto bs = *get_transitions(&c->b);

      for (auto &a : as) {
        for (auto &b : bs) {
          auto new_t = clone_transition(a.first);

          merge_into(new_t, b.first);

          add(new_t, (take_a ? a.second : b.second), &c->o);
        }
      }

      for (auto &a : as) {
        remove_transition(a.first);
      }

      for (auto &b : bs) {
        remove_transition(b.first);
      }
    } else {
      // Duplicate to both incoming ports.
      for (auto &t : *get_transitions(&c->o)) {
        t.first->add_out(c, (take_a ? &c->b : &c->a),
                         &input_types(c, (take_a ? 1 : 0)));

        add(t.first, (take_a ? t.second : sym_input_from(c)), &c->a);
        add(t.first, (take_a ? sym_input_from(c) : t.second), &c->b);
      }
    }
  }

  void visit(XMASFork *c) {
    if (!backwards_) {
      for (auto &t : *get_transitions(&c->i)) {
        add(t.first, t.second, &c->a);
        add(t.first, t.second, &c->b);
      }
    } else {
      // Make a copy of both transition lists, because iterators to the
      // originals will be invalidated.
      auto as = *get_transitions(&c->a);
      auto bs = *get_transitions(&c->b);

      for (auto &a : as) {
        for (auto &b : bs) {
          auto new_t = clone_transition(a.first);
          new_t->add_condition(sym_equals(a.second, b.second));

          merge_into(new_t, b.first);

          add(new_t, a.second, &c->i);
        }
      }

      for (auto &a : as) {
        remove_transition(a.first);
      }

      for (auto &b : bs) {
        remove_transition(b.first);
      }
    }
  }

  void visit(XMASMerge *c) {
    if (!backwards_) {
      for (auto &t : *get_transitions(&c->a)) {
        add(t.first, t.second, &c->o);
      }

      for (auto &t : *get_transitions(&c->b)) {
        add(t.first, t.second, &c->o);
      }
    } else {
      for (auto &t : *get_transitions(&c->o)) {
        auto new_t = clone_transition(t.first);

        t.first->add_condition(sym_contained_in(
            input_types(c, 0).begin(), input_types(c, 0).end(), t.second));
        new_t->add_condition(sym_contained_in(
            input_types(c, 1).begin(), input_types(c, 1).end(), t.second));

        add(t.first, t.second, &c->a);
        add(new_t, t.second, &c->b);
      }
    }
  }

  void visit(XMASSwitch *c) {
    auto *ext =
        c->getComponentExtension<SymbolicSwitchingFunctionExtension>(false);

    assert(ext);

    if (!backwards_) {
      for (auto &t : *get_transitions(&c->i)) {
        auto new_t = clone_transition(t.first);

        t.first->add_condition(sym_contained_in(ext->availablePackets.begin(),
                                                ext->availablePackets.end(),
                                                t.second));
        new_t->add_condition(sym_negate(
            sym_contained_in(ext->availablePackets.begin(),
                             ext->availablePackets.end(), t.second)));

        add(t.first, t.second, &c->a);
        add(new_t, t.second, &c->b);
      }
    } else {
      for (auto &t : *get_transitions(&c->a)) {
        t.first->add_condition(sym_contained_in(ext->availablePackets.begin(),
                                                ext->availablePackets.end(),
                                                t.second));
        add(t.first, t.second, &c->i);
      }

      for (auto &t : *get_transitions(&c->b)) {
        t.first->add_condition(sym_negate(
            sym_contained_in(ext->availablePackets.begin(),
                             ext->availablePackets.end(), t.second)));
        add(t.first, t.second, &c->i);
      }
    }
  }

  std::unordered_map<GenericTransition *, std::unique_ptr<GenericTransition>>
      transitions;

private:
  GenericTransition *new_transition() {
    auto t = new GenericTransition();
    transitions.insert(std::make_pair(t, std::unique_ptr<GenericTransition>(t)));
    return t;
  }

  GenericTransition *clone_transition(GenericTransition *gt) {
    // Create an identical |GenericTransition|, and insert it into the
    // |transitions| to indicate ownership.
    auto new_gt = new GenericTransition(*gt);
    transitions.insert(
        std::make_pair(new_gt, std::unique_ptr<GenericTransition>(new_gt)));

    insert_transition(new_gt, gt);

    return new_gt;
  }

  void insert_transition(GenericTransition *gt, GenericTransition *where) {
    // Find all transition lists that refer to |gt|.
    auto range = transition_lists.equal_range(where);

    // Delay the insertions into |transition_lists|, to not invalidate the
    // iterators in |range|.
    std::vector<TransitionPacketList *> inserted_transitions;


    for (auto it = range.first; it != range.second; ++it) {
      auto ts = it->second;
      inserted_transitions.push_back(ts);

      // Get the packet that was associated with |where| and associate it with
      // |gt|.
      auto &packet = ts->at(where);
      ts->emplace(gt, packet);
    }

    for (auto ts : inserted_transitions) {
      transition_lists.emplace(gt, ts);
    }
  }

  void remove_transition(GenericTransition *gt) {
    for (auto &ts : transition_lists) {
      ts.second->erase(gt);
    }

    transition_lists.erase(gt);

    // The delete of the |unique_ptr| in |transitions| also deletes the
    // |GenericTransition|.
    transitions.erase(gt);
  }

  void merge_into(GenericTransition *a, GenericTransition *b) {
    a->merge(*b);

    insert_transition(a, b);
  }

  std::function<SymbolicPacket(const SymbolicPacket &)> get_function(
      XMASFunction *c) {
    if (!backwards_) {
      auto ext = c->getComponentExtension<
          SymbolicFunctionExtension<std::vector<SymbolicPacket>(const std::vector<SymbolicPacket> &)>>(
          false);

      assert(ext);
      return [=](const SymbolicPacket &p) { return ext->function(std::vector<SymbolicPacket>({p}))[0]; };
    } else {
      auto ext = c->getComponentExtension<SymbolicInverseFunctionExtension<
          SymbolicPacket(const SymbolicPacket &)>>(false);

      assert(ext);
      return ext->function;
    }
  }

  SymbolicFunction get_sym_function(XMASFunction *c) {
    if (!backwards_) {
      auto ext = c->getComponentExtension<RealSymbolicFunctionExtension>(false);

      assert(ext);
      return ext->function;
    } else {
      auto ext = c->getComponentExtension<RealSymbolicInverseFunctionExtension>(false);

      assert(ext);
      return ext->function;
    }
  }

  bool does_join_take_a(XMASJoin *c) {
    auto *ext = c->getComponentExtension<SymbolicFunctionExtension<
        SymbolicPacket(const SymbolicPacket &, const SymbolicPacket &)>>(false);

    assert(ext);
    static auto a = SymbolicPacket{NAMED_ENUM("first", "a")};
    static auto b = SymbolicPacket{NAMED_ENUM("first", "b")};

    return ext->function(a, b) == a;
  }

  void add(GenericTransition *t, SymbolicFunction f, Port *p) {
    auto ts = get_transitions(p);

    ts->emplace(t, std::move(f));

    transition_lists.emplace(t, ts);
  }

  TransitionPacketList* get_transitions(Port *p) {
    auto ext =
        p->getInitiatorPort()->getPortExtension<TransitionContainer>(false);

    return &p->getInitiatorPort()->getPortExtension<TransitionContainer>()->transitions;
  }

  bool backwards_;

  std::unordered_multimap<GenericTransition *, TransitionPacketList *>
      transition_lists;
};


class TotalOrderBuilder : public XMASComponentVisitor {
 public:
  TotalOrderBuilder(const std::set<XMASComponent *> &components, bool back)
      : back_(back) {
    if (back) {
      get_next_component = backwards;
    } else {
      get_next_component = forwards;
    }

    init = true;
    for (auto c : components) {
      c->clearComponentExtension<VisitedExtension>();
      c->accept(*this);
    }

    build();
  }
  std::vector<XMASComponent *> result;

 private:
  void visit(XMASQueue *c) {
    if (init) {
      next.push_back(c);
      return;
    }

    if (!visited(c)) {
      next.push_back(get_next_component(c,0));
    }

    result.push_back(c);
    set_visited(c);
  }

  void visit(XMASFunction *c) {
    if (init) return;

    next.push_back(get_next_component(c, 0));
    result.push_back(c);
    set_visited(c);
  }

  void visit(XMASSink *c) {
    if (init) {
      if (back_) next.push_back(c);

      return;
    }

    if (back_) next.push_back(get_next_component(c, 0));

    result.push_back(c);
    set_visited(c);
  }

  void visit(XMASSource *c) {
    if (init) {
      if (!back_) next.push_back(c);

      return;
    }

    if (!back_) next.push_back(get_next_component(c, 0));

    result.push_back(c);
    set_visited(c);
  }

  void visit(XMASJoin *c) {
    if (init) return;

    if (back_) {
      next.push_back(backwards(c, 0));
      next.push_back(backwards(c, 1));
    } else if (!visited(c) && visited(backwards(c, 0)) &&
               visited(backwards(c, 1))) {
      next.push_back(forwards(c, 0));
    } else {
      // wait for the next visit.
      return;
    }

    result.push_back(c);
    set_visited(c);
  }

  void visit(XMASMerge *c) {
    if (init) return;

    if (back_) {
      next.push_back(backwards(c, 0));
      next.push_back(backwards(c, 1));
    } else if (!visited(c) && visited(backwards(c, 0)) &&
               visited(backwards(c, 1))) {
      next.push_back(forwards(c, 0));
    } else {
      // wait for the next visit.
      return;
    }

    result.push_back(c);
    set_visited(c);
  }

  void visit(XMASSwitch *c) {
    if (init) return;

    if (!back_) {
      next.push_back(forwards(c, 0));
      next.push_back(forwards(c, 1));
    } else if (!visited(c) && visited(forwards(c, 0)) &&
               visited(forwards(c, 1))) {
      next.push_back(backwards(c, 0));
    } else {
      // wait for the next visit.
      return;
    }

    result.push_back(c);
    set_visited(c);
  }

  void visit(XMASFork *c) {
    if (init) return;

    if (!back_) {
      next.push_back(forwards(c, 0));
      next.push_back(forwards(c, 1));
    } else if (!visited(c) && visited(forwards(c, 0)) &&
               visited(forwards(c, 1))) {
      next.push_back(backwards(c, 0));
    } else {
      // wait for the next visit.
      return;
    }

    result.push_back(c);
    set_visited(c);
  }

  bool init;
 
  void build() {
    init = false;

    while (!next.empty()) {
      auto c = next.front();
      next.pop_front();

      c->accept(*this);
    }
  }

  std::function<XMASComponent *(XMASComponent *, size_t)> get_next_component;

  bool back_;
  std::deque<XMASComponent *> next;
};

std::vector<XMASComponent *>
total_order(const std::set<XMASComponent *> &components, bool back) {

  // note that a queue occurs twice in the total order: first for the output of
  // the queue, second for the input of the queue.
  auto total_order = TotalOrderBuilder(components, back);
  return total_order.result;
}

} // END namespace anonymous

std::vector<GenericTransition> compute_transitions(
    const std::set<XMASComponent *> &allComponents, bool backwards = false) {

  auto total = total_order(allComponents, backwards);

  // std::cerr << "total order:" << std::endl;
  // for (auto c : total) {
  //   std::cerr << c->getName() << " ";
  // }
  // std::cerr << std::endl << std::endl;

  ComputeTransitions visitor(total, backwards);

  std::vector<GenericTransition> result;
  for (auto &pair : visitor.transitions) {
    result.push_back(std::move(*pair.second));
  }

  // int i = 0;
  // for (auto &gt : result) {
  //   std::cerr << "T" << i++ << " ";
  //   gt.print(std::cerr);
  //   std::cerr << std::endl;
  // }

  return result;
}

// std::vector<GenericTransition>
// compute_reverse_transitions(std::set<XMASComponent *> allComponents, bool print) {
//   ComputeReverseTransitions visitor;
//   for (auto component : allComponents) {
//     // std::cerr << component << std::endl;
//     component->accept(visitor);
//   }

//   if (print) {
//     int i = 0;
//     for (auto &t : visitor.reverseTransitions) {
//       // TODO(snnw): re-enable printing      
//       // std::cerr << i++ << ": " << t;
//     }
//   }

//   return std::move(visitor.reverseTransitions);
// }

namespace {

inline bool set_contains(const std::unordered_set<size_t>& S, size_t s) {
  return S.find(s) != S.end();
}

} // END namespace anonymous

bool isReachable(const std::set<XMASComponent *> &allComponents,
                 const XMASState &s, reachable_flags flags) {
  std::cerr << std::endl;
  if (s.initial_state()) {
    return true;
  }

  auto transitions =
      compute_transitions(allComponents, flags & reachable_reverse);

  size_t s_hash = s.hash();
  auto reached_states = std::unordered_set<size_t>{};
  reached_states.insert(s_hash);
  auto next_states = std::deque<XMASState>{s};

  while (!next_states.empty()) {
    auto &s = next_states.front();

    for (auto &generic_rev_trans : transitions) {
      for (auto trans : generic_rev_trans.transitions(s)) {
        if (!trans.is_enabled()) {
          continue;
        }

        auto s_prime = trans.execute();
        auto s_prime_hash = s_prime.hash();
        // std::cerr << s_prime_hash << std::endl;

        if (s_prime.initial_state()) {
          return true;
        }

        if (!set_contains(reached_states, s_prime_hash)) {
          if (flags & reachable_bfs) {
            next_states.push_back(std::move(s_prime));
          } else {
            next_states.push_front(std::move(s_prime));
          }
          // if (next_states.size() % 100 == 0) {
          //   std::cerr << next_states.size();
          // }
          reached_states.insert(s_prime_hash);
          if (reached_states.size() % 100 == 99) {
            std::cerr << ".";
          }
        }
      }
    }
    next_states.pop_front();

  }
  std::cerr << std::endl << "states: " << reached_states.size() << std::endl;

  return false;
}
