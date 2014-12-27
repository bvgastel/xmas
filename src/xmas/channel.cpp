#include "channel.h"

Channel::Channel(std::shared_ptr<OutPort> out, std::shared_ptr<InPort> in)
        : m_out(out), m_in(in)
{
}

Channel::~Channel()
{
    // no delete necessary
}

const QString Channel::name() const
{
    return m_name;
}

std::shared_ptr<const InPort> Channel::in() const
{
    return m_in;
}

std::shared_ptr<const OutPort> Channel::out() const
{
    return m_out;
}
