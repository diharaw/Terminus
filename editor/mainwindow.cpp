#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    isRunning = true;
    ui->setupUi(this);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    setDockNestingEnabled(true);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClick);
    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::onBtnExitClick);
    ui->centralWidget->hide();
    ui->txtStatus->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonClick()
{
    ui->txtStatus->setText("Pressed!");
}

void MainWindow::onBtnExitClick()
{
    isRunning = false;
}
