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
win32: CONFIG += static
unix: CONFIG += static dll
CONFIG += build_all

TARGET = xmd
CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

include (defines.pri)


XMAPPER_HEADERS = \
    xmapper/componentwalker.h \
    xmapper/controller.h \
    xmapper/fieldwalker.h \
    xmapper/logoutput.h \
    xmapper/network.h \
    xmapper/parseflatjsonfile.h \


HEADERS       = \ 
    xmdexception.h \
    $$XMAPPER_HEADERS
				
SOURCES       = \ 
    xmdexception.cpp \
    xmapper/componentwalker.cpp \
    xmapper/controller.cpp \
    xmapper/fieldwalker.cpp \
    xmapper/logoutput.cpp \
    xmapper/network.cpp \
    xmapper/parseflatjsonfile.cpp
    
################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib/xmd
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/xmd
    headerfiles.files = $$HEADERS
    headerfiles.files -= $$XMAPPER_HEADERS
    message(header files are $$headerfiles.files)
    INSTALLS += headerfiles

    headerfiles_xmapper.path = $$PWD/../../../include/xmd/xmapper
    headerfiles_xmapper.files = $$XMAPPER_HEADERS
    message(header files for xmapper are $$headerfiles_xmapper.files)
    INSTALLS += headerfiles_xmapper
}

INCLUDEPATH += uicontrols xobjects xmapper content
DEPENDPATH += uicontrols xobjects xmapper content

DISTFILES += mainWindow.qml \
    xobjects/fork.qml \
    xobjects/function.qml \
    xobjects/in.qml \
    xobjects/join.qml \
    xobjects/merge.qml \
    xobjects/out.qml \
    xobjects/queue.qml \
    xobjects/sink.qml \
    xobjects/source.qml \
    xobjects/spidergon.qml \
    xobjects/switch.qml \
    xobjects/primitive.qml \
    xobjects/XComponent.qml \
    xobjects/XWire.qml \
    xobjects/XChannel.qml \
    xobjects/XPort.qml \
    xobjects/XNetwork.qml \
    xmapper/controller.js \
    uicontrols/ScrollBar.qml \
    uicontrols/XToolBar.qml \
    uicontrols/XToolBarItem.qml \
    uicontrols/XDialog.qml \
    uicontrols/Selection.qml \
    uicontrols/OutputLog.qml \
    xobjects/xchannel.js \
    xobjects/xcomponent.js \
    uiqueries_readme.md \
    uicontrols/SelectionHandle.qml

RESOURCES += \
    xmd.qrc

################################################
# Internal dependencies
################################################

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

INCLUDEPATH += $$PWD/../../../include/plugins/interfaces  $$PWD/../../plugins/interfaces
DEPENDPATH += $$PWD/../../../include/plugins/interfaces  $$PWD/../../plugins/interfaces
