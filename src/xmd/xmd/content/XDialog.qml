import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import XMAS 1.0

//TODO : Replace with a custom frameless dialog
Dialog {
    visible: false
    modality: Qt.WindowModal
    standardButtons: StandardButton.Cancel | StandardButton.Ok
    onDiscard: visible = false
    onAccepted: null

//    RowLayout {
//        id: palette
//        anchors.fill: parent
//        spacing: 10
//    }

}

