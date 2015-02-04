
#ifndef REACHABILITY_MCRL2_H
#define REACHABILITY_MCRL2_H

#include <iosfwd>
#include <set>

class XMASComponent;
class XMASState;

void print_mcrl2(std::ostream &, const std::set<XMASComponent *> &,
                 const XMASState &);

void print_mu(std::ostream &, const std::set<XMASComponent *> &,
              const XMASState &);

#endif