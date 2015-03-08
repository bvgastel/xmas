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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QQuickItem>
#include "xmas.h"

/**
 * @brief The Component class
 *
 * A wrapper around XMASComponent from xmas.h to connect to
 * Qml using properties.
 */

namespace model {


class Component : public QQuickItem
{
public:
    enum CompType {Unknown=0, Source, Sink, Function, Queue, Join, Merge, Switch, Fork, In, Out, Composite};
private:
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_ENUMS(CompType)
    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(CompType comptype READ compType WRITE compType NOTIFY compTypeChanged)
    Q_PROPERTY(QVariant param READ param WRITE param NOTIFY paramChanged)

public:
    enum { Type = QVariant::UserType + 0 };
        int type() const Q_DECL_OVERRIDE { return Type; }

       CompType compType() const {return m_type;}
       void compType(CompType type) {m_type = type;}

    explicit Component(QQuickItem *parent = 0);
    ~Component();
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

signals:
    void nameChanged();
    void compTypeChanged();
    void paramChanged();
    void changeName(QString old_name, QString name);
    void writeLog(QString message, QColor color = Qt::blue);

public slots:

public:
    QString name() {
        if (m_component) {
            std::string name = m_component->getStdName();
            return QString(name.c_str());
        } else {
            return m_name;
        }
    }

    void name(QString name) {
        if (name != m_name) {
            m_name = name;
            if (m_component) {
                QString old_name = QString(m_component->getStdName().c_str());
                emit changeName(old_name, name); // TODO: have network catch this and change the index name

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
    XMASComponent *createComponent(CompType type, std::string name);

public:
private:
    QString m_name;
    CompType m_type;
    QVariant m_param;

    XMASComponent *m_component;
};

} // namespace model

#endif // COMPONENT_H
