#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"

#include <QThread>
#include <QObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mWorker(0)
    , mThread(0)
{
    ui->setupUi(this);

    initGUI();
}

MainWindow::~MainWindow()
{
    resetNetworkThread();
    delete ui;
}


void MainWindow::onBtnInitNetwork()
{
    resetNetworkThread();
    resetGUI();
    initNetworkThread();
}


void MainWindow::resetNetworkThread()
{
    if (mWorker && !mWorker->isExit())
    {
        mWorker->exit();
        //mWorker->deleteLater();
    }

    if (mThread)
    {
        mThread->exit(0);
        mThread->wait(1000);
        mThread->deleteLater();

    }

    mWorker = 0;
    mThread = 0;
}

void MainWindow::resetGUI()
{

}

void MainWindow::initNetworkThread()
{

    mThread = new QThread(this);
    mWorker = new CWorker(3,4, 0.00001, 0.01, 100000000);

    mWorker->moveToThread(mThread);

    connect(mThread, SIGNAL(started()), mWorker, SLOT(process()));

    mThread->start(QThread::HighPriority);


    QVector<double> vector;
    for(int i=0; i<15; i++)
        vector << i+1;

    mWorker->learn(vector);
}


void MainWindow::onBtnStartNetwork()
{
    if (mWorker)
        mWorker->start();
}
void MainWindow::onBtnStopNetwork()
{
    if (mWorker)
        mWorker->stop();
}


void MainWindow::onBtnResetNetwork()
{
    resetNetworkThread();
    resetGUI();
}


void MainWindow::initGUI()
{
    connect(ui->init, SIGNAL(clicked()),SLOT(onBtnInitNetwork()));
    connect(ui->start, SIGNAL(clicked()),SLOT(onBtnStartNetwork()));
    connect(ui->stop, SIGNAL(clicked()),SLOT(onBtnStopNetwork()));
    connect(ui->reset, SIGNAL(clicked()),SLOT(onBtnResetNetwork()));
}
