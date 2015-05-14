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
var channel = null;
var component = null;

// load component
function loadcomponent(outport,inport) {
    try{
        // component previously loaded?
        if (component !== null) {
            createComponent(outport,inport)
            return
        }
        component = Qt.createComponent("qrc:/xmas/xobjects/XChannel.qml")
        // Still loading qml? Call createObject on status changed
        if (component.status === Qjs.Component.Loading)
            component.statusChanged.connect(createComponent(outport,inport));
        else //...status must be ready or error immediately.
            createComponent(outport,inport)
    } catch(e) {
        log("[xchannel.js(loadcomponent) - ]" + e, "red")
    }
}

// create instance of qml component
function createComponent(outport,inport) {
    try {
        if (component.status === Qjs.Component.Ready && channel == null) {
            channel = component.createObject(network, {outport:outport,inport:inport})
        } else if (component.status === Qjs.Component.Error) {
            channel = null
            log(component.errorString(),"red")
        }
    } catch(e) {
        log("[xchannel.js(createComponent) - ]" + e, "red")
    }
}

// remove channel
function remove(channel) {
    try {
        if (channel) {
            if (channel.outport && channel.inport) {
                network.disconnect(channel.outport, channel.inport)
            }
        }
        destroy(channel)
    } catch(e) {
        log("[xchannel.js(remove) - ]" + e, "red")
    }
}

// connection to xmas (canvas action)
function doConnect(port1,port2) {
    try {
        var outport = port1.type === Model.XPort.OUTPORT ? port1 : port2
        var inport = port2.type === Model.XPort.INPORT ? port2 : port1
        channel = null
        loadcomponent(outport,inport)
        network.connect(outport,inport)
        network.modified = true
    } catch(e) {
        log("[xchannel.js(doConnect) - ]" + e, "red")
    }
}

// connection from xmas (network open)
function create(outcomp,outport,incomp,inport) {
    try {
        channel = null
        var out_comp = getComponent(outcomp)
        var out_port = getPort(out_comp,outport)
        var in_comp = getComponent(incomp)
        var in_port = getPort(in_comp,inport)
        //log("in: " + in_comp.name + "." + in_port.name,"black")
        loadcomponent(out_port,in_port)
        return true
    } catch(e) {
        log("[xchannel.js(create) - ]" + e, "red")
    }
    return false
}

// find qml component by name
function getComponent(name){
    try {
        var childs = network.children
        for (var child in childs) {
            var chld = childs[child]
            if(chld.objectName==="component"
                    && chld.name === name ) {
                return chld
            }
        }
    } catch(e) {
        log("[xchannel.js(getComponent) - ]" + e, "red")
    }
    return null
}

// find qml port by name
function getPort(comp,name) {
    try {
        var childs = []
        childs.push(comp)
        while (childs.length > 0) {
            if (childs[0].objectName === "port"
                    && childs[0].name === name) {
                return childs[0]
            }
            for (var i in childs[0].data) {
                childs.push(childs[0].data[i])
            }
            childs.splice(0, 1);
        }
    } catch(e) {
        log("[xchannel.js(getPort) - ]" + e, "red")
    }
    return null;
}

// abort connecting (canvas wire)
function abortConnecting(port) {
    port = null
    channel = null
    component = null
}
