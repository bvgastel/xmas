#ifndef NETWORK_H
#define NETWORK_H

#include <sstream>

#include <QUrl>

class Network
{
public:
    Network(std::string filename);
    Network(QUrl fileUrl);
    Network(std::stringstream &sstr);
    ~Network();

    size_t size();
private:

};

#endif // NETWORK_H
