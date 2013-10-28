#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QThread>
#include "neuralnetwork.h"
#include "worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void tmpStep();
    void openFile();
    void setOriginalImage(const QImage &img) const;
    void setCompressedImage(const QImage &img) const;
    void errorNetwork(double error);

    void start();
    void stop();
private:
    void initNetwork();


    Ui::MainWindow *ui;
    QString mFilePath;
    QImage mOriginalImage;
    CNeuralNetwork *mNetwork;
    CImage* mNeuralImage;
    QVector<Segment*>* mSegmentsArray;
    CWorker* mWorker;
    QThread* mThread;
};

#endif // MAINWINDOW_H
