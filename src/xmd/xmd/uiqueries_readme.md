# readme.md 2015

Author  Date    OK  Comment / reply
------  ----    --  -----------------------------------------
STEFAN  16/02   ok  Selecting a component doesn't deselect other items // opgelost
STEFAN  16/02       A connection is a rectangular line not crossing components
STEFAN  16/02       Sheet must have a grid with snap
STEFAN  16/02       Group move + common sheet bounds
STEFAN  16/02       Visualise valid expression entered
STEFAN  16/02       Extend component context menu with scale and rotation
STEFAN  16/02       Different cursor shape when move
STEFAN  16/02       Save dialog
STEFAN  16/02       Open dialog for composite
STEFAN  16/02       Reconnect when clicking on a connected port
STEFAN  16/02   ok  Clear sheet when open file starts (xmd) // opgelost
STEFAN  16/02       status bar is now just "ready" but is for VT progress + info
STEFAN  16/02       double click switch gives also 2 single click events which select and deselect
STEFAN  16/02   ok  shortcuts // opgelost
STEFAN  16/02   ok  focus optimization , for default enter and escape key handling // opgelost (was voor dialogs)
STEFAN  16/02       zoom fit must fit into window not 1on1 scale
STEFAN  16/02       zoom under cursor instead of left/top
STEFAN  16/02   ok  cancel of expression dialog must reset input //opgelost
STEFAN  16/02       sheet setup as a property from json file
STEFAN  16/02       minimum mouse area size for small components
STEFAN  16/02       view option - show component name or not
STEFAN  16/02       view option - show port names or not
STEFAN  16/02       cut/paste/copy
gbo     17-02       duidelijke naamgeving: Channel.create
gbo     17-02   ok  functie voor clearNetwork() in js files // opgelost
gbo     18-02   ok  sheet.clear(): what happens to children? // qml ruimt op
gbo     18-02   ok  pick up object by connected port // opgelost
gbo     18-02   ok  impossible to modify name label //opgelost
STEFAN  18-02   ok  console log output displays component refused and not created while succesfully open a network // tempory solved, see comment
gbo     18-02       Need a way to subscribe to log without qml knowing it.
gbo     18-02   --  Eliminate implicit create on click of primitive. Create on drop. // primitive is created on drag start to see its representation before drop so it can be aligned in the sheet
gbo     18-02   ok  Open file with ctrl-o.See: http://doc-snapshot.qt-project.org/qt5-5.4/qkeysequence.html (standard shortcuts)
                    reply: QKeySequence is a C++ class and not available in qml. In qml it's string or default keys
                    query: is QKeySequence available via a C++ signal? OK, finally found a way: "StandardKey.x" is the QML type for this.
gbo     18-02       Spread components without coordinates accross sheet. (now at 0,0)
gbo     18-02   ok  File "switch-and-merge.fjson" = type-error in channelCreation.js: 44 // opgelost - portnames xmd/vt must match
gbo     18-02       Full keyboard handling (low prio for now): creation, deletion, TAB, modification
STEFAN  18-02   ok  when a netwerk is opened deselect and group select doesn't work // opgelost
STEFAN  18-02   ok  component name is uniek --> moet deze kunnen aangepast worden
STEFAN  18-02       port name moet uniek zijn , kiezen we nummer of name
gbo     24-02       build alignment option: align to top, bottom, left, right.
gbo     24-02       add scaling to toolbar, 100% default, 75%, 125% etc.
gbo     04-03  ok   Kunnen we de vtNameList uit onPluginsLoaded ergens bewaren?
gbo     04-03       Add filename to header (or unnamed) or anywhere else (status bar?)
gbo     06-03       Open file, create queue (2x): results in duplicate name (xmas refuses creation)
