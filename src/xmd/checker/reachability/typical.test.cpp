
#include "test_util.h"

#include "../state.h"
#include "../symbolic-enum-field.h"
#include "../xmas.h"

#include "reachable.h"

class XMAS_TYPICAL : public TestNetwork {
 protected:
  void SetUp() override {
    connect(src0a.o, src_merge.a);
    connect(src0b.o, src_merge.b);
    connect(src_merge.o, fork.i);
    connect(src1.o, merge.b);
    connect(fork.a, q0.i);
    connect(fork.b, merge.a);
    connect(merge.o, q1.i);

    connect(q0.o, join.a);
    connect(q1.o, sw.i);
    connect(sw.a, join.b);
    connect(sw.b, s1.i);
    connect(join.o, s0.i);

    attachSwitchingFunction(&sw, {NAMED_ENUM("type", "rsp")});
    attachFunction(&join, [](const SymbolicPacket &a,
                             const SymbolicPacket &b) { return b; });

    attach(&src0a.o, {NAMED_ENUM("type", "rsp")});
    attach(&src0b.o, {NAMED_ENUM("type", "req")});
    attach(&src1.o, {NAMED_ENUM("type", "req")});

    components = {&s0,   &s1,        &q0,   &q1,    &src0a, &src0b,
                  &src1, &src_merge, &fork, &merge, &join,  &sw};

    auto state = XMASState{};
    state.add_queue(&q0, {});
    state.add_queue(&q1, {});
    reachable_states.insert({"initial state", std::move(state)});

    state = XMASState{};
    state.add_queue(&q0, {{{NAMED_ENUM("type", "rsp")}}});
    state.add_queue(&q1, {{{NAMED_ENUM("type", "rsp")}}});
    reachable_states.insert({"rsp in each queue", std::move(state)});

    state = XMASState{};
    state.add_queue(&q0, {{{NAMED_ENUM("type", "rsp")}}});
    state.add_queue(&q1, {});
    reachable_states.insert({"rsp in queue 0", std::move(state)});

    state = XMASState{};
    state.add_queue(
        &q0, {{{NAMED_ENUM("type", "rsp")}}, {{NAMED_ENUM("type", "rsp")}}});
    state.add_queue(&q1, {});
    unreachable_states.insert({"two rsp in queue 0", std::move(state)});
  }

  XMASSink s0{"s0"};
  XMASSink s1{"s1"};

  XMASQueue q0{"q0", 2};
  XMASQueue q1{"q1", 2};

  XMASSource src0a{"src0a"};
  XMASSource src0b{"src0b"};
  XMASSource src1{"src1"};

  XMASFork fork{"f"};
  XMASMerge merge{"m"};
  XMASMerge src_merge{"sm"};

  XMASJoin join{"j"};
  XMASSwitch sw{"sw"};
};

TEST_F(XMAS_TYPICAL, REACHABLE) {
  checkAll("explicit search", bfs_backward_checker);
}

TEST_F(XMAS_TYPICAL, NUSMV) {
  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}