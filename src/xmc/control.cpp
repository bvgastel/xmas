#include "control.h"

Control::Control(int argc, char *argv[])
{
    m_app = std::make_shared<QApplication>(argc, argv);
    m_app->setApplicationName(applicationName);
    m_app->setApplicationVersion(applicationVersion);

    std::cout << "Application name = " << m_app->applicationName().toStdString() << std::endl;
    std::cout << "Application file path = " << m_app->applicationFilePath().toStdString() << std::endl;
    std::cout << "Application dir path = " << m_app->applicationDirPath().toStdString() << std::endl;
    std::cout << std::endl;

    m_opt = getOptions();

    std::cout << "commandline = " << (m_opt->commandline ? "true" : "false") << std::endl;
    std::cout << "runAll = " << (m_opt->runAll ? "true" : "false") << std::endl;
    std::cout << "outputDir = '" << m_opt->outputDir.toStdString() << "'" << std::endl;
    std::cout << "positional Arguments = ";
    for (QString arg : m_opt->args) {
        std::cout << "'" << arg.toStdString() << "' ";
    }
    std::cout << std::endl;
}

int Control::exec() {
    std::cout << "============= Press Ctrl-C to end program ==============" << std::endl;
    return m_app->exec();
}

Control::~Control()
{

}

std::shared_ptr<Options> Control::getOptions()
{
    std::shared_ptr<Options> opt = std::make_shared<Options>();
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
    optParser.process(*m_app);

    opt->args = optParser.positionalArguments();
    opt->commandline = optParser.isSet(commandlineOption);
    opt->runAll = optParser.isSet(allOption);
    opt->outputDir = optParser.value(outputDirectoryOption);
    return opt;
}
