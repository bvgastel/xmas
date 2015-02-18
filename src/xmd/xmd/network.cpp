#include "network.h"
#include "xmdexception.h"

Network::Network(std::string filename) {
    Network(QUrl(filename.c_str()));
}

Network::Network(QUrl fileUrl)
{
    throw XmdException("File "+fileUrl.toDisplayString()+" not found.");
}

Network::Network(std::stringstream &sstr) {

    throw XmdException("Stream is invalid: " + sstr.str());
}

Network::~Network()
{

}

size_t Network::size() {
    return 0;
}

