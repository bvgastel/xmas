TEMPLATE = app

WARNINGS += -Wall

CONFIG += console
CONFIG -= qt

HEADERS += \


SOURCES += \
    main.cpp \
    parser.test.cpp \
    parser_json.test.cpp \
    simplestring.test.cpp \
    stringparse.test.cpp


CONFIG += C++11
CONFIG += link_prl

TARGET = bitpowdertest

# Windows needs _USE_MATH_DEFINES to define M_PI and so on.
win32: DEFINES += _USE_MATH_DEFINES

#include(deployment.pri)
#qtcAddDeployment()

unix|win32 {
    target.path = $$PWD/../../bin
    INSTALLS += target
}

################################################
# Dependencies
################################################
BASE=..
include(../bitpowder/bitpowder.pri)

linux|win32: LIBS += -L$$PWD/../../lib -lgtest

macx: INCLUDEPATH += $$PWD/../../include/googletest/
macx: SOURCES += $$PWD/../../include/googletest/src/gtest-all.cc

INCLUDEPATH += $$PWD/../../include/googletest/include
DEPENDPATH += $$PWD/../../include/googletest/include
