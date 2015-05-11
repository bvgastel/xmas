#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T08:56:05
#
#-------------------------------------------------

QT       += qml quick quickwidgets widgets

TARGET = xmd
TEMPLATE = lib

DEFINES += XMD_LIBRARY

WARNINGS += -Wall
CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32: CONFIG += static
linux: CONFIG += static dll
macx: CONFIG += staticlib

SOURCES += \
    datacontrol.cpp \
    plugincontrol.cpp \
    xmdexception.cpp \
    model/channel.cpp \
    model/component.cpp \
    model/network.cpp \
    model/port.cpp \
    model/util.cpp

HEADERS += \
    datacontrol.h \
    plugincontrol.h \
    xmdexception.h \
    model/channel.h \
    model/component.h \
    model/network.h \
    model/port.h \
    model/util.h

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib
    INSTALLS += target
}

RESOURCES += \
    images.qrc \
    javascripts.qrc \
    quick.qrc

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
    uicontrols/Selection.qml \
    uicontrols/XWire.qml \
    xobjects/xchannel.js \
    xobjects/xcomponent.js \
    uiqueries_readme.md \
    uicontrols/XPacketDialog.qml \
    xobjects/composite.qml \
    uicontrols/ModelSetupDialog.qml \
    uicontrols/ApplicationSetupDialog.qml \
    qml-xmas-readme.md \
    uicontrols/XToolBarPrimitiveItem.qml \
    uicontrols/XToolBarCompositeItem.qml \
    uicontrols/XConsole.qml \
    uicontrols/XPlugin.qml \
    uicontrols/Log.qml \
    findings.md \
    uicontrols/XExpressionDialog.qml \
    xmd.pri

################################################
# Dependencies
################################################
include(../../bitpowder/bitpowder.pri)
include(../../xmv/vt/vt.pri)
include(../../interfaces/interfaces.pri)
