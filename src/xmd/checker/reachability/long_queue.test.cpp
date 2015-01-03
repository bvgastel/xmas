
#include <gtest/gtest.h>

#include "../cycle.h"
#include "../state.h"
#include "../symbolic-enum-field.h"

#include "nusmv.h"
#include "test_util.h"

namespace {

// Much better unit test structure.  TODO(snnw): convert the other examples to
// something similar.
// 
// The fixture creates the network in |SetUp()|, after the derived classes'
// constructor has run.  Parameters can be used to influence the construction
// of the network.  Attaching packets to sources is left 
class XMAS_LONG_QUEUE : public TestNetwork {
 protected:
  void SetUp() override {
    // Create the queues.
    for (auto i = 0; i < num_queues; ++i) {
      queues.emplace_back("q" + std::to_string(i));
    }

    // Create the channels, connecting everything.
    for (auto i = 1; i < num_queues; ++i) {
      connect(queues[i-1].o, queues[i].i);
    }

    connect(src.o, queues[0].i);
    connect(queues[num_queues - 1].o, sink.i);

    // Build the |components| set. 
    components.insert(&src);
    components.insert(&sink);
    for (auto &q : queues) {
      components.insert(&q);
    }

    // Create the states that we're going to test.
    XMASState state;
    // for (const auto &q : queues) {
    //   state.add_queue(&q, {});
    // }
    // reachable_states.insert({"00_initial_state", std::move(state)});

    state = XMASState{};
    for (const auto &q : queues) {
      state.add_queue(&q, {});
    }
    state.add_queue(&queues[num_queues - 1],
                    {{SymbolicPacket({NAMED_ENUM("first", "a")})}});
    reachable_states.insert({"01_a_in_last", std::move(state)});   
  }

  XMASSink sink{"sink"};
  XMASSource src{"src"};

  // It seems to be a bug in libstdc++ 4.8.3 that |std::vector|'s
  // |emplace_back| uses copy constructors to copy the array.  Unfortunately,
  // XMASQueue's are not allowed to be copied :(, because of the embedded
  // |Stack| for extensions.  Using a |std::deque| instead.
  //
  // See for reference https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51452 and
  // apparently also http://cplusplus.github.io/LWG/lwg-active.html#2116 .
  // It might even be a bug in the standard then.
  std::deque<XMASQueue> queues;

  int num_queues = 2;
};

class XMAS_LONG_QUEUE_5 : public XMAS_LONG_QUEUE {
 protected:
  XMAS_LONG_QUEUE_5() { num_queues = 5; }
};

class XMAS_LONG_QUEUE_10 : public XMAS_LONG_QUEUE {
 protected:
  XMAS_LONG_QUEUE_10() { num_queues = 10; }
};

class XMAS_LONG_QUEUE_15 : public XMAS_LONG_QUEUE {
 protected:
  XMAS_LONG_QUEUE_15() { num_queues = 15; }
};


class XMAS_LONG_QUEUE_20 : public XMAS_LONG_QUEUE {
 protected:
  XMAS_LONG_QUEUE_20() { num_queues = 20; }
};

class XMAS_LONG_QUEUE_25 : public XMAS_LONG_QUEUE {
 protected:
  XMAS_LONG_QUEUE_25() { num_queues = 25; }
};

TEST_F(XMAS_LONG_QUEUE_5, REACHABLE_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  checkAll("dfs backward", dfs_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_5, REACHABLE_TWO_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  checkAll("dfs backward", dfs_backward_checker);
}


TEST_F(XMAS_LONG_QUEUE_5, REACHABLE_FOUR_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "c")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "d")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  checkAll("dfs backward", dfs_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_10, REACHABLE_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  checkAll("dfs backward", dfs_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_10, REACHABLE_TWO_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  checkAll("dfs backward", dfs_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_15, REACHABLE_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  checkAll("dfs backward", dfs_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_15, REACHABLE_TWO_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));

  checkAll("bfs foward", bfs_forward_checker);
  // checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  // checkAll("dfs backward", dfs_backward_checker);
}


TEST_F(XMAS_LONG_QUEUE_20, REACHABLE_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  // checkAll("dfs backward", dfs_backward_checker);
}


TEST_F(XMAS_LONG_QUEUE_25, REACHABLE_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("bfs foward", bfs_forward_checker);
  checkAll("dfs forward", dfs_forward_checker);
  checkAll("bfs backward", bfs_backward_checker);
  // checkAll("dfs backward", dfs_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_10, NUSMV_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_10, NUSMV_TWO_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_10, NUSMV_FOUR_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "c")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "d")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}


TEST_F(XMAS_LONG_QUEUE_15, NUSMV_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_15, NUSMV_TWO_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_15, NUSMV_FOUR_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "c")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "d")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}



TEST_F(XMAS_LONG_QUEUE_20, NUSMV_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_20, NUSMV_TWO_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_20, NUSMV_FOUR_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "c")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "d")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  // checkAll("nusmv backward", nusmv_backward_checker);
}


TEST_F(XMAS_LONG_QUEUE_25, NUSMV_COUNTER) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_25, NUSMV_TWO_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_LONG_QUEUE_25, NUSMV_FOUR_ENUM) {
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "a")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "b")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "c")}));
  attach(&src.o, SymbolicPacket({NAMED_ENUM("first", "d")}));

  checkAll("nusmv forward", nusmv_forward_checker);
  // checkAll("nusmv backward", nusmv_backward_checker);
}


} // namespace