#ifndef COMPOSITENETWORKEXTENSION
#define COMPOSITENETWORKEXTENSION

#include "xmas.h"

class CompositeNetworkExtension : public XMASNetworkExtension
{
public:
    CompositeNetworkExtension() {}
    CompositeNetworkExtension(const std::string& alias, int width, int height,
                              const std::string& imageName, const std::string& packet,
                              bool boxedImage)
        : alias{alias}, width{width}, height{height},
          imageName{imageName}, packet{packet}, boxedImage{boxedImage}
    {
    }

    std::string alias;
    int         width;
    int         height;
    std::string imageName;
    std::string packet;
    bool        boxedImage;
};

#endif // COMPOSITENETWORKEXTENSION
