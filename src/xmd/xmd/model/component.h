/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>

#include "memorypool.h"
#include "xmas.h"


/**
 * @brief The Component class
 *
 * A wrapper around XMASComponent from xmas.h to connect to
 * Qml using properties.
 */

namespace model {
class Component : public QObject
{
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(int x READ x WRITE x NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE y NOTIFY yChanged)

public:
    explicit Component(QObject *parent = 0);
    ~Component();

signals:
    void nameChanged();
    void xChanged();
    void yChanged();

public slots:

public:
    QString name() {
        return m_name;
    }

    void name(QString name) {
        // TODO: make sure to modify xmascomponent and map
        m_name = name;
    }

    virtual int x() {
        return m_x;
    }

    virtual void x(int x) {
        // TODO: make sure to modify xmascomponent and map
        m_x = x;
    }

    virtual int y() {
        return m_y;
    }

    virtual void y(int y) {
        // TODO: make sure to modify xmascomponent and map
        m_y = y;
    }

public:
    enum Orientation {
        North = 0,
        East = 90,
        South = 180,
        West = 270,
        NorthWest = 45,
        SouthWest = 225,
        NorthEast = 315,
        SouthEast = 135
    };

private:
    QString m_name;
    int m_x;
    int m_y;
    Orientation orientation;
    double scale;
    XMASComponent *m_component;
};

} // namespace model

#endif // COMPONENT_H
