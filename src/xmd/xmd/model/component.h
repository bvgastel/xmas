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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QQuickItem>
#include <QQmlParserStatus>

#include "xmas.h"

/**
 * @brief The Component class
 *
 * A wrapper around XMASComponent from xmas.h to connect to
 * Qml using properties.
 */

namespace model {

const QString xsource = "source";
const QString xsink = "sink";
const QString xfunction = "function";
const QString xqueue = "queue";
const QString xjoin = "join";
const QString xmerge = "merge";
const QString xfork = "fork";
const QString xswitch = "switch";
const QString xin = "in";
const QString xout = "out";
const QString xcomposite = "composite";

class Component : public QQuickItem //, public QQmlParserStatus
{
public:
    enum Orientation {
        North = 0,
        East = 90,
        South = 180,
        West = 270,
        NorthWest = 45,
        SouthWest = 225,
        NorthEast = 315,
        SouthEast = 135
    };
private:
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(QString comptype READ compType WRITE compType NOTIFY compTypeChanged)
    Q_PROPERTY(QVariant param READ param WRITE param NOTIFY paramChanged)

public:
    explicit Component(QQuickItem *parent = 0);
    ~Component();

    //enum { Type = QVariant::UserType + 0 };

signals:
    void nameChanged();
    void compTypeChanged();
    void paramChanged();
    void changeName(QString old_name, QString name);
    void writeLog(QString message, QColor color = Qt::blue);

public slots:

public:

    QString compType() const {return m_compType;}
    void compType(QString compType) {
        m_compType = compType;
    }

    QString name() {
        return m_name;
    }

    void name(QString name) {
        if (name != m_name) {
            m_name = name;
            if (m_component) {
                m_component->name(name.toStdString());
            }
        }
    }

    // TODO: find out how to store specifications
    QVariant param() {
        return m_param;
    }

    void param(QVariant param) {
        m_param = param;
    }

private:
    XMASComponent *createComponent(QString type, QString name);

public:

protected:
    XMASComponent *component() {
        return m_component;
    }

    void component(XMASComponent *component) {
        m_component = component;
    }

private:
    QString m_name;
    QString m_compType;
    QVariant m_param;

    XMASComponent *m_component;
};

} // namespace model

#endif // COMPONENT_H
