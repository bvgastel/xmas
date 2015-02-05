#include <iostream>

#include "designer.h"

Designer::Designer(QObject *parent) : QObject(parent)
{
    /**
     * @brief connect to signals from the GUI designer
     */
    connect(&m_process, SIGNAL(stateChanged(QProcess::ProcessState)), this,
            SLOT(handleProcessChange(QProcess::ProcessState)));
    m_process.setProgram(designer_exec_name);
    m_process.start();
}

Designer::~Designer()
{

}

void Designer::state() {
    std::cout << "Forcing a status report ..." << std::endl;
    handleProcessChange(m_process.state());
}

void Designer::handleProcessChange(QProcess::ProcessState state) {
    switch (state) {
    case QProcess::NotRunning:
        std::cout << "Process designer is not running at the moment.";
        std::cout << "   process id  = " << m_process.processId() << std::endl;
        std::cout << "   exit status = " << m_process.exitStatus() << std::endl;
        break;
    case QProcess::Running:
        std::cout << "Process designer is running.";
        std::cout << "   process id = " << m_process.processId() << std::endl;
        break;
    case QProcess::Starting:
        std::cout << "Process designer is starting.";
        std::cout << "   process id = " << m_process.processId() << std::endl;
        break;
    default:
        std::cout << "Process designer is -- I don't know what it's doing --.";
        std::cout << "   process id = " << m_process.processId() << std::endl;
    }


}

