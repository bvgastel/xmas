! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

QT += widgets

HEADERS       = mainwindow.h \
                modelwindow.h \
                projectmanager.h \
                setup.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                modelwindow.cpp \
				projectmanager.cpp
RESOURCES     = xmd.qrc


unix {
    target.path = /usr/lib
    INSTALLS += target
}

# install
# target.path = xmd
# INSTALLS += target

DISTFILES +=
