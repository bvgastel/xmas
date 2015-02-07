var channel = null;
var component = null;

function loadcomponent(port1,port2) {
    // component previously loaded?
    if (component !== null) {
        createComponent(port1,port2)
        return
    }
    component = Qt.createComponent("qrc:///qml/XChannel.qml")
    // Still loading qml? Call createObject on status changed
    if (component.status === Component.Loading)
        component.statusChanged.connect(createComponent(port1,port2));
    else //...status must be ready or error immediately.
        createComponent(port1,port2)
}

function createComponent(port1,port2) {
    if (component.status === Component.Ready && channel == null) {
        channel = component.createObject(sheet, {port1:port1,port2:port2})
    } else if (component.status === Component.Error) {
        channel = null
        console.log(component.errorString())
    }
}


function doConnect(port1,port2) {
    channel = null
    loadcomponent(port1,port2)

}


function abortConnecting(port) {
    //channel.destroy()
    //component.destroy()
    port.connected = false
    port = null
    channel = null
    component = null
}




