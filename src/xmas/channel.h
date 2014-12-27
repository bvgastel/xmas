#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>

#include <QString>

#include "inport.h"
#include "outport.h"

class Channel
{
public:
    explicit Channel(std::shared_ptr<OutPort> out, std::shared_ptr<InPort> in);
   ~Channel();

   const QString name() const;
   std::shared_ptr<const InPort> in() const;
   std::shared_ptr<const OutPort> out() const;

private:

   QString m_name;
   std::shared_ptr<InPort> m_in;
   std::shared_ptr<OutPort> m_out;

};

#endif // CHANNEL_H
