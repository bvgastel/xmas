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
#include "pluginthread.h"

#include <QUrl>
#include <QColor>

#include "xmapper/parseflatjsonfile.h"

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

bool PluginThread::parse_json_file(QUrl fileUrl) {

    auto parseSuccessful = ParseFlatJsonFile::parseJsonFile(fileUrl, m_componentMap, m_mp);

    if (parseSuccessful) {
    // Start the thread
    // Set up tracking and parameters
    // Execute the thread
    // Track the thread
    // Wait for thread to finish
    }

    return true;
}

bool PluginThread::run(QUrl &fileUrl) {

    std::cout << "Started process validate network for components." << std::endl;
    bitpowder::lib::MemoryPool mp;
    std::map<bitpowder::lib::String, XMASComponent *> componentMap;
    bool parseSuccesful = ParseFlatJsonFile::parseJsonFile(fileUrl, componentMap, mp);
    if (parseSuccesful) {

    }

    return parseSuccesful;
}

bool checkPorts(std::map<bitpowder::lib::String, XMASComponent *> &componentMap, mp) {

    return false;
}

