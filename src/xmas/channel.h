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
#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>

#include <QString>

#include "inport.h"
#include "outport.h"
/**
 * @brief The Channel class
 *
 * The wires between components on an Noc. Each
 * channel in reality consists of 3 wires:
 *
 * 1. The irdy or initiator ready signal wire
 * 2. The trdy or target ready signal wire
 * 3. The data wire.
 *
 * The data wire can have any type. The irdy
 * and trdy wires can send a signal when ready.
 * They are control wires.
 *
 * A channel has exactly two components with
 * on one end the output, from the channel reads
 * its data and on the other side the in port
 * to which the channel writes its data.
 *
 * The component attached to the outport sends
 * the signal irdy when ready. The other component
 * (inport bound) receives this signal.
 *
 * The compoent attached to the inport sends
 * the signal trdy when ready. The other component
 * (outport bound) receives this signal.
 *
 */
class Channel
{
public:
    explicit Channel(std::shared_ptr<OutPort> out, std::shared_ptr<InPort> in);
   ~Channel();

   const QString name() const;
   std::shared_ptr<const InPort> in() const;
   std::shared_ptr<const OutPort> out() const;

private:

   QString m_name;
   /**
    * @brief m_out The output port that this channel reads from.
    */
   std::shared_ptr<OutPort> m_out;
   /**
    * @brief m_in The input port that this channel writes to.
    */
   std::shared_ptr<InPort> m_in;

};

#endif // CHANNEL_H
