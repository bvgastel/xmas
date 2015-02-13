XmvTest build instructions
==========================

Overview
--------

The xmvtest is meant solely as a regression test and
a basis for feature building (using regular unit test
or even TDD).

Remarks: convert.cpp is temporarily in this directory until
         its proper place is determined.

Environment
-----------

You need Google test installed in order to compile and run
xmvtest.

bitpowder and xmv must be compiled and deployed.

Build procedure from qt-creator
-------------------------------

- run clean all if necessary
- run qmake
- run build all
- run deplay all

you should be able to run now.
Be sure to add the deploy step with make
and parameter install.




