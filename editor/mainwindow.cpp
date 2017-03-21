#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewport_dock.h"
#include <QMessageBox>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    setDockNestingEnabled(true);
    ui->centralWidget->hide();

	terminus::ViewportDock* viewport = new terminus::ViewportDock(tr("Viewport"), this);
	viewport->setObjectName("Viewport");
	viewport->setAllowedAreas(Qt::AllDockWidgetAreas);
	addDockWidget(Qt::LeftDockWidgetArea, viewport);
}

MainWindow::~MainWindow()
{
    delete ui;
}

