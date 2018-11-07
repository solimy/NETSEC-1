#ifndef CAPTUREOPTIONS_H
#define CAPTUREOPTIONS_H

#include "packetreader.h"
#include <QDialog>
#include <QLineEdit>

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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_returnPressed();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CaptureOptions *ui;
    QLineEdit* lineEdit;
};

#endif // CAPTUREOPTIONS_H
