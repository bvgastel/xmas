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
#ifndef DATACONTROL_H
#define DATACONTROL_H
#include <typeindex>
#include <map>

#include <QObject>
#include <QColor>

#include "logger.h"
#include "xmasproject.h"
/**
 * @brief The DataControl class
 *
 * The data control class has access to the xmas project.
 *
 * Also this class is available to all other c++ classes
 * using a global "extern DataControl *dataControl;" declaration.
 *
 * Furthermore it registers the data classes with Qml as required.
 *
 * Finally, it serves as a relay for messages through m_logger.
 * Qml only connects to this m_logger provided an entry for
 * all C++ classes that want access to the qml console log.
 *
 */
class DataControl : public QObject
{
    Q_OBJECT
public:
    explicit DataControl(QObject *parent = 0);
    ~DataControl();

    void registerTypes() const;
    /************************************************************
     * Signals
     ************************************************************/
signals:
    void writeLog(QString message,QColor color=Qt::black);
    /************************************************************
     * Data members
     ************************************************************/
public:
    std::shared_ptr<XMASProject> project();

private:
    Logger m_logger;
    std::shared_ptr<XMASProject> m_project;
};

#endif // DATACONTROL_H
