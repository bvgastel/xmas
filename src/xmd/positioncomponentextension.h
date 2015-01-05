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

    PositionComponentExtension& operator=(const PositionComponentExtension& b) {
        m_x = b.m_x; m_y = b.m_y; m_orientation = b.m_orientation;
        return *this;
    }

    int x() const { return m_x; }
    int y() const { return m_y; }
    Orientation orientation() const { return m_orientation; }

private:
    int m_x;
    int m_y;
    Orientation m_orientation;
};

#endif // POSITIONCOMPONENTEXTENSION_H
