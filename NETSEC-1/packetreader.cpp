#include "packetreader.h"

PacketReader::PacketReader()
{
}

void PacketReader::startCapture() {
    std::lock_guard<std::mutex> lock(routineSafety);
    if (routine == nullptr) {
        running = true;
        routine = new std::thread(&PacketReader::captureFromNetwork, this);
    }
}

void PacketReader::startCapture(std::string const& file) {
    std::lock_guard<std::mutex> lock(routineSafety);
    if (routine == nullptr) {
        running = true;
        routine = new std::thread(&PacketReader::captureFromFile, this, file);
    }
}

void PacketReader::stopCapture() {
    std::lock_guard<std::mutex> lock(routineSafety);
    running = false;
}

void PacketReader::captureFromFile(std::string const& file) {
//TODO
}

void PacketReader::captureFromNetwork() {
//TODO
}
