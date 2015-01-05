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

#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <memory>

#include "component.h"
#include "noc.h"

class Composite : public Component
{

public:
    Composite(std::shared_ptr<Noc> network,
              QString name,
              PortList inport_list,
              PortList outport_list,
              QString function=""
             );
    ~Composite();

private:
    std::shared_ptr<Noc> m_network;

};

#endif // COMPOSITE_H
