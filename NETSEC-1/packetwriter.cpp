#include "packetwriter.h"
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <inttypes.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

PacketWriter::PacketWriter()
{
}

PacketWriter::~PacketWriter()
{
    stopWriting();
}

int getIfindexFromMac(int fd, uint8_t* _mac) {
    struct ifaddrs* interface_addrs  = NULL;
    if( getifaddrs( &interface_addrs  ) == -1 )
    {
        return -1;
    }

    if( !interface_addrs )
    {
        return -1;
    }

    /// get MAC address for each interface
    for( struct ifaddrs* ifa = interface_addrs; ifa != NULL; ifa = ifa->ifa_next )
    {
        /// print MAC address
        if( ifa->ifa_data != 0 )
        {
            struct ifreq req;
            strcpy( req.ifr_name, ifa->ifa_name );
            if( ioctl( fd, SIOCGIFHWADDR, &req ) != -1 )
            {
                uint8_t* mac = (uint8_t*)req.ifr_ifru.ifru_hwaddr.sa_data;
                if (mac[0] != _mac[0] ||mac[1] != _mac[1] ||mac[2] != _mac[2] ||mac[3] != _mac[3] ||mac[4] != _mac[4] ||mac[5] != _mac[5])
                    continue;
                else {
                    printf("ifname=%s ", ifa->ifa_name);
                    return if_nametoindex(ifa->ifa_name);
                }
            }
        }
    }

    /// free memory allocated by getifaddrs
    freeifaddrs( interface_addrs );
    return -1;
}

//TODO
void PacketWriter::feed(const std::shared_ptr<PcapPacket> packet) {
    if (type != -1 && fd[type] > 0) {
        if (type == PCAP_FILE) {
            write(fd[type], packet->raw, sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len);
        }
        if (type == NETWORK) {
            EthernetRaw* eth_raw = (EthernetRaw*)(packet->raw);
            sockaddr_ll socket_address;
            socket_address.sll_halen = ETH_ALEN;
            socket_address.sll_ifindex = 1;//getIfindexFromMac(fd[type], (uint8_t*)eth_raw->ehternetHeader.h_dest);
            memcpy(socket_address.sll_addr, eth_raw->ehternetHeader.h_dest, 6);
            sendto(fd[type], packet->raw->pcapPayload, packet->raw->pcapHeader.incl_len, 0, (sockaddr*)&socket_address, sizeof(socket_address));
            printf("ifindex=%d %s\n", socket_address.sll_ifindex, strerror(errno));
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
