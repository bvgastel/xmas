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
.import "qrc:/javascripts/xobjects/xchannel.js" as ChannelJs
.import "qrc:/javascripts/xobjects/xcomponent.js" as ComponentJs

var start
var end
var time

// called by open network via datacontrol signal
function createNetwork(object) {
    try {
        if(!object) {
            log("Undefined network!","red")
            return
        }

        //clear network
        network.clear()

        // process list of components and create components
        var complist = object["complist"]
        for (var c in complist) {
            loadComponent(complist[c])
        }

        // process list of connections and create connections
        var connlist = object["channellist"]
        for (var conn in connlist) {
            var result = ChannelJs.create(
                        connlist[conn]["outcomp"],
                        connlist[conn]["outport"],
                        connlist[conn]["incomp"],
                        connlist[conn]["inport"]
                        )
        }
    } catch(e) {
        log("[xnetwork.js(createNetwork) - ]" + e, "red")
    }
}

// load component
function loadComponent(object) {
    try {
        var qml = ""
        var component = null
        var type = ""
        if(!object) {
            log("Undefined object!","red")
            return
        }

        if(object.type!==undefined){
            qml = getXQml(object.type)
        } else {
            log("Undefined type!" + object,"red")
            return
        }

        if(qml==="") return

        //    start = new Date().getTime()
        component = Qt.createComponent(qml)
        //    end = new Date().getTime()
        //    log("Execution time: " + (end - start) + "ms","red")

        if (component.status === Qjs.Component.Loading)
            component.statusChanged.connect(createComponent(network,component,object))
        else if (component.status === Qjs.Component.Ready)
            createComponent(network,component,object)
        else if (component.status === Qjs.Component.Error)
            log(component.errorString(),"red")
    } catch(e) {
        log("[xnetwork.js(loadComponent) - ]" + e, "red")
    }
}

// get qml resource by type
function getXQml(type) {
    try {
        switch(type) {
        case Model.XComponent.Queue:
            return "qrc:/xmas/xobjects/queue.qml"
        case Model.XComponent.Function:
            return "qrc:/xmas/xobjects/function.qml"
        case Model.XComponent.Fork:
            return "qrc:/xmas/xobjects/fork.qml"
        case Model.XComponent.Join:
            return "qrc:/xmas/xobjects/join.qml"
        case Model.XComponent.Switch:
            return "qrc:/xmas/xobjects/switch.qml"
        case Model.XComponent.Merge:
            return "qrc:/xmas/xobjects/merge.qml"
        case Model.XComponent.Sink:
            return "qrc:/xmas/xobjects/sink.qml"
        case Model.XComponent.Source:
            return "qrc:/xmas/xobjects/source.qml"
        case Model.XComponent.Composite:
            return "qrc:/xmas/xobjects/composite.qml"
        default:
            log("Unknown xmas type! (" + type + ")","red")
            return ""
        }
    } catch(e) {
        log("[xnetwork.js(getXQml) - ]" + e, "red")
    }
}

// create component
function createComponent(parent,component,object) {
    try {
        var item = null

        if (component.status === Qjs.Component.Ready) {
            item = component.createObject(parent,
                                          {
                                              "x":object.x,
                                              "y":object.y,
                                              "rotation":object.rotation,
                                              "scale":object.scale,
                                              "name":object.name
                                          });
            if (object.type === Model.XComponent.Queue) {
                item.capacity = object.capacity ? object.capacity : 0
                //item.capacityChanged()
            }
            if (object.type === Model.XComponent.Source
                    || object.type === Model.XComponent.Sink ) {
                item.required = object.required ? object.required : true
                //item.requiredChanged()
            }
            if (object.type === Model.XComponent.Function
                    || object.type === Model.XComponent.Join
                    || object.type === Model.XComponent.Switch
                    || object.type === Model.XComponent.Source
                    || object.type === Model.XComponent.Composite) {
                item.expression = object.expression ? object.expression : ""
                //item.expressionChanged()
            }
            if (object.type === Model.XComponent.Composite) {
                item.filename = object.filename ? object.filename : ""
                item.alias = object.alias ? object.alias : ""
                item.image = object.image ? "qrc:/symbols/content/symbols/" + object.image : ""
                item.boxed = object.boxed
                item.parametric = object.parametric
            }
            item.componentAdded()
        } else if (component.status === Qjs.Component.Error) {
            log(component.errorString(),"red")
        }
    } catch(e) {
        log("[xnetwork.js(createComponent) - ]" + e, "red")
    }
}
