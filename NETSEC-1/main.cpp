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
    int user = getuid();

    if (user == 0) {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        DEBUG::PacketDumper dumper;
        //dumper.subscribeToFeeder(&w.reader);

        return a.exec();
    } else {
        printf("Need root privileges!\n");
        return -1;
    }
}
