QT += widgets svg

HEADERS       = mainwindow.h \
                modelwindow.h \
                projectmanager.h \
                setup.h \
                connection.h \
                connector.h \
                component.h \
                complib.h
				
SOURCES       = main.cpp \
                mainwindow.cpp \
                modelwindow.cpp \
                projectmanager.cpp \
                connection.cpp \
                connector.cpp \
                component.cpp \
                complib.cpp
				
RESOURCES     = xmd.qrc

# install
target.path = xmd
TARGET = xmas-designer
INSTALLS += target
INCLUDEPATH += canvas
DISTFILES +=
