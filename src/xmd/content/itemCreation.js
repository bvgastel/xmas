
var itemComponent = null;
var draggedItem = null;
var startingMouse;
var posnInWindow;

function startDrag(mouse)
{
    posnInWindow = toolbarItem.mapToItem(scene, 0, 0);
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
    if (itemComponent.status == Component.Loading)  //Depending on the content, it can be ready or error immediately
        component.statusChanged.connect(createItem);
    else
        createItem();
}

function createItem() {
    // TODO: this if sequence has uncatered for "else branch" at the end
    // Needs closure w.r.t. if-statement branches
    if (itemComponent.status == Component.Ready && draggedItem == null) {
        draggedItem = itemComponent.createObject(scene, {"image": toolbarItem.image, "x": posnInWindow.x, "y": posnInWindow.y, "z": 3});
        // make sure created item is above the ground layer
    } else if (itemComponent.status == Component.Error) {
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
    draggedItem.x = mouse.x/scene.scale + posnInWindow.x - startingMouse.x;
    draggedItem.y = mouse.y/scene.scale + posnInWindow.y - startingMouse.y;
}

function endDrag(mouse)
{
    if (draggedItem == null)
        return;
    if (draggedItem.x < scene.x
            || draggedItem.x > scene.x + scene.width - draggedItem.width
            || draggedItem.y < scene.y
            || draggedItem.y > scene.y + scene.height - draggedItem.height) {
        draggedItem.destroy();
        draggedItem = null;
    } else {
        draggedItem.created = true;
        draggedItem = null;
    }
}
