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

// load component
function loadComponent(qml) {
    try {
        var component = Qt.createComponent(qml)
        if (component.status === Qjs.Component.Loading)
            component.statusChanged.connect(createComponent(network,component));
        else if (component.status === Qjs.Component.Ready)
            createComponent(network,component)
        else if (component.status === Qjs.Component.Error)
            log(component.errorString(),"red")
    } catch(e) {
        log("[xcomponent.js(loadComponent) - ]" + e, "red")
    }
}

// create instance of qml component
function createComponent(parent,component) {
    try {
        if (component.status === Qjs.Component.Ready && draggedItem == null) {
            draggedItem = component.createObject(parent,{"x":posnInWindow.x,"y": posnInWindow.y})
            draggedItem.name = generateName(draggedItem.prefix)
            if(draggedItem.type === Model.XComponent.Composite){
                draggedItem.filename = item.filename
                draggedItem.alias = item.alias
                draggedItem.image = item.image ? icon.source : ""
                draggedItem.boxed = item.boxed
            }
            if (!network.addComponent(draggedItem)){
                draggedItem.destroy()
                draggedItem = null
                return;
            }

        } else if (component.status === Qjs.Component.Error) {
            draggedItem = null
            log(component.errorString(),"red")
        }
    } catch(e) {
        log("[xcomponent.js(createComponent) - ]" + e, "red")
    }
}

// start drag item from XToolbar
function startDrag(mouse)
{
    try {
        posnInWindow = item.mapToItem(network,-50,-50)
        loadComponent(item.componentFile)
    } catch(e) {
        log("[xcomponent.js(startDrag) - ]" + e, "red")
    }
}

// while dragging item snap to grid
function continueDrag(mouse)
{
    try {
        if (draggedItem == null)
            return;
        draggedItem.x = mouse.x/network.scale + posnInWindow.x
        draggedItem.y = mouse.y/network.scale + posnInWindow.y
        if(network.gridSnap)doGridSnap(draggedItem)
    } catch(e) {
        log("[xcomponent.js(continueDrag) - ]" + e, "red")
    }
}

// on drop check if item is still within canvas bounds
function endDrag()
{
    try {
        if (draggedItem == null)
            return;
        if (draggedItem.x < network.x + network.margin
                || draggedItem.x > network.x + network.width - network.margin - draggedItem.width
                || draggedItem.y < network.y + network.margin
                || draggedItem.y > network.y + network.height - network.margin - draggedItem.height) {
            remove(draggedItem)
        }
        draggedItem.updateCanvasData()
        network.modified=true
        draggedItem = null;
    } catch(e) {
        log("[xcomponent.js(endDrag) - ]" + e, "red")
    }
}

// remove component
function remove(component) {
    try {
        if (network.removeComponent(component)) {
            component.destroy()
            network.modified = true
        } else {
            log("Remove of component " + component.name + " not confirmed by xmas!!","red")
        }
    } catch(e) {
        log("[xcomponent.js(remove) - ]" + e, "red")
    }
}

//// grid snap
function doGridSnap(item){
    try {
        var snapX = Math.round(((item.x-network.margin) / network.gridSize)) * network.gridSize + network.margin
        var snapY =  Math.round(((item.y-network.margin) / network.gridSize)) * network.gridSize + network.margin
        item.x = snapX
        item.y = snapY
    } catch(e) {
        log("[xcomponent.js(doGridSnap) - ]" + e, "red")
    }
}


// generate a unique name
function generateName(prefix) {
    try {
        var idx = 0
        var name = prefix + idx
        while(!isUniqueName(name)){
            ++idx
           name = prefix + idx
        }
        return name
    } catch(e) {
        log("[xcomponent.js(generateName) - ]" + e, "red")
    }
}

// check if name is unique
function isUniqueName(name) {
    try {
        for(var child in network.children){
            if(network.children[child].objectName==="component"){
                if(name === network.children[child].name) {
                    return false
                }
            }
        }
        return true
    } catch(e) {
        log("[xcomponent.js(isUniqueName) - ]" + e, "red")
    }
    return false
}
