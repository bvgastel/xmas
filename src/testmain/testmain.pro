# testmain.pro
# Contains the main program for the test programs of xmas

QT       += core
QT      += testlib
QT      += qml

CONFIG += C++11
CONFIG += console

TEMPLATE = app

HEADERS +=

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../test/release/ -ltest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../test/debug/ -ltest
else:unix: LIBS += -L$$OUT_PWD/../test/ -ltest

INCLUDEPATH += $$PWD/../test
DEPENDPATH += $$PWD/../test
