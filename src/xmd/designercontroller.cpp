#include "designercontroller.h"

DesignerController::DesignerController(Network& network)
  : m_network(network)
{
}

DesignerController::~DesignerController()
{
}

void DesignerController::addChannel(Output &output, Input &input)
{
    m_network.addChannel(output, input);
}
