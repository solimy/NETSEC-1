#ifndef NETSEC_UI_WINDOW_HH_
#define NETSEC_UI_WINDOW_HH_

#include "PcapFeedable.hh"

namespace NETSEC_UI {
  
  class Window : public NETSEC_NETWORK::PcapFeedable {
  public:
    
    Window();
    void display();
    
    virtual void feed(NETSEC_NETWORK::PcapPacket* packet);
    
    enum class State {
      PACKET_CAPTURE, PACKET_FORGING
    };
    
  private:
    State state = State::PACKET_CAPTURE;
  };
  
}

#endif /* !NETSEC_UI_WINDOW_HH_ */
