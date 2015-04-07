TEMPLATE = app

WARNINGS += -Wall

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \


SOURCES += \
    main.cpp \
    parser.test.cpp \
    parser_json.test.cpp \
    simplestring.test.cpp \
    stringparse.test.cpp


CONFIG += C++11
CONFID += link_prl

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
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../bitpowder

unix|win32|macx: LIBS += -L$$PWD/../../lib -lgtest

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
