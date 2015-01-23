import Model 1.0

Network {
    name : "network1"
    board : {
            network: "network1"
            width:5
            height:5
        }

    components : [
        ChipComponent {
            id : testcomp1
            name : "testcomp1"
            network : "network1"
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
        }
    ]

    channels: [
        Channel {
            name : "testchannel1"
            network : "network1"
            initiator : testcomp1
            init_port : "out1"
            target : "testcomp2"
            target_port : "in1"
            datatype : "test_datatype1"

            ptList : [
                GridPoint { x:2; y:2 },
                GridPoint { x:3; y:2 }
            ]

        }
    ]

}
