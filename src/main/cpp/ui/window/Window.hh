#ifndef NETSEC_UI_WINDOW_HH_
#define NETSEC_UI_WINDOW_HH_

#include "PcapFeedable"

namespace NETSEC_UI {
  
  class Window : public PcapFeedable {
  public:
    
    Window();
    void display();
    
    virtual void feed(PcapPacket* packet);
    
    enum class State {
      PACKET_CAPTURE, PACKET_FORGING
    };
    
  private:
    State state = PACKET_CAPTURE;
  };
  
}

#endif /* !NETSEC_UI_WINDOW_HH_ */
