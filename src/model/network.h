/*********************************************************************
  *
  * Copyright (C) Guus Bonnema, 2014
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

#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>

#include "component.h"
#include "channel.h"

namespace model {

class Network : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(QQmlListProperty<Component> components READ components WRITE components NOTIFY componentsChanged)
    Q_PROPERTY(QQmlListProperty<Channel> channels READ channels WRITE channels NOTIFY channelsChanged)

public:
    explicit Network(QObject *parent = 0);
    ~Network();

    QString name() const { return m_name; }
    void name(QString &name) { m_name = name; }

    QQmlListProperty<Component> components() { return m_components; }
    void components(QQmlListProperty<Component> &components) { m_components = components; }

    QQmlListProperty<Channel> channels() { return m_channels; }
    void channels(QQmlListProperty<Channel> &channels) { m_channels = channels; }

signals:
    void nameChanged();
    void componentsChanged();
    void channelsChanged();

public slots:

private:
    QString m_name;
    QQmlListProperty<Component> m_components;
    QQmlListProperty<Channel> m_channels;
};

} // namespace model

#endif // NETWORK_H
