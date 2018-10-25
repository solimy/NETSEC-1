#ifndef PCAPPACKET_H
#define PCAPPACKET_H

#include "protocolenum.h"

class PcapPacket {
public:
    virtual ~PcapPacket()=0;
    virtual ProtocolEnum getProtocol()=0;
};

#endif // PCAPPACKET_H
