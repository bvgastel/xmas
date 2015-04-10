Findings.md

type = bug, feature
prio =  high    means "directly decreases benefit from using tool in essential area"
        medium  means "decreases benefit from using tool in non-essential area"
        low     means "outside user interface (like vt) or not important to use"
                   or "in user interface, but workable or with workaround"
D    =  Developer
        b = Bernard, g = Guus, s = Stefan, j = Jeroen

   Nr   D   date        date                                                type
            open        solved      Problem                                 find    Prio   Cause (only with added info)
-----   -   -----       ----------  --------------------------------------- ----    -----  ------------------------------------------
   13   g   2015-04-02              Remove library composite                bug     high    Not implemented !
   15   g   2015-04-02              After new, file is still '*'            bug     medium  Confirmed 2015-04-10
   28   g   2015-04-09              New aborts (Linux only)                 bug     high    Confirmed 2015-04-10
   29   g   2015-04-10              Valgrind Memory Analyzer output         bug     high    xmd and xmv have access errors

   12   g   2015-04-02  fridge      Source parse expression does not accept bug     low     (Out of scope for user interface)
                                    a percentage token (%)
   23   g   2015-04-04  fridge      Plugin output too verbose               feature medium  needs one output line per execution
   24   g   2015-04-04  fridge      Plugin parameters can not modify        bug     medium
99999



   Nr   D   date        date                                                type
            open        solved      Problem                                 find    Prio   Cause (only with added info)
-----   -   -----       ----------  --------------------------------------- ----    -----  ------------------------------------------
    1   g   2015-03-31  2015-04-02  Create network, save file, restart app  bug     high    Solved: moved project.reset to datacontrol
    2   g   2015-03-31  2015-04-09  Create network, save file, restart app  bug     high    Parsing of expressions fail (all)
                                    Read network: Queue: size gone                          (see 17)
    3   g   2015-03-31  2015-04-02  Create network, save file, restart app  bug     high    Position is not written to json (see 11)
                                    Read network: position gone
    4   g   2015-03-31  2015-30-31  Adding expression to src or function    bug     high    Qml correction applied: didn't check correctly.
                                    even if connected
    5   g   2015-04-01  2015-04-09  Reading a file aborts                   bug     high    Solved dus to correcting other errors
                                                                                            Not sure what solved it.
    6   g   2015-04-01  2015-04-09  Composite Network ext. not filled       bug     high
                                    properly
    7   g   2015-04-01  2015-04-01  New: aborts                             bug     high    Removed some "exit(-1)" from xmv/datamodel
    8   g   2015-04-01  2015-04-09  Does not parse source expr correctly    bug     high
                                    from json when reading network.
    9   g   2015-04-01  2015-04-01  save json does not export PACKET_TYPE   bug     high    Solved.
   10   g   2015-04-01  2015-04-02  New aborts, only in certain connected   bug     medium  Delete during qml: solved by removing channel only once!
                                    conditions.
   11   g   2015-04-02  2015-04-02  Save json does not save the position    bug     high    Solved: added to ::Export(allcomponents)
   14   g   2015-04-02              After first save, file is still '*'     bug     medium
   16   g   2015-04-02  2015-04-10  src = green without expr (sometimes)    bug     medium
   17   g   2015-04-02  2015-04-09  Parsing of all expressions fail         bug     high
   18   g   2015-04-02  2015-04-04  Abort when closing application          bug     high
   19   g   2015-04-03  2015-04-03  When a file just read: already '*'      bug     medium  Annoys the hell out of me / solved
                                    Expected: not '*' until modified
   19x  g   2015-04-02  2015-04-04  New aborts again .... pffff             bug     high
   20   s   2015-04-03  2015-04-07  component delete --> no xmas delete     bug     high
   21   s   2015-04-03  2015-04-09  Retrieving ports for composites aborts  bug     high
                                    Composite.cpp::getPorts()
   22   g   2015-04-04  2015-04-09  new does not clear components from canv bug     high
   25   g   2015-04-04  2015-04-09  Random comps have no canvasdata in json bug     high
   26   g   2015-04-04  2015-04-07  Read file: components never connected   but     high    Solved when using proper project.reset.
   27   j   2015-04-04  2015-04-09  Component position not saved in         bug     high    Cause is qml timing (stefan is looking into it)
                                    extension upon first placement
99999

