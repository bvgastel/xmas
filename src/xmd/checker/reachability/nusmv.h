#ifndef REACHABLE_NUSMV_H
#define REACHABLE_NUSMV_H

#include <set>

#include "../symbolic-function.h"
#include "../xmas.h"

enum nusmv_flags {
  nusmv_none = 0x1,
  nusmv_reverse = 0x2
};

inline nusmv_flags operator|(nusmv_flags a, nusmv_flags b) {
  return static_cast<nusmv_flags>(static_cast<size_t>(a) |
                                  static_cast<size_t>(b));
}

inline bool operator&(nusmv_flags a, nusmv_flags b) {
  return static_cast<size_t>(a) & static_cast<size_t>(b);
}

bool isReachableNusmv(const std::set<XMASComponent *> &, const XMASState &,
                      nusmv_flags);

void print_nusmv(std::ostream &, const std::set<XMASComponent *> &,
                 const XMASState &, nusmv_flags);

#endif