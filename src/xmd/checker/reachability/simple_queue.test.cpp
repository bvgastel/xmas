
#include "test_util.h"

#include "../symbolic-enum-field.h"
#include "../state.h"
#include "../xmas.h"

#include "reachable-private.h"
#include "reachable.h"

class XMAS_SIMPLE_QUEUE : public TestNetwork {
 protected:
  XMASSink sink{"sink"};
  XMASQueue q{"queue"};
  XMASSource src{"source"};

  void SetUp() override {
    connect(q.o, sink.i);
    connect(src.o, q.i);

    components.insert(&sink);
    components.insert(&q);
    components.insert(&src);

    XMASState state;
    state.add_queue(&q, {});
    reachable_states.insert({"00_initial_state", std::move(state)});

    auto packet = SymbolicPacket({NAMED_ENUM("first", "a")});

    state = XMASState{};
    state.add_queue(&q, {{packet}});
    reachable_states.insert({"01_one_a", std::move(state)});
  }
};

TEST_F(XMAS_SIMPLE_QUEUE, TRANSITION_BUILDER) {
  auto packet = SymbolicPacket({NAMED_ENUM("first", "a")});
  attach(&src.o, packet);

  SymbolicTypes(components);

  auto rev_transitions = compute_transitions(components, true);

  EXPECT_EQ(2, rev_transitions.size());

  // queue to sink:
  auto &t1 = rev_transitions[1];
  EXPECT_EQ(0, t1.conditions().size());
  EXPECT_EQ(1, t1.out().size());  // sink
  EXPECT_EQ(0, t1.out_queues().size());
  EXPECT_EQ(1, t1.in_queues().size());  // q

  // source to queue
  auto &t2 = rev_transitions[0];
  EXPECT_EQ(1, t2.conditions().size()); // queue type check
  EXPECT_EQ(0, t2.out().size());
  EXPECT_EQ(1, t2.out_queues().size());  // q
  EXPECT_EQ(0, t2.in_queues().size());

  XMASState state;

  state.add_queue(&q, {{packet}});

  auto transitions = t2.transitions(state);
  EXPECT_EQ(1, transitions.size());
  auto trans0 = *transitions.begin();
  EXPECT_TRUE(trans0.is_enabled());

  auto new_state = trans0.execute();
  EXPECT_TRUE(new_state.initial_state());

  state = XMASState{};
  state.add_queue(&q, {{SymbolicPacket({NAMED_ENUM("first", "b")})}});

  auto transitions2 = t2.transitions(state);
  EXPECT_EQ(1, transitions2.size());
  auto trans1 = *transitions2.begin();
  EXPECT_FALSE(trans1.is_enabled());
}

TEST_F(XMAS_SIMPLE_QUEUE, REACHABLE) {
  auto packet = SymbolicPacket({NAMED_ENUM("first", "a")});
  attach(&src.o, packet);

  auto state = XMASState{};
  state.add_queue(&q, {{SymbolicPacket({NAMED_ENUM("first", "b")})}});
  unreachable_states.insert({"02_one_b", std::move(state)});

  checkAll("explicit search", bfs_backward_checker);
}

TEST_F(XMAS_SIMPLE_QUEUE, NUSMV) {
  auto packet = SymbolicPacket({NAMED_ENUM("first", "a")});
  attach(&src.o, packet);

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}
