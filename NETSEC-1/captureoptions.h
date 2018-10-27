#ifndef CAPTUREOPTIONS_H
#define CAPTUREOPTIONS_H

#include "packetreader.h"
#include <QDialog>

namespace Ui {
class CaptureOptions;
}

class CaptureOptions : public QDialog
{
    Q_OBJECT

public:
    explicit CaptureOptions(QWidget *parent = nullptr);
    ~CaptureOptions();
    PacketReader* reader;

    void setReader(PacketReader* reader) {
        this->reader = reader;
    }

    std::string file = "";

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::CaptureOptions *ui;
};

#endif // CAPTUREOPTIONS_H
