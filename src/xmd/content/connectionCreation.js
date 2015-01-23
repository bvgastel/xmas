
var connection = null;
var component = null;

function doConnect(port) {

    if(connection === null) {
        if (createConnection(port)) {
            connection.connector1 = port;
            console.log("Connection created");
            return true
        }
    }
    else
    {
        endConnect(port);
    }
    return false
}

function createConnection(port){
    component = Qt.createComponent("qrc:qml/XConnection.qml");
    connection = component.createObject(scene, {"x": port.x, "y": port.y});
    if (connection === null) {
        console.log("Error creating connection");
        return false
    }
    connection.connecting = true
    return true ;
}

function abortConnecting(port) {
    //connection.destroy()
    //component.destroy()
    port.connected = false
    port = null
    connection = null
    component = null
}

function continueConnecting(port) {
    //console.log("Continue connecting");
}

function endConnect(port) {
    if (connection.connector1 !== null   )
    {
        connection.connector2 = port;
        connection.connecting = false
        console.log("Connection made.");
    }
    else
    {
        component.destroy()
        connection.destroy();
        console.log("Connection invalid!");
    }
}

