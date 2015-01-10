! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

QT += widgets svg

CONFIG   += C++11
CONFIG   += staticlib

HEADERS       = mainwindow.h \
                modelwindow.h \
                projectmanager.h \
                setup.h \
                component.h \
                connector.h \
                connection.h \
                complib.h \
                positioncomponentextension.h \
                network.h \
                designercontroller.h \
                project.h \
                checker/xmas.h

SOURCES       = mainwindow.cpp \
                modelwindow.cpp \
                projectmanager.cpp \
                component.cpp \
                connector.cpp \
                connection.cpp \
                complib.cpp \
                network.cpp \
                designercontroller.cpp \
                project.cpp \
                checker/xmas.cpp

RESOURCES     = xmd.qrc


unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    target.path = xmd
    INSTALLS += target
}

TEMPLATE = lib

# install
# target.path = xmd
# INSTALLS += target

DISTFILES += XMAS_LIBRARY
