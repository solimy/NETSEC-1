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
        switch (packet->protocol) {
            case ProtocolEnum::UNKNOWN:
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
        default:
            break;
        }
        EthernetRaw* ethRaw = (EthernetRaw*)packet->raw;
        ethhdr ethHeader = ((EthernetRaw*)ethRaw)->ehternetHeader;
        ARPRaw* arpRaw;
        _arp_hdr arpHeader;
        char formated[100];
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("~Ethernet Header~");
        plainTextEdit->appendPlainText(ss.str().c_str());
        ss.str("");
        ss << "ethHeader.h_proto=" << std::hex << ntohs(ethHeader.h_proto);
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
        switch (packet->protocol) {
        case ProtocolEnum::ETHERNET:
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
        case ProtocolEnum::ARP:
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
            //arpRequest.arp_dev;
            return;
        default:
            break;
        }
    }

private:
    Ui::PacketDetails *ui;
    QPlainTextEdit* plainTextEdit;
};

#endif // PACKETDETAILS_H
