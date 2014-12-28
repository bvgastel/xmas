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
#ifndef INPORT_H
#define INPORT_H

#include <memory>


#include "component.h"
#include "port.h"

/**
 * @brief The InPort class
 *
 * The specialised port for reading signals on an NoC wire.
 * It inherits the name and component from Port.
 *
 */
class InPort : public Port
{
public:
    InPort(QString name, std::shared_ptr<Component> comp);
    ~InPort();
};

#endif // INPORT_H
