#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <tsm_exporter.h>
#include <filesystem.h>
#include <assimp_importer.h>
#include <QKeyEvent>

#include "keycode/keytable.h"
#include "keycode/keycode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnBrowseImport, SIGNAL(clicked()), this, SLOT(onBrowseImportPressed()));
    connect(ui->btnBrowseExport, SIGNAL(clicked()), this, SLOT(onBrowseExportPressed()));
    connect(ui->btnExport, SIGNAL(clicked()), this, SLOT(onExportPressed()));
    
    qApp->installEventFilter(this);
    
    _current_data = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    
    if (event->type() == QEvent::KeyPress) {
        
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        uint32 key = keyEvent->nativeVirtualKey();
        uint32 code = MAC_NATIVE_TO_HID[key];
        
        uint32 modifier = keyEvent->modifiers();
        
        if(modifier == Qt::ShiftModifier)
        {
            std::cout << "shift" << std::endl;
            
            uint32 mkey = MAC_NATIVE_TO_HID[keyEvent->nativeVirtualKey()];
            std::cout << mkey << std::endl;
        }
        
        switch(code)
        {
            case KEY_A:
            {
                std::cout << "A" << std::endl;
                break;
            }
            case KEY_B:
            {
                std::cout << "B" << std::endl;
                break;
            }
            case KEY_C:
            {
                std::cout << "C" << std::endl;
                break;
            }
            case KEY_D:
            {
                std::cout << "D" << std::endl;
                break;
            }
            case KEY_Tab:
            {
                std::cout << "Tab" << std::endl;
                break;
            }
            case KEY_Space:
            {
                std::cout << "Space" << std::endl;
                break;
            }
            case KEY_Escape:
            {
                std::cout << "Escape" << std::endl;
                break;
            }
        }

        
        
        return QObject::eventFilter(target, event);
    }
    
    return false;
}

void MainWindow::onBrowseImportPressed()
{
    QFileDialog fd(this);
    
    QString file = fd.getOpenFileName(this,
                                      tr("Open Model"),
                                      QString(),
                                      tr("Model Files (*.obj *.dae *.fbx)"));
    
    if(file != nullptr || file != "")
    {
        file = QDir::current().relativeFilePath(file);
        
        std::cout << file.toStdString() << std::endl;
        _selected_file = file.toStdString();
        ui->btnBrowseImport->setText(file);
        
        _current_data = assimp_importer::import_mesh(_selected_file.c_str());
    }

}

void MainWindow::onBrowseExportPressed()
{
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::FileMode::DirectoryOnly);
    
    QString file = fd.getExistingDirectory(this);
    
    if(file != nullptr || file != "")
    {
        _output_path = file.toStdString();
        ui->btnBrowseExport->setText(file);
    }
}

void MainWindow::onExportPressed()
{
    if(_current_data)
    {
        QString str;
        
        std::string path = _output_path;
        path += "/mesh";
        
        QDir::current().mkdir(str.fromStdString(path));
        
        path = _output_path;
        path += "/material";
        
        QDir::current().mkdir(str.fromStdString(path));
        
        path = _output_path;
        path += "/texture";
        
        QDir::current().mkdir(str.fromStdString(path));
        
        tsm_exporter::export_mesh(_current_data, _output_path.c_str());
        _current_data = nullptr;
        
        QMessageBox msgBox;
        msgBox.setText("Successfully exported model!");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Asset not selected!");
        msgBox.exec();
    }
    
}

