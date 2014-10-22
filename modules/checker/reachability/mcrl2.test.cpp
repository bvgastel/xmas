
#include <gtest/gtest.h>

#include "../cycle.h"
#include "../parse.h"
#include "../state.h"
#include "../symbolic-function.h"
#include "../symbolic-enum-field.h"
#include "../xmas.h"

#include "mcrl2.h"

namespace {

TEST(XMAS_MCRL2, SIMPLE_QUEUE) {
  XMASSink sink("sink");
  XMASQueue q("queue");
  XMASSource src("source");

  auto network = std::set<XMASComponent*>{&sink, &q, &src};

  connect(q.o, sink.i);
  connect(src.o, q.i);

  auto packet = SymbolicPacket({NAMED_ENUM("first", "a")});

  attach(&src.o, packet);
  SymbolicTypes(network);

  XMASState state;
  state.add_queue(&q, {});

  print_mcrl2(std::cerr, network, state);

  // std::cerr << std::endl << "==========" << std::endl << std::endl;

  state = XMASState{};
  state.add_queue(&q, {{packet}});

  print_mcrl2(std::cerr, network, state);
}

TEST(XMAS_MCRL2, TYPICAL) {
  XMASSink s0("s0");
  XMASSink s1("s1");

  XMASQueue q0("q0",2);
  XMASQueue q1("q1",2);

  XMASSource src0a("src0a");
  XMASSource src0b("src0b");
  XMASSource src1("src1");

  XMASFork fork("f");
  XMASMerge merge("m");
  XMASMerge src_merge("sm");

  XMASJoin join("j");
  XMASSwitch sw("sw");

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
  // , {NAMED_ENUM("type", "req")}});

  attach(&src1.o, {NAMED_ENUM("type", "req")});

  auto network = std::set<XMASComponent *>{&s0,    &s1,    &q0,   &q1,
                                           &src0a, &src0b, &src1, &src_merge,
                                           &fork,  &merge, &join, &sw};

  for (auto &it : network) {
      // std::cout << *it << std::endl;
      checkAssert(it->valid());
  }

  SymbolicTypes(network);

  auto state = XMASState{};
  state.add_queue(&q0, {});
  state.add_queue(&q1, {});

  // print_mcrl2(std::cerr, network, state);

  state = XMASState{};
  state.add_queue(&q0, {{{NAMED_ENUM("type", "rsp")}}});
  state.add_queue(&q1, {{{NAMED_ENUM("type", "rsp")}}});

  // print_mcrl2(std::cerr, network, state);

  state = XMASState{};
  state.add_queue(&q0, {{{NAMED_ENUM("type", "rsp")}}});
  state.add_queue(&q1, {});

  // print_mcrl2(std::cerr, network, state);

  state = XMASState{};
  state.add_queue(&q0, {{{NAMED_ENUM("type", "rsp")}},
                        {{NAMED_ENUM("type", "rsp")}}});
  state.add_queue(&q1, {});

  // NOT REACHABLE (yay!)
  print_mcrl2(std::cerr, network, state);
}

void set_queue_size(XMASComponent *c, size_t n) {
  auto *queue = dynamic_cast<XMASQueue *>(c);
  assert(queue);
  queue->c = n;
}

void join_first(XMASComponent *c) {
  auto *join = dynamic_cast<XMASJoin *>(c);
  assert(join);
  attachFunction(
      join, [](const SymbolicPacket &a, const SymbolicPacket &b) { return a; });
}

void join_second(XMASComponent *c) {
  auto *join = dynamic_cast<XMASJoin *>(c);
  assert(join);
  attachFunction(
      join, [](const SymbolicPacket &a, const SymbolicPacket &b) { return b; });
}

void switch_type(XMASComponent *c, const std::vector<SymbolicPacket>& ps) {
  auto *sw = dynamic_cast<XMASSwitch *>(c);
  assert(sw);
  for (auto p : ps) {
    attachSwitchingFunction(sw, p);
  }
}

void source_attach(XMASComponent *c, const std::vector<SymbolicPacket>& ps) {
  auto *src = dynamic_cast<XMASSource *>(c);
  assert(src);
  attachPackets(&src->o, ps);
}

template <class T>
void function_inverse(XMASComponent *c, T &&f) {
  auto *function = dynamic_cast<XMASFunction *>(c);
  assert(function);
  attachInverseFunction(function, f);

  // TODO: FIXME: EMPTY FUNCTION
  attachSymbolicInverseFunction(function, SymbolicFunction());
}

template <class T>
void function_normal(XMASComponent *c, T &&f) {
  auto *function = dynamic_cast<XMASFunction *>(c);
  assert(function);
  attachFunction(function, f);
}


TEST(XMAS_MCRL2, TWOAGENTS) {
  MemoryPool mp;
 
  auto components = Parse("../reachability/twoagents.fjson", mp);

  auto X = 2;
  auto B = 2;
  auto C = 2;
  auto Cbig = 2;

  auto queues = std::vector<XMASComponent *>{
      components["dx1"],    components["dx2"],    components["cx1"],
      components["cx2"],    components["cx3"],    components["cx4"],
      components["P_iq1"],  components["P_cq1"],  components["P_iq2"],
      components["P_cq2"],  components["Q_iq1"],  components["Q_cq1"],
      components["Q_iq2"],  components["Q_cq2"],  components["cc1_q2"],
      components["cc2_q2"], components["cc3_q2"], components["cc4_q2"]};

  // FUCK ME
  join_first(components["join0"]);
  set_queue_size(components["dx1"], X);
  set_queue_size(components["P_iq1"], B);
  set_queue_size(components["cx1"], X);
  set_queue_size(components["P_cq1"], C);
  set_queue_size(components["P_iq2"], B);
  set_queue_size(components["cx2"], X);
  set_queue_size(components["P_cq2"], C);
  join_second(components["join14"]);
  join_second(components["join16"]);
  join_first(components["join17"]);
  set_queue_size(components["dx2"], X);
  set_queue_size(components["cx3"], X);

  set_queue_size(components["Q_iq1"], B);
  set_queue_size(components["Q_iq2"], B);
  set_queue_size(components["Q_cq1"], Cbig);
  set_queue_size(components["Q_cq2"], Cbig);
  set_queue_size(components["cx4"], X);

  function_normal(components["f34"], [](const std::vector<SymbolicPacket> &a) {
    return std::vector<SymbolicPacket>{SymbolicPacket{NAMED_ENUM("type", "b")}};
  });

  auto sym_ext =
      components["f34"]
          ->getComponentExtension<RealSymbolicFunctionExtension>();

  sym_ext->function = SymbolicFunction();
  auto op = std::make_shared<ConstantOp>();
  op->p = SymbolicPacket{NAMED_ENUM("type", "b")};
  sym_ext->function.op = op;
  
  function_normal(components["f37"], [](const std::vector<SymbolicPacket> &a) {
    return std::vector<SymbolicPacket>{SymbolicPacket{NAMED_ENUM("type", "b")}};
  });

  sym_ext =
      components["f37"]->getComponentExtension<RealSymbolicFunctionExtension>();

  sym_ext->function = SymbolicFunction();
  op = std::make_shared<ConstantOp>();
  op->p = SymbolicPacket{NAMED_ENUM("type", "b")};
  sym_ext->function.op = op;

  // Set up the reverse functions.
  function_inverse(components["f34"], [](const SymbolicPacket &a) {
    return SymbolicPacket{NAMED_ENUM("type", "a")};
  });

  auto sym_inv_ext =
      components["f34"]
          ->getComponentExtension<RealSymbolicInverseFunctionExtension>();

  sym_inv_ext->function = SymbolicFunction();
  op = std::make_shared<ConstantOp>();
  op->p = SymbolicPacket{NAMED_ENUM("type", "a")};
  sym_inv_ext->function.op = op;

  function_inverse(components["f37"], [](const SymbolicPacket &a) {
    return SymbolicPacket{NAMED_ENUM("type", "a")};
  });

  sym_inv_ext =
      components["f37"]
          ->getComponentExtension<RealSymbolicInverseFunctionExtension>();

  sym_inv_ext->function = SymbolicFunction();
  op = std::make_shared<ConstantOp>();
  op->p = SymbolicPacket{NAMED_ENUM("type", "a")};
  sym_inv_ext->function.op = op;

  join_first(components["cc1_join1"]);
  set_queue_size(components["cc1_q2"], Cbig);

  join_first(components["cc2_join1"]);
  set_queue_size(components["cc2_q2"], Cbig);

  join_first(components["cc3_join1"]);
  set_queue_size(components["cc3_q2"], C);

  join_first(components["cc4_join1"]);
  set_queue_size(components["cc4_q2"], C);

  std::set<XMASComponent *> allComponents;
  for (auto &it : components) {
    // std::cout << it.first << ": " << it.second << std::endl;
    // std::cout << "checking " << it.first << std::endl;
    ASSERT_TRUE(it.second->valid());
    if (it.second)
      allComponents.insert(it.second);
  }

  ASSERT_TRUE(!CombinatorialCycleDetector(allComponents));

  SymbolicTypes(allComponents);

  XMASState initial_state = XMASState{};
  for (auto q: queues) {
    initial_state.add_queue(dynamic_cast<XMASQueue*>(q), {});
  }

  // print_mcrl2(std::cerr, allComponents, initial_state);

  auto state = XMASState{};
  for (auto q: queues) {
    if (q == components["cc1_q2"] || q == components["P_iq1"]) {
      state.add_queue(dynamic_cast<XMASQueue *>(q), {{SymbolicPacket{NAMED_ENUM("type", "a")}}});
    } else {
      state.add_queue(dynamic_cast<XMASQueue*>(q), {});
    }
  }

  print_mcrl2(std::cerr, allComponents, state);

  state = XMASState{};
  for (auto q: queues) {
    if (q == components["cc2_q2"]) {
      state.add_queue(dynamic_cast<XMASQueue *>(q), {{SymbolicPacket{NAMED_ENUM("type", "a")}}});
    } else if (q == components["P_iq2"]) {
      state.add_queue(dynamic_cast<XMASQueue *>(q), {{SymbolicPacket{NAMED_ENUM("type", "b")}}});
    } else {
      state.add_queue(dynamic_cast<XMASQueue*>(q), {});
    }
  }

  // print_nusmv(std::cerr, allComponents, state, nusmv_reverse);

  state = XMASState{};
  for (auto q: queues) {
    if (q == components["P_iq2"]) {
      state.add_queue(dynamic_cast<XMASQueue *>(q), {{SymbolicPacket{NAMED_ENUM("type", "b")}}});
    } else {
      state.add_queue(dynamic_cast<XMASQueue*>(q), {});
    }
  }

  // EXPECT FALSE
  // print_nusmv(std::cerr, allComponents, state, {});
}

} // namespace
