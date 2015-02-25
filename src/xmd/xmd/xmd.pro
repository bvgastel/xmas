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


#WARNINGS += -Wall

TEMPLATE = lib

CONFIG += C++11
CONFIG += create_prl
win32: CONFIG += static
unix: CONFIG += static dll

include (defines.pri)

QT += widgets quick qml quickwidgets

HEADERS       = \ 
    xmdexception.h \
    xmapper/componentwalker.h \
    xmapper/controller.h \
    xmapper/fieldwalker.h \
    xmapper/logoutput.h \
    xmapper/network.h
				
SOURCES       = \ 
    xmdexception.cpp \
    xmapper/componentwalker.cpp \
    xmapper/controller.cpp \
    xmapper/fieldwalker.cpp \
    xmapper/logoutput.cpp
    xmapper/network.cpp \
    
unix|win32 {
    target.path = $$PWD/../../../lib/xmd
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/xmd
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

INCLUDEPATH += uicontrols xobjects xmapper vplugin content ../plugins
DEPENDPATH += uicontrols xobjects xmapper vplugin content ../plugins

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
    readme.md \
    xobjects/xchannel.js \
    xobjects/xcomponent.js

RESOURCES += \
    xmd.qrc

FORMS +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugins/release/ -lplugins
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugins/debug/ -lplugins
else:unix: LIBS += -L$$OUT_PWD/../plugins/ -lplugins

INCLUDEPATH += $$PWD/../plugins
DEPENDPATH += $$PWD/../plugins

unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel
