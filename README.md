Inhoudsopgave
=============

- notulen			De notulen (alleen de tex sources)
- voorbereiding 	De werkbestanden tijdens de planningsfase
- xmas				Documentatie bestanden (pdf)


Gebruiksaanwijzing
==================

We gebruiken git voor communicatie en hebben de volgende
use cases:

1. Nieuwe repo

	Bij het creeren van een nieuwe repository voor een ander teamlid 
	of voor een backup gebruik je git bundle

	example:

	machine1/xmas> git bundle create ../xmas.bundle master

	Git creert nu een bestand xmas.bundle in de bovenliggende directory
	vanuit de branch master. Dit bestand is basis voor een kopie op
	een andere machine of bij een andere gebruiker. Stuur dit bestand
	via email of met ftp naar een andere machine of andere gebruiker.
	Bij aankomst op deze machine in de directory waar xmas.bundle staat

	machine2> git clone xmas.bundle xmas -b master
	
	Dit creert de subdirectory xmas vanuit de branch master. Alle history
	van master zit nu in de nieuwe xmas.

2. Patches

	Alle ontwikkelaars werken decentraal. We verklaren 1 git repo tot de 
	centrale repo. Deze centrale repo kan op een lokale machine staan, of
	op een daarvoor bedoelde server.

	De ontwikkelaars werken elk in hun eigen repo en maken voor hun wijzigingen 
	hun eigen branch. 

	Als de ontwikkelaar wijzigingen heeft, dan kan hij een patch file maken
	per commit en deze files doorsturen naar de anderen voor opname in hun
	repo (moeten we nog ervaring mee opdoen)

	Het gemakkelijkste is werken vanuit een branch. Stel je hebt een repo
	voor xmas en je wilt een modules aanpassen voor een specifieke feature
	copy-en-paste. Als eerste creeer je een nieuwe branch:

	1. git checkout -b copy-en-paste
	2. Doe je wijzigingen
	3. git commit -m "copy klaar voor QA"

	Naar aanleiding van de QA heb je wijzigen op de software en de documentatie
	en doe je weer een commit van de wijzigingen.

	4. git commit -m "copy QA akkoord"

	En na finaal akkoord kun je de patches uitbrengen. Wat je nu moet doen is
	naar git opgeven dat je de verschillen wilt **ten opzichte van de master**.
	
	5. git format-patch master

	Dit geeft een patch met precies de wijzigingen die gaan van master naar 
	een versie inclusief je wijzigingen. Deze patch genereert 2 files:

	0001-copy-klaar-voor-QA
	0002-copy-QA-akkoord

	Deze twee files moet een ieder verwerken in zijn/haar reposirtory om
	dezelfde te krijgen als de originele ontwikkelaar. Eerst




