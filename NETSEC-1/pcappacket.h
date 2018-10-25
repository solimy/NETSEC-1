#ifndef PCAPPACKET_H
#define PCAPPACKET_H

#include <type_traits>
#include <cstdint>

#include "protocolenum.h"

typedef struct pcaprec_hdr_s {
    uint32_t ts_sec;         /* timestamp seconds */
    uint32_t ts_usec;        /* timestamp microseconds */
    uint32_t incl_len;       /* number of octets of packet saved in file */
    uint32_t orig_len;       /* actual length of packet */
} pcaprec_hdr_t;

class PcapPacket {
public:
    struct Data {
        pcaprec_hdr_t pcap_header;
        uint8_t raw[0];
    } __attribute__((packed));

    class Factory {
    public:
        static PcapPacket* buildEmpty(ProtocolEnum protocol);
        static PcapPacket* buildCopy(PcapPacket* toCopy);
        static PcapPacket* buildFromPcapBuffer(void* buffer);
        static PcapPacket* buildFromRawBuffer(void* buffer, unsigned long length);
    };
    friend Factory;

    virtual ~PcapPacket();


    uint32_t get_ts_sec() const;
    uint32_t get_ts_usec() const;
    uint32_t get_incl_len() const;
    uint32_t get_orig_len() const;
    void set_ts_sec(uint32_t ts_sec);
    void set_ts_usec(uint32_t ts_usec);
    void set_incl_len(uint32_t incl_len);
    void set_orig_len(uint32_t orig_len);

    ProtocolEnum getProtocol() const;
    const void* getBuffer_withPcapHeader() const;
    const void* getBuffer_withoutPcapHeader() const;
    unsigned long getSize_withPcapHeader() const;
    unsigned long getSize_withoutPcapHeader() const;

protected:
    PcapPacket(ProtocolEnum protocol, void* data);

private:
    const ProtocolEnum protocol;
    void* rawData;
    Data* data;
};

#endif // PCAPPACKET_H
