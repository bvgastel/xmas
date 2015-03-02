TEMPLATE = lib

WARNINGS += -Wall

QT       += widgets
QT       -= gui

CONFIG += C++11
CONFIG += create_prl

CONFIG += static

TARGET = interfaces

SOURCES += \
    logger.cpp

HEADERS +=\
    vtplugininterface.h \
    result.h \
    logger.h



unix|win32 {
    target.path = $$PWD/../../../lib/plugins/interfaces
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/plugins/interfaces
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel
