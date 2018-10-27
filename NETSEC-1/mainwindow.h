#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "packetreader.h"
#include "pcapfeeder.h"
#include <deque>

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

    virtual void feed(const std::shared_ptr<PcapPacket> packet) {
        packets.push_front(packet);
    }

    PacketReader reader;

private slots:
    void on_toolButton_triggered(QAction *arg1);
    
    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
