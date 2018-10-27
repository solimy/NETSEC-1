#include "captureoptions.h"
#include "ui_captureoptions.h"
#include "mainwindow.h"

CaptureOptions::CaptureOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureOptions)
{
    ui->setupUi(this);
}

CaptureOptions::~CaptureOptions()
{
    delete ui;
}

void CaptureOptions::on_pushButton_clicked()
{
    reader->startCapture();
    close();
}

void CaptureOptions::on_pushButton_3_clicked()
{
    reader->stopCapture();
    close();
}

void CaptureOptions::on_lineEdit_returnPressed()
{

}
