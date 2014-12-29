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
#ifndef OUTPORT_H
#define OUTPORT_H

#include <memory>

#include <QString>

#include "port.h"
#include "component.h"

/**
 * @brief The OutPort class
 *
 * The specialised class for writing signals on an NoC wire.
 *
 */
class OutPort : public Port
{
public:
    OutPort(QString name, std::shared_ptr<Component> comp, QString irdy);
    ~OutPort();

    const QString irdy() const { return m_irdy; }

private:

    /**
     * @brief m_irdy contains the definition of o.irdy if o is the output port.
     *
     * The definition stems from the xmas primitives or a derivative.
     */
    const QString m_irdy;
};

#endif // OUTPORT_H
