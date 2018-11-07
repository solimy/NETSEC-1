#ifndef PACKETDETAILS_H
#define PACKETDETAILS_H

#include <QDialog>
#include <QPlainTextEdit>
#include <memory>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pcappacket.h"

namespace Ui {
class PacketDetails;
}

class PacketDetails : public QDialog
{
    Q_OBJECT

    void hexdumpToSStream(std::stringstream& ss, uint8_t* raw, uint64_t rawLength) {
        ss << std::hex;
        for (int i = 0, j = rawLength; i < j; ++i) {
            if (std::isprint(raw[i])) {
                ss << "<font color=\"red\">";
                ss << raw[i];
                ss << "</font>";
            } else {
                ss << " Ox";
                ss << (short)raw[i];
                ss << " ";
            }
        }
    }

public:
    explicit PacketDetails(QWidget *parent = nullptr);
    ~PacketDetails();
    void setPacket(std::shared_ptr<PcapPacket> packet) {
        if (packet.get() == nullptr)
            return;
        std::stringstream ss;
        ss << "Protocol : ";
        switch (packet->protocol) {
        case ProtocolEnum::UNKNOWN:
            ss << "UNKNOWN";
            break;
        case ProtocolEnum::ETHERNET:
            ss << "ETHERNET";
            break;
        case ProtocolEnum::ARP:
            ss << "ARP";
            break;
        case ProtocolEnum::IP:
            ss << "IP";
            break;
        case ProtocolEnum::ICMP:
            ss << "ICMP";
            break;
        case ProtocolEnum::TCP:
            ss << "TCP";
            break;
        case ProtocolEnum::UDP:
            ss << "UDP";
            break;
        case ProtocolEnum::HTTP:
            ss << "HTTP";
            break;
        case ProtocolEnum::DNS:
            ss << "DNS";
            break;
        }
        ss << "<br/>";
        PcapRaw* pcapRaw = (PcapRaw*)packet->raw;
        pcaprec_hdr_t pcapHeader = pcapRaw->pcapHeader;
        ss << "<br/>" << "~Pcap Header~" << "<br/>";
        ss << "pcapHeader.ts_sec=" << pcapHeader.ts_sec << "<br/>";
        ss << "pcapHeader.ts_usec=" << pcapHeader.ts_usec << "<br/>";
        ss << "pcapHeader.incl_len=" << pcapHeader.incl_len << "<br/>";
        ss << "pcapHeader.orig_len=" << pcapHeader.orig_len << "<br/>";
        if (packet->protocol == ProtocolEnum::UNKNOWN) {
            ss << "<br/>" << "~Payload~" << "<br/>";
            hexdumpToSStream(ss, pcapRaw->pcapPayload, packet->raw->pcapHeader.incl_len);
            plainTextEdit->setHtml(ss.str().c_str());
            return;
        }
        EthernetRaw* ethRaw = (EthernetRaw*)packet->raw;
        ethhdr ethHeader = ((EthernetRaw*)ethRaw)->ehternetHeader;
        ss << "<br/>" << "~Ethernet Header~" << "<br/>";
        ss << "ethHeader.h_proto=0x" << std::hex << ntohs(ethHeader.h_proto) << "<br/>";
        char mac[100];
        sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                ethHeader.h_source[0],
                ethHeader.h_source[1],
                ethHeader.h_source[2],
                ethHeader.h_source[3],
                ethHeader.h_source[4],
                ethHeader.h_source[5]);
        ss << "ethHeader.h_source=" << mac << "<br/>";
        sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                ethHeader.h_dest[0],
                ethHeader.h_dest[1],
                ethHeader.h_dest[2],
                ethHeader.h_dest[3],
                ethHeader.h_dest[4],
                ethHeader.h_dest[5]);
        ss << "ethHeader.h_dest=" << mac << "<br/>";
        if (packet->protocol == ProtocolEnum::ETHERNET) {
            ss << "<br/>" << "~Payload~" << "<br/>";
            hexdumpToSStream(ss, ethRaw->ehternetPayload, sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (EthernetRaw));
            plainTextEdit->setHtml(ss.str().c_str());
            return;
        }
        if (packet->protocol == ProtocolEnum::ARP) {
            ARPRaw* arpRaw;
            _arp_hdr arpHeader;
            arpRaw = (ARPRaw*)packet->raw;
            arpHeader = arpRaw->arpHeader;
            ss << "<br/>" << "~ARP Header~" << "<br/>";
            ss << "arpHeader.htype=" << ntohs(arpHeader.htype) << "<br/>";
            ss << "arpHeader.ptype=" << ntohs(arpHeader.ptype) << "<br/>";
            ss << "arpHeader.hlen=" << (short)arpHeader.hlen << "<br/>";
            ss << "arpHeader.plen=" << (short)arpHeader.plen << "<br/>";
            ss << "arpHeader.opcode=" << ntohs(arpHeader.opcode) << "<br/>";
            ss << "<br/>" << "~ARP Request~" << "<br/>";
            sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                    arpHeader.sender_mac[0],
                    arpHeader.sender_mac[1],
                    arpHeader.sender_mac[2],
                    arpHeader.sender_mac[3],
                    arpHeader.sender_mac[4],
                    arpHeader.sender_mac[5]);
            ss << "arpHeader.sender_mac=" << mac << "<br/>";
            ss << "arpHeader.sender_ip=" << inet_ntoa(*(in_addr*)arpHeader.sender_ip) << "<br/>";
            sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                    arpHeader.target_mac[0],
                    arpHeader.target_mac[1],
                    arpHeader.target_mac[2],
                    arpHeader.target_mac[3],
                    arpHeader.target_mac[4],
                    arpHeader.target_mac[5]);
            ss << "arpHeader.target_mac=" << mac << "<br/>";
            ss << "arpHeader.target_ip=" << inet_ntoa(*(in_addr*)arpHeader.target_ip) << "<br/>";
            plainTextEdit->setHtml(ss.str().c_str());
            return;
        }
        IPRaw* ipRaw = (IPRaw*)packet->raw;
        iphdr ipHeader = ipRaw->ipHeader;
        ss << "<br/>" << "~IP Header~" << "<br/>";
        ss << "ipHeader.ihl=" << ntohs(ipHeader.ihl) << "<br/>";
        ss << "ipHeader.version=" << ntohs(ipHeader.version) << "<br/>";
        ss << "ipHeader.tos=" << (short)ipHeader.tos << "<br/>";
        ss << "ipHeader.tot_len=" << ntohs(ipHeader.tot_len) << "<br/>";
        ss << "ipHeader.id=" << ntohs(ipHeader.id) << "<br/>";
        ss << "ipHeader.frag_off=" << ntohs(ipHeader.frag_off) << "<br/>";
        ss << "ipHeader.ttl=" << (short)ipHeader.ttl << "<br/>";
        ss << "ipHeader.protocol=0x" << std::hex << (short)ipHeader.protocol << std::dec << "<br/>";
        ss << "ipHeader.check=" << ntohs(ipHeader.check) << "<br/>";
        ss << "ipHeader.saddr=" << inet_ntoa(*(in_addr*)&ipHeader.saddr) << "<br/>";
        ss << "ipHeader.daddr=" << inet_ntoa(*(in_addr*)&ipHeader.daddr) << "<br/>";
        if (packet->protocol == ProtocolEnum::IP) {
            ss << "<br/>" << "~Payload~" << "<br/>";
            hexdumpToSStream(ss, ipRaw->ipPayload, sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (IPRaw));
            plainTextEdit->setHtml(ss.str().c_str());
            return;
        }
        if (packet->protocol == ProtocolEnum::ICMP) {
            ICMPRaw* icmpRaw = (ICMPRaw*)packet->raw;
            icmphdr icmpHeader = icmpRaw->icmpHeader;
            ss << "<br/>" << "~ICMP Header~" << "<br/>";
            ss << "icmpHeader.type=" << (short)icmpHeader.type << "<br/>";
            ss << "icmpHeader.code=" << (short)icmpHeader.code << "<br/>";
            ss << "icmpHeader.checksum=" << ntohs(icmpHeader.checksum) << "<br/>";
            ss << "icmpHeader.un.echo.id=" << ntohs(icmpHeader.un.echo.id) << "<br/>";
            ss << "icmpHeader.un.echo.sequence=" << ntohs(icmpHeader.un.echo.sequence) << "<br/>";
            ss << "icmpHeader.un.gateway=" << inet_ntoa(*(in_addr*)&icmpHeader.un.gateway) << "<br/>";
            ss << "icmpHeader.un.frag.mtu=" << ntohs(icmpHeader.un.frag.mtu) << "<br/>";
            ss << "<br/>" << "~Payload~" << "<br/>";
            hexdumpToSStream(ss, icmpRaw->icmpPayload, sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (ICMPRaw));
            plainTextEdit->setHtml(ss.str().c_str());
            return;
        } else if (packet->protocol == ProtocolEnum::UDP ||
                   packet->protocol == ProtocolEnum::DNS) {
            UDPRaw* udpRaw = (UDPRaw*)packet->raw;
            _udphdr udpHeader = udpRaw->udpHeader;
            ss << "<br/>" << "~UDP Header~" << "<br/>";
            ss << "udpHeader.source=" << ntohs(udpHeader.source) << "<br/>";
            ss << "udpHeader.dest=" << ntohs(udpHeader.dest) << "<br/>";
            ss << "udpHeader.len=" << ntohs(udpHeader.len) << "<br/>";
            ss << "udpHeader.check=" << ntohs(udpHeader.check) << "<br/>";
            ss << "<br/>" << "~Payload~" << "<br/>";
            hexdumpToSStream(ss, udpRaw->udpPayload, sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (UDPRaw));
            plainTextEdit->setHtml(ss.str().c_str());
            return;
        } else if (packet->protocol == ProtocolEnum::TCP ||
                   packet->protocol == ProtocolEnum::HTTP){
            TCPRaw* tcpRaw = (TCPRaw*)packet->raw;
            tcphdr tcpHeader = tcpRaw->tcpHeader;
            ss << "<br/>" << "~TCP Header~" << "<br/>";
            ss << "tcpHeader.source=" << ntohs(tcpHeader.source) << "<br/>";
            ss << "tcpHeader.dest=" << ntohs(tcpHeader.dest) << "<br/>";
            ss << "tcpHeader.seq=" << ntohs(tcpHeader.seq) << "<br/>";
            ss << "tcpHeader.ack_seq=" << ntohs(tcpHeader.ack_seq) << "<br/>";
            ss << "tcpHeader.res1=" << ntohs(tcpHeader.res1) << "<br/>";
            ss << "tcpHeader.doff=" << ntohs(tcpHeader.doff) << "<br/>";
            ss << "tcpHeader.fin=" << ntohs(tcpHeader.fin) << "<br/>";
            ss << "tcpHeader.syn=" << ntohs(tcpHeader.syn) << "<br/>";
            ss << "tcpHeader.rst=" << ntohs(tcpHeader.rst) << "<br/>";
            ss << "tcpHeader.psh=" << ntohs(tcpHeader.psh) << "<br/>";
            ss << "tcpHeader.ack=" << ntohs(tcpHeader.ack) << "<br/>";
            ss << "tcpHeader.urg=" << ntohs(tcpHeader.urg) << "<br/>";
            ss << "tcpHeader.res2=" << ntohs(tcpHeader.res2) << "<br/>";
            ss << "tcpHeader.window=" << ntohs(tcpHeader.window) << "<br/>";
            ss << "tcpHeader.check=" << ntohs(tcpHeader.check) << "<br/>";
            ss << "tcpHeader.urg_ptr=" << ntohs(tcpHeader.urg_ptr) << "<br/>";
            ss << "<br/>" << "~Payload~" << "<br/>";
            hexdumpToSStream(ss, tcpRaw->tcpPayload, sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (TCPRaw));
            plainTextEdit->setHtml(ss.str().c_str());
            return;
        }
    }

private:
    Ui::PacketDetails *ui;
    QTextEdit* plainTextEdit;
};

#endif // PACKETDETAILS_H
