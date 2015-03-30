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
    id:output
    spacing:2

    signal clear
    signal write(var message, var color)

    Rectangle{
        id:pluginControlPanel
        Layout.preferredWidth: 300
        Layout.fillHeight: true
        color:"black"
        radius:5
        anchors.margins: 5
        border.color:"darkgrey"
        border.width:2

        gradient: Gradient {
            GradientStop { position: 0.0; color: "darkgrey" }
            GradientStop { position: 1.0; color: "grey" }
        }

        ColumnLayout{
            anchors.margins: 10

        Button {action: runVtAction}

        Button {action: stopVtAction}

        //TODO replace with plugin progress value
        ProgressBar {
            id:progressbar
            value: 50
            indeterminate: false
            minimumValue: 0
            maximumValue: 100

            style: ProgressBarStyle {
                background: Rectangle {
                    radius: 5
                    color: "darkgray"
                    border.color: "darkgray"
                    border.width: 0
                    implicitWidth: 200
                    implicitHeight: 18
                }
                progress: Rectangle {
                    border.width:1
                    border.color:"steelblue"
                    radius: 4
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "steelblue" }
                        GradientStop { position: 0.4; color: "lightsteelblue" }
                        GradientStop { position: 1.0; color: "steelblue" }
                    }
                    Item {
                        anchors.fill: parent
                        anchors.margins: 1
                        visible: progressbar.indeterminate
                        clip: true
                        Row {
                            Repeater {
                                Rectangle {
                                    color: index % 2 ? "steelblue" : "lightsteelblue"
                                    width: 20 ; height: progressbar.height
                                }
                                model: progressbar.width / 20 + 2
                            }
                            XAnimator on x {
                                from: 0 ; to: -40
                                loops: Animation.Infinite
                                running: progressbar.indeterminate
                            }
                        }
                    }
                }
            }
        }
        }


    }

    Log{
        id:log
        Layout.fillWidth: true
        Layout.fillHeight: true
        Connections{
            target: output
            onWrite: log.write(message,color)
            onClear: log.clear()
        }
    }


}

