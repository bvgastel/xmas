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

TARGET = syntaxcheckerplugin

DEFINES += PLUGINS_LIBRARY

SOURCES += \
    syntaxcheckerplugin.cpp \
    syntaxcheckworker.cpp

HEADERS +=\
    syntaxcheckerplugin.h \
    syntaxcheckworker.h

unix|win32 {
    target.path = $$PWD/../../../lib/plugins
    INSTALLS += target

#    Plugin implementation should never need to export header files.
#    headerfiles.path=$$PWD/../../include/plugins/syntaxcheckerplugin
#    headerfiles.files = $$PWD/*.h
#    INSTALLS += headerfiles
}



unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel
unix|win32: LIBS += -L$$PWD/../../../lib/xmd -lxmd

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH += $$PWD/../../../include/xmd
DEPENDPATH += $$PWD/../../../include/xmd

DISTFILES += \
    vtplugin.json


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../interfaces/release/ -linterfaces
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../interfaces/debug/ -linterfaces
else:unix: LIBS += -L$$OUT_PWD/../interfaces/ -linterfaces

INCLUDEPATH += $$PWD/../interfaces
DEPENDPATH += $$PWD/../interfaces

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/release/libinterfaces.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/debug/libinterfaces.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/release/interfaces.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interfaces/debug/interfaces.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../interfaces/libinterfaces.a
