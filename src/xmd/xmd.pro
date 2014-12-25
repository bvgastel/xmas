QT += widgets

HEADERS       = mainwindow.h \
                modelwindow.h \
                projectmanager.h \
                setup.h \
    canvas/connection.h \
    canvas/connector.h \
    canvas/component.h \
    canvas/canvas.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                modelwindow.cpp \
                projectmanager.cpp \
    canvas/connection.cpp \
    canvas/connector.cpp \
    canvas/component.cpp \
    canvas/canvas.cpp
RESOURCES     = xmd.qrc


# install
target.path = xmd
TARGET = xmas-designer
INSTALLS += target
INCLUDEPATH += canvas
DISTFILES +=
