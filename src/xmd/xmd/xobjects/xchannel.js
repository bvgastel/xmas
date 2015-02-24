.import QtQuick 2.0 as Qjs
var channel = null;
var component = null;

function loadcomponent(port1,port2) {
    // component previously loaded?
    if (component !== null) {
        createComponent(port1,port2)
        return
    }
    component = Qt.createComponent("qrc:///xobjects/XChannel.qml")
    // Still loading qml? Call createObject on status changed
    if (component.status === Qjs.Component.Loading)
        component.statusChanged.connect(createComponent(port1,port2));
    else //...status must be ready or error immediately.
        createComponent(port1,port2)
}

function createComponent(port1,port2) {
    if (component.status === Qjs.Component.Ready && channel == null) {
        channel = component.createObject(sheet, {port1:port1,port2:port2})
    } else if (component.status === Qjs.Component.Error) {
        channel = null
        log(component.errorString(),"red")
    }
}


function doConnect(port1,port2) {
    channel = null
    loadcomponent(port1,port2)

}


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
