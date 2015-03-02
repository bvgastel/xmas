#-------------------------------------------------
#
# Project created by QtCreator 2015-02-22T19:47:15
#
#-------------------------------------------------

TEMPLATE = lib

WARNINGS += -Wall

QT       += widgets
QT       -= gui

CONFIG += plugin
CONFIG += C++11
CONFIG += create_prl
CONFIG += dll
CONFIG += build_all

TARGET = syntaxcheckerplugin
CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DEFINES += PLUGINS_LIBRARY

SOURCES += \
    syntaxcheckerplugin.cpp \
    syntaxcheckworker.cpp

HEADERS +=\
    syntaxcheckerplugin.h \
    syntaxcheckworker.h

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib/plugins
    INSTALLS += target

    # Plugin should never need to export header files
}

DISTFILES += \
    vtplugin.json

################################################
# Internal dependencies
################################################
#macx:CONFIG(debug, debug|release): LIBS += \
#    -L$$OUT_PWD/../interfaces/ -linterfaces_debug
#else:macx:CONFIG(release, debug|release): LIBS += \
#    -L$$OUT_PWD/../interfaces/ -linterfaces
#else:win32:CONFIG(debug, debug|release): LIBS += \
#    -L$$OUT_PWD/../interfaces/debug/ -linterfacesd
#else:win32:CONFIG(release, debug|release): LIBS += \
#    -L$$OUT_PWD/../interfaces/release/ -linterfaces
#else:unix: LIBS += \
#    -L$$OUT_PWD/../interfaces/ -linterfaces

#INCLUDEPATH += $$PWD/../interfaces
#DEPENDPATH += $$PWD/../interfaces

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/release/libinterfaces.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/debug/libinterfacesd.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/release/interfaces.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/debug/interfacesd.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../interfaces/libinterfaces.a

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel_debug \
    -L$$PWD/../../../lib/xmd/ -lxmd_debug \
    -L$$PWD/../../../lib/interfaces/ -linterfaces_debug

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/datamodel/ -ldatamodeld \
    -L$$PWD/../../../lib/xmd/ -lxmdd \
    -L$$PWD/../../../lib/interfaces/ -linterfacesd

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../../lib/xmd/ -lxmd \
    -L$$PWD/../../../lib/interfaces/ -linterfaces

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH +=$$PWD/../../../include/xmd
DEPENDPATH += $$PWD/../../../include/xmd

INCLUDEPATH += $$PWD/../../../include/interfaces
DEPENDPATH += $$PWD/../../../include/interfaces
