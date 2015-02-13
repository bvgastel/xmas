#ifndef SERIALIZE_NETWORK
#define SERIALIZE_NETWORK

#include <ostream>
#include <set>

class XMASComponent;

void serialize_network(std::ostream& os, std::set<XMASComponent*>& c);

#endif // SERIALIZE_NETWORK

