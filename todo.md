# TODO

## Stefan

*   -- 	plugin dialog verbeteren (8h, hoge prioriteit) 
*   -- 	plugin lijstje zou ik in c++ vullen en via een property aan qml geven ipv een javascript in mainwindow
*		Valid check visible
*		Required voor source en sink
*	--	Bij openen netwerk zijn porten niet connected: is xmas model wel gevuld ----> pending Jeroen's XMASProject effort.

*   -- 	commentaar uniform maken zoals al in sommige qml's het geval is (kijken of dit voldoet aan bvb Javadoc, of ander best practices)
*   -- 	is controller.js in xmapper nog nodig ? deze is een overblijfsel van de mapper , restanten via datacontroller.cpp of util.cpp
*    	zoomfit moet de canvas vullen , deze werkt nu als schaal 1:1 (0.5h)
*    	knooppunten in de connecties voorzien zoals Bernard voorstelde (4h werk , lage prioriteit)
*    	recent file list (0.5h werk)
*    	grid on-off-snap op de XPage (2h werk, lage prioriteit)

*   ok 	Koppeling netwerk met qml zal ik asap bekijken
*   ok 	settings voorzien (persistentie van window size , pos, aan-af vinkjes , outputlog , canvas,..) is in qml maar een klein uurtje werk voor een beduidende meerwaarde
*   xx 	save file dialog --> de widget versie heb ik getest maar het werk weegt niet op tegen dat van de open filedialog te gebruiken met een "open" knopje dan maar 
			(dit is een gekende workaround voor de saveAs)
*   ok 	packet window , wordt al iets gedaan met deze info? Er zou van deze inhoud een qml list moeten gemaakt worden zodat deze info kan gebruikt worden in de help bij de expressie input (is in oude tool ook zo)
*   ok 	toolbar, menubalk en contextmenu's in overeenstemming brengen (1h)
*   xx 	XNetwerk.qml inhoud splitsen in een XPage.qml onder uicontrols en XNetwerk.qml  + id "sheet" hernoemen naar "page" en "netwerk"  (30min werk)
*   ok 	Een kleine setup dialog voor het netwerk maken zodat de designer een alias , image, page size, kan toekennen aan het netwerk. 
		Bij hergebruik van het netwerk als composite wordt de alias en/of image getoont in de body
*   ok 	bedieningen met klavier consistent doorvoeren op alle items , bvb de delete knop moet ook werken op een geselecteerde connectie (1h)
*   ok 	Een map "symbols" maken zoals we voor lib en include hebben , daarin komen enkele icoontjes welke men via de netwerksetup kan kiezen , 
		bvb het agent symbol (klein qml werk met grote meerwaarde)
*   ok 	multitab op de outputlog (2h lage prioriteit)
*	ok	Openen netwerk lukt niet meer.

## Jeroen

*		XMASProject integratie met xmd / plugins.
*	--	Netwerk uitbreiden met extenties
*		XMASProject uitbreiden voor xmd / plugin use case.

* 	ok	xmas.h updaten ("required")
* 	ok	Flattener wijzigen en mergen, kopieren extenties
* 	ok	Parser updaten voor composites
* 	ok	Exporter updates voor composites


## Guus

*	--	lezen en schrijven files oplossen (export probleem) ---> samen met Stefan
*		compleet netwerk lezen en schrijven / testen
*   xx	plugin thread, main en process maken ---> stel uit, stel af
*		plugin integreren met qml --> met Stefan
*		opties voor plugins bepalen ---> met Stefan
*		plugin op timer uitvoeren
*	ok	plugin syntax checker volledig maken
*	--	plugin logger probleem oplossing (abort) -- removed
*	ok	abort onder linux bij deleten deel van connected netwerk
*	ok	packet aansluiting qml / xmas
*	ok	Bernard: VARS - PACKET_TYPE ? Doel, inhoud

