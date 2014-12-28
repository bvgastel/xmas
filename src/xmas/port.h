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
#ifndef PORT_H
#define PORT_H

#include <memory>

#include <QString>

#include "component.h"

enum PortType {
    ANY, IN, OUT
};

/**
 * @brief The Port class
 *
 * An abstract class as a base class for in ports and out ports. Most
 * of the functionality is in the base class.
 *
 */
class Port
{
public:
    Port(QString name, std::shared_ptr<Component> comp);
    ~Port();

    const QString name() const;

private:

    QString m_name;
    std::shared_ptr<Component> m_comp;

};

#endif // PORT_H
