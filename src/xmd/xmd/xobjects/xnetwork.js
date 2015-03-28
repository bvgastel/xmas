.import XMAS.model 1.0 as Model
.import XMAS 1.0 as XMAS
.import QtQuick 2.0 as Qjs
.import "qrc:/javascripts/xobjects/xchannel.js" as Channel

var start
var end
var time

// called by datacontrol signal
function createNetwork(object) {

    if(!object) {
        log(XMAS.Util.Designer,"Undefined network!","red")
        return
    }

    //clear network
    network.clear()

    // process list of components and create components
    var complist = object["complist"]
    for (var c in complist) {
        loadComponent(complist[c])
    }

    // process list of connections and create connections
    var connlist = object["channellist"]
    for (var conn in connlist) {
        var result = Channel.create(
                    connlist[conn]["outcomp"],
                    connlist[conn]["outport"],
                    connlist[conn]["incomp"],
                    connlist[conn]["inport"]
                    )
    }

}

function loadComponent(object) {
    var qml = ""
    var component = null
    var type = ""
    if(!object) {
        log(XMAS.Util.Designer,"Undefined object!","red")
        return
    }

    if(object.type!==undefined){
        qml = getXQml(object.type)
    } else {
        log(XMAS.Util.Designer,"Undefined type!" + object,"red")
        return
    }

    if(qml==="") return

//    start = new Date().getTime()
    component = Qt.createComponent(qml)
//    end = new Date().getTime()
//    log(XMAS.Util.Designer,"Execution time: " + (end - start) + "ms","red")

    if (component.status === Qjs.Component.Loading)
        component.statusChanged.connect(createComponent(network,component,object))
    else if (component.status === Qjs.Component.Ready)
        createComponent(network,component,object)
    else if (component.status === Qjs.Component.Error)
        log(XMAS.Util.Designer,component.errorString(),"red")

}


function getXQml(type) {
    switch(type) {
    case Model.XComponent.Queue:
        return "qrc:/xmas/xobjects/queue.qml"
    case Model.XComponent.Function:
        return "qrc:/xmas/xobjects/function.qml"
    case Model.XComponent.Fork:
        return "qrc:/xmas/xobjects/fork.qml"
    case Model.XComponent.Join:
        return "qrc:/xmas/xobjects/join.qml"
    case Model.XComponent.Switch:
        return "qrc:/xmas/xobjects/switch.qml"
    case Model.XComponent.Merge:
        return "qrc:/xmas/xobjects/merge.qml"
    case Model.XComponent.Sink:
        return "qrc:/xmas/xobjects/sink.qml"
    case Model.XComponent.Source:
        return "qrc:/xmas/xobjects/source.qml"
    case Model.XComponent.In:
        return "qrc:/xmas/xobjects/in.qml"
    case Model.XComponent.Out:
        return "qrc:/xmas/xobjects/out.qml"
    case Model.XComponent.Composite:
        return "qrc:/xmas/xobjects/composite.qml"
    default:
        log(XMAS.Util.Designer,"Unknown xmas type! (" + type + ")","red")
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
                                          "rotation":object.rotation,
                                          "scale":object.scale,
                                          "name":object.name,
                                          "expression":object.expression
                                      });

        //TODO : if shared memory VT - XMD continues
        // datacontrol emits the whole network at once
        // so check localy if everything goes well and
        // at the and feedback result as "networkCreated()"
        // NOTE: distinguish between sources of input (screen or json)
        //datacontrol.componentCreated(component) --> disabled, no longer necessary in new direct data model
    } else if (component.status === Qjs.Component.Error) {
        log(XMAS.Util.Designer,component.errorString(),"red")
    }
}

function destroy(component){
    component.destroy()
}


function destroyAll(network){
    //only delete xmas children who has
    //network as parent
    // destroy doesn't work on array items
    //TODO destroy network and create new one
    var temp = []

    for (var child in network.children) {
        if(network.children[child].objectName!=="component"
                && network.children[child].objectName!=="channel") {
            temp.push(network.children[child])
            }
    }
    network.children = temp
    //log(XMAS.Util.Designer,"network cleared.",XMAS.Util.Info)
}
