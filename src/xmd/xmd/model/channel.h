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

#ifndef CHANNEL_H
#define CHANNEL_H

#include <QQuickItem>
#include "component.h"
#include "port.h"

namespace model {

//class XPort;
//class Component;

class Channel : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(XPort *outport READ outport WRITE outport NOTIFY inportChanged)
    Q_PROPERTY(XPort *inport READ inport WRITE inport NOTIFY outportChanged)

signals:

    void inportChanged();
    void outportChanged();

    void writeLog(QString msg, QColor color = Qt::blue);

public slots:


public:
    explicit Channel(QQuickItem *parent = 0);
    ~Channel();

    virtual void classBegin();
    virtual void componentComplete();

    XPort *outport() {
        return m_outport;
    }

    void outport(XPort *firstPort){
        m_outport = firstPort;
        emit inportChanged();
    }

    XPort *inport() {
        return m_inport;
    }

    void inport(XPort *secondPort){
        m_inport = secondPort;
        emit outportChanged();
    }

private:
    bool portsOk(QString &errMsg);

private:

    XPort *m_outport;
    XPort *m_inport;
};

} // namespace model

#endif // CHANNEL_H
