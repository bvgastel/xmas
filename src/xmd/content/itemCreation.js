
var itemComponent = null;
var draggedItem = null;
var startingMouse;
var posnInWindow;

function startDrag(mouse)
{
    posnInWindow = toolbarItem.mapToItem(sheet, 0, 0);
    startingMouse = { x: mouse.x, y: mouse.y }
    loadComponent();
}

//Creation is split into two functions due to an asynchronous wait while
//possible external files are loaded.
function loadComponent() {
    if (itemComponent != null) { // component has been previously loaded
        createItem();
        return;
    }

    itemComponent = Qt.createComponent(toolbarItem.componentFile);
    if (itemComponent.status === Component.Loading)  //Depending on the content, it can be ready or error immediately
        Component.statusChanged.connect(createItem);
    else if (itemComponent.status === Component.Ready)
        createItem();
    else if (itemComponent.status === Component.Error)
        console.log("Error loading component")
}

function createItem() {
    // TODO: this if sequence has uncatered for "else branch" at the end
    // Needs closure w.r.t. if-statement branches
    if (itemComponent.status === Component.Ready && draggedItem == null) {
        draggedItem = itemComponent.createObject(sheet, {"image": toolbarItem.image, "x": posnInWindow.x, "y": posnInWindow.y, "z": 3});
        // make sure created item is above the ground layer
    } else if (itemComponent.status === Component.Error) {
        draggedItem = null;
        console.log("error creating component");
        console.log(itemComponent.errorString());
    }
    // What happens if neither ready nor error? Or ready, but dragged? Is this possible?
}

function continueDrag(mouse)
{
    if (draggedItem == null)
        return;
    draggedItem.x = mouse.x/sheet.scale + posnInWindow.x - startingMouse.x;
    draggedItem.y = mouse.y/sheet.scale + posnInWindow.y - startingMouse.y;
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
        draggedItem.created = true;
        //controller.componentCreated(draggedItem)

        draggedItem = null;
    }
}
