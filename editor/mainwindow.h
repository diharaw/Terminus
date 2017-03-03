#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool isRunning;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void onButtonClick();
    void onBtnExitClick();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
