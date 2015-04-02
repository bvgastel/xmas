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
    5   g   2015-04-01              Reading a file aborts                   bug     high
    6   g   2015-04-01              Composite Network ext. not filled       bug     high
                                    properly
    7   g   2015-04-01  2015-04-01  New: aborts                             bug     high    Removed some "exit(-1)" from xmv/datamodel
    8   g   2015-04-01              Does not parse source expr correctly    bug     medium
                                    from json when reading network.
    9   g   2015-04-01  2015-04-01  save json does not export PACKET_TYPE   bug     high    Solved.
   10   g   2015-04-01  2015-04-02  New aborts, only in certain connected   bug     medium  Delete during qml: solved by removing channel only once!
                                    conditions.
   11   g   2015-04-02              Save json does not save the position    bug     high
   99
