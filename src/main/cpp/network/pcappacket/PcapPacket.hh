#ifndef NETSEC_NETWORK_PCAPPACKET_HH_
#define NETSEC_NETWORK_PCAPPACKET_HH_

#include "ProtocolEnum.hh"

namespace NETSEC_NETWORK {
  class PcapPacket {
  public:
    ProtocolEnum getProtocol();
    int* getIpSrc();
    int* getIpDst();
    int* getPortSrc();
    int* getPortDst();
  };
}

#endif /* !NETSEC_NETWORK_PCAPPACKET_HH_ */
