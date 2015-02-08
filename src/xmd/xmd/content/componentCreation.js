var draggedItem = null;
var posnInWindow;

function startDrag(mouse)
{
    posnInWindow = toolbarItem.mapToItem(sheet);
    loadComponent(toolbarItem.componentFile);
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
    // TODO: this if sequence has uncatered for "else branch" at the end
    // Needs closure w.r.t. if-statement branches
    if (component.status === Component.Ready && draggedItem == null) {
        draggedItem = component.createObject(parent)
        //controller.componentCreated(component)
    } else if (component.status === Component.Error) {
        draggedItem = null
        log(component.errorString(),"red")
    }
    // What happens if neither ready nor error? Or ready, but dragged? Is this possible?
}


function continueDrag(mouse)
{
    if (draggedItem == null)
        return;
    draggedItem.x = mouse.x/sheet.scale + posnInWindow.x
    draggedItem.y = mouse.y/sheet.scale + posnInWindow.y
}

function endDrag(mouse)
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
        //TODO generate a unique name or id for each component
        draggedItem.name = draggedItem.type +  "_???"
        controller.componentCreated(draggedItem)
        log("component created!","green")
        draggedItem = null;
    }
}
