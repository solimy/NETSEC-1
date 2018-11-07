#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "captureoptions.h"
#include "packetdetails.h"
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    subscribeToFeeder(&reader);
    packetTable = findChild<QTableWidget*>(QString("tableWidget"),  Qt::FindChildrenRecursively);
    lineEdit = findChild<QLineEdit*>(QString("lineEdit"),  Qt::FindChildrenRecursively);
    int column = -1;
    //proto
    packetTable->setColumnWidth(++column, 120);
    //timstamp sec
    packetTable->setColumnWidth(++column, 120);
    //imestamp usec
    packetTable->setColumnWidth(++column, 120);
    //src mac
    packetTable->setColumnWidth(++column, 120);
    //dst mac
    packetTable->setColumnWidth(++column, 120);
    //src ip
    packetTable->setColumnWidth(++column, 120);
    //src port
    packetTable->setColumnWidth(++column, 120);
    //dst ip
    packetTable->setColumnWidth(++column, 120);
    //dst port
    packetTable->setColumnWidth(++column, 120);
    fileWriter.subscribeToFeeder(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_triggered(QAction *arg1)
{
}

void MainWindow::on_toolButton_clicked()
{
    CaptureOptions cOpt;
    cOpt.setReader(&reader);
    cOpt.exec();
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    PacketDetails pD;
    pD.setPacket(packets.at(row));
    pD.exec();
}

void MainWindow::on_lineEdit_editingFinished()
{
    filter.setFilters(lineEdit->text().toStdString());
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if (checked) {
        std::stringstream ss;
        ss << std::time(0);
        ss << ".cap";
        printf("filename: %s\n", ss.str().c_str());
        fileWriter.startWriting(ss.str());
    } else {
        fileWriter.stopWriting();
    }
}
