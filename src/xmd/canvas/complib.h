/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/
#ifndef COMPLIB
#define COMPLIB

#include <QtDeclarative>
#include "component.h"

class Component;

class CompLib
{

public:
    enum ComponentType { Queue=1, Function=2, Fork=3, Join=4, Switch=5, Merge=6, Sink=7, Source=8, In=9, Out=10, Composite=11,};

    Component *getComponent(int type);
    CompLib();
private:
    Component *m_queue;
    Component *m_function;
    Component *m_fork;
    Component *m_join;
    Component *m_switch;
    Component *m_merge;
    Component *m_sink;
    Component *m_source;
    Component *m_in;
    Component *m_out;
    Component *m_composite; //test
};

#endif // COMPLIB

