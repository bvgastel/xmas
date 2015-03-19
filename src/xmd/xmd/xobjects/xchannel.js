.import QtQuick 2.0 as Qjs
var channel = null;
var component = null;

function loadcomponent(outport,inport) {
    // component previously loaded?
    if (component !== null) {
        createComponent(outport,inport)
        return
    }
    component = Qt.createComponent("qrc:///xobjects/XChannel.qml")
    // Still loading qml? Call createObject on status changed
    if (component.status === Qjs.Component.Loading)
        component.statusChanged.connect(createComponent(outport,inport));
    else //...status must be ready or error immediately.
        createComponent(outport,inport)
}

function createComponent(outport,inport) {
    if (component.status === Qjs.Component.Ready && channel == null) {
        channel = component.createObject(sheet, {outport:outport,inport:inport})
    } else if (component.status === Qjs.Component.Error) {
        channel = null
        log(component.errorString(),"red")
    }
}


function doConnect(outport,inport) {
    channel = null
    loadcomponent(outport,inport)

}

//NOTE: isConnected() is a method in xmas: we should not set in qml
//TODO : portnames are references and must match json!!!
//TODO : avoid short naming
function create(iComp,iPort,tComp,tPort) {
    channel = null
    var ic = getComponent(iComp)
    var ip = getPort(ic,iPort)
    var tc = getComponent(tComp)
    var tp = getPort(tc,tPort)
    //log("target: " + tc.name + "." + tp.name)
    loadcomponent(ip,tp)
    ip.connected=true
    tp.connected=true
    return true
}

function getComponent(name){
    var childs = sheet.children
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
    //channel.destroy()
    //component.destroy()
    port.connected = false
    port = null
    channel = null
    component = null
}
