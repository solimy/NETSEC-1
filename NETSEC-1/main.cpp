#include "mainwindow.h"
#include <QApplication>
#include "packetreader.h"
#include "pcapfeeder.h"
#include <iostream>
#include <unistd.h>

#include "packetreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    PacketReader reader;
    DEBUG::PacketDumper dumper;
    dumper.subscribeToFeeder(&reader);
    reader.startCapture();
    return a.exec();
}
