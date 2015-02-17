//.import XMAS 1.0 as Ctrl
.import QtQuick 2.0 as Qjs
.import "content/channelCreation.js" as Channel

function createNetwork(object) {
    if(!object) {
        log("Undefined network!","red")
        return
    }

    clearNetwork();

    // process list of components and create components
    var complist = object["complist"]
    for (var c in complist) {
        loadComponent(complist[c])
    }

    // process list of connections and create connections
    var connlist = object["channellist"]
    for (var conn in connlist) {
        var result = Channel.create(
                    connlist[conn]["initiator"],
                    connlist[conn]["initiatorport"],
                    connlist[conn]["target"],
                    connlist[conn]["targetport"]
                    )
    }
}

function loadComponent(object) {
    var qml = ""
    var component = null
    var type = ""
    if(!object) {
        log("Undefined object!","red")
        return
    }

    if(object.type!==undefined){
        qml = getXQml(object.type)
    } else {
        log("Undefined type!" + object,"red")
        return
    }

    if(qml==="") return

    component = Qt.createComponent(qml)

    if (component.status === Qjs.Component.Loading)
        component.statusChanged.connect(createComponent(sheet,component,object));
    else if (component.status === Qjs.Component.Ready)
        createComponent(sheet,component,object)
    else if (component.status === Qjs.Component.Error)
        log(component.errorString(),"red")

}


function getXQml(type) {
    switch(type) {
    case "queue":
        return "../qml/queue.qml"
    case "function":
        return "../qml/function.qml"
    case "fork":
        return "../qml/fork.qml"
    case "join":
        return "../qml/join.qml"
    case "switch":
        return "../qml/switch.qml"
    case "merge":
        return "../qml/merge.qml"
    case "sink":
        return "../qml/sink.qml"
    case "source":
        return "../qml/source.qml"
    case "in":
        return "../qml/in.qml"
    case "out":
        return "../qml/out.qml"
    case "composite":
        return "../qml/spidergon.qml" //test
    default:
        //log("Unknown xmas type! (" + type + ")","red")
        return ""
    }
}



function createComponent(parent,component,object) {
    var item = null
    var x = 0
    var y = 0
    var orientation = 0
    var scale = 1.0
    var name = ""
    var param = ""
    if(object.x !== undefined) x=object.x
    if(object.y !== undefined) y=object.y
    if(object.orientation !== undefined) orientation=object.orientation
    if(object.scale !== undefined) scale=object.scale
    if(object.name !== undefined) name=object.name
    else console.log("name undefined")
    if(object.param !== undefined) param=object.param

    if (component.status === Qjs.Component.Ready) {
        item = component.createObject(parent,
                                      {
                                          "x":x,
                                          "y":y,
                                          "orientation":orientation,
                                          "scale":scale,
                                          "name":name
                                      })
        controller.componentCreated(item)
        log("Component created.","green")
    } else if (component.status === Qjs.Component.Error) {
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


