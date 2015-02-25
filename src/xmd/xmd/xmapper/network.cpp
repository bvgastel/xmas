
#include "network.h"
#include "parse.h"
#include "xmdexception.h"

Network::Network(Controller *controller, QUrl &fileUrl) : m_controller(controller)
{
    throw XmdException("File "+fileUrl.toDisplayString()+" not found.");
}

Network::Network(Controller *controller, std::stringstream &sstr) : m_controller(controller) {

    bitpowder::lib::MemoryPool mp;
    std::tie(m_componentMap, std::ignore) = parse_xmas_from_json(sstr.str(), mp);

}


size_t Network::size() {
    return 0;
}

