import Model 1.0

Network {
    id : network1
    name : "network1"
    width:5
    height:5

    components : [
        ChipComponent {
            id : testcomp1
            name : "testcomp1"
            network : network1
            x : 1
            y : 1
            orientation : Up
            func : "function"
            connectors : [

                Port {
                    id: testcomp1_in1
                    name : "in1"
                    rdy : "rdy1"
                    comp : testcomp1
                },

                Port {
                    id : testcomp1_out1
                    name : "out1"
                    rdy : "rdy2"
                    comp : testcomp1
                }
            ]
        },
        ChipComponent {
            id : testcomp2
            name : "testcomp2"
            network : network1
            x : 1
            y : 1
            orientation : Up
            func : "function"
            connectors : [

                Port {
                    id: testcomp2_in1
                    name : "in1"
                    rdy : "rdy1"
                    comp : testcomp1
                },

                Port {
                    id : testcomp2_out1
                    name : "out1"
                    rdy : "rdy2"
                    comp : testcomp1
                }
            ]
        }
    ]

    channels: [
        Channel {
            name : "testchannel1"
            network : "network1"
            initiator : testcomp1
            initiator_port : testcomp1_out1
            target : testcomp2
            target_port : testcomp2_in1
            datatype : "test_datatype1"

            ptList : [
                GridPoint { x:2; y:2 },
                GridPoint { x:3; y:2 }
            ]

        }
    ]

}
