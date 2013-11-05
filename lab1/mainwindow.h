#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QThread>
#include "neuralnetwork.h"
#include "worker.h"

QString Matrix2String(const Matrix2DF &matrix);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void openFile();

private slots:

    void setOriginalImage(const QImage &img) const;
    void setCompressedImage(const QImage &img) const;
    void errorNetwork(double error);
    void onBtnInitNetwork();

    void start();
    void stop();
    void resetNetwork();
    void resetUI();
    void onBtnReset();
    void showCompressedImage();
    void updateNetworkStateGUI();
    void showFirstLayerMatrix();
    void showSecondLayerMatrix();


    void setUpdateStep();
private:
    void initNetwork();


    Ui::MainWindow *ui;
    QString mFilePath;
    QImage mOriginalImage;
    CNeuralNetwork *mNetwork;
    CImage* mNeuralImage;
    QVector<Segment*>* mSegmentsArray;
    QVector<QWidget*> mOpenBrowserWindow;
    CWorker* mWorker;
    QThread* mThread;
};

#endif // MAINWINDOW_H
