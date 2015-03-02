# xmdmain.pro
# Contains the main program for xmd

# Be sure to add deploy step with make (parameter install)

TEMPLATE = app

QT += widgets
QT += svg
QT += quick
QT += qml
QT += quickwidgets

CONFIG += link_prl
CONFIG += C++11

TARGET = xmdmain

HEADERS       = \

SOURCES += \
    main.cpp

################################################
# INSTALL instructions
################################################
unix|win32|macx {
 target.path=$$PWD/../../../bin
 INSTALLS += target
}

DISTFILES += \
    readme.md

################################################
# Internal dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/ -lxmd_debug
else:macx:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/debug/ -lxmdd
else:win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/release/ -lxmd
else:unix: LIBS += \
    -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/release/libxmd.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/debug/libxmd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/release/xmd.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../xmd/debug/xmd.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../xmd/libxmd.a

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel_debug \
    -L$$PWD/../../../lib/plugins/interfaces -linterfaces_debug

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/datamodel/ -ldatamodeld \
    -L$$PWD/../../../lib/plugins/interfaces -linterfacesd

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../../lib/plugins/interfaces -linterfaces

INCLUDEPATH += $$PWD/../../../include/bitpowder $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder $$PWD/../../bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel $$PWD/../../xmv/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel $$PWD/../../xmv/datamodel

INCLUDEPATH += $$PWD/../../../include/plugins/interfaces
DEPENDPATH += $$PWD/../../../include/plugins/interfaces






