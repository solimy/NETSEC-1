#include "PcapPacketManager.hh"

NETSEC_NETWORK::PcapPacketManager::PcapPacketManager(PcapFeedable* ui) {
  this->ui = ui;
}

void NETSEC_NETWORK::PcapPacketManager::process() {
  if(read != NULL) (*this.*read)();
}

void NETSEC_NETWORK::PcapPacketManager::stopCaptureToFile() {
  if (fileWriter != NULL)
    delete fileWriter;
  fileWriter = NULL;
}

void NETSEC_NETWORK::PcapPacketManager::stopCapture() {
  stopCaptureToFile();
  if (inputSocket != NULL) {
    //close(inputSocket);
    inputSocket = NULL;
  }
}
