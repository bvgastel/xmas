//.import XMAS 1.0 as xmas
.import QtQuick 2.0 as Qjs
.import "../xobjects/xchannel.js" as Channel

var start
var end
var time

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

//    start = new Date().getTime()
    component = Qt.createComponent(qml)
//    end = new Date().getTime()
//    log("Execution time: " + (end - start) + "ms","red")

    if (component.status === Qjs.Component.Loading)
        component.statusChanged.connect(createComponent(sheet,component,object))
    else if (component.status === Qjs.Component.Ready)
        createComponent(sheet,component,object)
    else if (component.status === Qjs.Component.Error)
        log(component.errorString(),"red")

}


function getXQml(type) {
    switch(type) {
    case "queue":
        return "../xobjects/queue.qml"
    case "function":
        return "../xobjects/function.qml"
    case "fork":
        return "../xobjects/fork.qml"
    case "join":
        return "../xobjects/join.qml"
    case "switch":
        return "../xobjects/switch.qml"
    case "merge":
        return "../xobjects/merge.qml"
    case "sink":
        return "../xobjects/sink.qml"
    case "source":
        return "../xobjects/source.qml"
    case "in":
        return "../xobjects/in.qml"
    case "out":
        return "../xobjects/out.qml"
    case "composite":
        return "../xobjects/spidergon.qml" //test
    default:
        log("Unknown xmas type! (" + type + ")","red")
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
                                      });

        //TODO : if shared memory VT - XMD continues
        // datacontrol emits the whole network at once
        // so check localy if everything goes well and
        // at the and feedback result as "networkCreated()"
        // NOTE: distinguish between sources of input (screen or json)
        datacontrol.componentCreated(component)
    } else if (component.status === Qjs.Component.Error) {
        log(component.errorString(),"red")
    }
}

function destroy(component){
    component.destroy()
    datacontrol.componentDestroyed(component)
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

