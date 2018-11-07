#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "packetreader.h"
#include "packetwriter.h"
#include "pcapfeeder.h"
#include "filter.h"
#include <deque>
#include <unordered_map>
#include <QTableWidget>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Ui {
class MainWindow;
}

class MainWindow :
        public QMainWindow,
        public PcapFeeder::PcapFeedable,
        public PcapFeeder
{
    Q_OBJECT


private:
    void pushTimestampSec(const std::shared_ptr<PcapPacket>& packet) {
        std::stringstream stringStream;
        auto item = packetTable->item(packetTable->rowCount()-1, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 1, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        stringStream.str(std::string());
        stringStream << packet->raw->pcapHeader.ts_sec << std::flush;
        item->setText(QString::fromStdString(stringStream.str()));
    }

    void pushTimestampUsec(const std::shared_ptr<PcapPacket>& packet) {
        std::stringstream stringStream;
        auto item = packetTable->item(packetTable->rowCount()-1, 2);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 2, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        stringStream.str(std::string());
        stringStream << packet->raw->pcapHeader.ts_usec << std::flush;
        item->setText(QString::fromStdString(stringStream.str()));
    }

    void pushSourceMac(const std::shared_ptr<PcapPacket>& packet) {
        char formated[100];
        EthernetRaw* r = (EthernetRaw*)packet->raw;
        sprintf(formated, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
                r->ehternetHeader.h_source[0],
                r->ehternetHeader.h_source[1],
                r->ehternetHeader.h_source[2],
                r->ehternetHeader.h_source[3],
                r->ehternetHeader.h_source[4],
                r->ehternetHeader.h_source[5]);
        auto item = packetTable->item(packetTable->rowCount()-1, 3);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 3, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        item->setText(formated);
    }

    void pushDestinationMac(const std::shared_ptr<PcapPacket>& packet) {
        char formated[100];
        EthernetRaw* r = (EthernetRaw*)packet->raw;
        sprintf(formated, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
                r->ehternetHeader.h_dest[0],
                r->ehternetHeader.h_dest[1],
                r->ehternetHeader.h_dest[2],
                r->ehternetHeader.h_dest[3],
                r->ehternetHeader.h_dest[4],
                r->ehternetHeader.h_dest[5]);
        auto item = packetTable->item(packetTable->rowCount()-1, 4);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 4, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        item->setText(formated);
    }

    void pushSourceIp(const std::shared_ptr<PcapPacket>& packet) {
        char formated[100];
        IPRaw* r = (IPRaw*)packet->raw;
        struct sockaddr_in source;
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = r->ipHeader.saddr;
        sprintf(formated, "%s\n",
                inet_ntoa(source.sin_addr));
        auto item = packetTable->item(packetTable->rowCount()-1, 5);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 5, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        item->setText(formated);
        /*
        unsigned short iphdrlen;
        sprintf(formated, "\t|-Version : %d\n",
                (unsigned int)r->ipHeader.version);
        sprintf(formated, "\t|-Internet Header Length : %d DWORDS or %d Bytes\n",
                (unsigned int)r->ipHeader.ihl,
                ((unsigned int)(r->ipHeader.ihl))*4);
        sprintf(formated, "\t|-Type Of Service : %d\n",
                (unsigned int)r->ipHeader.tos);
        sprintf(formated, "\t|-Total Length : %d Bytes\n",
                ntohs(r->ipHeader.tot_len));
        sprintf(formated, "\t|-Identification : %d\n",
                ntohs(r->ipHeader.id));
        sprintf(formated, "\t|-Time To Live : %d\n",
                (unsigned int)r->ipHeader.ttl);
        sprintf(formated, "\t|-Protocol : %d\n",
                (unsigned int)r->ipHeader.protocol);
        sprintf(formated, "\t|-Header Checksum : %d\n",
                ntohs(r->ipHeader.check));
                */
    }

    void pushSourcePort(const std::shared_ptr<PcapPacket>& packet) {
        char formated[100];
        UDPRaw* r = (UDPRaw*)packet->raw;
        sprintf(formated, "%d\n" , ntohs(r->udpHeader.source));
        auto item = packetTable->item(packetTable->rowCount()-1, 6);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 6, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        item->setText(formated);
    }

    void pushDestinationIp(const std::shared_ptr<PcapPacket>& packet) {
        char formated[100];
        IPRaw* r = (IPRaw*)packet->raw;
        struct sockaddr_in dest;
        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = r->ipHeader.daddr;
        sprintf(formated, "%s\n",
                inet_ntoa(dest.sin_addr));
        auto item = packetTable->item(packetTable->rowCount()-1, 7);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 7, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        item->setText(formated);
    }

    void pushDestinationPort(const std::shared_ptr<PcapPacket>& packet) {
        char formated[100];
        UDPRaw* r = (UDPRaw*)packet->raw;
        sprintf(formated, "%d\n" , ntohs(r->udpHeader.dest));
        auto item = packetTable->item(packetTable->rowCount()-1, 8);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 8, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        item->setText(formated);
    }

    void pushProtocol(const std::shared_ptr<PcapPacket>& packet) {
        auto item = packetTable->item(packetTable->rowCount()-1, 0);
        if(!item)
        {
            item = new QTableWidgetItem;
            packetTable->setItem(packetTable->rowCount()-1, 0, item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        switch (packet->protocol) {
        case ProtocolEnum::UNKNOWN:
            item->setText("UNKNOWN");
            break;
        case ProtocolEnum::ETHERNET:
            item->setText("ETHERNET");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            break;
        case ProtocolEnum::ARP:
            item->setText("ARP");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            break;
        case ProtocolEnum::IP:
            item->setText("IP");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            pushSourceIp(packet);
            pushDestinationIp(packet);
            break;
        case ProtocolEnum::ICMP:
            item->setText("ICMP");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            pushSourceIp(packet);
            pushDestinationIp(packet);
            break;
        case ProtocolEnum::TCP:
            item->setText("TCP");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            pushSourceIp(packet);
            pushSourcePort(packet);
            pushDestinationIp(packet);
            pushDestinationPort(packet);
            break;
        case ProtocolEnum::UDP:
            item->setText("UDP");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            pushSourceIp(packet);
            pushSourcePort(packet);
            pushDestinationIp(packet);
            pushDestinationPort(packet);
            break;
        case ProtocolEnum::HTTP:
            item->setText("HTTP");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            pushSourceIp(packet);
            pushSourcePort(packet);
            pushDestinationIp(packet);
            pushDestinationPort(packet);
            break;
        case ProtocolEnum::DNS:
            item->setText("DNS");
            pushSourceMac(packet);
            pushDestinationMac(packet);
            pushSourceIp(packet);
            pushSourcePort(packet);
            pushDestinationIp(packet);
            pushDestinationPort(packet);
            break;
        }
    }

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::unordered_map<int, std::shared_ptr<PcapPacket>> packets;
    QTableWidget* packetTable;
    QLineEdit* lineEdit;

    virtual void feed(const std::shared_ptr<PcapPacket> packet) {

        if (filter.filter(packet)) {
            if (packets.size()>20000) {
                packets.clear();
                while (packetTable->rowCount() > 0)
                    packetTable->removeRow(0);
            }
            packets.insert({packetTable->rowCount(), packet});
            packetTable->insertRow(packetTable->rowCount());
            pushProtocol(packet);
            pushTimestampSec(packet);
            pushTimestampUsec(packet);
            feedSubscribers(packet);
        }
    }

    PacketReader reader;
    PacketWriter fileWriter;
    Filter filter;

private slots:
    void on_toolButton_triggered(QAction *arg1);
    
    void on_toolButton_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_lineEdit_editingFinished();

    void on_radioButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
