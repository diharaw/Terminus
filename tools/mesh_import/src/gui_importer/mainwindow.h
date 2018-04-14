#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

namespace Ui {
class MainWindow;
}

struct AssimpImportData;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *target, QEvent *event);
    
private:
    Ui::MainWindow *ui;
    std::string _selected_file;
    std::string _output_path;
    AssimpImportData* _current_data;
    
private slots:
    void onBrowseImportPressed();
    void onBrowseExportPressed();
    void onExportPressed();
};

#endif // MAINWINDOW_H
