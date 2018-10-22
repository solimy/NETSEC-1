/*
 * Main.cpp
 *
 *  Created on: 13 oct. 2018
 *      Author: daures_h
 */

#include <iostream>
#include <unistd.h>

#include "Panel.hh"
#include "Window.hh"
#include "PcapPacketManager.hh"

int main(int ac, char** av) {
  NETSEC_UI::Window window;
  NETSEC_NETWORK::PcapPacketManager ppm(NULL);
  while (42) {
    ppm.process();
    usleep(100);
  }
}
