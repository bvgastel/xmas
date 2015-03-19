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

#include "channel.h"
#include "xmas.h"

model::Channel::Channel(QQuickItem *parent)
    : QQuickItem(parent),
      m_outport(nullptr),
      m_port2(nullptr)
{
}

model::Channel::~Channel()
{

}

void model::Channel::componentComplete()
{
    QString errMsg;
    if (!portsOk(errMsg)) {
        emit writeLog(errMsg, Qt::red);
        return;
    }

    Output *out = dynamic_cast<Output *>(m_outport->getPort());
    Input *in = dynamic_cast<Input *>(m_port2->getPort());
    if (in && out) {
        Output &output = *out;
        Input &input = *in;
        ::connect(output, input);
    } else {
        throw std::exception();
    }
}

void model::Channel::classBegin()
{
    // No action needed.
}

bool model::Channel::portsOk(QString &errMsg)
{
    bool result = true;
    if (!m_outport) {
        errMsg += "\nComponent completion of channel failed due to missing output XPort.";
        result = false;
    }
    if (!m_port2) {
        errMsg += "\nComponent completion of channel failed due to missing input XPort.";
        result = false;
    }
    if (m_outport && !m_outport->getPort()) {
        errMsg += "\nComponent completion of channel failed due to lack of Output.";
        result =  false;
    }
    if (m_port2 && !m_port2->getPort()) {
        errMsg += "\nComponent completion of channel failed due to lack of Input.";
        result = false;
    }
    return result;
}
