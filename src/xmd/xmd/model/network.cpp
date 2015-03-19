#include "network.h"

model::Network::Network(QQuickItem *parent)
    : QQuickItem(parent)
{

}

model::Network::~Network()
{

}
/** Connect method for qml */
bool model::Network::connect(XPort *port_out, XPort *port_in) {
    Output *outport = dynamic_cast<Output *>(port_out->getPort());
    Input *inport = dynamic_cast<Input *>(port_in->getPort());
    if (inport && outport) {
        ::connect(*outport, *inport);
        return true;
    }
    return false;
}
/** Disconnect method from output port for qml
 *
 *  Remark that we can disconnect from either input or output port.
 *  It does not matter for the result which we use.
 *
 */
bool model::Network::disconnect(XPort *port) {
    Output *outport = dynamic_cast<Output *>(port->getPort());
    Input *inport = dynamic_cast<Input *>(port->getPort());
    if (outport && outport->isConnected()) {
        ::disconnect(*outport);
        return true;
    } else if (inport && inport->isConnected()) {
        ::disconnect(*inport);
        return true;
    }
    return false;
}

