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


# install
target.path = XMD
INSTALLS += target

DISTFILES +=
