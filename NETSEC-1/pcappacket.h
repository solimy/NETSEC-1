#ifndef PCAPPACKET_H
#define PCAPPACKET_H

#include <type_traits>
#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <chrono>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <net/ethernet.h>
#include <net/if_arp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include "protocolenum.h"

typedef struct pcaprec_hdr_s {
    uint32_t ts_sec;         /* timestamp seconds */
    uint32_t ts_usec;        /* timestamp microseconds */
    uint32_t incl_len;       /* number of octets of packet saved in file */
    uint32_t orig_len;       /* actual length of packet */
} pcaprec_hdr_t;

//UNKNOWN
struct PcapRaw {
    pcaprec_hdr_t pcapHeader;
    uint8_t payload[0];
} __attribute__((packed));

//ETHERNET
struct EthernetRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    uint8_t payload[0];
} __attribute__((packed));

//IP
struct IPRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    iphdr ipHeader;
    uint8_t payload[0];
} __attribute__((packed));

//ARP
struct ARPRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    iphdr ipHeader;
    arphdr arpHeader;
    uint8_t payload[0];
} __attribute__((packed));

//ICMP
struct ICMPRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    iphdr ipHeader;
    icmphdr icmpHeader;
    uint8_t payload[0];
} __attribute__((packed));

//TCP
struct TCPRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    iphdr ipHeader;
    tcphdr tcpHeader;
    uint8_t payload[0];
} __attribute__((packed));

//UDP
struct UDPRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    iphdr ipHeader;
    udphdr udpHeader;
    uint8_t payload[0];
} __attribute__((packed));

//HTTP
struct HTTPRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    iphdr ipHeader;
    tcphdr tcpHeader;
    char httpPayload[0];
} __attribute__((packed));

//DNS
struct DNSRaw {
    pcaprec_hdr_t pcapHeader;
    ethhdr ehternetHeader;
    iphdr ipHeader;
    tcphdr tcpHeader;
    char dnsPayload[0];
} __attribute__((packed));

class PcapPacket {
public:
    static ProtocolEnum protocolFinder(TCPRaw* raw) {
        return ProtocolEnum::TCP;
    }

    static ProtocolEnum protocolFinder(UDPRaw* raw) {
        return ProtocolEnum::UDP;
    }

    static ProtocolEnum protocolFinder(IPRaw* raw) {
        switch (raw->ipHeader.protocol) {
        case IPPROTO_TCP:
            return protocolFinder((TCPRaw*)raw);
        case IPPROTO_UDP:
            return protocolFinder((UDPRaw*)raw);
        case IPPROTO_ICMP:
            return ProtocolEnum::ICMP;
        default:
            return ProtocolEnum::IP;
        }
    }

    static ProtocolEnum protocolFinder(EthernetRaw* raw) {
        printf("%u :: IP=%d, TCP=%d\n", raw->ehternetHeader.h_proto, ETHERTYPE_IP, IPPROTO_TCP);
        if (raw->ehternetHeader.h_proto <= 1500)
            return protocolFinder((IPRaw*)raw);
        return ProtocolEnum::ETHERNET;
    }

    static ProtocolEnum protocolFinder(PcapRaw* raw) {
        if (raw->pcapHeader.incl_len < sizeof (ethhdr)) {
            return ProtocolEnum::UNKNOWN;
        }
        return protocolFinder((EthernetRaw*)raw);
    }

    PcapPacket(PcapRaw* raw) {
        this->raw = raw;
        protocol = protocolFinder(this->raw);
    }

    PcapPacket(uint8_t* raw, uint64_t len) {
        struct timespec ti;
        clock_gettime(CLOCK_REALTIME, &ti);
        this->raw = (PcapRaw*)malloc(sizeof (PcapRaw) + len);
        this->raw->pcapHeader.ts_sec = ti.tv_sec;
        this->raw->pcapHeader.ts_usec = ti.tv_nsec/1000;
        this->raw->pcapHeader.incl_len = len;
        this->raw->pcapHeader.orig_len = len;
        memcpy(this->raw->payload, raw, len);
        protocol = protocolFinder(this->raw);
    }

    ~PcapPacket() {
        free(raw);
    }

    ProtocolEnum protocol;
    PcapRaw* raw;
};

#endif // PCAPPACKET_H
