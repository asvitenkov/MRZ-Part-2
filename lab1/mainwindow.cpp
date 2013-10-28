#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mNetwork(0),
    mSegmentsArray(0),
    mWorker(0),
    mNeuralImage(0),
    mThread(0)
{
    ui->setupUi(this);


    connect(ui->actionOpen,SIGNAL(triggered()),this, SLOT(openFile()));


    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(tmpStep()));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::openFile()
{
    mFilePath = QFileDialog::getOpenFileName();

    if (mFilePath.isEmpty())
        return;

    mOriginalImage = QImage(mFilePath);

    setOriginalImage(mOriginalImage);

    initNetwork();
}



void MainWindow::setOriginalImage(const QImage &img) const
{
    ui->lblOriginalImage->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::setCompressedImage(const QImage &img) const
{
    ui->lblCompressedImege->setPixmap(QPixmap::fromImage(img));
}


void MainWindow::initNetwork()
{
    if (mNetwork)
        delete mNetwork;

    if (mSegmentsArray)
    {
        qDeleteAll(mSegmentsArray->begin(), mSegmentsArray->end());
        mSegmentsArray->clear();
        delete mSegmentsArray;
    }

    if(mNeuralImage)
        delete mNeuralImage;

    if(mWorker)
        delete mWorker;

    if(mThread)
        delete mThread;

    mNetwork = new CNeuralNetwork(3 * ui->nValue->value() * ui->mValue->value(), ui->pValue->value(),ui->alphaValue->value());

    mNeuralImage = new CImage(CImage::fromImage(mOriginalImage));
    mSegmentsArray = mNeuralImage->split(ui->nValue->value(), ui->mValue->value());

    mWorker = new CWorker(mNetwork,mSegmentsArray);



    mThread = new QThread();

    mWorker->moveToThread(mThread);
    mWorker->stop();

    connect(ui->btnStart,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->btnStop,SIGNAL(clicked()),this,SLOT(stop()));

    connect(mThread,SIGNAL(started()),mWorker,SLOT(process()));

    connect(mWorker,SIGNAL(errorValue(double)),this,SLOT(errorNetwork(double)));
    mThread->start(QThread::HighestPriority);
}

void MainWindow::tmpStep()
{

    double error = 100000;

    mThread->start();

    QVector<Segment*>* newVec =  mNetwork->process(*mSegmentsArray);

    QImage newImg = CImage::unite(ui->nValue->value(), ui->mValue->value(), mOriginalImage.height(), mOriginalImage.width(), newVec).toImage();

    setCompressedImage(newImg);
}

void MainWindow::errorNetwork(double error)
{
    qDebug() << error;
}

void MainWindow::start()
{
    mWorker->start();
}

void MainWindow::stop()
{
    mWorker->stop();
}
