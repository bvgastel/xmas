QT += widgets

HEADERS       = MainWindow.h \
    			setup.h \
				ModelWindow.h \
				ProjectWindow.h

SOURCES       = main.cpp \
                MainWindow.cpp \
			    ModelWindow.cpp \
				ProjectWindow.cpp

RESOURCES     = xmd.qrc


# install
target.path = XMD

INSTALLS += target

DISTFILES +=
