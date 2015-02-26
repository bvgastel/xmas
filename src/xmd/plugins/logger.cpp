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
#include <QString>
#include <QColor>

#include "logger.h"
#include "simplestring.h"

Logger::~Logger()
{

}

void Logger::log(const std::string message, const QColor color) {
    log(QString::fromUtf8(message.c_str()), color);
}

void Logger::log(const QString message, const QColor color) {
    emit writeLog(m_prefix + message ,color);
}

void Logger::log(const bitpowder::lib::String message, const QColor color) {
    log(QString::fromUtf8(message.stl().c_str()), color);
}
