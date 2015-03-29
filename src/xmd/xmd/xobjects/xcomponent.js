.import XMAS.model 1.0 as Model
.import XMAS 1.0 as XMAS
.import QtQuick 2.0 as Qjs

var draggedItem = null;
var posnInWindow;

function startDrag(mouse)
{
    posnInWindow = item.mapToItem(network,-50,-50)
    loadComponent(item.componentFile)
}


function loadComponent(qml) {
    var component = Qt.createComponent(qml)
    if (component.status === Qjs.Component.Loading)
        component.statusChanged.connect(createComponent(network,component));
    else if (component.status === Qjs.Component.Ready)
        createComponent(network,component)
    else if (component.status === Qjs.Component.Error)
        log(XMAS.Util.Designer,component.errorString(),"red")
}

function createComponent(parent,component) {
    if (component.status === Qjs.Component.Ready && draggedItem == null) {
        draggedItem = component.createObject(parent,{"x":posnInWindow.x, "y": posnInWindow.y})
        if(draggedItem.type === Model.XComponent.Composite){
            draggedItem.url = item.url
            draggedItem.alias = item.alias
            draggedItem.image = item.source
            draggedItem.boxed = item.boxed
        }
    } else if (component.status === Qjs.Component.Error) {
        draggedItem = null
        log(XMAS.Util.Designer,component.errorString(),"red")
    }
}

function continueDrag(mouse)
{
    if (draggedItem == null)
        return;
    draggedItem.x = mouse.x/network.scale + posnInWindow.x
    draggedItem.y = mouse.y/network.scale + posnInWindow.y
}

function endDrag()
{
    if (draggedItem == null)
        return;
    if (draggedItem.x < network.x
            || draggedItem.x > network.x + network.width - draggedItem.width
            || draggedItem.y < network.y
            || draggedItem.y > network.y + network.height - draggedItem.height) {
        draggedItem.destroy();
        draggedItem = null;
    } else {
        draggedItem.index = generateTagIndex(draggedItem)
        draggedItem.name = draggedItem.prefix + draggedItem.index
        if (!network.addComponent(draggedItem)){
            draggedItem.destroy();
        }
        draggedItem = null;
    }
}

function generateTagIndex(item)
{
    var max = -1
    for(var child in network.children){
        if(network.children[child].objectName==="component"){
            if(item.type === network.children[child].type) {
                max = Math.max(network.children[child].index,max)
            }
        }
    }
    //idList.sort(function(a, b){return a-b})
    return (++max)
}

