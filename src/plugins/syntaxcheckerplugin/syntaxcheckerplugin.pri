include(../../bitpowder/bitpowder.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lsyntaxcheckermodel

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/xmv/syntaxcheckermodel/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/xmv/syntaxcheckermodel/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/syntaxcheckermodel/release/syntaxcheckermodel.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/syntaxcheckermodel/debug/syntaxcheckermodel.lib
}

unix {
        LIBS += -L$$OUT_PWD/$$BASE/xmv/syntaxcheckermodel/
        PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/syntaxcheckermodel/libsyntaxcheckermodel.a

	QMAKE_LFLAGS += -fprofile-arcs
}
