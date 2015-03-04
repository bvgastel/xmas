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
# Internal dependencies
################################################

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../lib -lgtest_main -lgtest

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../lib -lgtest_main -lgtest

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../lib -lgtest_main -lgtest

INCLUDEPATH += $$PWD/../../include/bitpowder
DEPENDPATH += $$PWD/../../include/bitpowder

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
