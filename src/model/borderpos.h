/*********************************************************************
  *
  * Copyright (C) Guus Bonnema, 2014
  *
  * This file is part of the xmas-design tool.
  *
  * The xmas-design tool is free software: you can redistribute it
  * and/or modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation, either version 3 of
  * the License, or (at your option) any later version.
  *
  * The xmas-design tool is distributed in the hope that it will be
  * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
  * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with the xmas-design tool.  If not, see
  * <http://www.gnu.org/licenses/>.
  *
  **********************************************************************/

#ifndef BORDERPOS_H
#define BORDERPOS_H

#include <QObject>

namespace model {

/**
 * @brief The borderPos class
 *
 * This class implements a specific position of an object on the
 * border of a rectangular area. It is meant to be used with Qml 2.0
 * In this case it must be registered with qml.
 *
 * Remark: the position must be a number between 0 and 100. Any attempt
 * outside these boundaries are overwritten with 0.
 *
 */
class BorderPos : public QObject
{
    Q_OBJECT

    Q_ENUMS(BorderSide)
    Q_PROPERTY(BorderSide border READ border WRITE border NOTIFY borderChanged)
    Q_PROPERTY(int position READ position WRITE position NOTIFY positionChanged)
public:
    enum BorderSide { Left, Right, Top, Bottom };

    explicit BorderPos(QObject *parent = 0);
    ~BorderPos();

    BorderSide border() { return m_borderSide; }
    void border(BorderSide &borderSide) { m_borderSide = borderSide; }

    int position() { return m_position; }
    void position(int &position) { m_position = (position < 0 || position > 100) ? 0 : position; }

signals:
    void borderChanged();
    void positionChanged();

public slots:

private:

    /**
     * @brief m_borderSide One of the 4 out of BorderSide enum.
     */
    BorderSide m_borderSide;
    /**
     * @brief m_position The relative position, a number between 0 and 100.
     */
    int m_position;

};

} // namespace model

#endif // BORDERPOS_H
