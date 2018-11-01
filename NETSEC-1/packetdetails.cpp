#include "packetdetails.h"
#include "ui_packetdetails.h"

PacketDetails::PacketDetails(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PacketDetails)
{
    ui->setupUi(this);
}

PacketDetails::~PacketDetails()
{
    delete ui;
}
