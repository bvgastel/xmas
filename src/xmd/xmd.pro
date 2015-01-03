<<<<<<< HEAD
<<<<<<< HEAD
QT += widgets svg
=======
=======
>>>>>>> 9fca7606f280fcf837ff6d0f256798a37423a8c9
! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

QT += widgets
>>>>>>> Still creating build environment that works. Not yet. One include error.

HEADERS       = mainwindow.h \
                modelwindow.h \
                projectmanager.h \
<<<<<<< HEAD
<<<<<<< HEAD
                setup.h \
                connection.h \
                connector.h \
                component.h \
                complib.h
				
=======
                setup.h
>>>>>>> Still creating build environment that works. Not yet. One include error.
=======
                setup.h
>>>>>>> 9fca7606f280fcf837ff6d0f256798a37423a8c9
SOURCES       = main.cpp \
                mainwindow.cpp \
                modelwindow.cpp \
                projectmanager.cpp \
                connection.cpp \
                connector.cpp \
                component.cpp \
                complib.cpp
				
RESOURCES     = xmd.qrc

<<<<<<< HEAD
# install
target.path = xmd
TARGET = xmas-designer
INSTALLS += target
INCLUDEPATH += canvas
=======

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# install
# target.path = xmd
# INSTALLS += target

>>>>>>> Still creating build environment that works. Not yet. One include error.
DISTFILES +=
