#include "packetreader.h"

#include <iostream>

PacketReader::PacketReader()
{
}

void PacketReader::startCapture() {
    std::lock_guard<std::mutex> lock(routineSafety);
    if (routine != nullptr) {
        running = false;
        routine->join();
        delete routine;
    }
    running = true;
    routine = new std::thread(&PacketReader::captureFromNetwork, this);
}

void PacketReader::startCapture(std::string const& file) {
    std::lock_guard<std::mutex> lock(routineSafety);
    if (routine != nullptr) {
        running = false;
        routine->join();
        delete routine;
    }
    running = true;
    routine = new std::thread(&PacketReader::captureFromFile, this, file);
}

void PacketReader::stopCapture() {
    std::lock_guard<std::mutex> lock(routineSafety);
    running = false;
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
