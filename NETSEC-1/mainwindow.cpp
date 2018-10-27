#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "captureoptions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    subscribeToFeeder(&reader);
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
    cOpt.setModal(true);
    cOpt.exec();
}
