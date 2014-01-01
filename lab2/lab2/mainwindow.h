#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class CWorker;
class QThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    
private:
    void initGUI();
    void resetNetworkThread();
    void resetGUI();
    void initNetworkThread();


    Ui::MainWindow *ui;
    CWorker *mWorker;
    QThread *mThread;

private slots:
    void onBtnInitNetwork();
    void onBtnStartNetwork();
    void onBtnStopNetwork();
    void onBtnResetNetwork();
};

#endif // MAINWINDOW_H
