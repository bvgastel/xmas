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

	Als een gebruiker wijzigingen heeft, dan kan hij een patch file maken
	per commit en deze files doorsturen naar de anderen voor opname in hun
	repo (moeten we nog ervaring mee opdoen)


