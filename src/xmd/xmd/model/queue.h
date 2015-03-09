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

#ifndef QUEUE_H
#define QUEUE_H

#include "component.h"

namespace model {

class Queue : public Component
{
    Q_OBJECT
    Q_PROPERTY(unsigned int size READ size WRITE size NOTIFY sizeChanged)

public:
    Queue(QQuickItem *parent);
    ~Queue();

signals:
    void sizeChanged();

public slots:

public:
    unsigned int size() {
        return m_size;
    }

    void size(unsigned int size) {
        m_size = size;
    }

private:
    unsigned int m_size;

};

} // namespace model

#endif // QUEUE_H
