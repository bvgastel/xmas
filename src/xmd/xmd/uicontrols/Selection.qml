import QtQuick 2.4

Rectangle{
    id:selection
    color:"transparent"
    border.width: 1
    border.color: "steelblue"
    x: Math.min(from.x, to.x)
    y: Math.min(from.y, to.y)
    width: Math.abs(from.x - to.x)
    height: Math.abs(from.y - to.y)
    state: "UNSELECT"

    property point from : Qt.point(0, 0)
    property point to : Qt.point(0, 0)
    property int margin:5
    property bool initialSizing: false
    property var selectedItems: []

    property rect rubberBandRectangle: Qt.rect(0,0,0,0)

    signal sizeChanged
    signal positionChanged
    signal showContextMenu

    function start(mouse){
        from = Qt.point(mouse.x,mouse.y)
        to = from
        state = "START"

    }

    function contains(item)
    {
        var r = itemRect(item)
        return (r.left >= selection.x
                && r.right <= selection.x + selection.width
                && r.top >= selection.y
                && r.bottom <= selection.y + selection.height)
    }

    function selectItems(items){
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

    function rubberBand(){
        var r = itemsRect()
        from = Qt.point(r.x,r.y)
        to = Qt.point(r.x + r.width,r.y + r.height)
        state = from===to ? "UNSELECT":"SELECT"
    }

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

    function updateInitialSize(mouse){
        if(initialSizing){
            to = Qt.point(mouse.x,mouse.y)
        }
    }

    function mapGroupTo(group,target) {
        for (var i = group.length; i-- > 0; ){
            var item = group[i]
            item.x = item.parent.mapToItem(target,item.x,0).x
            item.y = item.parent.mapToItem(target,0,item.y).y
            item.parent = target
        }
    }

    function map(item,point){
        return item.mapToItem(selection.parent,point.x,point.y)
    }

    //do not add children!!
    Item {id:container}

    MouseArea {
        id:mouseArea
        anchors.fill: parent
        preventStealing: false
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: parent
        drag.minimumX: -margin
        drag.minimumY: -margin
        drag.maximumX: sheet.width - selection.width + margin
        drag.maximumY: sheet.height - selection.height + margin
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
            var newFrom = Qt.point(selection.x,selection.y)
            var newTo = Qt.point(selection.x + selection.width, selection.y + selection.height)
            from = newFrom
            to = newTo
            mouseArea.cursorShape = Qt.ArrowCursor
        }
        onPositionChanged: selection.positionChanged()
    }

    SelectionHandle {
        id:tlh
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.top
        cursor:Qt.SizeFDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) {from.x = map(this,mouse).x; from.y = map(this,mouse).y}}
    }

    SelectionHandle {
        id:trh
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.top
        cursor:Qt.SizeBDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) {to.x = map(this,mouse).x; from.y = map(this,mouse).y}}
    }

    SelectionHandle {
        id:blh
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.bottom
        cursor:Qt.SizeBDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) {from.x = map(this,mouse).x; to.y = map(this,mouse).y}}
    }

    SelectionHandle {
        id:brh
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.bottom
        cursor:Qt.SizeFDiagCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) {to.x = map(this,mouse).x; to.y = map(this,mouse).y}}
    }
    SelectionHandle {
        id:lh
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.verticalCenter
        cursor:Qt.SizeHorCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) from.x = map(this,mouse).x}
    }

    SelectionHandle {
        id:rh
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.verticalCenter
        cursor:Qt.SizeHorCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) to.x = map(this,mouse).x}
    }

    SelectionHandle {
        id:th
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        cursor:Qt.SizeVerCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) from.y = map(this,mouse).y}
    }

    SelectionHandle {
        id:bh
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        cursor:Qt.SizeVerCursor
        onReleased: sizeChanged()
        onPositionChanged: {if(pressed) to.y = map(this,mouse).y}
    }


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
            }
        },
        State {
            name: "UNSELECT"
            PropertyChanges {
                target: selection
                visible: false
                initialSizing: false
            }
        }
    ]

}

