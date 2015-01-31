#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <iostream>
#include <memory>
#include <map>

#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QVector>
#include <QProcess>

#include "setup.h"
#include "consolereader.h"
#include "designer.h"

namespace xmas {
    namespace xmc {

        struct Options {
            QStringList args;
            bool commandline;
            bool runAll;
            QString outputDir;
        };

        class Control : QObject
        {
            Q_OBJECT

        public:
            Control(int argc, char *argv[]);
            ~Control();

            int exec();
            std::stringstream &appData(std::stringstream &);
            std::stringstream &paramData(std::stringstream &data);

        signals:
            void finished();
            void textReceived(QString line);

        public slots:
            void processCommand(QString);
            void aboutToQuitApp();
            void quit();

        private:
            void getOptions(Options &);

            QApplication m_app;
            Options m_opt;
            ConsoleReader m_consoleReader;
            std::shared_ptr<Designer> m_designer;
        };

    } // namespace xmc
} // namespace xmas

#endif // CONTROL_H
