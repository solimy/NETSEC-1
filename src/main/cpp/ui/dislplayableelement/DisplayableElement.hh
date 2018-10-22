#ifndef NETSEC_UI_DISPLAYABLEELEMENT_HH_
#define NETSEC_UI_DISPLAYABLEELEMENT_HH_

namespace NETSEC_UI {
  
  class DisplayableElement {
  public:
    
    DisplayableElement();
    void show();
    void hide();
    
  private:
        bool hidden;
  };
  
}

#endif /* !NETSEC_UI_DISPLAYABLEELEMENT_HH_ */
