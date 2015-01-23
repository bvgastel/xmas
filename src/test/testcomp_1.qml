import Model 1.0
//import QtQuick 2.0 // for QPoint

ChipComponent {

    id : testcomp1
    name : "testcomp1"
    network : "testnetwork1"
    x : 1
    y : 1
    orientation : Up
    func : "function"
    connectors : [

        Port {
            name : "testport1"
            rdy : "rdy1"
            comp : testcomp1
        },

        Port {
            name : "testport2"
            rdy : "rdy2"
            comp : testcomp1
        }
    ]
}
