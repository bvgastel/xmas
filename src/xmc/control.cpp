#include <string>
#include <sstream>
#include "control.h"

Control::Control(int argc, char *argv[]) :
    m_app(argc, argv),
    m_consoleReader(this)
{
    m_app.setApplicationName(applicationName);
    m_app.setApplicationVersion(applicationVersion);

    getOptions(m_opt);

    connect(this, SIGNAL(finished()), this, SLOT(quit()));
    connect(&m_consoleReader, SIGNAL(textReceived(QString)),this, SLOT(processCommand(QString)));


}

std::stringstream &Control::appData(std::stringstream &data) {
    data << "Application name = " << m_app.applicationName().toStdString() << std::endl;
    data << "Application file path = " << m_app.applicationFilePath().toStdString() << std::endl;
    data << "Application dir path = " << m_app.applicationDirPath().toStdString() << std::endl;
    data << std::endl;
    return data;
}

std::stringstream &Control::paramData(std::stringstream &data) {
    data << "commandline = " << (m_opt.commandline ? "true" : "false") << std::endl;
    data << "runAll = " << (m_opt.runAll ? "true" : "false") << std::endl;
    data << "outputDir = '" << m_opt.outputDir.toStdString() << "'" << std::endl;
    data << "positional Arguments = ";
    for (QString arg : m_opt.args) {
        data << "'" << arg.toStdString() << "' ";
    }
    data << std::endl;
    return data;
}

int Control::exec() {
    std::cout << "============= Enter quit to end the program ==============" << std::endl;
    return m_app.exec();
}

Control::~Control()
{

}

void Control::run() {
    emit finished();
}

void Control::quit() {
    std::cout << "Shutting down ...." << std::endl;
    m_app.quit();
}

void Control::processCommand(QString line) {
    std::cout << "received line = '"<< line.toStdString() << "'"<< std::endl;
    if (line.toStdString() == "quit") {
        std::cout << "starting to emit finished()" << "... ";
        emit finished();
        std::cout << "finished() emitted ... " << std::endl;
    }
}

void Control::aboutToQuitApp() {
    std::cout << "About to quit app" << std::endl;
}

void Control::getOptions(Options &opt)
{
    QCommandLineParser optParser;
    optParser.setApplicationDescription(applicationDescr);

    QCommandLineOption commandlineOption(QStringList() << "c" << "commandline",
                                         QCoreApplication::translate("main", "Use the commandline (not the gui)"));
    optParser.addOption(commandlineOption);
    QCommandLineOption allOption(QStringList() << "a" << "all",
                                 QCoreApplication::translate("main", "Run all verification tools."));
    optParser.addOption(allOption);
    QCommandLineOption outputDirectoryOption(QStringList() << "o" << "output-directory",
            QCoreApplication::translate("main", "Store output files in <directory>."),
            QCoreApplication::translate("main", "directory"));
    optParser.addOption(outputDirectoryOption);

    optParser.addPositionalArgument("main", "Input model to process.");
    optParser.process(m_app);

    opt.args = optParser.positionalArguments();
    opt.commandline = optParser.isSet(commandlineOption);
    opt.runAll = optParser.isSet(allOption);
    opt.outputDir = optParser.value(outputDirectoryOption);
}
