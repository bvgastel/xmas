

function loadComponent(type) {
    var qml = ""
    var component = null;
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
    component = Qt.createComponent(qml)
    if (component.status === Component.Loading)
        component.statusChanged.connect(createComponent(sheet,component));
    else if (component.status === Component.Ready)
        createComponent(sheet,component)
    else if (component.status === Component.Error)
        console.log(component.errorString())

}



function createComponent(parent,component) {
    var item = null
    if (component.status === Component.Ready) {
        item = component.createObject(parent)
        controller.componentCreated(component)
    } else if (component.status === Component.Error) {
        console.log(component.errorString())
    }
}



function destroy(component){
    component.destroy()
    controller.componentDestroyed(component)
}


