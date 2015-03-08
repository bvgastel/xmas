var draggedItem = null;
var posnInWindow;

function startDrag(mouse)
{
    posnInWindow = toolbarItem.mapToItem(sheet,75,75)
    loadComponent(toolbarItem.componentFile)
}


function loadComponent(qml) {
    var component = Qt.createComponent(qml)
    if (component.status === Component.Loading)
        component.statusChanged.connect(createComponent(sheet,component));
    else if (component.status === Component.Ready)
        createComponent(sheet,component)
    else if (component.status === Component.Error)
        log(component.errorString(),"red")
}

function createComponent(parent,component) {
    if (component.status === Component.Ready && draggedItem == null) {
        draggedItem = component.createObject(parent,{"type": component.type, "x":posnInWindow.x, "y": posnInWindow.y})
     } else if (component.status === Component.Error) {
        draggedItem = null
        log(component.errorString(),"red")
    }
}

function continueDrag(mouse)
{
    if (draggedItem == null)
        return;
    draggedItem.x = mouse.x/sheet.scale + posnInWindow.x
    draggedItem.y = mouse.y/sheet.scale + posnInWindow.y
}

function endDrag()
{
    if (draggedItem == null)
        return;
    if (draggedItem.x < sheet.x
            || draggedItem.x > sheet.x + sheet.width - draggedItem.width
            || draggedItem.y < sheet.y
            || draggedItem.y > sheet.y + sheet.height - draggedItem.height) {
        draggedItem.destroy();
        draggedItem = null;
    } else {
        draggedItem.uid = generateId()
        draggedItem.index = generateTagIndex(draggedItem)
        draggedItem.name = draggedItem.prefix + draggedItem.index
        //datacontrol.componentCreated(draggedItem)
        //log("component created!")

        draggedItem = null;
    }
}


function generateId()
{
    var max = -1
    for(var child in sheet.children){
        if(sheet.children[child].objectName==="component"){
            max = Math.max(sheet.children[child].cid,max)
        }
    }
    return (++max)
}

function generateTagIndex(item)
{
    var max = -1
    for(var child in sheet.children){
        if(sheet.children[child].objectName==="component"){
            if(item.type === sheet.children[child].type) {
                max = Math.max(sheet.children[child].index,max)
            }
        }
    }
    //idList.sort(function(a, b){return a-b})
    return (++max)
}

