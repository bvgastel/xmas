var beginPort = null;
var endPort = null;
var connection = null

function beginConnecting(port) {
    beginPort = port;
    if (connection != null) { // Connection has been previously loaded
        createConnection();
        return;
    }

    connection = Qt.createComponent("../qml/XConnection.qml");
    if (connection.status === Component.Loading)  //Depending on the content, it can be ready or error immediately
        connection.statusChanged.connect(createConnection);
    else
        createConnection();
}

function createConnection(){
console.log("Connection created");
}

function continueConnecting(port) {
console.log("Continue connecting");
}

function endConnecting(port) {
console.log("End connecting");
}

