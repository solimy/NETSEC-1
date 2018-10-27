#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "packetreader.h"
#include "pcapfeeder.h"
#include <deque>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public PcapFeeder::PcapFeedable
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::deque<std::shared_ptr<PcapPacket>> packets;
    QTableWidget* packetTable;

    virtual void feed(const std::shared_ptr<PcapPacket> packet) {
        packets.push_front(packet);
        packetTable->insertRow(0);
        auto proto = packetTable->item(0, 0);
        if(!proto)
        {
            proto = new QTableWidgetItem;
            packetTable->setItem(0, 0, proto);
        }        switch (packet->protocol) {
        case ProtocolEnum::UNKNOWN:
            proto->setText("UNKNOWN");
            break;
        case ProtocolEnum::ETHERNET:
            proto->setText("ETHERNET");
            break;
        case ProtocolEnum::ARP:
            proto->setText("ARP");
            break;
        case ProtocolEnum::IP:
            proto->setText("IP");
            break;
        case ProtocolEnum::ICMP:
            proto->setText("ICMP");
            break;
        case ProtocolEnum::TCP:
            proto->setText("TCP");
            break;
        case ProtocolEnum::UDP:
            proto->setText("UDP");
            break;
        case ProtocolEnum::HTTP:
            proto->setText("HTTP");
            break;
        case ProtocolEnum::DNS:
            proto->setText("DNS");
            break;
        }
    }

    PacketReader reader;

private slots:
    void on_toolButton_triggered(QAction *arg1);
    
    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
