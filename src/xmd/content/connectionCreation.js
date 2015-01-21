
var connection = null;
var component = null;

function doConnect(port) {

    if(connection === null) {
        if (createConnection(port)) {
            connection.connector1 = port;
            console.log("Connection created");
        }
    }
    else
    {
        endConnect(port);

    }
}

function createConnection(port){
    component = Qt.createComponent("../qml/XConnection.qml");
    connection = component.createObject(scene, {"x": 0, "y": 0});
    if (connection === null) {
        console.log("Error creating connection");
        return false
    }
    return true ;
}


function continueConnecting(port) {
    //console.log("Continue connecting");
}

function endConnect(port) {
    if (connection.connector1 !== null   )
    {
        connection.connector2 = port;
        console.log("Connection made.");
    }
    else
    {
        connection.destroy();
        console.log("Connection invalid!");
    }
}

