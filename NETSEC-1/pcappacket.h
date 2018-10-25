#ifndef PCAPPACKET_H
#define PCAPPACKET_H

#include "protocolenum.h"

class PcapPacket {
public:
    virtual ~PcapPacket()=0;
    virtual ProtocolEnum getProtocol() const =0;
    virtual const char* getBuffer() const =0;
    virtual unsigned long getSize() const =0;
};

#endif // PCAPPACKET_H
