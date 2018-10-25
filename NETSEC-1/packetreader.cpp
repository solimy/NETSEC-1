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
    running = false;
    if (routine != nullptr) {
        routine->join();
        delete routine;
    }
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
    }
}

//TODO
void PacketReader::captureFromFile(std::string const& file) {
    while (running == true) {
        PcapPacket* packet = nullptr;
        std::cout << "capturing from file(\"" << file << "\") !!" << std::endl;
        feedSubscribers(packet);
    }
}

//TODO
void PacketReader::captureFromNetwork() {
    while (running == true) {
        PcapPacket* packet = nullptr;
        std::cout << "capturing from network !!" << std::endl;
        feedSubscribers(packet);
    }
}
