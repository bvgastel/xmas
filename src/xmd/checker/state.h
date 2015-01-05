
#ifndef STATE_H
#define STATE_H

#include "symbolic.h"
#include "xmas.h"

using PacketExpression = std::vector<SymbolicPacket>;

class XMASState {
 public:
  XMASState() {};

  // Returns the initial state.
  XMASState(const std::set<XMASComponent *> &network);

  // TODO: rename to set_queue
  void add_queue(const XMASQueue *q, std::deque<PacketExpression> ps) {
    state_[q] = std::move(ps);
  }

  std::deque<PacketExpression>const &get_queue(XMASQueue *q) const {
    return state_.at(q);
  }

  void push(XMASQueue *q, SymbolicPacket p) {
    state_.at(q).push_front({std::move(p)});
  }

  bool empty(XMASQueue *q) const {
    return state_.at(q).empty();
  }

  bool full(XMASQueue *q) const {
    return state_.at(q).size() >= q->c;
  }

  SymbolicPacket top(XMASQueue *q) const {
    return state_.at(q).back()[0];
  }

  void pop(XMASQueue *q) {
    state_.at(q).pop_back();
  }

  size_t hash() const;
  bool initial_state() const;

 private:
  std::map<const XMASQueue *, std::deque<PacketExpression>> state_;
};

#endif