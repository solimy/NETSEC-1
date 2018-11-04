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
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        PcapRaw* pcapRaw = (PcapRaw*)packet->raw;
        pcaprec_hdr_t pcapHeader = pcapRaw->pcapHeader;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("~Pcap Header~");
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "pcapHeader.ts_sec=" << pcapHeader.ts_sec;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "pcapHeader.ts_usec=" << pcapHeader.ts_usec;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "pcapHeader.incl_len=" << pcapHeader.incl_len;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "pcapHeader.orig_len=" << pcapHeader.orig_len;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        if (packet->protocol == ProtocolEnum::UNKNOWN) {
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~Payload~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
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
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("~Ethernet Header~");
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ethHeader.h_proto=0x" << std::hex << ntohs(ethHeader.h_proto);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        char mac[100];
        sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                ethHeader.h_source[0],
                ethHeader.h_source[1],
                ethHeader.h_source[2],
                ethHeader.h_source[3],
                ethHeader.h_source[4],
                ethHeader.h_source[5]);
        ss << "ethHeader.h_source=" << mac;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                ethHeader.h_dest[0],
                ethHeader.h_dest[1],
                ethHeader.h_dest[2],
                ethHeader.h_dest[3],
                ethHeader.h_dest[4],
                ethHeader.h_dest[5]);
        ss << "ethHeader.h_dest=" << mac;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        if (packet->protocol == ProtocolEnum::ETHERNET) {
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~Payload~");
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
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~ARP Header~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "arpHeader.htype=" << ntohs(arpHeader.htype);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "arpHeader.ptype=" << ntohs(arpHeader.ptype);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "arpHeader.hlen=" << (short)arpHeader.hlen;
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "arpHeader.plen=" << (short)arpHeader.plen;
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "arpHeader.opcode=" << ntohs(arpHeader.opcode);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~ARP Request~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                    arpHeader.sender_mac[0],
                    arpHeader.sender_mac[1],
                    arpHeader.sender_mac[2],
                    arpHeader.sender_mac[3],
                    arpHeader.sender_mac[4],
                    arpHeader.sender_mac[5]);
            ss << "arpHeader.sender_mac=" << mac;
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "arpHeader.sender_ip=" << inet_ntoa(*(in_addr*)arpHeader.sender_ip);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            sprintf(mac, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                    arpHeader.target_mac[0],
                    arpHeader.target_mac[1],
                    arpHeader.target_mac[2],
                    arpHeader.target_mac[3],
                    arpHeader.target_mac[4],
                    arpHeader.target_mac[5]);
            ss << "arpHeader.target_mac=" << mac;
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "arpHeader.sender_ip=" << inet_ntoa(*(in_addr*)arpHeader.target_ip);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            return;
        }
        IPRaw* ipRaw = (IPRaw*)packet->raw;
        iphdr ipHeader = ipRaw->ipHeader;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("~IP Header~");
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.ihl=" << ntohs(ipHeader.ihl);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.version=" << ntohs(ipHeader.version);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.tos=" << (short)ipHeader.tos;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.tot_len=" << ntohs(ipHeader.tot_len);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.id=" << ntohs(ipHeader.id);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.frag_off=" << ntohs(ipHeader.frag_off);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.ttl=" << (short)ipHeader.ttl;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.protocol=0x" << std::hex << (short)ipHeader.protocol << std::dec;
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.check=" << ntohs(ipHeader.check);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.saddr=" << inet_ntoa(*(in_addr*)&ipHeader.saddr);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ipHeader.daddr=" << inet_ntoa(*(in_addr*)&ipHeader.daddr);
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        if (packet->protocol == ProtocolEnum::IP) {
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~Payload~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
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
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~ICMP Header~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "icmpHeader.type=" << (short)icmpHeader.type;
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "icmpHeader.code=" << (short)icmpHeader.code;
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "icmpHeader.checksum=" << ntohs(icmpHeader.checksum);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "! WARNING : header not fully implemented !";
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            //TODO
            switch (icmpHeader.type) {
            default:
                break;
            }
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~Payload~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
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
            udphdr udpHeader = udpRaw->udpHeader;
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~UDP Header~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "udpHeader.source=" << ntohs(udpHeader.source);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "udpHeader.dest=" << ntohs(udpHeader.dest);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "udpHeader.len=" << ntohs(udpHeader.len);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "udpHeader.check=" << ntohs(udpHeader.check);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~Payload~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
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
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~TCP Header~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.source=" << ntohs(tcpHeader.source);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.dest=" << ntohs(tcpHeader.dest);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.seq=" << ntohs(tcpHeader.seq);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.ack_seq=" << ntohs(tcpHeader.ack_seq);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.res1=" << ntohs(tcpHeader.res1);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.doff=" << ntohs(tcpHeader.doff);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.fin=" << ntohs(tcpHeader.fin);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.syn=" << ntohs(tcpHeader.syn);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.rst=" << ntohs(tcpHeader.rst);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.psh=" << ntohs(tcpHeader.psh);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.ack=" << ntohs(tcpHeader.ack);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.urg=" << ntohs(tcpHeader.urg);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.res2=" << ntohs(tcpHeader.res2);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.window=" << ntohs(tcpHeader.window);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.check=" << ntohs(tcpHeader.check);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            ss << "tcpHeader.urg_ptr=" << ntohs(tcpHeader.urg_ptr);
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("~Payload~");
            plainTextEdit->appendPlainText(ss.str().c_str());
            ss.str("");
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
