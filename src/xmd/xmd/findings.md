Findings.md

type = bug, feature
prio =  high    means "directly decreases benefit from using tool in essential area"
        medium  means "decreases benefit from using tool in non-essential area"
        low     means "outside user interface (like vt) or not important to use"
                   or "in user interface, but workable or with workaround"
D    =  Developer   b = Bernard, g = Guus, s = Stefan, j = Jeroen

   Nr   D   date        date                                                type
            open        solved      Problem                                 find    Prio   Cause (only with added info)
-----   -   -----       ----------  --------------------------------------- ----    -----  ------------------------------------------
    1   g   2015-03-31              Create network, save file, restart app  bug     high
                                    Read network: No Connections
    2   g   2015-03-31              Create network, save file, restart app  bug     high
                                    Read network: Queue: size gone
    3   g   2015-03-31              Create network, save file, restart app  bug     high
                                    Read network: position gone
    4   g   2015-03-31  2015-30-31  Adding expression to src or function    bug     high    Qml correction applied: didn't check correctly.
                                    even if connected
   99
