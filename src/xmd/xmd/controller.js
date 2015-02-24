.import XMAS 1.0 as Ctrl
.import QtQuick 2.0 as Qjs
.import "content/channelCreation.js" as Channel

function createNetwork(object) {
    if(!object) {
        log("Undefined network!","red")
        return
    }

    //clear sheet
    sheet.clear()

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
    //TODO check if name is unique
    if (component.status === Qjs.Component.Ready) {
        item = component.createObject(parent,
                                      {
                                          "x":object.x,
                                          "y":object.y,
                                          "orientation":object.orientation,
                                          "scale":object.scale,
                                          "name":object.name,
                                          "param":object.param
                                      })
        //TODO : if shared memory VT - XMD continues
        // controller emits the whole network at once
        // so check localy if everything goes well and
        // at the and feedback result as "networkCreated()"
        // NOTE: distinguish between sources of input (screen or json)
    } else if (component.status === Qjs.Component.Error) {
        log(component.errorString(),"red")
    }
}

function destroy(component){
    component.destroy()
    controller.componentDestroyed(component)
}


function destroyAll(sheet){
    //only delete xmas children who has
    //sheet as parent
    // destroy doesn't work on array items
    //TODO destroy sheet and create new one
    var temp = []

    for (var child in sheet.children) {
        if(sheet.children[child].objectName!=="component"
                && sheet.children[child].objectName!=="channel") {
            temp.push(sheet.children[child])
            }
    }
    sheet.children = temp
    //log("Sheet cleared.","black")
}


// TODO : can be removed when xmd is finished
// tempory test function
function showXItems(sheet){
    var ccnt = 0
    var cnt1 = 0
    for (var child in sheet.children) {
        if(sheet.children[child].objectName==="channel") {
            ccnt++
        }
        if(sheet.children[child].objectName==="component") {
            cnt1++
        }
    }
    log("sheet has " + ccnt + " channels and " + cnt1 + " components","blue")
}

