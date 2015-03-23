# TODO

## Stefan

*    	Koppeling netwerk met qml zal ik asap bekijken
*    	plugin lijstje zou ik in c++ vullen en via een property aan qml geven ipv een javascript in mainwindow
*    	is controller.js in xmapper nog nodig ? deze is een overblijfsel van de mapper , restanten via datacontroller.cpp of util.cpp
*    	settings voorzien (persistentie van window size , pos, aan-af vinkjes , outputlog , canvas,..) is in qml maar een klein uurtje werk voor een beduidende meerwaarde
*    	save file dialog --> de widget versie heb ik getest maar het werk weegt niet op tegen dat van de open filedialog te gebruiken met een "open" knopje dan maar 
			(dit is een gekende workaround voor de saveAs)
*    	packet window , wordt al iets gedaan met deze info? Er zou van deze inhoud een qml list moeten gemaakt worden zodat deze info kan gebruikt worden in de help bij de expressie input (is in oude tool ook zo)
*    	Een kleine setup dialog voor het netwerk maken zodat de designer een alias , image, page size, kan toekennen aan het netwerk. Bij hergebruik van het netwerk als composite wordt de alias en/of image getoont in de body
*    	XNetwerk.qml inhoud splitsen in een XPage.qml onder uicontrols en XNetwerk.qml  + id "sheet" hernoemen naar "page" en "netwerk"  (30min werk)
*    	grid on-off-snap op de XPage (2h werk, lage prioriteit)
*    	knooppunten in de connecties voorzien zoals Bernard voorstelde (4h werk , lage prioriteit)
*    	recent file list (0.5h werk)
*    	toolbar, menubalk en contextmenu's in overeenstemming brengen (1h)
*    	bedieningen met klavier consistent doorvoeren op alle items , bvb de delete knop moet ook werken op een geselecteerde connectie (1h)
*    	commentaar uniform maken zoals al in sommige qml's het geval is (kijken of dit voldoet aan bvb Javadoc, of ander best practices)
*    	plugin dialog verbeteren (8h, hoge prioriteit) 
*    	multitab op de outputlog (2h lage prioriteit)
*    	Een map "symbols" maken zoals we voor lib en include hebben , daarin komen enkele icoontjes welke men via de netwerksetup kan kiezen , bvb het agent symbol (klein qml werk met grote meerwaarde)
*    	zoomfit moet de canvas vullen , deze werkt nu als schaal 1:1 (0.5h)

## Jeroen

* 		xmas.h updaten ("required")
* 		Flattener wijzigen en mergen
* 		Parser updaten voor composites
* 		Exporter updates voor composites

## Guus

* af	plugin syntax checker volledig maken
*   	plugin thread, main en process maken
*		plugin logger probleem oplossing (abort)
*		plugin integreren met qml
*		opties voor plugins bepalen
*		lezen en schrijven files oplossen (export probleem)
*		compleet netwerk lezen en schrijven / testen
*		abort onder linux bij deleten deel van connected netwerk

