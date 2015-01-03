
#include "test_util.h"

#include "../cycle.h"
#include "../state.h"
#include "../symbolic.h"

#include "reachable.h"
#include "nusmv.h"

void TestNetwork::checkAll(
    const std::string &name,
    const std::function<
        bool(const std::set<XMASComponent *> &, const XMASState &)> &checker) {
  ASSERT_TRUE(!CombinatorialCycleDetector(components));

  SymbolicTypes(components);

  for (const auto &state : reachable_states) {
    std::cerr << "-- " << state.first << " " << name;

    EXPECT_TRUE(checker(components, state.second))
        << "with state " << state.first << " and checker " << name;
  }

  for (const auto &state : unreachable_states) {
    std::cerr << "-- " << state.first << " " << name;

    EXPECT_FALSE(checker(components, state.second))
        << "with state " << state.first << " and checker " << name;
  }
}

bool nusmv_forward_checker(const std::set<XMASComponent *> &components,
                           const XMASState &state) {
  return isReachableNusmv(components, state, {});
}

bool nusmv_backward_checker(const std::set<XMASComponent *> &components,
                            const XMASState &state) {
  return isReachableNusmv(components, state, nusmv_reverse);
}


bool bfs_forward_checker(const std::set<XMASComponent *> &components,
                           const XMASState &state) {
  return isReachable(components, state, reachable_bfs);
}
bool dfs_forward_checker(const std::set<XMASComponent *> &components,
                           const XMASState &state) {
  return isReachable(components, state, {});
}
bool bfs_backward_checker(const std::set<XMASComponent *> &components,
                           const XMASState &state) {
  return isReachable(components, state, reachable_bfs | reachable_reverse);
}
bool dfs_backward_checker(const std::set<XMASComponent *> &components,
                           const XMASState &state) {
  return isReachable(components, state, reachable_reverse);
}
