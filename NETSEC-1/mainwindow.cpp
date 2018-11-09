#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "captureoptions.h"
#include "packetdetails.h"
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    subscribeToFeeder(&reader);
    packetTable = findChild<QTableWidget*>(QString("tableWidget"),  Qt::FindChildrenRecursively);
    lineEdit = findChild<QLineEdit*>(QString("lineEdit"),  Qt::FindChildrenRecursively);
    arpOp = findChild<QLineEdit*>(QString("lineEdit_2"),  Qt::FindChildrenRecursively);
    arpHwsrc = findChild<QLineEdit*>(QString("lineEdit_3"),  Qt::FindChildrenRecursively);
    arpHwdst = findChild<QLineEdit*>(QString("lineEdit_4"),  Qt::FindChildrenRecursively);
    arpPsrc = findChild<QLineEdit*>(QString("lineEdit_5"),  Qt::FindChildrenRecursively);
    arpPdst = findChild<QLineEdit*>(QString("lineEdit_6"),  Qt::FindChildrenRecursively);
    arpInterface = findChild<QLineEdit*>(QString("lineEdit_7"),  Qt::FindChildrenRecursively);
    udpInterface = findChild<QLineEdit*>(QString("lineEdit_8"),  Qt::FindChildrenRecursively);
    udpHwsrc = findChild<QLineEdit*>(QString("lineEdit_10"),  Qt::FindChildrenRecursively);
    udpHwdst = findChild<QLineEdit*>(QString("lineEdit_12"),  Qt::FindChildrenRecursively);
    udpIpsrc = findChild<QLineEdit*>(QString("lineEdit_11"),  Qt::FindChildrenRecursively);
    udpIpdst = findChild<QLineEdit*>(QString("lineEdit_9"),  Qt::FindChildrenRecursively);
    udpPortsrc = findChild<QLineEdit*>(QString("lineEdit_13"),  Qt::FindChildrenRecursively);
    udpPortdst = findChild<QLineEdit*>(QString("lineEdit_14"),  Qt::FindChildrenRecursively);
    udpPayload = findChild<QPlainTextEdit*>(QString("plainTextEdit"),  Qt::FindChildrenRecursively);
    int column = -1;
    //proto
    packetTable->setColumnWidth(++column, 120);
    //timstamp sec
    packetTable->setColumnWidth(++column, 120);
    //imestamp usec
    packetTable->setColumnWidth(++column, 120);
    //src mac
    packetTable->setColumnWidth(++column, 120);
    //dst mac
    packetTable->setColumnWidth(++column, 120);
    //src ip
    packetTable->setColumnWidth(++column, 120);
    //src port
    packetTable->setColumnWidth(++column, 120);
    //dst ip
    packetTable->setColumnWidth(++column, 120);
    //dst port
    packetTable->setColumnWidth(++column, 120);
    fileWriter.subscribeToFeeder(this);
    netWriter.startWriting();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_triggered(QAction *arg1)
{
}

void MainWindow::on_toolButton_clicked()
{
    CaptureOptions cOpt;
    cOpt.setReader(&reader);
    cOpt.exec();
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    PacketDetails pD;
    pD.setPacket(packets.at(row));
    pD.exec();
}

void MainWindow::on_lineEdit_editingFinished()
{
    filter.setFilters(lineEdit->text().toStdString());
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if (checked) {
        std::stringstream ss;
        ss << std::time(0);
        ss << ".cap";
        printf("filename: %s\n", ss.str().c_str());
        fileWriter.startWriting(ss.str());
    } else {
        fileWriter.stopWriting();
    }
}

unsigned char* ConverMacAddressStringIntoByte
(const char *pszMACAddress, unsigned char* pbyAddress)
{
    for (int iConunter = 0; iConunter < 6; ++iConunter)
    {
        unsigned int iNumber = 0;
        char ch;

        ch = tolower (*pszMACAddress++);

        if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
        {
            return NULL;
        }

        iNumber = isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);
        ch = tolower (*pszMACAddress);

        if ((iConunter < 5 && ch != '-' && ch != ':') ||
                (iConunter == 5 && ch != '\0' && !isspace (ch)))
        {
            ++pszMACAddress;

            if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
            {
                return NULL;
            }

            iNumber <<= 4;
            iNumber += isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);
            ch = *pszMACAddress;

            if (iConunter < 5 && ch != '-' && ch != ':')
            {
                return NULL;
            }
        }
        pbyAddress[iConunter] = (unsigned char) iNumber;
        ++pszMACAddress;
    }
    return pbyAddress;
}

void MainWindow::on_pushButton_clicked()
{
    std::shared_ptr<PcapPacket> arpPacket = std::make_shared<PcapPacket>(new ARPRaw());
    arpPacket->raw->pcapHeader.incl_len = sizeof (ARPRaw) - sizeof (PcapRaw);
    arpPacket->interface = arpInterface->text().toStdString();

    ARPRaw* arpRaw = (ARPRaw*)arpPacket->raw;
    arpRaw->ehternetHeader.h_proto = htons(ETHERTYPE_ARP);
    arpRaw->arpHeader.hlen = 6;
    arpRaw->arpHeader.plen = 4;
    arpRaw->arpHeader.htype = htons(1);
    arpRaw->arpHeader.ptype = htons(ETHERTYPE_IP);

    arpRaw->arpHeader.opcode = htons(std::stoi(arpOp->text().toStdString()));
    ConverMacAddressStringIntoByte(arpHwsrc->text().toStdString().c_str(), arpRaw->arpHeader.sender_mac);
    *(unsigned int*)(&arpRaw->arpHeader.sender_ip) = inet_addr(arpPsrc->text().toStdString().c_str());
    ConverMacAddressStringIntoByte(arpHwdst->text().toStdString().c_str(), arpRaw->arpHeader.target_mac);
    *(unsigned int*)(&arpRaw->arpHeader.target_ip) = inet_addr(arpPdst->text().toStdString().c_str());

    memcpy(arpRaw->ehternetHeader.h_source, arpRaw->arpHeader.sender_mac, 6);
    memcpy(arpRaw->ehternetHeader.h_dest, arpRaw->arpHeader.target_mac, 6);

    forceFeed(&netWriter, arpPacket);
}


uint16_t udp_checksum(const void* buff, size_t len, in_addr_t src_addr, in_addr_t dest_addr) {
    const uint16_t *buf=(uint16_t *)buff;
    uint16_t *ip_src=(uint16_t *)&src_addr, *ip_dst=(uint16_t *)&dest_addr;
    uint32_t sum;
    size_t length=len;
    sum = 0;
    while (len > 1)
    {
        sum += *buf++;

        if (sum & 0x80000000)
            sum = (sum & 0xFFFF) + (sum >> 16);
        len -= 2;
    }
    if ( len & 1 )
        sum += *((uint8_t *)buf);
    sum += *(ip_src++);
    sum += *ip_src;
    sum += *(ip_dst++);
    sum += *ip_dst;
    sum += htons(IPPROTO_UDP);
    sum += htons(length);
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    return ( (uint16_t)(~sum)  );
}

unsigned short ip_checksum(unsigned short* buff, int _16bitword)
{
    unsigned long sum;
    for(sum=0;_16bitword>0;_16bitword--)
        sum+=htons(*(buff)++);
    sum = ((sum >> 16) + (sum & 0xFFFF));
    sum += (sum>>16);
    return htons((unsigned short)(~sum));
}

void MainWindow::on_pushButton_2_clicked() {
    std::string interface = udpInterface->text().toStdString();
    std::string hwsrc = udpHwsrc->text().toStdString();
    std::string hwdst = udpHwdst->text().toStdString();
    std::string ipsrc = udpIpsrc->text().toStdString();
    std::string ipdst = udpIpdst->text().toStdString();
    std::string portsrc = udpPortsrc->text().toStdString();
    std::string portdst = udpPortdst->text().toStdString();
    std::string payload = udpPayload->document()->toPlainText().toStdString();

    std::shared_ptr<PcapPacket> udpPacket = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<UDPRaw>(payload.length()));
    udpPacket->raw->pcapHeader.incl_len = sizeof (UDPRaw) - sizeof (PcapRaw) + payload.length();
    udpPacket->interface = udpInterface->text().toStdString();

    UDPRaw* udp = (UDPRaw*)udpPacket->raw;
    ConverMacAddressStringIntoByte(hwsrc.c_str(), udp->ehternetHeader.h_source);
    ConverMacAddressStringIntoByte(hwdst.c_str(), udp->ehternetHeader.h_dest);
    udp->ehternetHeader.h_proto = htons(ETHERTYPE_IP);

    udp->ipHeader.ihl = 5;
    udp->ipHeader.version = 4;
    udp->ipHeader.tos = 0;
    udp->ipHeader.tot_len = htons(sizeof(UDPRaw) - sizeof (EthernetRaw) + payload.length());
    udp->ipHeader.id = htons(300);
    udp->ipHeader.frag_off =  0x000;
    udp->ipHeader.ttl = 64;
    udp->ipHeader.protocol = IPPROTO_UDP;
    udp->ipHeader.saddr = inet_addr(ipsrc.c_str());
    udp->ipHeader.daddr = inet_addr(ipdst.c_str());

    udp->udpHeader.source = htons(std::stoi(portsrc));
    udp->udpHeader.dest = htons(std::stoi(portdst));
    udp->udpHeader.len = htons(sizeof(UDPRaw) - sizeof (IPRaw) + payload.length());
    memcpy(udp->udpPayload, payload.c_str(), payload.length());

    udp->udpHeader.check = udp_checksum(udp->ipPayload, ntohs(udp->udpHeader.len), udp->ipHeader.saddr, udp->ipHeader.daddr);
    udp->ipHeader.check = 0;
    udp->ipHeader.check = ip_checksum((unsigned short*)&udp->ipHeader, sizeof(struct iphdr)/2);

    forceFeed(&netWriter, udpPacket);
}
