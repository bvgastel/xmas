TEMPLATE = lib

WARNINGS += -Wall

QT       += widgets
QT       -= gui

CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
CONFIG += static dll
#CONFIG += build_all

TARGET = interfaces
CONFIG(debug, debug|release) {
    mac: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DEFINES += INTERFACES_LIBRARY

SOURCES += \
    logger.cpp \
    result.cpp

HEADERS +=\
    vtplugininterface.h \
    result.h \
    logger.h \
    loggerinterface.h \
    resultinterface.h \
    commoninterface.h

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../lib/interfaces
    INSTALLS += target

    headerfiles.path=$$PWD/../../include/interfaces
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

DISTFILES += \
    readme.md

################################################
# Internal dependencies
################################################

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../lib/datamodel/ -ldatamodel_debug

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../lib/datamodel/ -ldatamodeld

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../lib/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../../include/bitpowder
DEPENDPATH += $$PWD/../../include/bitpowder

INCLUDEPATH += $$PWD/../../include/datamodel
DEPENDPATH += $$PWD/../../include/datamodel

