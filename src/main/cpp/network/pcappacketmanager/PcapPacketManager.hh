#ifndef NETSEC_NETWORK_PCAPPACKETMANAGER_HH_
#define NETSEC_NETWORK_PCAPPACKETMANAGER_HH_

#include <memory>

#include "PcapFeedable.hh"

namespace NETSEC_NETWORK {
  class PcapPacketManager {
  private:
    int* filter_ip_src = NULL;
    int* filter_ip_dst = NULL;
    int* filter_port_src = NULL;
    int* filter_port_dst = NULL;
    int* filter_proto = NULL;

    PcapFeedable* fileWriter = NULL;
    PcapFeedable* ui = NULL;

    int* inputSocket = NULL;
    void readFromFile();
    void readFromNetwork();
    void (PcapPacketManager::*read)() = NULL;
    
  public:
    PcapPacketManager(PcapFeedable* ui);

    void startCapture();
    void stopCapture();
    void startCaptureToFile();
    void stopCaptureToFile();

    void process();    
  };
}

#endif /* !NETSEC_NETWORK_PCAPPACKETMANAGER_HH_ */
