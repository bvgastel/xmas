.import XMAS.model 1.0 as Model
.import XMAS 1.0 as XMAS
.import QtQuick 2.0 as Qjs
var channel = null;
var component = null;

function loadcomponent(outport,inport) {
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
}

function createComponent(outport,inport) {
    if (component.status === Qjs.Component.Ready && channel == null) {
        channel = component.createObject(network, {outport:outport,inport:inport})
    } else if (component.status === Qjs.Component.Error) {
        channel = null
        log(XMAS.Util.Designer,component.errorString(),"red")
    }
}

function remove(channel) {
    if (channel) {
       network.disconnect(channel.outport, channel.inport)
    }
    destroy(channel)
}


function doConnect(port1,port2) {
    var outport = port1.type === Model.XPort.OUTPORT ? port1 : port2
    var inport = port2.type === Model.XPort.INPORT ? port2 : port1
    channel = null
    loadcomponent(outport,inport)
    network.connect(outport,inport)

}

//TODO : portnames are references and must match json!!!
//TODO : avoid short naming
function create(outcomp,outport,incomp,inport) {
    channel = null
    var out_comp = getComponent(outcomp)
    var out_port = getPort(out_comp,outport)
    var in_comp = getComponent(incomp)
    var in_port = getPort(in_comp,inport)
    loadcomponent(out_port,in_port)
    return true
}

function getComponent(name){
    var childs = network.children
    for (var child in childs) {
        var chld = childs[child]
        if(chld.objectName==="component"
                && chld.name === name ) {
            return chld
        }
    }
    return null
}

function getPort(comp,name){
    for (var port in comp.children) {
        if(comp.children[port].objectName==="port"
                && comp.children[port].name === name ) {
            return comp.children[port]
        }
    }
    return null
}

function abortConnecting(port) {
//    port.connected = false
    port = null
    channel = null
    component = null
}
