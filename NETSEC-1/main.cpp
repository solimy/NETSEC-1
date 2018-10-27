#include "mainwindow.h"
#include <QApplication>
#include "packetreader.h"
#include "pcapfeeder.h"
#include <iostream>
#include <unistd.h>
#include <deque>

#include "packetreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DEBUG::PacketDumper dumper;
    dumper.subscribeToFeeder(&w.reader);

    return a.exec();
}
