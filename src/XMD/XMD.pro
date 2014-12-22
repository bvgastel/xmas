QT += widgets

HEADERS       = mainwindow.h \
                modelWindow.h \
    setup.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                modelwindow.cpp
RESOURCES     = XMD.qrc


# install
target.path = XMD
INSTALLS += target

DISTFILES +=
