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

#ifndef RESULTINTERFACE
#define RESULTINTERFACE

#include <memory>

#include <QObject>

struct ResultObject {
    bool m_error;
    QString m_errorMessage;
    QString m_errorObjectName;
    QString m_stepName;
    QString m_stepMessage;

    ResultObject(bool error, QString stepName, QString errorMessage, QString errorObjectName)
        : m_error(error),
          m_errorMessage(errorMessage),
          m_errorObjectName(errorObjectName),
          m_stepName(stepName),
          m_stepMessage() {}

    ResultObject(QString stepName, QString stepMessage)
        : m_error(false),
          m_errorMessage(),
          m_errorObjectName(),
          m_stepName(stepName),
          m_stepMessage(stepMessage) {}
};


/**
 * @brief The ResultInterface class
 *
 *
 */
class ResultInterface : public QObject
{
    Q_OBJECT

public:
    ResultInterface(QObject *parent) : QObject(parent) {

    }

    virtual ~ResultInterface() {

    }

    virtual const QList<std::shared_ptr<ResultObject>> &errorList() const = 0;
    virtual void addError(bool error, QString stepName, QString errorMessage, QString errorObjectName) = 0;
    virtual void addStep(QString stepName, QString stepMessage) = 0;

};

#endif // RESULTINTERFACE

