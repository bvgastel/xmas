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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

RowLayout{
    id:plugin

    property string name:""
    property bool enabled:true

    signal clear
    signal write(var message, var color)

    Item{
        id:pluginControlPanel
        Layout.preferredWidth: 300
        Layout.fillHeight: true
        ColumnLayout{
            anchors.fill: pluginControlPanel
            spacing: 4
            Rectangle{
                color:"darkgrey"
                border.width: 0
                height: 25
                Layout.fillWidth: true
                radius:5
                RowLayout{
                    anchors.fill:parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    spacing: 2
                    CheckBox {
                        id:enable
                        text:"enable"
                        focus: false
                        checked: plugin.enabled
                        onCheckedChanged: plugin.enabled = checked
                    }
                    ToolButton{action: timerAction; implicitHeight: 20; implicitWidth: 20}
                    ToolButton{action: runAction; implicitHeight: 20; implicitWidth: 20}
                    ToolButton{action: stopAction; implicitHeight: 20; implicitWidth: 20}
                    ProgressBar {
                        id:progressbar
                        value: plugin.enabled ? 50 : 0
                        //indeterminate: plugin.isRunning
                        minimumValue: 0
                        maximumValue: 100
                        implicitHeight: 15
                        enabled: plugin.enabled
                        Layout.fillWidth: true
                    }
                }
            }

            Rectangle{
                color:"lightgrey"
                border.width: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
                radius:5
                ListView {
                    anchors.fill: parent
                    anchors.margins: 5
                    contentWidth: parent.width-10
                    contentHeight: 10

                    header: Rectangle{
                        color:"darkgray"
                        anchors.left:parent.left
                        anchors.right:parent.right
                        height: 15
                        clip:true
                        Text{
                            text:"parameters"
                            anchors.fill: parent
                            anchors.centerIn: parent
                            color:"white"
                        }
                    }
                    clip:true
                    headerPositioning:ListView.OverlayHeader
                    enabled: plugin.enabled
                    model: plugincontrol.pluginParams(name)
                    delegate: Text{anchors.margins:10; text: name +  " - " + value}
                }
            }
        }
    }

    Log{
        id:log
        Layout.fillWidth: true
        Layout.fillHeight: true
        Connections{
            target: plugin
            onWrite: log.write(message,color)
            onClear: log.clear()
        }
    }

    Action {
        id: timerAction
        text: "Timer"
        tooltip: "Timer triggered run"
        iconSource: "qrc:/icons/content/timer.ico"
        iconName: "timer"
        enabled: plugin.enabled
        checkable: true
        checked: false
    }

    Action {
        id: runAction
        text: "Run"
        tooltip: "Run plugin"
        shortcut: "Ctrl+R"
        iconSource: "qrc:/icons/content/run.ico"
        iconName: "run"
        enabled: plugin.enabled && !timerAction.checked
        onTriggered: {
            plugincontrol.startPlugin(name)
            var params = plugincontrol.pluginParams(name)
            console.log("Param scanning....")
            for (var prop in params) {
                        console.log("Param :", prop, "=", params[prop])
                    }
        }

    }

    Action {
        id: stopAction
        text: "Stop"
        tooltip: "Stop plugin"
        iconSource: "qrc:/icons/content/stop.ico"
        iconName: "stop"
        enabled: plugin.enabled && !timerAction.checked
        onTriggered: plugincontrol.stopPlugin(name)
    }

    Timer {
        id: autoRunTimer
        interval: 10000 //every 10s
        running: timerAction.enabled && timerAction.checked
        repeat: true
        onTriggered: plugincontrol.startPlugin(name)
    }
}
