#include "worker.h"

#include "neuralnetwork.h"

CWorker::CWorker(CNeuralNetwork *network, QVector<Segment *> *vector, QObject *parent) :
    QObject(parent), mNetwork(network), mIsStopped(false), mSegmantArray(vector)
{
    Q_ASSERT(mNetwork);
}


void CWorker::process()
{

    //mEventLoop.exec();
    double error;

    while(true)
    {
        mSync.lock();
        if (mIsStopped)
            mPauseCond.wait(&mSync);
        mSync.unlock();
        error = mNetwork->learn(*mSegmantArray);
        if (mNetwork->getStep() % 1000 == 0 )
            emit errorValue(mNetwork->getError(*mSegmantArray));
    }
}


void CWorker::stop()
{
    mIsStopped = true;
    //mEventLoop.exec();
}



void CWorker::start()
{
    mIsStopped = false;
    //mEventLoop.exit();
    mPauseCond.wakeAll();
}
