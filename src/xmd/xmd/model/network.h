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
#include "port.h"

namespace model {

const model::Component::CompType xsource = model::Component::CompType::Source;
const model::Component::CompType xsink = model::Component::CompType::Sink;
const model::Component::CompType xfunction = model::Component::CompType::Function;
const model::Component::CompType xqueue = model::Component::CompType::Queue;
const model::Component::CompType xjoin = model::Component::CompType::Join;
const model::Component::CompType xmerge = model::Component::CompType::Merge;
const model::Component::CompType xfork = model::Component::CompType::Fork;
const model::Component::CompType xswitch = model::Component::CompType::Switch;

class Network : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString fileName MEMBER m_fileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString folder MEMBER m_folder NOTIFY folderChanged)
    Q_PROPERTY(QString alias MEMBER m_alias NOTIFY aliasChanged)
    Q_PROPERTY(QSize size MEMBER m_size NOTIFY sizeChanged)
    Q_PROPERTY(QString imageName MEMBER m_imageName NOTIFY imageNameChanged)
    Q_PROPERTY(bool boxedImage MEMBER m_boxedImage NOTIFY boxedImageChanged)
    Q_PROPERTY(QString packet READ packet WRITE setPacket NOTIFY packetChanged)
    Q_PROPERTY(QVariantList compositeLibrary READ compositeLibrary NOTIFY compositeLibraryChanged)

private:

signals:
    void fileNameChanged();
    void folderChanged();
    void packetChanged();
    void aliasChanged();
    void sizeChanged();
    void imageNameChanged();
    void boxedImageChanged();
    void compositeLibraryChanged();
    void writeLog(QString message, QColor color = Qt::blue);
    bool createNetwork(const QVariantMap &object);

public slots:
    bool connect(XPort *port_out, XPort *port_in);
    bool disconnect(XPort *outport, XPort *inport);

    bool addComponent(model::Component *component);
    bool removeComponent(model::Component *component);
    bool setCompositeNetworkData();

    bool saveFile(QUrl fileUrl);
    bool openFile(QUrl fileUrl);
    bool closeFile();
    bool newFile();
    bool loadComposite(QUrl url);
    bool unloadComposite(QString name);

private:
    QList<Component *> getAllComponents();

    bool emitNetwork(XMASNetwork &network);
    void convertToQml(QVariantMap &map, XMASComponent *comp);
    void connectInQml(QVariantList &list, XMASComponent *comp);

    bool portError(XPort *outport, QString errMsg);

    bool xmasError(Output *xmas_outport, Input *xmas_inport, QString errMsg);
    bool xmasDisconnectOk(Output *xmas_outport, Input *xmas_inport);
    bool xmasConnectOk(Output *xmas_outport, Input *xmas_inport);

    bool connect(Output *xmas_outport, Input *xmas_inport);
    bool disconnect(Output *xmas_outport, Input *xmas_inport);

    bool addComposite(XMASNetwork* xmas_network);
    bool clearCompositeLibrary();

public:
    explicit Network(QQuickItem *parent = 0);
    ~Network();

    QString packet();
    bool setPacket(QString expression);

    QVariantList compositeLibrary();

private:

    Logger m_logger;
    QString m_fileName;
    QString m_folder;
    QString m_alias;
    QSize m_size;
    QString m_imageName;
    bool m_boxedImage;
    QVariantList m_compositeLibrary;

    std::map<std::type_index, model::Component::CompType> m_type_index_map = {
        {std::type_index(typeid(XMASSource)), xsource},
        {std::type_index(typeid(XMASSink)), xsink},
        {std::type_index(typeid(XMASFunction)), xfunction},
        {std::type_index(typeid(XMASQueue)), xqueue},
        {std::type_index(typeid(XMASJoin)), xjoin},
        {std::type_index(typeid(XMASMerge)), xmerge},
        {std::type_index(typeid(XMASFork)), xfork},
        {std::type_index(typeid(XMASSwitch)), xswitch},
        {std::type_index(typeid(XMASComposite)), model::Component::Composite},
    };
};
} // namespace model
#endif // NETWORK_H
