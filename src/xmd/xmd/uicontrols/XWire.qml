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
import QtQuick 2.4

Rectangle {
    id: l
    property alias x1: l.x
    property alias y1: l.y

    property real x2: l.x
    property real y2: l.y

    property alias size: l.height

    color: "black"
    height: 2

    transformOrigin: Item.Left
    transform: Translate { y:-size/2 }

    width: getWidth(x1,y1,x2,y2);
    rotation: getSlope(x1,y1,x2,y2);

    function getWidth(sx1,sy1,sx2,sy2)
    {
        var w=Math.sqrt(Math.pow((sx2-sx1),2)+Math.pow((sy2-sy1),2));
        return w;
    }

    function getSlope(sx1,sy1,sx2,sy2)
     {
         var a
         var dx=sx2-sx1
         var dy=sy2-sy1
         if (dx===0)
             return dy > 0 ? 90 : -90
         a=Math.atan(dy/dx)*180/Math.PI
         if (dy<0 && dx<0)
             return a+180
         else if (dy>=0 && dx>=0)
             return a
         else if (dy<0 && dx>=0)
             return a
         else if (dy>=0 && dx<0)
             return a+180
         else
             return 0
     }
}

