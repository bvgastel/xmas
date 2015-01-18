/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QQuickItem>
#include "connector.h"

class Connector;
class ConnectorType;

/**
 * @brief The Component class
 */
class Component : public QQuickItem
{    
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_PROPERTY(quint32 id READ id)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged )
    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged )
    Q_PROPERTY(QQmlListProperty<Connector> connectors READ connectors)

public:

    enum Orientation { Up, Down, Left, Right };

    Component(QQuickItem * parent=0);
    virtual ~Component();

    quint32 id() const {return m_id;}

    QString name() const {return m_name;}
    void setName(const QString &name) {m_name = name;}

    QPoint position() {return m_position;}
    void setPosition(QPoint &position) {m_position = position;}

    Orientation orientation() {return m_orientation;}
    void setOrientation(Orientation &orientation) {m_orientation = orientation;}

    QQmlListProperty<Connector> connectors();

signals:
    void nameChanged();
    void positionChanged();
    void orientationChanged();

private:
    quint32 m_id;
    QPoint m_position;
    Orientation m_orientation;
    static void append_connector(QQmlListProperty<Connector> *list, Connector *connector);
    QString m_name;
    QList<Connector *> m_connectors;
};


#endif // COMPONENT_H
