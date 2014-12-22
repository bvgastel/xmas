QT += widgets

HEADERS       = mainwindow.h \
                _setup.h \
                modelWindow.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                modelwindow.cpp
RESOURCES     = XMD.qrc


# install
target.path = XMD
INSTALLS += target

DISTFILES +=
