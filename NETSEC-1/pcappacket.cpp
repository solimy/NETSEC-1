#include "pcappacket.h"

PcapPacket::PcapPacket(ProtocolEnum protocol)
    :
      protocol(protocol)
{
}


PcapPacket::~PcapPacket() {

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
