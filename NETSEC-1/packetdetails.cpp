#include "packetdetails.h"
#include "ui_packetdetails.h"

PacketDetails::PacketDetails(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PacketDetails)
{
    ui->setupUi(this);
    plainTextEdit = findChild<QPlainTextEdit*>(QString("plainTextEdit"),  Qt::FindChildrenRecursively);
    plainTextEdit->setReadOnly(true);
}

PacketDetails::~PacketDetails()
{
    delete ui;
}
