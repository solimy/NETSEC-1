#ifndef PACKETDETAILS_H
#define PACKETDETAILS_H

#include <QDialog>

namespace Ui {
class PacketDetails;
}

class PacketDetails : public QDialog
{
    Q_OBJECT

public:
    explicit PacketDetails(QWidget *parent = nullptr);
    ~PacketDetails();

private:
    Ui::PacketDetails *ui;
};

#endif // PACKETDETAILS_H
