#include "memory.h"

// FIXME: make some way to purge cached instances at regular intervals (like once per 10 minutes)
namespace bitpowder {
namespace lib {

std::set<CacheOfInstancesBase*>* CacheOfInstancesBase::getAllInstanceCaches() {
    static std::set<CacheOfInstancesBase*> retval;
    return &retval;
}

void CacheOfInstancesBase::purgeAll() {
    for (auto it = getAllInstanceCaches()->begin(); it != getAllInstanceCaches()->end(); ++it) {
        (*it)->purge();
    }
}

}
}
