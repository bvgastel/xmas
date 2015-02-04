#ifndef TRANSITION_H
#define TRANSITION_H

#include <functional>
#include <ostream>
#include <unordered_map>

//#include <boost/optional.hpp>
// #include <optional>

#include "../state.h"
#include "../symbolic-function.h"

class GenericTransition;

class Transition {
 public:
  Transition(const GenericTransition &gen_trans,
             std::vector<SymbolicPacket> out_packets,
             const XMASState &state)
      : gt_(gen_trans),
        out_packets_(std::move(out_packets)),
        state_(state) {}

  XMASState execute() const;
  bool is_enabled() const;

 private:
  void build_map() const;

  mutable bool has_map = false;
  mutable bool enabled = false;
  mutable std::unordered_map<XMASComponent *, SymbolicPacket> map_;

  const GenericTransition &gt_;
  const std::vector<SymbolicPacket> out_packets_;
  const XMASState &state_;
};

struct trans_iterator {
  friend bool operator!=(const trans_iterator&, const trans_iterator&);

  trans_iterator(const XMASState &s, const GenericTransition &gt,
                 const std::vector<int> &sizes, int i)
      : s_(s), gt_(gt), sizes_(sizes), permutation_(i) {}

  Transition operator*() const;

  trans_iterator& operator++() {
    permutation_++;
    return *this;
  }

 private:
  const XMASState &s_;
  const GenericTransition &gt_;
  const std::vector<int>& sizes_;
  int permutation_ = 0;
};

class Transitions {
 public:
  Transitions(const XMASState &s, const GenericTransition &gt);

  trans_iterator begin() const {
    return trans_iterator(s_, gt_, sizes_, 0);
  }

  trans_iterator end() const {
    return trans_iterator(s_, gt_, sizes_, product_);
  }

  size_t size() const {
    return product_;
  }

 private:
  const XMASState &s_;
  const GenericTransition &gt_;
  std::vector<int> sizes_;
  int product_ = 1;
};

class GenericTransition {
 public:
  void set_id(size_t i) { identifier = i; }
  size_t id() const { return identifier; }

  void add_out(XMASComponent *c, Port *p,
               const std::vector<SymbolicPacket> *o) {
    out_.emplace_back(c, p);
    out_packets_.push_back(o);
  }

  void add_out_queue(XMASQueue *q) {
    out_queues_.push_back(q);
  }

  void add_in(XMASQueue *q, SymbolicFunction f) {
    in_queues_.push_back(q);
    in_funcs_.push_back(std::move(f));
  }

  void add_condition(SymbolicCondition cond) {
    cond_.push_back(cond);
  }

  const std::vector<std::pair<XMASComponent *, Port *>> &out() const {
    return out_;
  }

  const std::vector<const std::vector<SymbolicPacket> *> &out_packets() const {
    return out_packets_;
  }

  const std::vector<XMASQueue *> &out_queues() const {
    return out_queues_;
  }

  const std::vector<XMASQueue *> &in_queues() const {
    return in_queues_;
  }

  const std::vector<SymbolicFunction> &in_functions() const {
    return in_funcs_;
  }

  const std::vector<SymbolicCondition> &conditions() const {
    return cond_;
  }

  Transitions transitions(const XMASState &s) {
    return Transitions(s, *this);
  }

  void merge(const GenericTransition &other);

  void print(std::ostream &);

 private:
  size_t identifier;
  std::vector<std::pair<XMASComponent *, Port*>> out_;
  std::vector<const std::vector<SymbolicPacket>*> out_packets_;
  std::vector<XMASQueue *> out_queues_;
  std::vector<XMASQueue *> in_queues_;
  std::vector<SymbolicFunction> in_funcs_;
  std::vector<SymbolicCondition> cond_;
};


// class Transition {
//  public:
//   using Packet = XMASState::Packet;
//   using InputPackets = std::unordered_map<XMASComponent *, Packet>;
//   using OutputFunction = std::function<Packet(InputPackets)>;
//   using ConditionFunction = std::function<bool(InputPackets)>;

//   // FIXME: stub!
//   void add_initiator_queue(XMASQueue *q) {}
//   void add_target_queue(XMASQueue *q, OutputFunction t) {}
//   //void add_sink(XMASSink *s /*, OutputFunction t */) {}
//   void add_source(XMASSource *s) {}

//   // FIXME: stub!
//   bool is_enabled(const XMASState& current) const { return false; }
//   XMASState execute(const XMASState& current) const { return current; }
//   InputPackets execute(XMASState *current) const { return InputPackets{}; }
//   void undo(XMASState *current, const InputPackets& p) const {}
//   void add_condition(ConditionFunction f) {}

//  private:
//   std::vector<ConditionFunction> _conditions;
//   std::vector<XMASQueue *> _outgoing_queues;
//   std::unordered_map<XMASQueue *, OutputFunction> _new_packets;
// };

// class ReverseTransition {
//  public:
//   using Packet = XMASState::Packet;
//   using OutputPackets = std::unordered_map<XMASComponent *, Packet>;
//   using InputFunction = std::function<Packet(OutputPackets)>;
//   using ConditionFunction = std::function<bool(OutputPackets)>;

//   using QueueOperationsType =
//       std::unordered_map<XMASComponent *, ReverseTransition::Packet>;

//   // FIXME: stub!
//   void add_initiator_queue(XMASQueue *q, InputFunction t) {
//     _new_packets.insert({q, std::move(t)});
//   }

//   void add_target_queue(XMASQueue *q) {
//     _target_queues.push_back(q);
//   }

//   void add_sink(XMASComponent *s, int port_number) { _sinks.push_back({s, port_number}); }
//   void add_condition(ConditionFunction f) {
//     _conditions.push_back(std::move(f));
//   }

//   // FIXME: stub!
//   std::vector<QueueOperationsType> is_enabled(const XMASState &current) const;

//   XMASState execute(const XMASState &current,
//                     const QueueOperationsType &queue_ops) const;
//   void execute(XMASState *current, const QueueOperationsType &queue_ops) const;
//   void undo(XMASState *current, const QueueOperationsType &queue_ops) const;

//   const std::vector<ConditionFunction> &get_conditions() const {
//     return _conditions;
//   }
//   const std::vector<std::pair<XMASComponent *, int>> &get_sinks() const {
//     return _sinks;
//   }
//   const std::vector<XMASQueue *> &get_target_queues() const {
//     return _target_queues;
//   }
//   const std::unordered_map<XMASQueue *, InputFunction> &get_new_packets() const {
//     return _new_packets;
//   }

//  private:
//   std::vector<ConditionFunction> _conditions;
//   std::vector<std::pair<XMASComponent *, int>> _sinks;
//   std::vector<XMASQueue *> _target_queues;
//   std::unordered_map<XMASQueue *, InputFunction> _new_packets;

//   std::vector<QueueOperationsType>
//   generate_queue_ops(QueueOperationsType) const;
// };

// std::ostream &operator <<(std::ostream &out, const ReverseTransition &t);

#endif