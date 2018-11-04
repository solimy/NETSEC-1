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
    if (type != -1 && fd[type] > 0) {
        if (type == FILE) {
            struct pcap_pkthdr pkthdr;

            pkthdr.len = pkthdr.caplen = packet->raw.pcapHeader.orig_len;
            pkthdr.ts.tv_sec = packet->raw.pcapHeader.ts_sec; // Some unix timestamp
            write(fd[type], &pkthdr, sizeof(pkthdr));
            write(fd[type], &(packet->raw.pcapPayload), sizeof(packet->raw.pcapPayload));
        }
        //TODO si fichier
        //write(socket, packet->getBuffer_withPcapHeader(), packet->getSize_withPcapHeader());
        //TODO si network
        //write(socket, packet->getBuffer_withoutPcapHeader(), packet->getSize_withoutPcapHeader());
    }
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
    struct pcap_file_header fh;
    
    stopWriting();
    type = FILE;
    fd[type] = open(file.c_str(), O_APPEND | O_CREAT);
    fh.magic = TCPDUMP_MAGIC;
    fh.sigfigs = 0;
    fh.version_major = 2;
    fh.version_minor = 4;
    fh.snaplen = USHRT_MAX;
    fh.thiszone = 0;
    fh.linktype = LINKTYPE_ETHERNET;
    write(fd[type], &fh, sizeof(fh));
}
