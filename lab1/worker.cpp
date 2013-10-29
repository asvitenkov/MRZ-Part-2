#include "worker.h"

#include "neuralnetwork.h"

CWorker::CWorker(CNeuralNetwork *network, QVector<Segment *> *vector, QObject *parent) :
    QObject(parent), mNetwork(network), mIsStopped(true), mSegmantArray(vector), mIsExit(false), mDisableUpdate(false), mUpdateStep(1)
{
    Q_ASSERT(mNetwork);
}


void CWorker::process()
{
    while(true)
    {
        if (mIsExit)
            break;
        mSync.lock();
        if (mIsStopped)
            mPauseCond.wait(&mSync);
        mSync.unlock();
        mNetwork->learn(*mSegmantArray);
        mNetwork->getError(*mSegmantArray);
        if (!mDisableUpdate && mNetwork->getStep() % mUpdateStep == 0)
            emit stepOver();

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

void CWorker::exit()
{
    mIsExit = true;
}
