
#include "test_util.h"

#include "../cycle.h"
#include "../parse.h"
#include "../symbolic-function.h"
#include "../symbolic-enum-field.h"

namespace {

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

class XMAS_TWOAGENTS : public TestNetwork {
 private:
  // :(
  void setup_functions() {
    function_normal(network["f34"],
                    [](const std::vector<SymbolicPacket> &a) {
      return std::vector<SymbolicPacket>{
          SymbolicPacket{NAMED_ENUM("type", "b")}};
    });

    auto sym_ext = network["f34"]
                       ->getComponentExtension<RealSymbolicFunctionExtension>();

    sym_ext->function = SymbolicFunction();
    auto op = std::make_shared<ConstantOp>();
    op->p = SymbolicPacket{NAMED_ENUM("type", "b")};
    sym_ext->function.op = op;

    function_normal(network["f37"],
                    [](const std::vector<SymbolicPacket> &a) {
      return std::vector<SymbolicPacket>{
          SymbolicPacket{NAMED_ENUM("type", "b")}};
    });

    sym_ext = network["f37"]
                  ->getComponentExtension<RealSymbolicFunctionExtension>();

    sym_ext->function = SymbolicFunction();
    op = std::make_shared<ConstantOp>();
    op->p = SymbolicPacket{NAMED_ENUM("type", "b")};
    sym_ext->function.op = op;

    // Set up the reverse functions.
    function_inverse(network["f34"], [](const SymbolicPacket &a) {
      return SymbolicPacket{NAMED_ENUM("type", "a")};
    });

    auto sym_inv_ext =
        network["f34"]
            ->getComponentExtension<RealSymbolicInverseFunctionExtension>();

    sym_inv_ext->function = SymbolicFunction();
    op = std::make_shared<ConstantOp>();
    op->p = SymbolicPacket{NAMED_ENUM("type", "a")};
    sym_inv_ext->function.op = op;

    function_inverse(network["f37"], [](const SymbolicPacket &a) {
      return SymbolicPacket{NAMED_ENUM("type", "a")};
    });

    sym_inv_ext =
        network["f37"]
            ->getComponentExtension<RealSymbolicInverseFunctionExtension>();

    sym_inv_ext->function = SymbolicFunction();
    op = std::make_shared<ConstantOp>();
    op->p = SymbolicPacket{NAMED_ENUM("type", "a")};
    sym_inv_ext->function.op = op;
  }

 protected:
  void SetUp() override {
    network = Parse("../reachability/twoagents.fjson", mp);

    queues = {network["dx1"],    network["dx2"],    network["cx1"],
              network["cx2"],    network["cx3"],    network["cx4"],
              network["P_iq1"],  network["P_cq1"],  network["P_iq2"],
              network["P_cq2"],  network["Q_iq1"],  network["Q_cq1"],
              network["Q_iq2"],  network["Q_cq2"],  network["cc1_q2"],
              network["cc2_q2"], network["cc3_q2"], network["cc4_q2"]};

    set_queue_size(network["dx1"], transfer_queue_size);
    set_queue_size(network["dx2"], transfer_queue_size);

    set_queue_size(network["cx1"], transfer_queue_size);
    set_queue_size(network["cx2"], transfer_queue_size);
    set_queue_size(network["cx3"], transfer_queue_size);
    set_queue_size(network["cx4"], transfer_queue_size);

    set_queue_size(network["P_iq1"], buffer_queue_size);
    set_queue_size(network["P_cq1"], buffer_queue_size);
    set_queue_size(network["P_iq2"], buffer_queue_size);
    set_queue_size(network["P_cq2"], buffer_queue_size);

    set_queue_size(network["Q_iq1"], buffer_queue_size);
    set_queue_size(network["Q_iq2"], buffer_queue_size);
    set_queue_size(network["Q_cq1"], buffer_queue_size);
    set_queue_size(network["Q_cq2"], buffer_queue_size);

    join_first(network["join0"]);
    join_second(network["join14"]);
    join_second(network["join16"]);
    join_first(network["join17"]);

    join_first(network["cc1_join1"]);
    set_queue_size(network["cc1_q2"], buffer_queue_size);

    join_first(network["cc2_join1"]);
    set_queue_size(network["cc2_q2"], buffer_queue_size);

    join_first(network["cc3_join1"]);
    set_queue_size(network["cc3_q2"], buffer_queue_size);

    join_first(network["cc4_join1"]);
    set_queue_size(network["cc4_q2"], buffer_queue_size);

    setup_functions();

    for (auto &it : network) {
      // std::cout << it.first << ": " << it.second << std::endl;
      // std::cout << "checking " << it.first << std::endl;
      ASSERT_TRUE(it.second->valid());
      if (it.second) components.insert(it.second);
    }

    ASSERT_TRUE(!CombinatorialCycleDetector(components));

    SymbolicTypes(components);

    XMASState state = XMASState{};
    // for (auto q : queues) {
    //   state.add_queue(dynamic_cast<XMASQueue *>(q), {});
    // }
    // reachable_states.insert({"00_initial_state", std::move(state)});

    state = XMASState{};
    for (auto q : queues) {
      if (q == network["cc1_q2"] || q == network["P_iq1"]) {
        state.add_queue(dynamic_cast<XMASQueue *>(q),
                        {{SymbolicPacket{NAMED_ENUM("type", "a")}}});
      } else {
        state.add_queue(dynamic_cast<XMASQueue *>(q), {});
      }
    }
    reachable_states.insert({"01_balanced", std::move(state)});

    state = XMASState{};
    for (auto q : queues) {
      if (q == network["P_iq2"]) {
        state.add_queue(dynamic_cast<XMASQueue *>(q),
                        {{SymbolicPacket{NAMED_ENUM("type", "b")}}});
      } else {
        state.add_queue(dynamic_cast<XMASQueue *>(q), {});
      }
    }

    unreachable_states.insert({"02_unbalanced", std::move(state)});
  }

  MemoryPool mp;
  std::map<bitpowder::lib::String, XMASComponent *> network;
  std::vector<XMASComponent *> queues;

  int transfer_queue_size = 2;
  int buffer_queue_size = 2;
};

class XMAS_TWOAGENTS_T2_B2 : public XMAS_TWOAGENTS {
};

class XMAS_TWOAGENTS_T2_B3 : public XMAS_TWOAGENTS {
 protected:
  XMAS_TWOAGENTS_T2_B3() { buffer_queue_size = 3; }
};

class XMAS_TWOAGENTS_T2_B4 : public XMAS_TWOAGENTS {
 protected:
  XMAS_TWOAGENTS_T2_B4() { buffer_queue_size = 4; }
};

class XMAS_TWOAGENTS_T3_B2 : public XMAS_TWOAGENTS {
 protected:
  XMAS_TWOAGENTS_T3_B2() { transfer_queue_size = 3; }
};

class XMAS_TWOAGENTS_T4_B2 : public XMAS_TWOAGENTS {
 protected:
  XMAS_TWOAGENTS_T4_B2() { transfer_queue_size = 4; }
};

class XMAS_TWOAGENTS_T6_B2 : public XMAS_TWOAGENTS {
 protected:
  XMAS_TWOAGENTS_T6_B2() { transfer_queue_size = 6; }
};

TEST_F(XMAS_TWOAGENTS_T2_B2, NUSMV) {
  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_TWOAGENTS_T2_B2, SPACE_SEARCH) {
  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}



TEST_F(XMAS_TWOAGENTS_T2_B3, NUSMV) {
  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}


TEST_F(XMAS_TWOAGENTS_T2_B4, NUSMV) {
  checkAll("nusmv forward", nusmv_forward_checker);
  // checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_TWOAGENTS_T3_B2, NUSMV) {
  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_TWOAGENTS_T4_B2, NUSMV) {
  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}

TEST_F(XMAS_TWOAGENTS_T6_B2, NUSMV) {
  checkAll("nusmv forward", nusmv_forward_checker);
  checkAll("nusmv backward", nusmv_backward_checker);
}



} // namespace