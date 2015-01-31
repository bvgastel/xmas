

function loadComponent(object) {

    var qml = ""
    var type = "fork"
    var xComponent = null;
    switch(type) {
    case "queue":
        qml =  "../qml/queue.qml"
        break;
    case "function":
        qml =  "../qml/function.qml"
        break;
    case "fork":
        qml =  "../qml/fork.qml"
        break;
    case "join":
        qml =  "../qml/join.qml"
        break;
    case "switch":
        qml =  "../qml/switch.qml"
        break;
    case "merge":
        qml =  "../qml/merge.qml"
        break;
    case "sink":
        qml =  "../qml/sink.qml"
        break;
    case "source":
        qml =  "../qml/source.qml"
        break;
    case "in":
        qml =  "../qml/in.qml"
        break;
    case "out":
        qml =  "../qml/out.qml"
        break;
    default:
        qml =  "../qml/spidergon.qml" //test
    }
    xComponent = Qt.createComponent(qml);
    if (xComponent.status === Component.Loading)
        xComponent.statusChanged.connect(createComponent(xComponent));
    else if (xComponent.status === Component.Ready)
        createComponent(xComponent);
    else if (xComponent.status === Component.Error)
        console.log(xComponent.errorString())

}

function createComponent(component) {
    var item
    if (component.status === Component.Ready) {
        item = component.createObject(sheet, {"x": 200, "y": 200, "z": 3});

    } else if (component.status === Component.Error) {
        item = null;
        console.log(component.errorString());
    }

}

