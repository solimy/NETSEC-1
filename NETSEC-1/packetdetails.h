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
        ss << "\n";
        PcapRaw* pcapRaw = (PcapRaw*)packet->raw;
        pcaprec_hdr_t pcapHeader = pcapRaw->pcapHeader;
        ss << "\n" << "~Pcap Header~" << "\n";
        ss << "pcapHeader.ts_sec=" << pcapHeader.ts_sec << "\n";
        ss << "pcapHeader.ts_usec=" << pcapHeader.ts_usec << "\n";
        ss << "pcapHeader.incl_len=" << pcapHeader.incl_len << "\n";
        ss << "pcapHeader.orig_len=" << pcapHeader.orig_len << "\n";
        if (packet->protocol == ProtocolEnum::UNKNOWN) {
            ss << "\n" << "~Payload~" << "\n";
            ss << std::hex;
            for (int i = 0, j = packet->raw->pcapHeader.incl_len; i < j; ++i) {
                if (std::isprint(pcapRaw->pcapPayload[i]))
                    ss << pcapRaw->pcapPayload[i] << " ";
                else
                    ss << "Ox" << (short)pcapRaw->pcapPayload[i] << " ";
            }
            plainTextEdit->appendPlainText(ss.str().c_str());
            return;
        }
        EthernetRaw* ethRaw = (EthernetRaw*)packet->raw;
        ethhdr ethHeader = ((EthernetRaw*)ethRaw)->ehternetHeader;
        ss << "\n" << "~Ethernet Header~" << "\n";
        ss << "ethHeader.h_proto=0x" << std::hex << ntohs(ethHeader.h_proto) << "\n";
        char mac[100];
        sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                ethHeader.h_source[0],
                ethHeader.h_source[1],
                ethHeader.h_source[2],
                ethHeader.h_source[3],
                ethHeader.h_source[4],
                ethHeader.h_source[5]);
        ss << "ethHeader.h_source=" << mac << "\n";
        sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                ethHeader.h_dest[0],
                ethHeader.h_dest[1],
                ethHeader.h_dest[2],
                ethHeader.h_dest[3],
                ethHeader.h_dest[4],
                ethHeader.h_dest[5]);
        ss << "ethHeader.h_dest=" << mac << "\n";
        if (packet->protocol == ProtocolEnum::ETHERNET) {
            ss << "\n" << "~Payload~" << "\n";
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << std::hex;
            for (int i = 0, j = sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (EthernetRaw); i < j; ++i) {
                if (std::isprint(ethRaw->ehternetPayload[i]))
                    ss << ethRaw->ehternetPayload[i] << " ";
                else
                    ss << "Ox" << (short)ethRaw->ehternetPayload[i] << " ";
            }
            plainTextEdit->appendPlainText(ss.str().c_str());
            return;
        }
        if (packet->protocol == ProtocolEnum::ARP) {
            ARPRaw* arpRaw;
            _arp_hdr arpHeader;
            arpRaw = (ARPRaw*)packet->raw;
            arpHeader = arpRaw->arpHeader;
            ss << "\n" << "~ARP Header~" << "\n";
            ss << "arpHeader.htype=" << ntohs(arpHeader.htype) << "\n";
            ss << "arpHeader.ptype=" << ntohs(arpHeader.ptype) << "\n";
            ss << "arpHeader.hlen=" << (short)arpHeader.hlen << "\n";
            ss << "arpHeader.plen=" << (short)arpHeader.plen << "\n";
            ss << "arpHeader.opcode=" << ntohs(arpHeader.opcode) << "\n";
            ss << "\n" << "~ARP Request~" << "\n";
            sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                    arpHeader.sender_mac[0],
                    arpHeader.sender_mac[1],
                    arpHeader.sender_mac[2],
                    arpHeader.sender_mac[3],
                    arpHeader.sender_mac[4],
                    arpHeader.sender_mac[5]);
            ss << "arpHeader.sender_mac=" << mac << "\n";
            ss << "arpHeader.sender_ip=" << inet_ntoa(*(in_addr*)arpHeader.sender_ip) << "\n";
            sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                    arpHeader.target_mac[0],
                    arpHeader.target_mac[1],
                    arpHeader.target_mac[2],
                    arpHeader.target_mac[3],
                    arpHeader.target_mac[4],
                    arpHeader.target_mac[5]);
            ss << "arpHeader.target_mac=" << mac << "\n";
            ss << "arpHeader.target_ip=" << inet_ntoa(*(in_addr*)arpHeader.target_ip) << "\n";
            plainTextEdit->appendPlainText(ss.str().c_str());
            return;
        }
        IPRaw* ipRaw = (IPRaw*)packet->raw;
        iphdr ipHeader = ipRaw->ipHeader;
        ss << "\n" << "~IP Header~" << "\n";
        ss << "ipHeader.ihl=" << ntohs(ipHeader.ihl) << "\n";
        ss << "ipHeader.version=" << ntohs(ipHeader.version) << "\n";
        ss << "ipHeader.tos=" << (short)ipHeader.tos << "\n";
        ss << "ipHeader.tot_len=" << ntohs(ipHeader.tot_len) << "\n";
        ss << "ipHeader.id=" << ntohs(ipHeader.id) << "\n";
        ss << "ipHeader.frag_off=" << ntohs(ipHeader.frag_off) << "\n";
        ss << "ipHeader.ttl=" << (short)ipHeader.ttl << "\n";
        ss << "ipHeader.protocol=0x" << std::hex << (short)ipHeader.protocol << std::dec << "\n";
        ss << "ipHeader.check=" << ntohs(ipHeader.check) << "\n";
        ss << "ipHeader.saddr=" << inet_ntoa(*(in_addr*)&ipHeader.saddr) << "\n";
        ss << "ipHeader.daddr=" << inet_ntoa(*(in_addr*)&ipHeader.daddr) << "\n";
        if (packet->protocol == ProtocolEnum::IP) {
            ss << "\n" << "~Payload~" << "\n";
            ss << std::hex;
            for (int i = 0, j = sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (IPRaw); i < j; ++i) {
                if (std::isprint(ipRaw->ipPayload[i]))
                    ss << ipRaw->ipPayload[i] << " ";
                else
                    ss << "Ox" << (short)ipRaw->ipPayload[i] << " ";
            }
            plainTextEdit->appendPlainText(ss.str().c_str());
            return;
        }
        if (packet->protocol == ProtocolEnum::ICMP) {
            ICMPRaw* icmpRaw = (ICMPRaw*)packet->raw;
            icmphdr icmpHeader = icmpRaw->icmpHeader;
            ss << "\n" << "~ICMP Header~" << "\n";
            ss << "icmpHeader.type=" << (short)icmpHeader.type << "\n";
            ss << "icmpHeader.code=" << (short)icmpHeader.code << "\n";
            ss << "icmpHeader.checksum=" << ntohs(icmpHeader.checksum) << "\n";
            ss << "icmpHeader.un.echo.id=" << ntohs(icmpHeader.un.echo.id) << "\n";
            ss << "icmpHeader.un.echo.sequence=" << ntohs(icmpHeader.un.echo.sequence) << "\n";
            ss << "icmpHeader.un.gateway=" << inet_ntoa(*(in_addr*)&icmpHeader.un.gateway) << "\n";
            ss << "icmpHeader.un.frag.mtu=" << ntohs(icmpHeader.un.frag.mtu) << "\n";
            ss << "\n" << "~Payload~" << "\n";
            ss << std::hex;
            for (int i = 0, j = sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (ICMPRaw); i < j; ++i) {
                if (std::isprint(icmpRaw->icmpPayload[i]))
                    ss << icmpRaw->icmpPayload[i] << " ";
                else
                    ss << "Ox" << (short)icmpRaw->icmpPayload[i] << " ";
            }
            plainTextEdit->appendPlainText(ss.str().c_str());
            return;
        } else if (packet->protocol == ProtocolEnum::UDP ||
                   packet->protocol == ProtocolEnum::DNS) {
            UDPRaw* udpRaw = (UDPRaw*)packet->raw;
            _udphdr udpHeader = udpRaw->udpHeader;
            ss << "\n" << "~UDP Header~" << "\n";
            ss << "udpHeader.source=" << ntohs(udpHeader.source) << "\n";
            ss << "udpHeader.dest=" << ntohs(udpHeader.dest) << "\n";
            ss << "udpHeader.len=" << ntohs(udpHeader.len) << "\n";
            ss << "udpHeader.check=" << ntohs(udpHeader.check) << "\n";
            ss << "\n" << "~Payload~" << "\n";
            ss << std::hex;
            for (int i = 0, j = sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (UDPRaw); i < j; ++i) {
                if (std::isprint(udpRaw->udpPayload[i]))
                    ss << udpRaw->udpPayload[i] << " ";
                else
                    ss << "Ox" << (short)udpRaw->udpPayload[i] << " ";
            }
            plainTextEdit->appendPlainText(ss.str().c_str());
            return;
        } else if (packet->protocol == ProtocolEnum::TCP ||
                   packet->protocol == ProtocolEnum::HTTP){
            TCPRaw* tcpRaw = (TCPRaw*)packet->raw;
            tcphdr tcpHeader = tcpRaw->tcpHeader;
            ss << "\n" << "~TCP Header~" << "\n";
            ss << "tcpHeader.source=" << ntohs(tcpHeader.source) << "\n";
            ss << "tcpHeader.dest=" << ntohs(tcpHeader.dest) << "\n";
            ss << "tcpHeader.seq=" << ntohs(tcpHeader.seq) << "\n";
            ss << "tcpHeader.ack_seq=" << ntohs(tcpHeader.ack_seq) << "\n";
            ss << "tcpHeader.res1=" << ntohs(tcpHeader.res1) << "\n";
            ss << "tcpHeader.doff=" << ntohs(tcpHeader.doff) << "\n";
            ss << "tcpHeader.fin=" << ntohs(tcpHeader.fin) << "\n";
            ss << "tcpHeader.syn=" << ntohs(tcpHeader.syn) << "\n";
            ss << "tcpHeader.rst=" << ntohs(tcpHeader.rst) << "\n";
            ss << "tcpHeader.psh=" << ntohs(tcpHeader.psh) << "\n";
            ss << "tcpHeader.ack=" << ntohs(tcpHeader.ack) << "\n";
            ss << "tcpHeader.urg=" << ntohs(tcpHeader.urg) << "\n";
            ss << "tcpHeader.res2=" << ntohs(tcpHeader.res2) << "\n";
            ss << "tcpHeader.window=" << ntohs(tcpHeader.window) << "\n";
            ss << "tcpHeader.check=" << ntohs(tcpHeader.check) << "\n";
            ss << "tcpHeader.urg_ptr=" << ntohs(tcpHeader.urg_ptr) << "\n";
            ss << "\n" << "~Payload~" << "\n";
            ss << std::hex;
            for (int i = 0, j = sizeof(PcapRaw) + packet->raw->pcapHeader.incl_len - sizeof (TCPRaw); i < j; ++i) {
                if (std::isprint(tcpRaw->tcpPayload[i]))
                    ss << tcpRaw->tcpPayload[i] << " ";
                else
                    ss << "Ox" << (short)tcpRaw->tcpPayload[i] << " ";
            }
            plainTextEdit->appendPlainText(ss.str().c_str());
            return;
        }
    }

private:
    Ui::PacketDetails *ui;
    QPlainTextEdit* plainTextEdit;
};

#endif // PACKETDETAILS_H
