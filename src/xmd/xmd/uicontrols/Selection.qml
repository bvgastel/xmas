import QtQuick 2.4

Rectangle{
    id:selection

    // Properties
    color:"transparent"
    border.width: 1
    border.color: "steelblue"
    x: Math.min(from.x, to.x)
    y: Math.min(from.y, to.y)
    z:100
    width: Math.abs(from.x - to.x)
    height: Math.abs(from.y - to.y)
    state: "UNSELECT"
    property point from : Qt.point(0, 0)
    property point to : Qt.point(0, 0)
    property int margin:5
    property bool initialSizing: false
    property var selectedItems: []
    property rect rubberBandRectangle: Qt.rect(0,0,0,0)

    // Signals
    signal sizeChanged
    signal positionChanged
    signal showContextMenu

    // Event Handling
    Keys.onDeletePressed: deleteSelected(selectedItems)
    Keys.onEscapePressed: clear()
    Keys.onLeftPressed: doMove(-sheet.gridsize,0)
    Keys.onRightPressed: doMove(sheet.gridsize,0)
    Keys.onDownPressed: doMove(0,sheet.gridsize)
    Keys.onUpPressed: doMove(0,-sheet.gridsize)
    Keys.onPressed: { if(event.modifiers=== Qt.ControlModifier) sheet.selectionMode = true }
    Keys.onReleased: sheet.selectionMode = false


    // JavaScripts

    function start(mouse){
        clear()
        from = Qt.point(mouse.x,mouse.y)
        to = from
        state = "START"
    }

    // Clear selection
    function clear(){
        for (var item in selectedItems){
            if(selectedItems[item].selected !== undefined)
                    selectedItems[item].selected = false
        }
        selectedItems = []
        rubberBand()
    }

    // Check if item is in selected area
    function contains(item)
    {
        var r = itemRect(item)
        return (r.left >= selection.x
                && r.right <= selection.x + selection.width
                && r.top >= selection.y
                && r.bottom <= selection.y + selection.height)
    }

    // Select items
    function select(items){
        clear()
        for (var i in items){
            if(items[i].selected !== undefined)items[i].selected = true
                selectedItems.push(items[i])
         }
        rubberBand()
    }


    // Find items whitin selection bounds
    function find(items){
        selectedItems = []
        for (var item in items){
            if(contains(items[item]))
            {
                if(items[item].selected !== undefined)items[item].selected = true
                selectedItems.push(items[item])
            }
        }
        rubberBand()
    }

    // Rubberband on selected items
    function rubberBand(){
        var r = itemsRect()
        from = Qt.point(r.x,r.y)
        to = Qt.point(r.x + r.width,r.y + r.height)
        state = from===to ? "UNSELECT":"SELECT"
    }

    // Item bounds (left,top,right,bottom)
    function itemRect(item)
    {
        var arcsin = Math.abs(Math.sin(item.rotation * Math.PI / 180))
        var arccos = Math.abs(Math.cos(item.rotation * Math.PI / 180))
        var x0 = item.x + item.width * 0.5
        var y0 = item.y + item.height * 0.5
        var x1 = (item.width * arccos + item.height * arcsin) * item.scale * 0.5
        var y1 = (item.height * arccos + item.width * arcsin) * item.scale * 0.5
        return {left:x0 - x1,top:y0 - y1,right:x0 + x1,bottom:y0 + y1}
    }

    // Selection bounds (left,top,right,bottom)
    function itemsRect() {
        //Qml Item.childrenRect not working properly
        // in combination with anchors --> bug in Qt.5.4.0?!
        var list = selectedItems
        var xmin=1000000,xmax=-1,ymin=1000000,ymax=-1
        for (var item in list){
            var r = itemRect(list[item])
            xmin= Math.min(xmin, r.left - margin)
            xmax= Math.max(xmax, r.right + margin)
            ymin= Math.min(ymin, r.top - margin)
            ymax= Math.max(ymax, r.bottom + margin)
        }

        if(ymax<0) return Qt.rect(0,0,0,0)
        return Qt.rect(xmin,ymin,xmax-xmin,ymax-ymin)
    }

    // First time size is dragged by mousepointer
    // (Bottom right handle doesn't have the focus yet)
    function updateInitialSize(mouse){
        if(initialSizing){
            to = Qt.point(mouse.x,mouse.y)
        }
    }

    // Map group to the target its coordinates
    function mapGroupTo(group,target) {
        for (var i = group.length; i-- > 0; ){
            var item = group[i]
            item.x = item.parent.mapToItem(target,item.x,0).x
            item.y = item.parent.mapToItem(target,0,item.y).y
            item.parent = target
        }
    }

    // From at top left of To
    function fromTo(){
        var newFrom = Qt.point(selection.x,selection.y)
        var newTo = Qt.point(selection.x + selection.width, selection.y + selection.height)
        from = newFrom
        to = newTo
    }


    // Move selection (navigation keys)
    function doMove(dX,dY){
        fromTo()
        mapGroupTo(selectedItems,container)
        // horizontal
        if (from.x + dX < sheet.margin) {
            to.x += sheet.margin - from.x
            from.x = sheet.margin
        } else if (to.x + dX > sheet.width - sheet.margin){
            from.x += sheet.width - sheet.margin - to.x
            to.x = sheet.width - sheet.margin
        } else {
            to.x += dX
            from.x += dX
        }
        // vertical
        if (from.y + dY < sheet.margin) {
            to.y += sheet.margin - from.y
            from.y = sheet.margin
        } else if (to.y + dY > sheet.height - sheet.margin) {
            from.y += sheet.height - sheet.margin - to.y
            to.y = sheet.height - sheet.margin
        } else {
            from.y += dY
            to.y += dY
        }
        mapGroupTo(container.children,selection.parent)
        positionChanged()
    }

    // Delete selected items
    function deleteSelected(){
        var group = selectedItems
        for (var i = group.length; i-- > 0; ){
            if (group[i].selected)
                group[i].destroy()
        }
        clear()
    }

    // Container for selected items
    //do not add children!!
    Item {id:container}

    // Mouse area
    MouseArea {
        id:mouseArea
        anchors.fill: parent
        preventStealing: false
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: parent
        drag.minimumX: sheet.margin
        drag.minimumY: sheet.margin
        drag.maximumX: sheet.width - selection.width - sheet.margin
        drag.maximumY: sheet.height - selection.height - sheet.margin
        onPressed: {
            if (mouse.button == Qt.RightButton)
                showContextMenu()
            else {
                mapGroupTo(selectedItems,container)
                mouseArea.cursorShape = Qt.SizeAllCursor
            }
        }
        onReleased: {
            mapGroupTo(container.children,selection.parent)
            fromTo()
            mouseArea.cursorShape = Qt.ArrowCursor
        }
        onPositionChanged: selection.positionChanged()
    }

    // top left handle
    SelectionHandle {
        id:tlh
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.top
        cursor:Qt.SizeFDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) {
                from.x = mapToItem(selection.parent,mouse.x,0).x
                from.y = mapToItem(selection.parent,0,mouse.y).y
            }
        }
    }

    // top right handle
    SelectionHandle {
        id:trh
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.top
        cursor:Qt.SizeBDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) {
                to.x = mapToItem(selection.parent,mouse.x,0).x
                from.y = mapToItem(selection.parent,0,mouse.y).y
            }
        }
    }

    // bottom left handle
    SelectionHandle {
        id:blh
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.bottom
        cursor:Qt.SizeBDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) {
                from.x = mapToItem(selection.parent,mouse.x,0).x
                to.y = mapToItem(selection.parent,0,mouse.y).y
            }
        }
    }

    // bottom right handle
    SelectionHandle {
        id:brh
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.bottom
        cursor:Qt.SizeFDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) {
                to.x = mapToItem(selection.parent,mouse.x,0).x
                to.y = mapToItem(selection.parent,0,mouse.y).y
            }
        }
    }

    // left handle
    SelectionHandle {
        id:lh
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.verticalCenter
        cursor:Qt.SizeHorCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) from.x = mapToItem(selection.parent,mouse.x,0).x
        }
    }

    // right handle
    SelectionHandle {
        id:rh
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.verticalCenter
        cursor:Qt.SizeHorCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) to.x = mapToItem(selection.parent,mouse.x,0).x
        }
    }

    // top handle
    SelectionHandle {
        id:th
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        cursor:Qt.SizeVerCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) from.y = mapToItem(selection.parent,0,mouse.y).y
        }
    }

    // bottom handle
    SelectionHandle {
        id:bh
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        cursor:Qt.SizeVerCursor
        onReleased: sizeChanged()
        onPositionChanged: {
            if(pressed) to.y = mapToItem(selection.parent,0,mouse.y).y
        }
    }

    // selection states
    states: [
        State {
            name: "START"
            PropertyChanges {
                target: selection
                visible: true
                initialSizing: true
            }
        },
        State {
            name: "SELECT"
            PropertyChanges {
                target: selection
                visible: true
                initialSizing: false
                focus: true
            }
        },
        State {
            name: "UNSELECT"
            PropertyChanges {
                target: selection
                visible: false
                initialSizing: false
                focus: true
            }
        }
    ]
}
