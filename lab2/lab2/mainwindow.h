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
    
    typedef enum
    {
          Periodic
        , Power
        , Fibonacci
        , Natural
    } SequenceType;

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    void initGUI();
    void resetNetworkThread();
    void resetGUI();
    void initNetworkThread();
    QVector<double> createLearnSequence(int p, int m, SequenceType type) const;
    void display(const QString &msg);
    SequenceType sequenceType() const ;

    Ui::MainWindow *ui;
    CWorker *mWorker;
    QThread *mThread;
    QVector<QWidget*> mOpenBrowserWindow;

private slots:
    void onBtnInitNetwork();
    void onBtnStartNetwork();
    void onBtnStopNetwork();
    void onBtnResetNetwork();
    void onBtnPredict();

    void update(double error, int iteration);
    void updateError(double error);
    void updateIteration(int iteration);

    void updateStepValueChanged();
    void delayValueChanged();

    void showFirstLayerMatrix();
    void showSecondLayerMatrix();
	void showContextMatrix();

	void resetContextNeurons();
};

#endif // MAINWINDOW_H
