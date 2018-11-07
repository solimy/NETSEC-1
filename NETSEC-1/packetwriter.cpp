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
        if (type == PCAP_FILE) {
            write(fd[type], packet->raw, sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len);
        }
        if (type == NETWORK) {
            EthernetRaw* eth_raw = (EthernetRaw*)(packet->raw);
            struct sockaddr *sock_dst = (struct sockaddr*)(eth_raw->ehternetHeader.h_dest);
            sendto(fd[type], packet->raw->pcapPayload, packet->raw->pcapHeader.incl_len, 0, sock_dst, sizeof(struct sockaddr));
        }
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
    fd[type] = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
}

//TODO
void PacketWriter::startWriting(std::string const& file) {
    struct pcap_file_header fh;
    
    stopWriting();
    type = PCAP_FILE;
    fd[type] = open(file.c_str(), O_RDWR | O_APPEND | O_CREAT);
    fh.magic = TCPDUMP_MAGIC;
    fh.sigfigs = 0;
    fh.version_major = PCAP_VERSION_MAJOR;
    fh.version_minor = PCAP_VERSION_MINOR;
    fh.snaplen = USHRT_MAX;
    fh.thiszone = 0;
    fh.linktype = LINKTYPE_ETHERNET;
    write(fd[type], &fh, sizeof(fh));
}
