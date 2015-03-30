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

#include "datacontrol.h"
#include "export.h"
#include "component.h"

namespace model {

class Network : public QQuickItem
{
    Q_OBJECT
    //NOTE : alias, size , imageName and boxedImage must be serialized in the json in the network object
    Q_PROPERTY(QString alias MEMBER m_alias NOTIFY aliasChanged)
    Q_PROPERTY(QSize size MEMBER m_size NOTIFY sizeChanged)
    Q_PROPERTY(QString imageName MEMBER m_imageName NOTIFY imageNameChanged)
    Q_PROPERTY(bool boxedImage MEMBER m_boxedImage NOTIFY boxedImageChanged)
    Q_PROPERTY(QString packet READ packet WRITE setPacket NOTIFY packetChanged)
    Q_PROPERTY(QVariantList compositeLibrary READ compositeLibrary NOTIFY compositeLibraryChanged)

private:

signals:
    void packetChanged();
    void aliasChanged();
    void sizeChanged();
    void imageNameChanged();
    void boxedImageChanged();
    void compositeLibraryChanged();
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

    //bool addComponent(model::Component *component);

    bool saveFile(QUrl url);
    bool openFile(QUrl url);
    bool closeFile();
    bool newFile();
    bool addLibraryComposite(QUrl url);
    bool removeLibraryComposite(QUrl url);

private:
    QList<Component *> getAllComponents();

    bool portError(XPort *outport, QString errMsg);

    bool xmasError(Output *xmas_outport, Input *xmas_inport, QString errMsg);
    bool xmasDisconnectOk(Output *xmas_outport, Input *xmas_inport);
    bool xmasConnectOk(Output *xmas_outport, Input *xmas_inport);

    bool connect(Output *xmas_outport, Input *xmas_inport);
    bool disconnect(Output *xmas_outport, Input *xmas_inport);

public:
    explicit Network(QQuickItem *parent = 0);
    ~Network();

    QString packet();
    void setPacket(QString expression);

    QVariantList compositeLibrary();

private:

    Logger m_logger;
    QString m_alias;
    QSize m_size;
    QString m_imageName;
    QString m_packet;
    bool m_boxedImage;
    QVariantList m_compositeLibrary;
    bool addComposite(QUrl url);

};

} // namespace model
#endif // NETWORK_H
