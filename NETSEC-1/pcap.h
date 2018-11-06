#ifndef PCAP_H
#define PCAP_H

#define PCAP_VERSION_MAJOR 2
#define PCAP_VERSION_MINOR 4
#define TCPDUMP_MAGIC 0xa1b2c3d4
#define LINKTYPE_ETHERNET 1

#include <stdint.h>

struct pcap_pkthdr {
    struct timeval ts;	/* time stamp */
    int32_t caplen;	/* length of portion present */
    int32_t len;	/* length this packet (off wire) */
};

struct pcap_file_header {
    int32_t magic;
    unsigned short version_major;
    unsigned short version_minor;
    int32_t thiszone;	/* gmt to local correction; this is always 0 */
    int32_t sigfigs;	/* accuracy of timestamps; this is always 0 */
    int32_t snaplen;	/* max length saved portion of each pkt */
    int32_t linktype;	/* data link type (LINKTYPE_*) */
};

#endif // PCAP_H
