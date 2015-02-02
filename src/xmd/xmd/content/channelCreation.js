
var channel = null;
var component = null;

function doConnect(port1,port2) {
    if(channel === null) {
        if (createChannel(port1,port2)) {
            channel.port1 = port1;
            channel.port2 = port2;
            controller.channelCreated(channel)
            return true
        }
    }
    return false
}

function createChannel(port1,port2){
    component = Qt.createComponent("qrc:///qml/XChannel.qml");
    channel = component.createObject(sheet);
    if (channel === null) {
        console.log("Error creating channel");
        return false
    }
    channel.port1 = port1;
    channel.port2 = port2;
    return true ;
}

function abortConnecting(port) {
    //channel.destroy()
    //component.destroy()
    port.connected = false
    port = null
    channel = null
    component = null
}


//function checkTarget(connector) {
//    if (wire.connector1 && wire.connector1 !== connector && wire.connector2 !== connector) {
//        wire.connector2 = connector
//        wire.mouseX = connector.x + connector.parent.x
//        wire.mouseY = connector.y + connector.parent.y
//        wire.requestPaint()
//    } else {
//        wire.connector2 =  wire.connecting ? null : wire.connector2
//    }
//}

//function wiring(connector) {
//    console.log(connector.name)
//    if (wire.connector1) {
//        console.log("connectie gemaakt")
//        wire.connector1 = null
//        wire.connecting = false
//        //Code.doConnect(connector)
//        wire.requestPaint()
//    } else {
//        wire.connector1 = connector
//        wire.mouseX = connector.x + connector.parent.x
//        wire.mouseY = connector.y + connector.parent.y
//        wire.connecting = true
//        console.log("connectie bezig")
//    }

//}


