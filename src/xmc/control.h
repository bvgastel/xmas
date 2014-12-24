#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <iostream>
#include <memory>

#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QVector>

#include "setup.h"
#include "consolereader.h"

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
    void run();
    void processCommand(QString);
    void aboutToQuitApp();
    void quit();

private:
    QApplication m_app;
    Options m_opt;
    ConsoleReader m_consoleReader;

    void getOptions(Options &);
};

#endif // CONTROL_H
