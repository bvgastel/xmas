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

#ifndef GRID_H
#define GRID_H

#include <QObject>

namespace model {

class Grid : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString network READ network WRITE network NOTIFY networkChanged)
    Q_PROPERTY(int width READ width WRITE width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE height NOTIFY heightChanged)

public:
    explicit Grid(QObject *parent = 0);
    Grid(const Grid &rhs);
    ~Grid();

    QString network() const { return m_network; }
    void network(QString &network);

    int width() { return m_width; }
    void width(int &width) { m_width = width > 0 ? width : 100; }

    int height() { return m_height; }
    void height(int &height) { m_height = height > 0 ? height : 100; }

    Grid &operator=(const Grid &rhs);

signals:
    void networkChanged();
    void widthChanged();
    void heightChanged();

public slots:

private:
    /**
     * @brief m_network the name of the network
     */
    QString m_network;
    /**
     * @brief m_width width of the grid, must be > 0.
     */
    int m_width;
    /**
     * @brief m_height height of the grid, must be > 0.
     */
    int m_height;
};

} // namespace model

#endif // GRID_H
