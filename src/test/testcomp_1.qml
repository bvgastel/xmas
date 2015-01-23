import Model 1.0
//import QtQuick 2.0 // for QPoint

ChipComponent {

    id : testcomp1
    name : "testcomp1"
    network : null
    x : 1
    y : 1
    orientation : Up
    func : "function"
    connectors : [

        Inport {
            name : "testport1"
            rdy : "rdy1"
        },

        Outport {
            name : "testport2"
            rdy : "rdy2"
        }
    ]
}
