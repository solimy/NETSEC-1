#include "captureoptions.h"
#include "ui_captureoptions.h"
#include "mainwindow.h"

CaptureOptions::CaptureOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureOptions)
{
    ui->setupUi(this);
    lineEdit = findChild<QLineEdit*>(QString("lineEdit"),  Qt::FindChildrenRecursively);
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

void CaptureOptions::on_pushButton_4_clicked()
{
}

void CaptureOptions::on_pushButton_2_clicked()
{
    std::cout << "loading file " << lineEdit->text().toStdString() << std::endl;
    reader->startCapture(lineEdit->text().toStdString());
    close();
}
