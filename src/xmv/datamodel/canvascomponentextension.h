#ifndef CANVASCOMPONENTEXTENSION
#define CANVASCOMPONENTEXTENSION

#include "xmas.h"

class CanvasComponentExtension : public XMASComponentExtension
{
public:
    CanvasComponentExtension() {}
    CanvasComponentExtension(int x, int y, int orientation, float scale)
      : m_x{x}, m_y{y}, m_orientation{orientation}, m_scale{scale}
    {}

    int x() const           { return m_x; }
    int y() const           { return m_y; }
    int orientation() const { return m_orientation; }
    float scale() const     { return m_scale; }

private:
    int     m_x             = 0;
    int     m_y             = 0;
    int     m_orientation   = 0;
    float   m_scale         = 1.0f;
};

#endif // CANVASCOMPONENTEXTENSION

