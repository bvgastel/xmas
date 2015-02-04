#ifndef REACHABLE_PRIVATE_H
#define REACHABLE_PRIVATE_H

#include "transition.h"


// class WhatsBeen;

// class SendBack : XMASComponentVisitor {
//  public:
//   SendBack(WhatsBeen *beenWhatter) : beenWhatter(beenWhatter) {}
//   /**
//    * Wrapper around a call to the visitor, handling the arguments.
//    */
//   std::vector<GenericTransition> send_back(
//       std::vector<GenericTransition> reverseTransitions, XMASComponent *c,
//       XMASComponent *prev, SymbolicFunction packet);

//   virtual void visit(XMASSink *) {
//     // We cannot arrive at a sink going backwards.
//     assert(false);
//   }

//   virtual void visit(XMASSource *c) {
//     // nothing to do here
//   }

//   virtual void visit(XMASQueue *c);
//   virtual void visit(XMASFunction *c);
//   virtual void visit(XMASSwitch *c);
//   virtual void visit(XMASFork *c);
//   virtual void visit(XMASMerge *c);
//   virtual void visit(XMASJoin *c);

//  private:
//   WhatsBeen *beenWhatter;

//   std::vector<GenericTransition> _reverseTransitions;
//   XMASComponent *_prev;
//   SymbolicFunction _packet;
// };

// class WhatsBeen : XMASComponentVisitor {
//  public:
//   WhatsBeen(SendBack *backSender) : backSender(backSender) {}

//   std::vector<std::pair<std::vector<GenericTransition>,
//                         SymbolicFunction>>
//   whats_been(std::vector<GenericTransition> T, XMASComponent *c,
//              XMASComponent *prev);

//   virtual void visit(XMASSink *c);

//   virtual void visit(XMASSource *c) {
//     // We cannot arrive at a source going forwards.
//     assert(false);
//   }

//   virtual void visit(XMASQueue *c);
//   virtual void visit(XMASFunction *c);
//   virtual void visit(XMASSwitch *c);
//   virtual void visit(XMASFork *c);
//   virtual void visit(XMASMerge *c);
//   virtual void visit(XMASJoin *c);

//  private:
//   SendBack *backSender;

//   std::vector<std::pair<std::vector<GenericTransition>,
//                         SymbolicFunction>> _output;
//   std::vector<GenericTransition> _reverseTransitions;
//   XMASComponent *_prev;
// };


// class ComputeReverseTransitions : public XMASComponentVisitor {
//  public:
//   std::vector<GenericTransition> reverseTransitions;

//   ComputeReverseTransitions()
//       : backSender(&beenWhatter), beenWhatter(&backSender) {}

//   virtual void visit(XMASSink *c);
//   virtual void visit(XMASQueue *c);

//   virtual void visit(XMASSource *) {};
//   virtual void visit(XMASFunction *) {};
//   virtual void visit(XMASSwitch *) {};
//   virtual void visit(XMASFork *) {};
//   virtual void visit(XMASMerge *) {};
//   virtual void visit(XMASJoin *) {};

//  private:
//   SendBack backSender;
//   WhatsBeen beenWhatter;
// };

std::vector<GenericTransition>
compute_transitions(const std::set<XMASComponent *> &, bool);

// std::vector<GenericTransition>
// compute_reverse_transitions(std::set<XMASComponent *> allComponents, bool print = false);

#endif