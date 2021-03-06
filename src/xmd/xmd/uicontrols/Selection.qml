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

Rectangle{
    id:selection

    // Properties
    color:"transparent"
    border.width: 1
    border.color: "steelblue"
    x: Math.min(from.x, to.x)
    y: Math.min(from.y, to.y)
    z:100
    width: Math.abs(from.x - to.x)
    height: Math.abs(from.y - to.y)
    state: "UNSELECT"
    property point from : Qt.point(0, 0)
    property point to : Qt.point(0, 0)
    property int margin:5
    property bool initialSizing: false
    property var selectedItems: []
    property rect rubberBandRectangle: Qt.rect(0,0,0,0)

    // Signals
    signal positionChanged
    signal showContextMenu

    // Event Handling
    Keys.onDeletePressed: deleteSelected(selectedItems)
    Keys.onEscapePressed: clear()
    Keys.onLeftPressed: doMove(-network.gridSize,0)
    Keys.onRightPressed: doMove(network.gridSize,0)
    Keys.onDownPressed: doMove(0,network.gridSize)
    Keys.onUpPressed: doMove(0,-network.gridSize)

    // JavaScripts

    function start(mouse){
        clear()
        from = Qt.point(mouse.x,mouse.y)
        to = from
        state = "START"
    }

    // Clear selection
    function clear(){
        for (var item in selectedItems){
            if(selectedItems[item].selected !== undefined)
                    selectedItems[item].selected = false
        }
        selectedItems = []
        rubberBand()
    }

    // Check if item is in selected area
    function contains(item)
    {
        var r = itemRect(item)
        return (r.left >= selection.x
                && r.right <= selection.x + selection.width
                && r.top >= selection.y
                && r.bottom <= selection.y + selection.height)
    }

    // Select items
    function select(items,append){
        if(!append)clear()
        for (var i in items){
            if(items[i].selected !== undefined)items[i].selected = true
                selectedItems.push(items[i])
          }
        rubberBand()
    }

    // Find items whitin selection bounds
    function find(items){
        selectedItems = []
        for (var item in items){
            if(contains(items[item]))
            {
                if(items[item].selected !== undefined)items[item].selected = true
                selectedItems.push(items[item])
            } else {
                if(items[item].selected !== undefined)items[item].selected = false
            }
        }
        rubberBand()
    }

    // Rubberband on selected items
    function rubberBand(){
        var r = itemsRect()
        from = Qt.point(r.x,r.y)
        to = Qt.point(r.x + r.width,r.y + r.height)
        state = from===to ? "UNSELECT":"SELECT"
    }

    // Item bounds (left,top,right,bottom)
    function itemRect(item)
    {
        var arcsin = Math.abs(Math.sin(item.rotation * Math.PI / 180))
        var arccos = Math.abs(Math.cos(item.rotation * Math.PI / 180))
        var x0 = item.x + item.width * 0.5
        var y0 = item.y + item.height * 0.5
        var x1 = (item.width * arccos + item.height * arcsin) * item.scale * 0.5
        var y1 = (item.height * arccos + item.width * arcsin) * item.scale * 0.5
        return {left:x0 - x1,top:y0 - y1,right:x0 + x1,bottom:y0 + y1}
    }

    // Selection bounds (left,top,right,bottom)
    function itemsRect() {
        //Qml Item.childrenRect not working properly
        // in combination with anchors --> bug in Qt.5.4.0?!
        var list = selectedItems
        var xmin=1000000,xmax=-1,ymin=1000000,ymax=-1
        for (var item in list){
            var r = itemRect(list[item])
            xmin= Math.min(xmin, r.left - margin)
            xmax= Math.max(xmax, r.right + margin)
            ymin= Math.min(ymin, r.top - margin)
            ymax= Math.max(ymax, r.bottom + margin)
        }

        if(ymax<0) return Qt.rect(0,0,0,0)
        return Qt.rect(xmin,ymin,xmax-xmin,ymax-ymin)
    }

    // First time size is dragged by mousepointer
    // (Bottom right handle doesn't have the focus yet)
    function updateInitialSize(mouse){
        if(initialSizing){
            to = Qt.point(mouse.x,mouse.y)
        }
    }

    // Map group to the target its coordinates
    function mapGroupTo(group,target) {
        for (var i = group.length; i-- > 0; ){
            var item = group[i]
            item.x = item.parent.mapToItem(target,item.x,0).x
            item.y = item.parent.mapToItem(target,0,item.y).y
            item.parent = target
        }
    }

    // From at top left of To
    function fromTo(){
        var newFrom = Qt.point(selection.x,selection.y)
        var newTo = Qt.point(selection.x + selection.width, selection.y + selection.height)
        from = newFrom
        to = newTo
    }


    // Move selection (navigation keys)
    function doMove(dX,dY){
        fromTo()
        mapGroupTo(selectedItems,container)
        // horizontal
        if (from.x + dX < network.margin) {
            to.x += network.margin - from.x
            from.x = network.margin
        } else if (to.x + dX > network.width - network.margin){
            from.x += network.width - network.margin - to.x
            to.x = network.width - network.margin
        } else {
            to.x += dX
            from.x += dX
        }
        // vertical
        if (from.y + dY < network.margin) {
            to.y += network.margin - from.y
            from.y = network.margin
        } else if (to.y + dY > network.height - network.margin) {
            from.y += network.height - network.margin - to.y
            to.y = network.height - network.margin
        } else {
            from.y += dY
            to.y += dY
        }
        mapGroupTo(container.children,selection.parent)
        positionChanged()
    }

    // Delete selected items
    function deleteSelected(){
        var group = selectedItems
        for (var i = group.length; i-- > 0; ){
            if (group[i].selected) {
                    group[i].remove()
            }
        }
        clear()
    }



    // Container for selected items
    //do not add children!!
    Item {id:container}

    // Mouse area
    MouseArea {
        id:mouseArea
        anchors.fill: parent
        preventStealing: false
        propagateComposedEvents:true
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: parent
        drag.minimumX: network.margin
        drag.minimumY: network.margin
        drag.maximumX: network.width - selection.width - network.margin
        drag.maximumY: network.height - selection.height - network.margin
        onDoubleClicked: {}
        onPressed: {
            if (mouse.button == Qt.RightButton)
                showContextMenu()
            else {
                mapGroupTo(selectedItems,container)
                mouseArea.cursorShape = Qt.SizeAllCursor
            }
        }
        onReleased: {
            mapGroupTo(container.children,selection.parent)
            fromTo()
            mouseArea.cursorShape = Qt.ArrowCursor
        }
        onPositionChanged:selection.positionChanged()
    }

    // selection states
    states: [
        State {
            name: "START"
            PropertyChanges {
                target: selection
                visible: true
                border.width:1
                initialSizing: true
            }
        },
        State {
            name: "SELECT"
            PropertyChanges {
                target: selection
                visible: true
                border.width:0 //set this to 1 to see rubberband
                initialSizing: false
                focus: true
            }
        },
        State {
            name: "UNSELECT"
            PropertyChanges {
                target: selection
                visible: false
                border.width:0
                initialSizing: false
                focus: true
            }
        }
    ]
}
