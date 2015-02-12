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


WARNINGS += -Wall

TEMPLATE = lib

CONFIG += C++11
CONFIG += create_prl
CONFIG += static

include (defines.pri)

QT += widgets svg quick qml quickwidgets

HEADERS       = \ 
    controller.h
				
SOURCES       = \ 
    controller.cpp
    
unix {
    target.path = $$PWD/../../../lib/xmd
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/xmd
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

win32 {
    target.path = $$PWD/../../../lib/xmd
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/xmd
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

INCLUDEPATH += content qml

TEMPLATE = lib
CONFIG += C++11
CONFIG += create_prl
CONFIG += static

DISTFILES += qml/fork.qml \
    qml/function.qml \
    qml/in.qml \
    qml/join.qml \
    qml/merge.qml \
    qml/out.qml \
    qml/queue.qml \
    qml/sink.qml \
    qml/source.qml \
    qml/spidergon.qml \
    qml/switch.qml \
	mainWindow.qml \
    content/ScrollBar.qml \
    content/MenuBar.qml \
    content/XmasToolBar.qml \
    content/ToolBarItem.qml \
    Sheet.qml \
    qml/XComponent.qml \
    qml/Line.qml \
	controller.js \
    qml/XChannel.qml \
    qml/XPort.qml \
    content/channelCreation.js \
    content/componentCreation.js \
    content/Selection.qml \
    content/XDialog.qml \
    content/XDialogItem.qml \
    content/OutputLog.qml \
    content/XTag.qml

RESOURCES += \
    xmd.qrc

FORMS +=

unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel
