# TODO

## Stefan

*   ok 	Koppeling netwerk met qml zal ik asap bekijken
*   ok 	settings voorzien (persistentie van window size , pos, aan-af vinkjes , outputlog , canvas,..) is in qml maar een klein uurtje werk voor een beduidende meerwaarde
*   xx 	save file dialog --> de widget versie heb ik getest maar het werk weegt niet op tegen dat van de open filedialog te gebruiken met een "open" knopje dan maar 
			(dit is een gekende workaround voor de saveAs)
*   ok 	packet window , wordt al iets gedaan met deze info? Er zou van deze inhoud een qml list moeten gemaakt worden zodat deze info kan gebruikt worden in de help bij de expressie input (is in oude tool ook zo)
*   ok 	toolbar, menubalk en contextmenu's in overeenstemming brengen (1h)
*   xx 	XNetwerk.qml inhoud splitsen in een XPage.qml onder uicontrols en XNetwerk.qml  + id "sheet" hernoemen naar "page" en "netwerk"  (30min werk)
*   -- 	Een kleine setup dialog voor het netwerk maken zodat de designer een alias , image, page size, kan toekennen aan het netwerk. 
		Bij hergebruik van het netwerk als composite wordt de alias en/of image getoont in de body
*   -- 	bedieningen met klavier consistent doorvoeren op alle items , bvb de delete knop moet ook werken op een geselecteerde connectie (1h)
*    	plugin dialog verbeteren (8h, hoge prioriteit) 
*    	plugin lijstje zou ik in c++ vullen en via een property aan qml geven ipv een javascript in mainwindow
*		Openen netwerk lukt niet meer.
*		C++ extention op qml
*    	recent file list (0.5h werk)
*		Valid check visible
*    	knooppunten in de connecties voorzien zoals Bernard voorstelde (4h werk , lage prioriteit)
*    	grid on-off-snap op de XPage (2h werk, lage prioriteit)
*    	commentaar uniform maken zoals al in sommige qml's het geval is (kijken of dit voldoet aan bvb Javadoc, of ander best practices)
*    	Een map "symbols" maken zoals we voor lib en include hebben , daarin komen enkele icoontjes welke men via de netwerksetup kan kiezen , bvb het agent symbol (klein qml werk met grote meerwaarde)
*    	zoomfit moet de canvas vullen , deze werkt nu als schaal 1:1 (0.5h)
*    	is controller.js in xmapper nog nodig ? deze is een overblijfsel van de mapper , restanten via datacontroller.cpp of util.cpp
*    	multitab op de outputlog (2h lage prioriteit)

## Jeroen

* 	ok	xmas.h updaten ("required")
* 	--	Flattener wijzigen en mergen, kopieren extenties
* 	--	Parser updaten voor composites
* 		Exporter updates voor composites
*		XMASProject integratie met xmd / plugins.
*		XMASProject uitbreiden voor xmd / plugin use case.
*	--	Netwerk uitbreiden met extenties

## Guus

* af	plugin syntax checker volledig maken
*		plugin logger probleem oplossing (abort)
*		lezen en schrijven files oplossen (export probleem)
*   	plugin thread, main en process maken
*		plugin integreren met qml
*		opties voor plugins bepalen
*		compleet netwerk lezen en schrijven / testen
*		abort onder linux bij deleten deel van connected netwerk
*		packet aansluiting qml / xmas
*		plugin op timer uitvoeren
*		Bernard: VARS - PACKET_TYPE ? Doel, inhoud

