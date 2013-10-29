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
    connect(ui->btnStart,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->btnStop,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ui->btnReset,SIGNAL(clicked()),this,SLOT(onBtnReset()));
    connect(ui->btnShowCompressedImg,SIGNAL(clicked()),this,SLOT(showCompressedImage()));
    connect(ui->btnInitNetwork,SIGNAL(clicked()),this,SLOT(onBtnInitNetwork()));
    connect(ui->updateStep,SIGNAL(valueChanged(int)),this,SLOT(setUpdateStep()));

    ui->btnStart->hide();
    ui->btnStop->hide();
    ui->btnReset->hide();
    ui->btnInitNetwork->hide();

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

    //resetNetwork();

    mOriginalImage = QImage(mFilePath);

    onBtnReset();
    //initNetwork();



    setOriginalImage(mOriginalImage);

    //ui->btnStart->show();
    //ui->btnReset->show();
    ui->btnInitNetwork->show();
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

    resetNetwork();

    mNetwork = new CNeuralNetwork(3 * ui->nValue->value() * ui->mValue->value(), ui->pValue->value(),ui->alphaValue->value());

    mNeuralImage = new CImage(CImage::fromImage(mOriginalImage));
    mSegmentsArray = mNeuralImage->split(ui->nValue->value(), ui->mValue->value());

    mWorker = new CWorker(mNetwork,mSegmentsArray);
    mWorker->setUpdateStep(ui->updateStep->value());

    mThread = new QThread();

    mWorker->moveToThread(mThread);
    //mWorker->stop();

    connect(mThread,SIGNAL(started()),mWorker,SLOT(process()));

    connect(mWorker,SIGNAL(errorValue(double)),this,SLOT(errorNetwork(double)));
    mThread->start(QThread::HighestPriority);

    connect(mWorker,SIGNAL(stepOver()),this,SLOT(updateNetworkStateGUI()));
}

//void MainWindow::tmpStep()
//{

//    double error = 100000;

//    mThread->start();

//    QVector<Segment*>* newVec =  mNetwork->process(*mSegmentsArray);

//    QImage newImg = CImage::unite(ui->nValue->value(), ui->mValue->value(), mOriginalImage.height(), mOriginalImage.width(), newVec).toImage();

//    setCompressedImage(newImg);
//}

void MainWindow::errorNetwork(double error)
{
    //qDebug() << error;
}

void MainWindow::start()
{
    if(mWorker)
        mWorker->start();
    ui->btnStart->hide();
    ui->btnStop->show();
}

void MainWindow::stop()
{
    if(mWorker)
        mWorker->stop();
    ui->btnStop->hide();
    ui->btnStart->show();
}


void MainWindow::resetNetwork()
{
    //initNetwork();

    if(mWorker)
        mWorker->deleteLater();

    if(mThread)
    {
        mThread->deleteLater();
    }

    if (mNetwork)
        delete mNetwork;

    if(mNeuralImage)
        delete mNeuralImage;


    if (mSegmentsArray)
    {
        qDeleteAll(mSegmentsArray->begin(), mSegmentsArray->end());
        mSegmentsArray->clear();
        delete mSegmentsArray;
    }

    mNetwork = 0;
    mThread = 0;
    mNeuralImage = 0;
    mSegmentsArray = 0;
    mWorker = 0;


}


void MainWindow::resetUI()
{
    //ui->lblOriginalImage->setText(" ");
    ui->lblCompressedImege->setText(" ");

    ui->btnStart->hide();
    ui->btnStop->hide();
    ui->btnReset->hide();
    ui->btnInitNetwork->show();
    ui->error->setText("");
    ui->iteration->setText("");
}


void MainWindow::onBtnReset()
{
    stop();
    if(mThread)
        mThread->wait(1000);
    resetNetwork();
    resetUI();
}


void MainWindow::showCompressedImage()
{

    if(!mThread)
        return;

    stop();

    mThread->wait(200);

    QVector<Segment*>* newVec =  mNetwork->process(*mSegmentsArray);



    QImage newImg = CImage::unite(ui->nValue->value(), ui->mValue->value(), mOriginalImage.height(), mOriginalImage.width(), newVec).toImage();

    setCompressedImage(newImg);

    qDeleteAll(newVec->begin(), newVec->end());
    newVec->clear();
    delete newVec;

    start();
}


void MainWindow::onBtnInitNetwork()
{
    initNetwork();
    ui->btnStart->show();
    ui->btnReset->show();
    ui->btnInitNetwork->hide();
}



void MainWindow::updateNetworkStateGUI()
{
    if(mWorker)
    {
        ui->error->setText(QString::number(mNetwork->getError(*mSegmentsArray)));
        ui->iteration->setText(QString::number(mNetwork->getStep()));
    }
}


void MainWindow::setUpdateStep()
{
    if(mWorker)
        mWorker->setUpdateStep(ui->updateStep->value());
}
