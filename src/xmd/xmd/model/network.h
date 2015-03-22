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
    //NOTE : alias, size , imageName and asSymbol must be serialized in the json in the network object
    Q_PROPERTY(QString alias MEMBER m_alias NOTIFY aliasChanged)
    Q_PROPERTY(QSize size MEMBER m_size NOTIFY sizeChanged)
    Q_PROPERTY(QString imageName MEMBER m_imageName NOTIFY imageNameChanged)
    Q_PROPERTY(bool asSymbol MEMBER m_asSymbol NOTIFY asSymbolChanged)
    Q_PROPERTY(QQmlListProperty<model::Component> components READ components NOTIFY componentsChanged)

private:

signals:
    void aliasChanged();
    void sizeChanged();
    void imageNameChanged();
    void asSymbolChanged();
    void componentsChanged();
    void writeLog(QString message, QColor color = Qt::blue);

public slots:
    /**
     * @brief connect
     *
     * Connect the specified outport to the specified
     * inport. If one of the ports is already
     * connected the results are unspecified.
     *
     * @param port_out The output XPort
     * @param port_in The input XPort
     * @return true if the operation is successful, false otherwise
     */
    bool connect(XPort *port_out, XPort *port_in);
    /**
     * @brief disconnect
     *
     * Disconnect an output port from an input port.
     *
     * @param outport The outport that needs disconnection
     * @param inport The inport connected to the outport
     * @return true if the disconnect was successful, false otherwise
     */
    bool disconnect(XPort *outport, XPort *inport);
    /**
     * @brief toJson
     *
     *  Transfrom a given network of Component classes to
     *  a QString containing JSON for the network.
     *
     * @param allComponents A QList of Component pointers
     * @return a QString containing the json for the network.
     *
     */
    QString toJson();

    bool addComponent(model::Component *component);

    bool saveFile(QUrl url);
    bool openFile(QUrl url);
    bool closeFile();
    bool newFile();

    QQmlListProperty<Component> components();

private:
    //#################################################################################################
    // instead of qmllist we can use this simple loop to read all components on the canvas
    // (linking a qmllist directly needs a slightly different approach at qml side too,can be done later if time left)
    QList<Component *> getAllComponents() {
        m_components.clear();

        for(QQuickItem *item : this->childItems()) {
            Component *c = qobject_cast<Component *>(item);
            if(c){
                m_components.append(c);
            }
        }
        qDebug() << "Total comps in list = " << m_components.count();
        return m_components;
    }

    //#################################################################################################

    static void append_components(QQmlListProperty<Component> *property, Component *comp);
    static int count_components(QQmlListProperty<Component> *property);
    static Component *at_components(QQmlListProperty<Component> *property, int index);
    static void clear_components(QQmlListProperty<Component> *property);

public:
    explicit Network(QQuickItem *parent = 0);
    ~Network();

private:

    QList<Component *> m_components;
    std::set<XMASComponent *> m_xmas_comp_list;

    QString m_alias;
    QSize m_size;
    QString m_imageName;
    bool m_asSymbol;

};

} // namespace model
#endif // NETWORK_H
