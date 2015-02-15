

function loadComponent(type,object) {
    var qml = ""
    var component = null;
    console.log("type --> " + type + "object --> " + object)

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
        log(component.errorString(),"red")

}



function createComponent(parent,component) {
    var item = null
    if (component.status === Component.Ready) {
        item = component.createObject(parent)
        controller.componentCreated(component)
        log("Component created.","green")
    } else if (component.status === Component.Error) {
        log(component.errorString(),"red")
    }
}



function destroy(component){
    component.destroy()
    controller.componentDestroyed(component)
    log("Component deleted.","black")
}

function destroyAll(){
    sheet.children = ""
    log("Sheet cleared.","black")
}


