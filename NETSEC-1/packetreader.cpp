#include "packetreader.h"

#include <iostream>

PacketReader::PacketReader()
{
    running = false;
    routine = nullptr;
}

PacketReader::~PacketReader()
{
    stopCapture();
}

void PacketReader::startCapture() {
    stopCapture();
    std::lock_guard<std::mutex> lock(routineSafety);
    running = true;
    routine = new std::thread(&PacketReader::captureFromNetwork, this);
}

void PacketReader::startCapture(std::string const& file) {
    stopCapture();
    std::lock_guard<std::mutex> lock(routineSafety);
    running = true;
    routine = new std::thread(&PacketReader::captureFromFile, this, file);
}

//TODO close(socket) !!
void PacketReader::stopCapture() {
    std::lock_guard<std::mutex> lock(routineSafety);
    running = false;
    if (routine != nullptr) {
        routine->join();
        delete routine;
        routine = nullptr;
    }
}

//TODO
void PacketReader::captureFromFile(std::string const& file) {
    while (running == true) {
        std::shared_ptr<PcapPacket> packet = nullptr;
        feedSubscribers(packet);
    }
}

//TODO
void PacketReader::captureFromNetwork() {
    int raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    unsigned char buffer[65536];
    while (running == true) {
        memset(buffer,0,65536);
        struct sockaddr saddr;
        int saddr_len = sizeof (saddr);
        int buflen=recvfrom(raw_socket,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);
        std::shared_ptr<PcapPacket> packet = std::make_shared<PcapPacket>(buffer, buflen);
        feedSubscribers(packet);
    }
}
