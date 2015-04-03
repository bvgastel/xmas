#ifndef COMPOSITENETWORKEXTENSION
#define COMPOSITENETWORKEXTENSION

#include "xmas.h"

class CompositeNetworkExtension : public XMASNetworkExtension
{
public:
    CompositeNetworkExtension() {}
    CompositeNetworkExtension(const std::string& alias, const std::string& imageName,
                              const std::string& packet, bool boxedImage)
        : alias{alias}, imageName{imageName}, packet{packet}, boxedImage{boxedImage}
    {
    }

    std::string alias;
    std::string imageName;
    std::string packet;
    bool        boxedImage;
};

#endif // COMPOSITENETWORKEXTENSION
