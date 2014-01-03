#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"
#include "defines.h"

#include <QThread>
#include <QObject>
#include <QTextBrowser>

#include <qmath.h>


QString dV2String(const QVector<double> &vector)
{
    QString out;

    if (vector.isEmpty())
        return out;

    out += QString::number(vector[0]);

    for(int i=1; i < vector.size(); i++)
        out+= ", " + QString::number(vector[i], 'g', 4);

    return out;
}

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
    ui->btnPredict->show();

    ui->fnNatural->setEnabled(false);
    ui->fnFibonacci->setEnabled(false);
    ui->fnPeriodic->setEnabled(false);
    ui->fnPower->setEnabled(false);

    ui->mValue->setEnabled(false);
    ui->pValue->setEnabled(false);
    ui->maxIterationsValue->setEnabled(false);
    ui->errorValue->setEnabled(false);
    ui->alphaValue->setEnabled(false);
}


void MainWindow::resetNetworkThread()
{

    for(int i=0; i<mOpenBrowserWindow.size(); i++)
    {
        QWidget *w = mOpenBrowserWindow.at(i);
        w->hide();
        delete w;
    }

    mOpenBrowserWindow.clear();

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
    ui->btnPredict->hide();

    ui->errorDisplay->clear();
    ui->iterationDisplay->clear();

    ui->fnNatural->setEnabled(true);
    ui->fnFibonacci->setEnabled(true);
    ui->fnPeriodic->setEnabled(true);
    ui->fnPower->setEnabled(true);

    ui->mValue->setEnabled(true);
    ui->pValue->setEnabled(true);
    ui->maxIterationsValue->setEnabled(true);
    ui->errorValue->setEnabled(true);
    ui->alphaValue->setEnabled(true);

    ui->log->clear();
}

void MainWindow::initNetworkThread()
{

    mThread = new QThread(this);

    int p = ui->pValue->value();
    int m = ui->mValue->value();
    double error = ui->errorValue->value();
    double alpha = ui->alphaValue->value();
    int maxIt = ui->maxIterationsValue->value();

    mWorker = new CWorker(p, m, alpha, error, maxIt);
    mWorker->setDelay(ui->delayValue->value());
    mWorker->setUpdateStep(ui->updateStepValue->value());

    mWorker->moveToThread(mThread);

    connect(mThread, SIGNAL(started()), mWorker, SLOT(process()));

    mThread->start(QThread::HighPriority);


    connect(mWorker, SIGNAL(update(double,int)), SLOT(update(double,int)));
    connect(mWorker, SIGNAL(error(double)), SLOT(updateError(double)));
    connect(mWorker, SIGNAL(iteration(int)), SLOT(updateIteration(int)));

    SequenceType fn = sequenceType();

    QVector<double> vector = createLearnSequence(p, m, fn);

    display(dV2String(vector));

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
    connect(ui->btnPredict, SIGNAL(clicked()),SLOT(onBtnPredict()));

    connect(ui->updateStepValue, SIGNAL(valueChanged(int)), SLOT(updateStepValueChanged()));
    connect(ui->delayValue, SIGNAL(valueChanged(int)), SLOT(delayValueChanged()));

    connect(ui->actionContextMatrix, SIGNAL(triggered()), SLOT(showContextMatrix()));
    connect(ui->actionFirstLayerMatrix, SIGNAL(triggered()), SLOT(showFirstLayerMatrix()));
    connect(ui->actionSecondLayerMatrix, SIGNAL(triggered()), SLOT(showSecondLayerMatrix()));

    resetGUI();
}


void MainWindow::update(double error, int iteration)
{
    ui->errorDisplay->setValue(error);
    ui->iterationDisplay->setValue(iteration);
}

void MainWindow::updateIteration(int iteration)
{
    ui->iterationDisplay->setValue(iteration);
}

void MainWindow::updateError(double error)
{
    ui->errorDisplay->setValue(error);
}

void MainWindow::updateStepValueChanged()
{
    if (!mWorker)
        return;

    mWorker->setUpdateStep(ui->updateStepValue->value());
}

void MainWindow::delayValueChanged()
{
    if (!mWorker)
        return;

    mWorker->setDelay(ui->delayValue->value());
}


QVector<double> MainWindow::createLearnSequence(int p, int m, SequenceType type) const
{
    QVector<double> lVector;

    int size = p+m;

    switch (type)
    {
        case Fibonacci:
        lVector << 1 << 1;
        for(int i = 0; i < size; i++)
            lVector << lVector[i] + lVector[i+1];
        break;

    case Power:
        for(int i = 0; i < size; i++)
            lVector << pow((qreal)i+1,(qreal) 2);
        break;

    case Periodic:
    {
        QMap<int, int> map; map.insert(1,1); map.insert(2,0); map.insert(3,-1); map.insert(4,0);
        lVector << 1;
        for(int i=1; i < size; i++)
        {
            int last = lVector.last();
            if (last == 1)
                lVector << 2;
            else if (last == 2)
                lVector << 3;
            else if (last == 3)
                lVector << 4;
            if (last == 4)
                lVector << 1;
        }

        for(int i=0; i < lVector.size(); i++)
            lVector[i] = map[lVector[i]];
    }
        break;

    case Natural:
        for(int i=0; i< size; i++)
            lVector << i+1;
        break;

    }


    return lVector;
}


void MainWindow::display(const QString &msg)
{
    ui->log->setText(msg);
}

void MainWindow::onBtnPredict()
{
    if (!mWorker)
        return;

//    onBtnStopNetwork();

    QVector<double> lVector = createLearnSequence(ui->pValue->value(), ui->mValue->value(),sequenceType());

    QVector<double> predicted = mWorker->predict(lVector, ui->nValue->value());

    QString msg = "Training sequence: " + dV2String(lVector) + "\n";
    msg += "Predicted sequence: " + dV2String(predicted) + "\n";

    QVector<double> real = createLearnSequence(ui->pValue->value() + ui->nValue->value(), ui->mValue->value(), sequenceType());

    while (!lVector.isEmpty())
    {
        real.pop_front();
        lVector.pop_back();
    }

    msg += "Real sequence: " + dV2String(real);

    display(msg);
}

MainWindow::SequenceType MainWindow::sequenceType() const
{
    SequenceType fn = Fibonacci;

    if(ui->fnNatural->isChecked())
        fn = Natural;
    else if(ui->fnPower->isChecked())
        fn = Power;
    else if(ui->fnPeriodic->isChecked())
        fn = Periodic;

    return fn;
}

QString Matrix2String(const CMatrix &matrix)
{
    QString out;
    QString tmp;

    out+=QString("rows: %1 \ncols: %2\n").arg(QString::number(matrix.n_rows)).arg(QString::number(matrix.n_cols));

    for(uint i=0; i < matrix.n_rows; i++)
    {

        for(uint j=0; j< matrix.n_cols; j++)
        {
            tmp = QString::number(matrix(i,j),'f',4);
            if(tmp.size() == 6)
                tmp = " "+tmp;
            out+=QString("%1     ").arg(tmp);
        }
        out+="\n";
    }

    return out;
}


void MainWindow::showFirstLayerMatrix()
{
    if(!mWorker)
        return;

    onBtnStopNetwork();

    QTextBrowser *br = new QTextBrowser;
    br->setLineWrapMode(QTextEdit::NoWrap);
    br->setWindowTitle("First layer matrix");

    br->setText(Matrix2String(mWorker->firstLayerMatrix()));

    br->show();

    mOpenBrowserWindow.push_back(br);
}

void MainWindow::showSecondLayerMatrix()
{
    if(!mWorker)
        return;

    onBtnStopNetwork();

    QTextBrowser *br = new QTextBrowser;
    br->setLineWrapMode(QTextEdit::NoWrap);
    br->setWindowTitle("Second layer matrix");

    br->setText(Matrix2String(mWorker->secondLayerMatrix()));

    br->show();
    mOpenBrowserWindow.push_back(br);
}

void MainWindow::showContextMatrix()
{
    if(!mWorker)
        return;

    onBtnStopNetwork();

    QTextBrowser *br = new QTextBrowser;
    br->setLineWrapMode(QTextEdit::NoWrap);
    br->setWindowTitle("Context matrix");

    br->setText(Matrix2String(mWorker->contextMatrix()));

    br->show();
    mOpenBrowserWindow.push_back(br);
}
