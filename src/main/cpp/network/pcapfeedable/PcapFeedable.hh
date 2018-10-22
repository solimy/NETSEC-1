#ifndef NETSEC_NETWORK_PCAPFEEDABLE_HH_
#define NETSEC_NETWORK_PCAPFEEDABLE_HH_

#include "PcapPacket.hh"

namespace NETSEC_NETWORK {
  class PcapFeedable {
    virtual void feed(PcapPacket* packet)=0;
  };
}

#endif /* !NETSEC_NETWORK_PCAPFEEDABLE_HH_ */
