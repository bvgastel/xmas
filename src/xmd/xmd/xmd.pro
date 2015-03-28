#
# Instructions for use:
#
# 1. When building, the output goes solely to the
#    build directory. This is usually something
#    like build-libraryname-platform-compiler
#    where libraryname, platform and compiler vary.
# 2. When cleaning, only the build directory is cleaned.
#
# 3. When deploying, the library files (both dll/so and
#    .a) and the header files are copied to the lib and
#    include directory right below the git-root. This is
#    the most vulnerable piece of code.
#    REMARK: when cleaning, this does not get touched.
# 3a. The include directory has a subdir for this project's
#     header files.
#
# IMPORTANT: Be sure to have qtcreator execute a make install
#            as one step in the local deployment.
#            As long as this step is not added to qtcreator
#            (which writes the deploy step in .user.pro file)
#            the header files and xmd lib will not occur in
#            the directories lib or include under the root.
#
#

TEMPLATE = lib

WARNINGS += -Wall

QT += widgets
QT += quick
QT += qml
QT += quickwidgets

CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32: CONFIG += static
unix: CONFIG += static dll

# Some problems are easier to debug using no optimization
# In that case: uncomment the following line (-O1 might be enough)
#QMAKE_CXXFLAGS_DEBUG += -O0

#CONFIG += build_all

TARGET = xmd
CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DEFINES += XMD_LIBRARY

HEADERS       = \
    xmdexception.h \
    plugincontrol.h \
    datacontrol.h \
    model/util.h \
    model/component.h \
    model/channel.h \
    model/port.h \
    model/network.h

SOURCES       = \
    xmdexception.cpp \
    plugincontrol.cpp \
    datacontrol.cpp \
    model/util.cpp \
    model/component.cpp \
    model/channel.cpp \
    model/port.cpp \
    model/network.cpp



################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib/xmd
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/xmd
    headerfiles.files = $$HEADERS
    INSTALLS += headerfiles
}

INCLUDEPATH += uicontrols xobjects content
DEPENDPATH += uicontrols xobjects content

DISTFILES += mainWindow.qml \
    xobjects/fork.qml \
    xobjects/function.qml \
    xobjects/join.qml \
    xobjects/merge.qml \
    xobjects/queue.qml \
    xobjects/sink.qml \
    xobjects/source.qml \
    xobjects/switch.qml \
    xobjects/primitive.qml \
    xobjects/XComponent.qml \
    xobjects/XChannel.qml \
    xobjects/XPort.qml \
    xobjects/XNetwork.qml \
    xobjects/xnetwork.js \
    uicontrols/ScrollBar.qml \
    uicontrols/XToolBar.qml \
    uicontrols/XToolBarItem.qml \
    uicontrols/XDialog.qml \
    uicontrols/Selection.qml \
    uicontrols/OutputLog.qml \
    uicontrols/XWire.qml \
    xobjects/xchannel.js \
    xobjects/xcomponent.js \
    uiqueries_readme.md \
    uicontrols/SelectionHandle.qml \
    uicontrols/XPacketDialog.qml \
    xobjects/composite.qml \
    uicontrols/ModelSetupDialog.qml \
    uicontrols/ApplicationSetupDialog.qml \
    qml-xmas-readme.md

RESOURCES += \
    images.qrc \
    javascripts.qrc \
    quick.qrc

################################################
# Internal dependencies
################################################

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel_debug \
    -L$$PWD/../../../lib/vt/ -lvt_debug \
    -L$$PWD/../../../lib/interfaces/ -linterfaces_debug \
#    -L$$PWD/../../../lib/plugins/ -lworkerinterfaces_debug \


else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/datamodel/ -ldatamodeld \
    -L$$PWD/../../../lib/vt/ -lvtd \
    -L$$PWD/../../../lib/interfaces/ -linterfacesd \
#    -L$$PWD/../../../lib/plugins/ -lworkerinterfacesd \


else:unix|CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../../lib/vt/ -lvt \
    -L$$PWD/../../../lib/interfaces/ -linterfaces \
    -L$$PWD/../../../lib/plugins/  \
#    -L$$PWD/../../../lib/plugins/ -lworkerinterfaces \


INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH += $$PWD/../../../include/vt
DEPENDPATH += $$PWD/../../../include/vt

INCLUDEPATH += $$PWD/../../../include/interfaces
DEPENDPATH += $$PWD/../../../include/interfaces

