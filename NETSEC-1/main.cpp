#include "mainwindow.h"
#include <QApplication>
#include "packetreader.h"
#include "pcapfeeder.h"

int main(int argc, char *argv[])
{
    PacketReader reader;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
