
#include "transition.h"

#include "../lib/zip.h"
#include "../symbolic-enum-field.h"

void GenericTransition::merge(const GenericTransition &gt) {
  out_.insert(out_.end(), gt.out_.begin(), gt.out_.end());
  out_packets_.insert(out_packets_.end(), gt.out_packets_.begin(),
                      gt.out_packets_.end());
  out_queues_.insert(out_queues_.end(), gt.out_queues_.begin(),
                     gt.out_queues_.end());
  in_queues_.insert(in_queues_.end(), gt.in_queues_.begin(),
                    gt.in_queues_.end());
  in_funcs_.insert(in_funcs_.end(), gt.in_funcs_.begin(), gt.in_funcs_.end());
  cond_.insert(cond_.end(), gt.cond_.begin(), gt.cond_.end());
}

void GenericTransition::print(std::ostream &o) {
  o << "(";
  bool first = true;
  for (auto &out : out_) {
    o << (first ? "" : ", ") << out.first->getName();
    first = false;
  }
  for (auto &q : out_queues_) {
    o  << (first ? "" : ", ") << q->getName();
    first = false;
  }
  o << ") --> (";
  first = true;
  for (auto &q : in_queues_) {
    o << (first ? "" : ", ") << q->getName();
    first = false;
  }
  o << ")";
}

static
const std::vector<SymbolicEnumField::Type>&
get_first_enum_field_values(const SymbolicPacket& p) {
  // FIXME: assuming there is just one field.
  auto &field = *p.fields.begin();

  auto enum_field = dynamic_cast<SymbolicEnumField *>(field.second.get());

  return enum_field->values;
}

Transition trans_iterator::operator*() const {
  std::vector<SymbolicPacket> choices;

  int counter = permutation_;
  for (auto pair : zip(sizes_, gt_.out_packets())) {
    auto choice = counter % pair.first;

    for (auto &p : *pair.second) {
      auto values = get_first_enum_field_values(p);
      
      if (values.size() > choice) {
        auto &field = *p.fields.begin();

        choices.push_back(
            SymbolicPacket{NAMED_ENUM(field.first, values[choice])});
        break;
      } else {
        choice -= values.size();
      }
    }

    counter /= pair.first;
  }

  return Transition(gt_, {std::move(choices)}, s_);
}

bool operator!=(const trans_iterator &a, const trans_iterator &b) {
  return a.permutation_ != b.permutation_;
}

Transitions::Transitions(const XMASState &s, const GenericTransition &gt)
    : s_(s), gt_(gt) {

  // for every source / sink
  for (auto &ps : gt.out_packets()) {
    sizes_.push_back(0);

    // for every symbolic packet
    for (auto &p : *ps) {
      auto &values = get_first_enum_field_values(p);

      // for every possible enum field value
      sizes_.back() += values.size();
    }

    product_ *= sizes_.back();
  }
}

void Transition::build_map() const {
  if (has_map) {
    return;
  }

  for (auto q : gt_.out_queues()) {
    map_.insert({q, state_.top(q)});
  }

  for (auto pair : zip(gt_.out(), out_packets_)) {
    map_.emplace(pair.first.first, pair.second);
  }

  has_map = true;
}

bool Transition::is_enabled() const {
  for (auto q : gt_.out_queues()) {
    if (state_.empty(q)) {
      return false;
    }
  }

  for (auto q : gt_.in_queues()) {
    if (state_.full(q)) {
      return false;
    }
  }

  build_map();

  for (auto &c: gt_.conditions()) {
    if (!c(map_)) {
      return false;
    }
  }

  enabled = true;
  return true;
}

XMASState Transition::execute() const {
  assert(has_map);
  assert(enabled);

  XMASState result(state_);

  for (auto q : gt_.out_queues()) {
    result.pop(q);
  }

  for (auto pair: zip(gt_.in_queues(), gt_.in_functions())) {
    result.push(pair.first, pair.second(map_));
  }

  return std::move(result);
}

// // FIXME: utility header
// const std::vector<SymbolicPacket> &port_types(Port *port);

// const std::vector<SymbolicPacket> &input_types(XMASComponent *t, size_t i);

// std::vector<ReverseTransition::QueueOperationsType>
// generate_queue_ops(ReverseTransition::QueueOperationsType base, const std::vector<std::pair<XMASComponent *, int>> &sinks,
//                    int i) {
//   std::vector<ReverseTransition::QueueOperationsType> result;

//   if (i >= sinks.size()) {
//     return {std::move(base)};
//   }

//   // std::cerr << "num input types: "
//   //           << input_types(sinks[i].first, sinks[i].second).size() << std::endl;

//   for (auto &p : input_types(sinks[i].first, sinks[i].second)) {
//     auto newBase = base;

//     // FIXME: assuming there is just one field.
//     auto &field = *p.fields.begin();

//     auto enum_field = dynamic_cast<SymbolicEnumField*>(field.second.get());

//     for (auto &value : enum_field->values) {
//       newBase.insert({
//         sinks[i].first, SymbolicPacket{NAMED_ENUM(field.first, value)}});
//     }

//     auto ops = generate_queue_ops(newBase, sinks, i + 1);
//     result.insert(result.end(), ops.begin(), ops.end());
//   }

//   return std::move(result);
// }

// std::vector<ReverseTransition::QueueOperationsType>
// ReverseTransition::generate_queue_ops(QueueOperationsType base) const {
//   std::vector<QueueOperationsType> result;

//   result = ::generate_queue_ops(std::move(base), _sinks, 0);

//   return std::move(result);
// }

// auto ReverseTransition::is_enabled(
//     const XMASState &current) const -> std::vector<QueueOperationsType> {

//   std::vector<QueueOperationsType> enabled_operations;
//   std::vector<QueueOperationsType> queue_operations_list;

//   QueueOperationsType base;
//   base.reserve(_target_queues.size());

//   for (auto q : _target_queues) {
//     if (current.queue_empty(q)) {
//       return {};
//     }
 
//     base.insert({q, current.queue_back(q)});
//   }

//   queue_operations_list = generate_queue_ops(std::move(base));

//   for (auto &queue_operations : queue_operations_list) {

//     bool enabled = true;
//     for (auto elem : _new_packets) {
//       if (current.queue_full(elem.first)) {
//         enabled = false;
//       }
//     }

//     for (auto &f : _conditions) {
//       if (!f(queue_operations)) {
//         enabled = false;
//       }
//     }

//     if (enabled) {
//       enabled_operations.push_back(queue_operations);
//     }

//   }

//   return std::move(enabled_operations);
// }

// XMASState
// ReverseTransition::execute(const XMASState &current,
//                            const QueueOperationsType &queue_ops) const {
//   auto res = current;

//   execute(&res, queue_ops);

//   return std::move(res);
// }

// void ReverseTransition::execute(XMASState *current,
//                                 const QueueOperationsType &queue_ops) const {
//   for (auto q : _target_queues) {
//     // std::cerr << "pop: " << current->queue_back(q) << std::endl;
//     current->pop_back(q);
//   }

//   for (auto elem : _new_packets) {
//     current->push_front(elem.first, elem.second(queue_ops));
//   }
// }

// void ReverseTransition::undo(XMASState *current, const QueueOperationsType &queue_ops) const {
//   for (auto q : _target_queues) {
//     current->push_back(q, queue_ops.at(q));
//   }

//   for (auto elem: _new_packets) {
//     current->pop_front(elem.first);
//   }
// }

// std::ostream &operator <<(std::ostream &out, const ReverseTransition &t) {
//   out << "to:";
//   for (auto q : t.get_target_queues()) {
//     out << q << ",";
//   }
//   for (auto s : t.get_sinks()) {
//     out << s.first << ",";
//   }
//   out << " from:";
//   for (auto q : t.get_new_packets()) {
//     out << q.first << ",";
//   }
//   out << " c: " << t.get_conditions().size() << std::endl;
//   return out;
// }
