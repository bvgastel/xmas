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

#include <QUrl>
#include <QColor>

#include "xmapper/parseflatjsonfile.h"
#include "pluginthread.h"

QString PluginThread::name() {
    return m_name;
}

void PluginThread::name(QString name) {
    m_name = name;
}

QMap<QString, QString> PluginThread::parameters() {
    return m_paramMap;
}

void PluginThread::parameters(QMap<QString, QString> paramMap) {
    m_paramMap = paramMap;
}

Logger *PluginThread::logger() {
    return m_logger;
}

void PluginThread::xstart(const QString &json) override {
    bitpowder::lib::unused(json);
    // no action yet
}

bool PluginThread::run(const QString &) {
    return false;
}

bool PluginThread::run(const QUrl &fileUrl) {

    std::cout << "Started process validate network for components." << std::endl;
    bitpowder::lib::MemoryPool mp;
    std::map<bitpowder::lib::String, XMASComponent *> componentMap;
    bool parseSuccesful = ParseFlatJsonFile::parseJsonFile(fileUrl, componentMap, mp);
    if (parseSuccesful) {
        std::cout << "Started parsing file " << fileUrl.fileName().toStdString() << std::endl;
        std::cout << std::string("Not implemented yet") << std::endl;
        std::cout << "Finished parsing file " << fileUrl.fileName().toStdString() << std::endl;
    }

    return parseSuccesful;
}

bool checkPorts(std::map<bitpowder::lib::String, XMASComponent *> &componentMap, bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::unused(componentMap, mp);
    return false;
}

