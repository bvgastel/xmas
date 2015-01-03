
#include <gtest/gtest.h>

#include <map>
#include <functional>

class XMASComponent;
class XMASState;

bool nusmv_forward_checker(const std::set<XMASComponent *> &components,
                           const XMASState &state);

bool nusmv_backward_checker(const std::set<XMASComponent *> &components,
                            const XMASState &state);

bool bfs_forward_checker(const std::set<XMASComponent *> &components,
                         const XMASState &state);
bool bfs_backward_checker(const std::set<XMASComponent *> &components,
                          const XMASState &state);
bool dfs_forward_checker(const std::set<XMASComponent *> &components,
                         const XMASState &state);
bool dfs_backward_checker(const std::set<XMASComponent *> &components,
                          const XMASState &state);

class TestNetwork : public ::testing::Test {
 protected:
  void checkAll(const std::string &name,
                const std::function<bool(const std::set<XMASComponent *> &,
                                         const XMASState &)> &checker);

  std::map<std::string, XMASState> reachable_states;
  std::map<std::string, XMASState> unreachable_states;

  std::set<XMASComponent *> components;
};
