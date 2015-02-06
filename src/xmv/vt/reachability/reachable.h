#ifndef REACHABLE_H
#define REACHABLE_H

#include <set>

#include "../state.h"
#include "../xmas.h"

enum reachable_flags {
  reachable_bfs = 0x1,
  reachable_reverse = 0x2
};

inline reachable_flags operator|(reachable_flags a, reachable_flags b) {
  return static_cast<reachable_flags>(static_cast<size_t>(a) |
                                      static_cast<size_t>(b));
}

inline bool operator&(reachable_flags a, reachable_flags b) {
  return static_cast<size_t>(a) & static_cast<size_t>(b);
}

bool isReachable(const std::set<XMASComponent *> &allComponents,
                 const XMASState &s, reachable_flags);

#endif