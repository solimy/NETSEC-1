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
void PacketWriter::feed(const std::shared_ptr<PcapPacket> packet) {
}

void PacketWriter::stopWriting() {
    if (type != -1 && fd[type] > 0) {
        close(fd[type]);
        fd[type] = -1;
    }
    type = -1;
}

//TODO
void PacketWriter::startWriting() {
    stopWriting();
    type = NETWORK;
    fd[type] = -1;
}

//TODO
void PacketWriter::startWriting(std::string const& file) {
}
