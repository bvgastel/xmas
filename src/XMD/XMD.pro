QT += widgets

HEADERS       = mainWindow.h \
    _setup.h \
    modelWindow.h
SOURCES       = main.cpp \
                mainWindow.cpp \
    modelWindow.cpp
RESOURCES     = XMD.qrc


# install
target.path = XMD
INSTALLS += target

DISTFILES +=
