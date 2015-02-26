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
#include "validplugin.h"

#include <QUrl>
#include <QColor>

QString ValidPlugin::name() {
    return m_name;
}

void ValidPlugin::name(QString name) {
    m_name = name;
}

QMap<QString, QString> ValidPlugin::parameters() {
    return m_paramMap;
}

void ValidPlugin::parameters(QMap<QString, QString> paramMap) {
    m_paramMap = paramMap;
}

bool ValidPlugin::run(const QString &jsonString) {
    size_t len = 10 < jsonString.length() ? 10 : jsonString.length();
    std::cout << "Started process validate network for json starting with '"
              << jsonString.toStdString().substr(0, len)
              << "'" << std::endl;
    return true;
}

bool ValidPlugin::run(const QUrl &fileUrl) {
    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    m_logger->log("Opening file " + filename, QColor("black"));

    std::cout << "Started process validate network for file '"
              << filename
              << std::endl;

    return true;
}

bool ValidPlugin::run_json_xmas(std::map<std::string, XMASComponent * > ) {
    std::cout << "Started process validate network for components." << std::endl;
    return true;
}


