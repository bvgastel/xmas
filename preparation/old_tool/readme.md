Beschrijving WickedXmas gebruik op Windows PC
=============================================

Tools
-----

-WickedXmas.zip uitpakken bvb naar C:\WickedXmas
-TwoAgents.zip uitpakken in C:\WickedXmas\TwoAgents
-compiler van http://www.mingw.org/ onder C:\MinGW plaatsen
-omgevingsvariable PATH uitbreiden met C:\MinGW\bin


Stappen
-------
-start WickedXmas.exe
-open c:\WickedXmas\TwoAgents\TwoAgents.wck
-Verification 1 : bvb bewaren als twoagents.fjson
-Verification 2 : main.exe en genLP.exe worden gemaakt
-Verification 3 : toont de invarianten dmv genLP.exe
-Verification 4 : start main.exe voor deadlock detectie.
	          Venster met deadlock configuratie opent
                  en wordt in model getoont. (rode queue's)