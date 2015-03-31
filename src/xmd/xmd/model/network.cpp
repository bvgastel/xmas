#include <QList>
#include <QUrl>

#include "util.h"
#include "commoninterface.h"
#include "network.h"

#include "xmas.h"
#include "composite-network-extension.h"

/* Access to the global datacontrol pointer */
extern DataControl *dataControl;

model::Network::Network(QQuickItem *parent)
    : QQuickItem(parent),
      m_logger("Network.cpp")
{
    QObject::connect(&m_logger, &Logger::writeLog, dataControl, &DataControl::writeLog );
}

model::Network::~Network() {}

QString model::Network::packet() {
    return m_packet ;
}

void model::Network::setPacket(QString expression) {
    if (expression != m_packet) {
        m_packet = expression;
    }
    emit packetChanged();
    m_logger.log(QString("packet expression = ")+m_packet);
    qDebug() << "packet expression = " << m_packet;
}

/*
 * portError(port, errMsg) always return false to support error messaging.
 *
 */
bool model::Network::portError(XPort *port, QString errMsg) {
    if (port) {
        errMsg += (port->getType() == model::XPort::OUTPORT ? " port = output_port! "
                                                            : " port = input_port! ");
        errMsg += port->getComponent()->getName()+"."+port->getName() + ". ";
        if (!port->getConnected()) {
            errMsg += "Port is not connected. ";
        }
    }
    emit writeLog(errMsg, Qt::red);
    qDebug() << errMsg;
    return false;
}

bool model::Network::xmasError(Output *xmas_outport, Input *xmas_inport, QString errMsg) {
    QString out_owner = "null_output";
    QString out_port_name = "null";
    bool out_port_is_connected = false;
    QString in_owner = "null_input";
    QString in_port_name = "null";
    bool in_port_is_connected = false;

    if (xmas_outport) {
        if (xmas_outport->m_owner)
            out_owner = xmas_outport->m_owner->getStdName().c_str();
        out_port_name = xmas_outport->getName();
        out_port_is_connected = xmas_outport->isConnected();
    }
    if (xmas_inport) {
        if (xmas_inport->m_owner)
            in_owner = xmas_inport->m_owner->getStdName().c_str();
        in_port_name = xmas_inport->getName();
        in_port_is_connected = xmas_inport->isConnected();
    }

    errMsg += " " + out_owner+"."+out_port_name;
    errMsg += "->" + in_owner + "." + in_port_name + " ";
    if (in_port_is_connected && out_port_is_connected) {
        errMsg += "(both connected).";
    } else if (in_port_is_connected){
        errMsg += "(inport connected, outport not connected.";
    } else if (out_port_is_connected){
        errMsg += "(inport not connected).";
    } else {
        errMsg += "(both not connected).";
    }

    emit writeLog(errMsg);
    qDebug() << errMsg;
    return false;
}

bool model::Network::xmasConnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_inport || !xmas_outport) {
        return xmasError(xmas_outport, xmas_inport, "[Network::connect()] Connect failed: inport or outport null. ");
    }
    if (xmas_inport->isConnected() || xmas_outport->isConnected()) {
        return xmasError(xmas_outport, xmas_inport, "[Network::connect()] Connect failed: inport or outport already connected. ");
    }
    return true;
}

bool model::Network::connect(Output *xmas_outport, Input *xmas_inport) {
    if (xmasConnectOk(xmas_outport, xmas_inport)) {
        ::connect(*xmas_outport, *xmas_inport);
        return true;
    }
    return false;
}

/*
 * @brief model::Network::xmasDisconnectOk
 *
 *  The Qml system calls the disconnect on request of the qml
 * javascript both explicitly and implicitly. Due to the way Qml
 * works, at closing time, Qml might ask for disconnect of ports
 * that are not connected. This is not an error. The cause is a
 * difference in structure on the canvas and in xmas-components.
 *
 * On the canvas the channel is an object that has 2 ports that are
 * also objects. When deleting the canvas, Qml deletes each port in
 * sequence. This causes two disconnect requests in sequence. The first
 * will always be ok, as the components are connected. The second
 * will meet with components that are already disconnect. In xmas
 * the disconnects leads to automatic disconnection of both ends of
 * the channel, while in Qml the system tries to disconnect both ports
 * in sequence. So, we ignore this fact en execute the disconnect anyways,
 * because xmas.cpp will not do anything for already disconnected ports.
 *
 * For that reason the disconnect responds to a request to disconnect
 * already disconnected ports as if the disconnect was successful.
 *
 * @param xmas_outport
 * @param xmas_inport
 * @return
 */
bool model::Network::xmasDisconnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_outport->connectedTo(xmas_inport->m_owner)) {
        if (xmas_inport && !xmas_inport->isConnected()
                && xmas_outport && !xmas_outport->isConnected()) {
            return true;
        }
        xmasError(xmas_outport, xmas_inport,
                  "[Network::disconnect(xmas)] no out->in connection.");
        return false;
    }
    return true;
}

/*
 * @brief model::Network::disconnect
 *
 * Disconnecting already disconnected ports does not lead to error,
 * because xmas.cpp ::disconnect checks first and deletes after ok.
 * If not output.valid() it does nothing.
 *
 * @param xmas_outport
 * @param xmas_inport
 * @return
 */
bool model::Network::disconnect(Output *xmas_outport, Input *xmas_inport) {
    bool success = xmasDisconnectOk(xmas_outport, xmas_inport);
    if (success) {
        ::disconnect(*xmas_outport);
        return true;
    }
    return false;
}

/**
 * Connect method for qml
 *
 * @brief model::Network::connect
 * @param outport
 * @param inport
 * @return xmas connection succeeded
 */
bool model::Network::connect(XPort *outport, XPort *inport) {

    // Check ports
    if (!outport) return portError(outport, "[Network::connect()] outport is null: ");
    if (!inport) return portError(inport, "[Network::connect()] inport is null.");

    // obtain and check xmas inport and outport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    bool success = connect(xmas_outport, xmas_inport);

    if (success) {
        if (outport->getComponent()) {
            emit outport->getComponent()->validChanged();
        }
        if (inport->getComponent()) {
            emit inport->getComponent()->validChanged();
        }
        emit outport->connectedChanged();
        emit inport->connectedChanged();
    }
    return success;
}

bool model::Network::disconnect(XPort *outport, XPort *inport) {

    // check outport
    if (!outport) return portError(outport, "[Network::disconnect()] outport is null: ");
    if (!inport) return portError(inport, "[Network::disconnect()] inport is null.");

    //  check outport for being xmas outport to inport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (xmas_outport == nullptr || xmas_inport == nullptr) {
        xmasError(xmas_outport, xmas_inport, "[model::Network::disconnect()] xmas ports null: ");
        return false;
    }
    if (disconnect(xmas_outport, xmas_inport)) {
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    }

    return false;
}


bool model::Network::openFile(QUrl url) {

    QString json = Util::openFile(url);
    // TODO: convert json to XMASComponent
    return true;
}

bool model::Network::saveFile(QUrl url) {
    return Util::saveFile(url, toJson());
}

bool model::Network::closeFile() {
    //TODO destroy the model and reset everything
    return true;
}

bool model::Network::newFile() {
    return closeFile();
}

QString model::Network::toJson() {

    bitpowder::lib::String result;
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::JSONData globals = bitpowder::lib::JSONData::AllocateMap(mp);
    bitpowder::lib::JSONData packet = bitpowder::lib::JSONData::AllocateVector(mp);
    //packet.insert(m_packet);
    globals["PACKET_TYPE"] = packet;
    globals["VARS"] = bitpowder::lib::JSONData::AllocateMap(mp);

    auto xset = std::set<XMASComponent *>();
    if (dataControl->project()) {
        auto network = dataControl->project()->getRootNetwork();
        if (network) {
            network->getComponentSet(xset);
        }
    }
    result = ::Export(xset, globals, mp);

    QString jsonString = QString(result.stl().c_str());
    return jsonString;
}

bool model::Network::addComposite(model::Component *component) {

    auto project = dataControl->project();

    if (!project) {
        emit writeLog(QString("Project not existing! All will fail!"));
        return false;
    }

    QUrl fileUrl = QUrl(component->property("url").toString());

    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    std::string name = component->getName().toStdString();

    auto xnetwork = project->loadNetwork(filename);
    project->insertComposite(name, std::ref(*xnetwork));

    return true;
}

bool model::Network::addComponent(model::Component *component) {

    auto project = dataControl->project();
    if (!project) {
        emit writeLog(QString("Project not existing! All will fail!"));
        return false;
    }

    std::string name = component->getName().toStdString();
    model::Component::CompType type = component->getType();

    bool result = true;
    switch(type) {
    case model::Component::CompType::Source :
        project->insertSource(name);
        break;
    case model::Component::CompType::Sink :
        project->insertSink(name);
        break;
    case model::Component::CompType::Function :
        project->insertFunction(name);
        break;
    case model::Component::CompType::Queue :
        project->insertQueue(name);
        break;
    case model::Component::CompType::Join :
        project->insertJoin(name);
        break;
    case model::Component::CompType::Merge :
        project->insertMerge(name);
        break;
    case model::Component::CompType::Switch :
        project->insertSwitch(name);
        break;
    case model::Component::CompType::Fork :
        project->insertFork(name);
        break;
    case model::Component::CompType::Composite :
        result = addComposite(component);
        break;
    default :
        emit writeLog(QString("Unknown component type!"), Qt::red);
        return false;
    }
    return result;
}



//#############################################################################################################
//#############################################################################################################
//##
//##        Example of how to populate composite list
//##        This must be started when a netwerk is opened and loop through
//##        the data from the Composite section of the json.
//##
//##
//#############################################################################################################
//#############################################################################################################
/**
 * Gets the composite list of this network
 *      (Related to the composites section in json)
 * @brief model::Network::getComposites
 * @return Network composite list
 */
//TODO replace example data with json data
QVariantList model::Network::compositeLibrary() {
//    m_compositeLibrary.clear();
//    //example composite 1
//    QVariantMap composite1;
//    composite1.insert("url", "file://xmas-models/test.json");
//    composite1.insert("alias", "Credit Counter");
//    composite1.insert("symbol", "counter.png");
//    composite1.insert("boxed", false);
//    m_compositeLibrary.append(composite1);

//    //example composite 2
//    QVariantMap composite2;
//    composite2.insert("url", "");
//    composite2.insert("alias", "Delay");
//    composite2.insert("symbol", "delay.png");
//    composite2.insert("boxed", false);
//    m_compositeLibrary.append(composite2);

//    //example composite 3
//    QVariantMap composite3;
//    composite3.insert("url", "");
//    composite3.insert("alias", "MuxSrc");
//    composite3.insert("symbol", "muxsource.png");
//    composite3.insert("boxed", false);
//    m_compositeLibrary.append(composite3);

//    //example composite 4
//    QVariantMap composite4;
//    composite4.insert("url", "");
//    composite4.insert("alias", "mySubnet");
//    composite4.insert("symbol", "");
//    composite4.insert("boxed", true);
//    m_compositeLibrary.append(composite4);

//    example composite 5
//    QVariantMap composite5;
//    composite5.insert("url", "");
//    composite5.insert("alias", "spidergon");
//    composite5.insert("symbol", "spidergon.ico");
//    composite5.insert("boxed", true);
//    m_compositeLibrary.append(composite5);

    return m_compositeLibrary;
}

/**
 * Adds a composite to this network its composite library
 *      (Related to the composites section in json)
 * @brief model::Network::addLibraryComposite
 * @param url
 * @return True is composite has been added to the library.
 */
// NOTE: are these remarks important for the code?
//4.1 url doesn't exist
//4.2 url already in list
//4.3 parser failed to read composite
bool model::Network::addLibraryComposite(QUrl url){
    qDebug() << "Add library composite with url = " << url;

    //1 - send url to xmas and parse as composite
    std::string name = url.toLocalFile().toStdString();
    try {
        XMASNetwork* xmas_network = dataControl->project()->loadNetwork(name);
        auto cn_ext = xmas_network->getNetworkExtension<CompositeNetworkExtension>(false);
        if (!cn_ext)
            return false;   // Composite network information missing, can't use as a composite network!

        QVariantMap map;
        map.insert("url", url);
        map.insert("alias", QString::fromStdString(cn_ext->alias));
        map.insert("symbol", QString::fromStdString(cn_ext->imageName));
        map.insert("boxed", cn_ext->boxedImage);
        map.insert("xmas_network", qVariantFromValue((void*)xmas_network));
        m_compositeLibrary.append(map);
        emit compositeLibraryChanged();
    } catch (bitpowder::lib::Exception) {
        return false;
    }

    return true;
}

/**
 * Removes a composite in this network its composite library
 *      (Related to the composites section in json)
 * @brief model::Network::removeLibraryComposite
 * @param url
 * @return True is composite has been removed from the library.
 */
bool model::Network::removeLibraryComposite(QUrl url){
    qDebug() << "Remove library composite with url = " << url;
    //1 - send url to xmas find composite in library by url
    //2 - remove composite in xmas if not used in current network
    //3 - if ok ; remove composite in library based on url
    //4 - if not ok return false without emit
    //4.1 url doesn't exist in xmas composite library
    //4.2 url still used in current network
    //4.3 xmas failed to delete composite

    emit compositeLibraryChanged();
    return true;
}


bool model::Network::addComposite(QUrl url){
    qDebug() << "add composite with url = " << url;
    return true;
}
