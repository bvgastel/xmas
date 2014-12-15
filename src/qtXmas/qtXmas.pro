QT += widgets

HEADERS       = mainwindow.h \
                mdichild.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                mdichild.cpp
RESOURCES     = qtXmas.qrc

# install
target.path = qtXmas
INSTALLS += target
