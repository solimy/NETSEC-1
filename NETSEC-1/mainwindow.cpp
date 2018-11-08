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
    ARPRaw* arpRaw = (ARPRaw*)arpPacket->raw;
    arpPacket->raw->pcapHeader.incl_len = sizeof (ARPRaw) - sizeof (PcapRaw);
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
