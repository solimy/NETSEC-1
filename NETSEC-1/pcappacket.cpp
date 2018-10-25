#include "pcappacket.h"

#include <cstring>
#include <ctime>
#include <chrono>
#include <net/ethernet.h>

PcapPacket::PcapPacket(ProtocolEnum protocol, void* data)
    :
      protocol(protocol),
      rawData(data),
      data((Data*)data)
{
}


PcapPacket::~PcapPacket() {
    delete data;
}

uint32_t PcapPacket::get_ts_sec() const {
    return data->pcap_header.ts_sec;
}

uint32_t PcapPacket::get_ts_usec() const {
    return data->pcap_header.ts_usec;
}

uint32_t PcapPacket::get_incl_len() const {
    return data->pcap_header.incl_len;
}

uint32_t PcapPacket::get_orig_len() const {
    return data->pcap_header.ts_usec;
}

void PcapPacket::set_ts_sec(uint32_t ts_sec) {
    data->pcap_header.ts_sec = ts_sec;
}

void PcapPacket::set_ts_usec(uint32_t ts_usec) {
    data->pcap_header.ts_usec = ts_usec;
}

void PcapPacket::set_incl_len(uint32_t incl_len) {
    data->pcap_header.incl_len = incl_len;
}

void PcapPacket::set_orig_len(uint32_t orig_len) {
    data->pcap_header.orig_len = orig_len;
}

ProtocolEnum PcapPacket::getProtocol() const {
    return protocol;
}

const void* PcapPacket::getBuffer_withPcapHeader() const {
    return data;
}

const void* PcapPacket::getBuffer_withoutPcapHeader() const {
    return data->raw;
}

unsigned long PcapPacket::getSize_withPcapHeader() const {
    return sizeof (pcaprec_hdr_t) + get_incl_len();
}

unsigned long PcapPacket::getSize_withoutPcapHeader() const {
    return get_incl_len();
}

PcapPacket* PcapPacket::Factory::buildEmpty(ProtocolEnum protocol) {
    PcapPacket* packet = nullptr;
    switch (protocol) {
    case ProtocolEnum::ETHERNET:
    case ProtocolEnum::ARP:
    case ProtocolEnum::ICMP:
    case ProtocolEnum::IP:
    case ProtocolEnum::TCP:
    case ProtocolEnum::UDP:
    case ProtocolEnum::HTTP:
    case ProtocolEnum::DNS:
    case ProtocolEnum::UNKNOWN__RAW:
        packet = new PcapPacket(ProtocolEnum::UNKNOWN__RAW, new Data());
        break;
    }
    return packet;
}

PcapPacket* PcapPacket::Factory::buildCopy(PcapPacket* toCopy) {
    void* data = operator new(toCopy->getSize_withPcapHeader());
    std::memcpy(data, toCopy->getBuffer_withPcapHeader(), toCopy->getSize_withPcapHeader());
    PcapPacket* packet = new PcapPacket(toCopy->getProtocol(), data);
    return packet;
}

PcapPacket* PcapPacket::Factory::buildFromPcapBuffer(void* buffer) {
    PcapPacket* packet = nullptr;
    pcaprec_hdr_t* pcapHeader = (pcaprec_hdr_t*)buffer;

    if (pcapHeader->incl_len >= sizeof (pcaprec_hdr_t) + sizeof (ethhdr)) {
        packet = new PcapPacket(ProtocolEnum::ETHERNET, buffer);
    } else {
        packet = new PcapPacket(ProtocolEnum::UNKNOWN__RAW, buffer);
    }
    return packet;
}

PcapPacket* PcapPacket::Factory::buildFromRawBuffer(void* buffer, unsigned long length) {
    void* rawdata = operator new(sizeof (pcaprec_hdr_t) + length);
    pcaprec_hdr_t* data = (pcaprec_hdr_t*)rawdata;
    data->incl_len = length;
    data->orig_len = length;
    data->ts_sec = std::time(nullptr);
    data->ts_usec = std::chrono::microseconds(std::time(nullptr)).count();
    return buildFromPcapBuffer(data);
}
