# readme.md 2015

Author  Date   OK  Comment / reply
------  ----   --  -----------------------------------------
Stefan  26-03      Tussenpunten op connecties
gbo     18-02      Need a way to subscribe to log without qml knowing it.
gbo     22-03      ABORT: (alleen linux) bij f0-f1-f2, f0->f1, f1->f2, select (f0, f1), delete ==> bam! qml abort tijdens een delete actie
gbo     22-03      Global variables in JSON: need the same in user interface. Concerns: VARS, COMPOSITE_OBJECTS and PACKET_TYPE.
gbo     24-02      add scaling to toolbar, 100% default, 75%, 125% etc.
gbo     24-02  --  build alignment option: align to top, bottom, left, right. (partially solved with grid snap)
gbo     18-02  --  Eliminate implicit create on click of primitive. Create on drop.
                   // primitive is created on drag start to see its representation before drop so it can be aligned in the network
STEFAN  16/02  --  Reconnect when clicking on a connected port (very low priority)
gbo     18-02  --  Spread components without coordinates accross network. (now at 0,0) (too complicated)
STEFAN  16/02  --  cut/paste/copy (very low priority)
gbo     18-02  --  Full keyboard handling (low prio for now): creation, deletion, TAB, modification
                   // (as much as possible, its a drawing app and many things are difficult to handle with just keys)
gbo     17-03  ??  Queue size somehow does not trigger writeLogs: does it call Component? Function does show writeLogs.
STEFAN  16/02  xx  A connection is a rectangular line not crossing components (not nessecary anymore)
STEFAN  16/02  ok  Selecting a component doesn't deselect other items // opgelost
STEFAN  16/02  ok  network must have a grid with snap
STEFAN  16/02  ok  Group move + common network bounds
STEFAN  16/02  ok  Visualise valid expression entered
STEFAN  16/02  ok  Extend component context menu with scale and rotation
STEFAN  16/02  ok  Different cursor shape when move
STEFAN  16/02  ok  Save dialog
STEFAN  16/02  ok  Open dialog for composite
STEFAN  16/02  ok  Clear network when open file starts (xmd) // opgelost
STEFAN  16/02  ok  status bar is now just "ready" but is for VT progress + info
STEFAN  16/02  --  double click switch gives also 2 single click events which select and deselect (removed with restrictive join expression dialog)
STEFAN  16/02  ok  shortcuts // opgelost
STEFAN  16/02  ok  focus optimization , for default enter and escape key handling // opgelost (was voor dialogs)
STEFAN  16/02  ok  zoom fit must fit into window not 1on1 scale
STEFAN  16/02  --  zoom under cursor instead of left/top (clashes with flickable)
STEFAN  16/02  ok  cancel of expression dialog must reset input //opgelost
STEFAN  16/02  ok  network setup as a property from json file
STEFAN  16/02  ok  minimum mouse area size for small components
STEFAN  16/02  ok  view option - show component name or not
STEFAN  16/02  ok  view option - show port names or not
gbo     17-02  ok  duidelijke naamgeving: Channel.create (javascript imports naming is ChannelJs)
gbo     17-02  ok  functie voor clearNetwork() in js files // opgelost
gbo     18-02  ok  network.clear(): what happens to children? // qml ruimt op
gbo     18-02  ok  pick up object by connected port // opgelost
gbo     18-02  ok  impossible to modify name label //opgelost
STEFAN  18-02  ok  console log output displays component refused and not created while succesfully open a network // tempory solved, see comment
gbo     18-02  ok  Open file with ctrl-o.See: http://doc-snapshot.qt-project.org/qt5-5.4/qkeysequence.html (standard shortcuts)
                   // reply: QKeySequence is a C++ class and not available in qml. In qml it's string or default keys
                   // query: is QKeySequence available via a C++ signal? OK, finally found a way: "StandardKey.x" is the QML type for this.
gbo     18-02  ok  File "switch-and-merge.fjson" = type-error in channelCreation.js: 44 // opgelost - portnames xmd/vt must match
STEFAN  18-02  ok  when a netwerk is opened deselect and group select doesn't work // opgelost
STEFAN  18-02  ok  component name is uniek --> moet deze kunnen aangepast worden
STEFAN  18-02  ok  port name moet uniek zijn , kiezen we nummer of name
gbo     04-03  ok  Kunnen we de vtNameList uit onPluginsLoaded ergens bewaren?
gbo     04-03  ok  Add filename to header (or unnamed) or anywhere else (status bar?)
gbo     06-03  ok  Open file, create queue (2x): results in duplicate name (xmas refuses creation)
gbo     17-03  ok  source expression while not connected fails (correct). After connection need source expression again: during connect in c++?
gbo     23-03  ok  What gets entered into PACKET_TYPE? How to check? (network.cpp)
gbo     23-03  ok  What gets entered into VARS? How to check? (network.cpp & qml) ---> empty for now
gbo     23-03  ok  Don't ask for quit/save if network is empty (after deletion for instance)
gbo     23-03  ok  Really quit question discuss --> in setup
gbo     26-03  ok  Saving default home folder does not work
gbo     26-03  ok  Expression valid: bolletje blijft rood
gbo     26-03  ok  2x disconnect
