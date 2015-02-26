TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS +=


SOURCES += \
    main.cpp \
    parser.test.cpp \
    parser_json.test.cpp \
    simplestring.test.cpp \
    stringparse.test.cpp


CONFIG += C++11

# Windows needs _USE_MATH_DEFINES to define M_PI and so on.
win32: DEFINES += _USE_MATH_DEFINES

include(deployment.pri)
qtcAddDeployment()

# Google test library
unix|win32: LIBS += -L$$PWD/../../lib -lgtest_main -lgtest

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

# Remark: bitpowder is external, so use $$PWD, not $$OUT_PWD.

unix|win32: LIBS += -L$$PWD/../../lib/bitpowder -lbitpowder

INCLUDEPATH += $$PWD/../../include/bitpowder
DEPENDPATH += $$PWD/../../include/bitpowder

unix|win32 {
    target.path = $$PWD/../../bin
    INSTALLS += target
}
