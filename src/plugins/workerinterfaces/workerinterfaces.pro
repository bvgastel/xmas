#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T16:46:55
#
#-------------------------------------------------

QT       -= gui

TARGET = workerinterfaces
TEMPLATE = lib

WARNINGS += -Wall

CONFIG += C++11
CONFIG += create_prl
CONFIG += dll

CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}


DEFINES += WORKERINTERFACES_LIBRARY

SOURCES += workerinterface.cpp

HEADERS += workerinterface.h\
        workerinterfaces_global.h

################################################
# INSTALL instructions
################################################
unix {
    target.path = $$PWD/../../../lib/plugins
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/plugins
    headerfiles.files = $$HEADERS
    INSTALLS += headerfiles
}

################################################
# Internal dependencies
################################################

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel_debug

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/datamodel/ -ldatamodeld

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

