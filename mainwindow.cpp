#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //showMaximized();
    setWindowState(Qt::WindowFullScreen);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
