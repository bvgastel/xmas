TEMPLATE = app

WARNINGS += -Wall

CONFIG += console
CONFIG -= qt

HEADERS +=

SOURCES += \
    main.cpp \
    testcyclechecker.cpp \
    testdatamodel.cpp \
    jsonprinter.test.cpp \
    testsourcespec.cpp \
    testfunctionspec.cpp \
    testswitchfunctionspec.cpp \
    testjoinfunctionspec.cpp \
    flatten.test.cpp \
    testexport.cpp


CONFIG += C++11
CONFIG += link_prl

CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DISTFILES += \
    readme.md

################################################
# INSTALL instructions
################################################
unix|win32 {
 target.path=$$PWD/../../bin
 INSTALLS += target
}

################################################
# Dependencies
################################################
BASE=..
include(../xmv/vt/vt.pri)

linux|win32: LIBS += -L$$PWD/../../lib -lgtest

macx: INCLUDEPATH += $$PWD/../../include/googletest/
macx: SOURCES += $$PWD/../../include/googletest/src/gtest-all.cc

INCLUDEPATH += $$PWD/../../include/googletest/include
DEPENDPATH += $$PWD/../../include/googletest/include


