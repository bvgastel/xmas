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

#include <QObject>

/**
 * @brief The DataControl class
 *
 * The data control class has access to the complete datamodel and is able to
 * manage the data (create, modify, destroy and cleanup).
 * It also registers the data classes with Qml as required.
 *
 *
 */
class DataControl : public QObject
{
    Q_OBJECT


public:
    explicit DataControl(QObject *parent = 0);
    ~DataControl();


signals:

public slots:

private:
};

#endif // DATACONTROL_H
