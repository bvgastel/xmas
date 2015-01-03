#ifndef POSITIONCOMPONENTEXTENSION_H
#define POSITIONCOMPONENTEXTENSION_H

#include "checker/xmas.h"

enum class Orientation
{
    North,
    East,
    South,
    West
};

class PositionComponentExtension : public XMASComponentExtension
{
public:
    PositionComponentExtension() {}
    PositionComponentExtension(int x, int y, Orientation orientation):
        m_x{x}, m_y{y}, m_orientation{orientation}
    {}

    int x() const { return m_x; }
    int y() const { return m_y; }
    Orientation orientation() const { return m_orientation; }

private:
    int m_x;
    int m_y;
    Orientation m_orientation;
};

#endif // POSITIONCOMPONENTEXTENSION_H
