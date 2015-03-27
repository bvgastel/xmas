/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QDir>

class Util : public QObject
{
    Q_OBJECT
    Q_ENUMS(messageType)
    Q_ENUMS(logType)

public:
    explicit Util(QObject *parent = 0);
    ~Util();

    enum messageType {
        Info = Qt::black,
        Success = Qt::green,
        Warning = Qt::yellow,
        Error = Qt::red
        };
    enum logType {Designer=0,Plugin=1};


signals:
    void writeLog(logType log, QString message, messageType color = Info);

public slots:

    static QString modelPath();


public:
    static bool saveFile(QUrl fileUrl, QString contents);
    static QString openFile(QUrl fileUrl);

};

#endif // UTIL_H
