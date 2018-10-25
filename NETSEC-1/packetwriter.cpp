#include "packetwriter.h"

#include <unistd.h>

PacketWriter::PacketWriter()
{
}

PacketWriter::~PacketWriter()
{
    stopWriting();
}

void PacketWriter::feed(const PcapPacket* packet) {
    if (socket > 0) {
        write(socket, packet->getBuffer(), packet->getSize());
    }
}

void PacketWriter::stopWriting() {
    if (socket > 0) {
        close(socket);
    }
}

//TODO
void PacketWriter::startWriting() {
    stopWriting();

}

//TODO
void PacketWriter::startWriting(std::string const& file) {
    stopWriting();

}
