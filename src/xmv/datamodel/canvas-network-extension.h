#ifndef CANVASNETWORKEXTENSION
#define CANVASNETWORKEXTENSION

#include "xmas.h"

class CanvasNetworkExtension : public XMASNetworkExtension
{
public:
    CanvasNetworkExtension() {}
    CanvasNetworkExtension(int width, int height)
        : width{width}, height{height}
    {
    }

    int         width       = 2500;     // default canvas width
    int         height      = 2500;     // default canvas height
};

#endif // CANVASNETWORKEXTENSION

