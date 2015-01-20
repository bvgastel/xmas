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

#ifndef POINT_H
#define POINT_H

#include <QObject>

namespace model {

/**
 * @brief The Point class
 *
 * The x and y coordinates, both must be >= 0.
 */
class GridPoint : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int x READ x WRITE x NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE y NOTIFY yChanged)

public:
    explicit GridPoint(QObject *parent = 0);
    ~GridPoint();

    int x() { return m_x; }
    void x(int &x) { m_x = x >= 0 ? x : 0; }

    int y() { return m_y; }
    void y(int &y) { m_y = y >= 0 ? y : 0; }


signals:
    void xChanged();
    void yChanged();

public slots:

private:
    int m_x;
    int m_y;
};

} // namespace model

#endif // POINT_H
