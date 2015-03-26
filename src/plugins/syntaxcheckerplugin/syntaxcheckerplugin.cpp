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

#include "syntaxcheckworker.h"
#include "syntaxcheckerplugin.h"
#include "loggerfactory.h"

QString SyntaxCheckerPlugin::name() {
    return m_name;
}

SyntaxCheckerPlugin::SyntaxCheckerPlugin(QObject *parent) : QObject(parent),
    m_name("syntax checker"),
    m_paramMap({{"runthread", "main"}, {"timer (sec)", "20"}}),
    m_logger(LoggerFactory::MakeLogger("syntaxchecker"))
{
}

SyntaxCheckerPlugin::~SyntaxCheckerPlugin() {
    m_workerThread.quit();
    m_workerThread.wait();
    // NOTE: from windows we should use kill() because the program
    // does not respond to the WM_CLOSE message
    // we need an ifdef
    m_process.terminate();

}

void start(XMap &componentMap) {
    SyntaxCheckWorker *worker = new SyntaxCheckWorker;
    worker->doWork(componentMap);
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
void SyntaxCheckerPlugin::startProcess(const QString &json) {
    bitpowder::lib::unused(json);

    m_process.setProcessChannelMode(QProcess::ForwardedChannels);
    // TODO: How to start processes?
    m_process.setProgram("dir");  // fixed program
    QStringList argList = {};
    m_process.setArguments(argList);   // variable argument
    m_process.start();
}


void SyntaxCheckerPlugin::handleResults(const ResultInterface &result) {
    // Don't know what to do yet, with the results. Show them I guess
    auto list = result.errorList();
    for (ErrorObject err : list) {
        if (err.error) {
            m_logger->log(err.errorMessage);
            m_logger->log(err.errorObjectName);
        } else {
            m_logger->log(result.description());
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

LoggerInterface *SyntaxCheckerPlugin::logger() {
    return m_logger;
}

