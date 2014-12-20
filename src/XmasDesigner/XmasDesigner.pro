QT += widgets

HEADERS       = mainWindow.h \
    canvasWindow.h \
    _setup.h
SOURCES       = main.cpp \
                mainWindow.cpp \
    canvasWindow.cpp
RESOURCES     = \
    XmasDesigner.qrc

# install
target.path = XmasDesigner
INSTALLS += target

DISTFILES +=
