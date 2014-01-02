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

    ui->btnInitNetwork->hide();
    ui->btnStartNetwork->show();
    ui->btnResetNetwork->show();
}


void MainWindow::resetNetworkThread()
{
    if (mWorker && !mWorker->isExit())
    {
        mWorker->exit();
        mWorker->deleteLater();
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
    ui->btnInitNetwork->show();
    ui->btnStopNetwork->hide();
    ui->btnResetNetwork->hide();
    ui->btnStartNetwork->hide();
}

void MainWindow::initNetworkThread()
{

    mThread = new QThread(this);
    mWorker = new CWorker(3,4, 0.001, 0.001, 100000000);

    mWorker->moveToThread(mThread);

    connect(mThread, SIGNAL(started()), mWorker, SLOT(process()));

    mThread->start(QThread::HighPriority);


    QVector<double> vector;
    vector << 1 << 1;
    for(int i=0; i<10; i++)
        vector << vector[i] + vector[i+1];

    mWorker->learn(vector);
}


void MainWindow::onBtnStartNetwork()
{
    if (mWorker)
    {
        mWorker->start();
        ui->btnStartNetwork->hide();
        ui->btnStopNetwork->show();
    }
}
void MainWindow::onBtnStopNetwork()
{
    if (mWorker)
    {
        mWorker->stop();
        ui->btnStopNetwork->hide();
        ui->btnStartNetwork->show();
    }
}


void MainWindow::onBtnResetNetwork()
{
    resetNetworkThread();
    resetGUI();
}


void MainWindow::initGUI()
{
    connect(ui->btnInitNetwork, SIGNAL(clicked()),SLOT(onBtnInitNetwork()));
    connect(ui->btnStartNetwork, SIGNAL(clicked()),SLOT(onBtnStartNetwork()));
    connect(ui->btnStopNetwork, SIGNAL(clicked()),SLOT(onBtnStopNetwork()));
    connect(ui->btnResetNetwork, SIGNAL(clicked()),SLOT(onBtnResetNetwork()));

    resetGUI();
}
