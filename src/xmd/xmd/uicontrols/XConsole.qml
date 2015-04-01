/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import "qrc:/ui/uicontrols/"
import XMAS 1.0 as XMAS

Rectangle{
    id:output
    z:10
    property int lastHeight
    property int headerHeight:25
    property bool open: false

    gradient: Gradient {
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 1.0; color: "grey" }
    }


    function log(message,color,name){
        if(color === "" || color === null || color === undefined) color ="black"
        if(name===undefined || name==="") {
            tabview.designerLogSig(message,color)
        } else {
            tabview.pluginLogSig(message,color,name)
        }
    }

    function addPlugin(name){
        var tab = tabview.addTab(name,plug)
        if(tab){
            tab.active = true
            tab.item.write("Plugin " + name + " loaded!","green")
            tab.item.name = name
        }
    }

    Settings {
        category: "outputLog"
        property alias lastHeight: output.lastHeight
        property alias open: output.open
    }

    // Plugin component to create tabs
    Component{id:plug
        XPlugin{
            id:pluginLog
            anchors.fill: parent
            anchors.margins: 5
            visible: open //to hide scrollbar
            Connections{
                target: tabview
                onPluginLogSig: if(pluginLog.name===name)pluginLog.write(message,color)
                onClearLog:if(pluginLog.parent.visible)pluginLog.clear()
            }
        }
    }

    TabView{
        id:tabview
        anchors.fill: parent
        tabPosition: Qt.TopEdge
        frameVisible: false
        signal designerLogSig(var message, var color)
        signal pluginLogSig(var message, var color)
        signal clearLog()

        Tab {
            id:designerTab
            title: "Designer"
            active: true
            Log{
                id:designerLog
                anchors.fill: parent
                anchors.margins: 5
                visible: open //to hide scrollbar
                Connections{
                    target: tabview
                    onDesignerLogSig: designerLog.write(message,color)
                    onClearLog:if(designerTab.visible)designerLog.clear()
                }
            }
        }

        style: TabViewStyle {
            frameOverlap: 1
            tab: Rectangle {
                gradient: styleData.selected ? blueGradient : greyGradient
                implicitWidth: Math.max(text.width + 4, 80)
                implicitHeight: headerHeight
                radius: 2
                Text {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title
                    color: styleData.selected ? "white" : "black"
                }
            }
            tabBar:
                Rectangle {
                height: headerHeight
                Layout.fillWidth: true
                gradient:greyGradient
                border.width: 0
            }
        }
    }

    RowLayout{
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        Layout.minimumHeight: headerHeight
        Layout.maximumHeight: headerHeight
        ToolButton{
            action: clearLogAction
            implicitHeight: 20
            implicitWidth: 20
        }

        Image {
            id:arrow
            source: "qrc:/icons/content/arrow.png"
            MouseArea{
                anchors.fill:parent
                hoverEnabled: true
                onEntered: arrow.source = "qrc:/icons/content/arrowLit.png"
                onExited: arrow.source = "qrc:/icons/content/arrow.png"
                onClicked: open = !open
            }
        }
    }


    Gradient {
        id:greyGradient
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 0.4; color: "grey" }
        GradientStop { position: 1.0; color: "black" }
    }
    Gradient {
        id:blueGradient
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 0.4; color: "steelblue" }
        GradientStop { position: 1.0; color: "black" }
    }


    states: [
        State {
            when: !open
            PropertyChanges { target: arrow; rotation: 180 }
            PropertyChanges { target: output; height: headerHeight }
        }
        ,
        State {
            when: open
            PropertyChanges { target: arrow; rotation: 0 }
            PropertyChanges { target: output; height: lastHeight }
        }
    ]

    transitions:
        Transition {
        id: transOpenClose
        enabled: false
        PropertyAnimation { target: arrow; properties: "rotation"; duration: 250 }
        PropertyAnimation { target: output; properties: "height"; duration: 100 }
    }

    // release animation when settings are restored
    Component.onCompleted: {
        transOpenClose.enabled = true
    }

    Connections {
        target: mainwindow
        onClearLog: tabview.clearLog()
    }

}


