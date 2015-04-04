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
.import XMAS.model 1.0 as Model
.import XMAS 1.0 as XMAS
.import QtQuick 2.4 as Qjs

var draggedItem = null;
var posnInWindow;

function startDrag(mouse)
{
    posnInWindow = item.mapToItem(network,-50,-50)
    loadComponent(item.componentFile)
}


function loadComponent(qml) {
    var component = Qt.createComponent(qml)
    if (component.status === Qjs.Component.Loading)
        component.statusChanged.connect(createComponent(network,component));
    else if (component.status === Qjs.Component.Ready)
        createComponent(network,component)
    else if (component.status === Qjs.Component.Error)
        log(component.errorString(),"red")
}

function createComponent(parent,component) {
    if (component.status === Qjs.Component.Ready && draggedItem == null) {
        draggedItem = component.createObject(parent,{"x":posnInWindow.x, "y": posnInWindow.y})
        if(draggedItem.type === Model.XComponent.Composite){
            draggedItem.url = item.url
            draggedItem.alias = item.alias
            draggedItem.image = item.image ? item.source : ""
            draggedItem.boxed = item.boxed
        }
    } else if (component.status === Qjs.Component.Error) {
        draggedItem = null
        log(component.errorString(),"red")
    }
}

function continueDrag(mouse)
{
    if (draggedItem == null)
        return;
    draggedItem.x = mouse.x/network.scale + posnInWindow.x
    draggedItem.y = mouse.y/network.scale + posnInWindow.y
    if(network.gridSnap)doGridSnap(draggedItem)
}

function endDrag()
{
    if (draggedItem == null)
        return;
    if (draggedItem.x < network.x + network.margin
            || draggedItem.x > network.x + network.width - network.margin - draggedItem.width
            || draggedItem.y < network.y + network.margin
            || draggedItem.y > network.y + network.height - network.margin - draggedItem.height) {
        draggedItem.destroy();
        draggedItem = null;
    } else {
        draggedItem.index = generateTagIndex(draggedItem)
        draggedItem.name = draggedItem.prefix + draggedItem.index
        if (!network.addComponent(draggedItem)){
          draggedItem.destroy();
        }
        draggedItem = null;
    }
}

function remove(component) {
    if (network.removeComponent(component)) {
        destroy(component)
    }
}

// Grid Snap
function doGridSnap(item){
    var snapX = Math.round(((item.x-network.margin) / network.gridSize)) * network.gridSize + network.margin
    var snapY =  Math.round(((item.y-network.margin) / network.gridSize)) * network.gridSize + network.margin
    item.x = snapX
    item.y = snapY
}

function generateTagIndex(item)
{
    var max = -1
    for(var child in network.children){
        if(network.children[child].objectName==="component"){
            if(item.type === network.children[child].type) {
                max = Math.max(network.children[child].index,max)
            }
        }
    }
    //idList.sort(function(a, b){return a-b})
    return (++max)
}

