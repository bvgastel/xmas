/*********************************************************************
 *
 * Copyright (C) Guus Bonnema, 2015
 *
 * This file is part of the xmas-design tool.
 *
 * The xmas-design tool is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * The xmas-design tool is distributed in the hope that it will be
 * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the xmas-design tool.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 **********************************************************************/
#include <chrono>

#include <QUrl>
#include <QColor>
#include <QBuffer>
#include <QDataStream>

#include "syntaxcheckworker.h"
#include "syntaxcheckerplugin.h"
#include "loggerfactory.h"

SyntaxCheckerPlugin::SyntaxCheckerPlugin(QObject *parent) : QObject(parent),
    m_name("syntax checker"),
    m_paramMap({{"runthread", "main"}, {"timer (sec)", "20"}}),
    m_sharedMemory("syntaxchecker")
{
}

SyntaxCheckerPlugin::~SyntaxCheckerPlugin() {
    m_workerThread.quit();
    m_workerThread.wait();
    // NOTE: from windows we should use kill() because the program
    // does not respond to the WM_CLOSE message if there is no event loop.
    // we need an ifdef
    m_process.terminate();

}

QString SyntaxCheckerPlugin::name() {
    return m_name;
}

void SyntaxCheckerPlugin::start(std::shared_ptr<XProject> project) {
    SyntaxCheckWorker *worker = new SyntaxCheckWorker;
    connect(worker, &SyntaxCheckWorker::resultReady, this, &SyntaxCheckerPlugin::handleResults);
    worker->doWork(project);
}

/**
 * @brief PluginThread::startThread This method starts the workerthread
 *
 * This method uses a workerthread to have the syntax checker do its work in a separate
 * thread that returns the result in a Result object.
 *
 * @param json the string containing the network in a flat json format
 */
void SyntaxCheckerPlugin::startThread(const QString &json) {
    SyntaxCheckWorker *worker = new SyntaxCheckWorker;
    worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SyntaxCheckerPlugin::operate, worker, &SyntaxCheckWorker::doThreadWork);
    connect(worker, &SyntaxCheckWorker::resultReady, this, &SyntaxCheckerPlugin::handleResults);

    m_workerThread.start();

    emit operate(json);

}

// Not implemented yet. needs some more thought.
// Query: is slot/signal over process feasible? If so, it is simpeler
void SyntaxCheckerPlugin::startProcess(const QString &programName, const QString &json, const QStringList &argList) {
    if (share(json)) {
        m_process.setProcessChannelMode(QProcess::ForwardedChannels);
        m_process.setProgram(programName);
        m_process.setArguments(argList);
        m_process.start();
    }
}

bool SyntaxCheckerPlugin::share(const QString &json) {
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << json;
    int size = buffer.size();
    if (!m_sharedMemory.create(size)) {
        std::cerr << "Could not share memory from SyntaxChecker." << std::endl;
        return false;
    }
    m_sharedMemory.lock();
    char *to = (char*)m_sharedMemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(m_sharedMemory.size(), size));
    m_sharedMemory.unlock();
    return true;
}


void SyntaxCheckerPlugin::handleResults(const ResultInterface &result) {
    // Don't know what to do yet, with the results. Show them I guess
    auto list = result.errorList();
    for (auto resObj : list) {
        if (resObj->m_error) {
            std::cerr << resObj->m_errorMessage.toStdString() << std::endl;
            std::cerr << resObj->m_errorObjectName.toStdString() << std::endl;
        } else {
            std::cout << "[" << resObj->m_stepName.toStdString() << "] " << resObj->m_stepMessage.toStdString()  << std::endl;
        }

    }
}

void SyntaxCheckerPlugin::name(QString name) {
    m_name = name;
}

QVariantMap SyntaxCheckerPlugin::paramMap() {
    QVariantMap map(m_paramMap);
    return map;
}

void SyntaxCheckerPlugin::parameters(QVariantMap paramMap) {
    m_paramMap = paramMap;
}

//LoggerInterface *SyntaxCheckerPlugin::logger() {
//    return m_logger;
//}

