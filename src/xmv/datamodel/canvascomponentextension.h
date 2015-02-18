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

    // NOTE: Extension currently has a copy constructor and copy assignment operator
    // Using these functions (e.g. in derived Extension classes) can compromise
    // the internal "E* next" pointer which will be copied (overwritten) as well.
    // Suggestion: delete the copy constructor and assignment functions in Extension.

    // In the custom implementations defined below, the base Extension members are not copied.
    CanvasComponentExtension(CanvasComponentExtension& b)
      : m_x{b.m_x}, m_y{b.m_y}, m_orientation{b.m_orientation}, m_scale{b.m_scale}
    {}

    // NOTE: gbo: added return type (void)
    void operator =(CanvasComponentExtension& b)
    {
        m_x = b.m_x; m_y = b.m_y; m_orientation = b.m_orientation; m_scale = b.m_scale;
    }

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
