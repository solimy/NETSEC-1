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
#include <string>

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

struct _arp_hdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
} __attribute__((packed));

struct _udphdr
{
  u_int16_t source;
  u_int16_t dest;
  u_int16_t len;
  u_int16_t check;
}__attribute__((packed));

struct _tcp_header {
    u_int16_t src_port;		/* source port */
    u_int16_t dst_port;		/* destination port */
    tcp_seq seq;		/* sequence number */
    tcp_seq ack;		/* acknowledgement number */
# if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int8_t th_x2:4;		/* (unused) */
    u_int8_t data_offset:4;		/* data offset */
# endif
# if __BYTE_ORDER == __BIG_ENDIAN
    u_int8_t data_offset:4;		/* data offset */
    u_int8_t th_x2:4;		/* (unused) */
# endif
    u_int8_t flags;
    u_int16_t window_size;		/* window */
    u_int16_t checksum;		/* checksum */
    u_int16_t urgent_p;		/* urgent pointer */
} __attribute__((packed));

//UNKNOWN
struct PcapRaw {
    pcaprec_hdr_t pcapHeader;
    uint8_t pcapPayload[0];
} __attribute__((packed));

//ETHERNET
struct EthernetRaw : public PcapRaw {
    ethhdr ehternetHeader;
    uint8_t ehternetPayload[0];
} __attribute__((packed));

//IP
struct IPRaw : public EthernetRaw {
    iphdr ipHeader;
    uint8_t ipPayload[0];
} __attribute__((packed));

//ARP
struct ARPRaw : public EthernetRaw {
    _arp_hdr arpHeader;
    uint8_t payload[0];
} __attribute__((packed));

//ICMP
struct ICMPRaw : public IPRaw {
    icmphdr icmpHeader;
    uint8_t icmpPayload[0];
} __attribute__((packed));

//TCP
struct TCPRaw : public IPRaw {
    _tcp_header tcpHeader;
    uint8_t tcpPayload[0];
} __attribute__((packed));

//UDP
struct UDPRaw : public IPRaw {
    _udphdr udpHeader;
    uint8_t udpPayload[0];
} __attribute__((packed));

//HTTP
struct HTTPRaw : public TCPRaw {
    char httpPayload[0];
} __attribute__((packed));

//DNS
struct DNSRaw : public UDPRaw {
    char dnsPayload[0];
} __attribute__((packed));

class PcapPacket {
public:
    static ProtocolEnum protocolFinder(TCPRaw* raw) {
        switch (ntohs(((UDPRaw*)raw)->udpHeader.dest)) {
        case 53 :
            return ProtocolEnum::DNS;
        case 80:
        case 443:
            return ProtocolEnum::HTTP;
        default:
            return ProtocolEnum::TCP;
        }
    }

    static ProtocolEnum protocolFinder(UDPRaw* raw) {
        switch (ntohs(raw->udpHeader.dest)) {
        case 53 :
            return ProtocolEnum::DNS;
        default:
            return ProtocolEnum::UDP;
        }
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
        switch (ntohs(raw->ehternetHeader.h_proto)) {
        case ETHERTYPE_IP:
            return protocolFinder((IPRaw*)raw);
        case ETHERTYPE_ARP:
            return ProtocolEnum::ARP;
        case ETHERTYPE_IPV6:
        default:
            return ProtocolEnum::ETHERNET;
        }
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
        memcpy(this->raw->pcapPayload, raw, len);
        protocol = protocolFinder(this->raw);
    }

    ~PcapPacket() {
        free(raw);
    }

    ProtocolEnum protocol;
    PcapRaw* raw;
    std::string interface;

    template<typename T>
    static T* forgeRaw(uint64_t paylodLen) {
        return (T*) malloc(sizeof (T) + paylodLen);
    }
};

#endif // PCAPPACKET_H
