#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "xmas.h"
#include <set>

// return true if cycle (should not occur!)
bool DeadlockChecker(std::set<XMASComponent*> allComponents);

#endif // DEADLOCK_H
