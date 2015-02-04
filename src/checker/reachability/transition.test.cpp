#include <gtest/gtest.h>

#include "../state.h"
#include "../symbolic-enum-field.h"
#include "../symbolic.h"
#include "../xmas.h"

#include "transition.h"


namespace {

// TEST(XMAS_TRANSITION, INTERFACE) {
//   auto reverse_trans = ReverseTransition{};

//   XMASSink sink("sink");
//   XMASQueue q("queue");
//   XMASSource src("source");

//   connect(q.o, sink.i);
//   connect(src.o, q.i);

//   attach(&src.o,SymbolicPacket({NAMED_ENUM("first", "a")}));
//   SymbolicTypes({&sink, &q, &src});

//   XMASState state = XMASState({&sink, &q, &src});
//   ASSERT_EQ(1, state.queue_capacity(&q));

//   auto queue_sink_trans = ReverseTransition{};
//   queue_sink_trans.add_sink(&sink, 0);
//   queue_sink_trans.add_initiator_queue(&q, [&sink](ReverseTransition::OutputPackets in) { return in[&sink]; });

//   auto src_queue_trans = ReverseTransition{};
//   // src_queue_trans.add_source(&src);
//   src_queue_trans.add_target_queue(&q);
  
//   auto queue_ops1 = queue_sink_trans.is_enabled(state);
//   auto queue_ops2 = src_queue_trans.is_enabled(state);

//   // is enabled?
//   EXPECT_TRUE(!queue_ops1.empty());
//   EXPECT_FALSE(!queue_ops2.empty());
  
//   XMASState old_state = state;

//   queue_sink_trans.execute(&state, queue_ops1[0]);

//   auto queue_ops3 = queue_sink_trans.is_enabled(state);
//   auto queue_ops4 = src_queue_trans.is_enabled(state);

//   EXPECT_FALSE(!queue_ops3.empty());
//   EXPECT_TRUE(!queue_ops4.empty());
//   EXPECT_EQ(1, state.queue_size(&q));

//   queue_sink_trans.undo(&state, queue_ops1[0]);

//   EXPECT_EQ(old_state, state);
// }

// TEST(XMAS_TRANSITION, COPY) {
//   auto reverse_trans = ReverseTransition{};

//   XMASSink sink("sink");
//   XMASQueue q("queue");
//   XMASSource src("source");

//   connect(q.o, sink.i);
//   connect(src.o, q.i);

//   attach(&src.o,SymbolicPacket({NAMED_ENUM("first", "a")}));
//   SymbolicTypes({&sink, &q, &src});

//   XMASState state = XMASState({&sink, &q, &src});
//   ASSERT_EQ(1, state.queue_capacity(&q));

//   auto queue_sink_trans = ReverseTransition{};
//   queue_sink_trans.add_sink(&sink, 0);
//   queue_sink_trans.add_initiator_queue(&q, [&sink](ReverseTransition::OutputPackets in) { return in[&sink]; });

//   auto src_queue_trans = ReverseTransition{};
//   // src_queue_trans.add_source(&src);
//   src_queue_trans.add_target_queue(&q);
  
//   auto queue_ops1 = queue_sink_trans.is_enabled(state);
//   auto queue_ops2 = src_queue_trans.is_enabled(state);

//   // is enabled?
//   EXPECT_TRUE(!queue_ops1.empty());
//   EXPECT_FALSE(!queue_ops2.empty());

//   auto new_state = queue_sink_trans.execute(state, queue_ops1[0]);

//   auto queue_ops3 = queue_sink_trans.is_enabled(new_state);
//   auto queue_ops4 = src_queue_trans.is_enabled(new_state);

//   EXPECT_FALSE(!queue_ops3.empty());
//   EXPECT_TRUE(!queue_ops4.empty());
//   EXPECT_EQ(1, new_state.queue_size(&q));

//   queue_sink_trans.undo(&new_state, queue_ops1[0]);

//   EXPECT_EQ(state, new_state);
// }
}