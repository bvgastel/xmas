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
#ifndef NETWORK_H
#define NETWORK_H

#include <QQuickItem>

#include "export.h"
#include "component.h"

namespace model {

class Network : public QQuickItem
{
    Q_OBJECT
    //Q_PROPERTY(QVariantList compList READ compList WRITE compList NOTIFY compListChanged)

private:

signals:
    void compListChanged();

    void writeLog(QString message, QColor color = Qt::blue);

public slots:
    /** Connect method for qml */
    bool connect(XPort *port_out, XPort *port_in);
    /** Disconnect method from output port for qml */
    bool disconnect(XPort *port);

    QString toJson(QList<Component *> allComponents) {
        bitpowder::lib::String result;
        bitpowder::lib::MemoryPool mp;
        bitpowder::lib::JSONData globals;

        std::set<XMASComponent *> allComp;
        for (Component *comp : allComponents) {
            auto c = comp->getXMASComponent();
            allComp.insert(c);
        }
        result = ::Export(allComp,globals,mp);
        QString jsonString = QString(result.stl().c_str());
        return jsonString;
    }

// TODO: Property List met 4 callbacks

//    QVariantList compList() {
//        return m_compList;
//    }
//    void compList(QVariantList compList) {
//        m_compList = compList;
//    }

public:
    explicit Network(QQuickItem *parent = 0);
    ~Network();

private:
    //QVariantList m_compList;
};

} // namespace model
#endif // NETWORK_H
