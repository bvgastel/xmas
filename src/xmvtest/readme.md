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
- run deploy all

you should be able to run now.

If you are continuously testing, but do not need
to publish, you can disable or remove
the make install deployment step from your
run configuration.

Don't forget to add it, when you do want to publish.

When running, by default you also deploy.




