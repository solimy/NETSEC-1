#include "packetwriter.h"

#include <unistd.h>

PacketWriter::PacketWriter()
{
}

PacketWriter::~PacketWriter()
{
    stopWriting();
}

//TODO
void PacketWriter::feed(const PcapPacket* packet) {
    if (socket > 0) {

        //TODO si fichier
        write(socket, packet->getBuffer_withPcapHeader(), packet->getSize_withPcapHeader());
        //TODO si network
        write(socket, packet->getBuffer_withoutPcapHeader(), packet->getSize_withoutPcapHeader());
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
