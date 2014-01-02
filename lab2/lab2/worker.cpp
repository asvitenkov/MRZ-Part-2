#include "worker.h"
#include "neuralnetwork.h"

#include <QThread>
#include <QDebug>


CWorker::CWorker(int wSize, int imgNumber, double lCoef, double maxError, quint64 maxIter, QObject *parent /* = 0 */)
	: QObject(parent)
	, mIsStopped(true)
	, mIsExit(false)
	, mMaxError(maxError)
	, mMaxIterations(maxIter)
    , mDelay(0)
    , mUpdateStep(10000)
{
	mNetwork = new CNeuralNetwork(wSize, imgNumber, lCoef, this);
}

CWorker::~CWorker()
{
	if (mNetwork)
		mNetwork->deleteLater();
}

void CWorker::process()
{

	double totalError = std::numeric_limits<double>::max();

	while (!mIsExit && totalError > mMaxError && mNetwork->iteration() < mMaxIterations)
	{
		mSync.lock();
		if (mIsStopped)
			mPauseCond.wait(&mSync);
		mSync.unlock();

		if (mIsExit)
			break;

        // обучение

		mNetwork->learn(mSequence);
	
        totalError = mNetwork->error(mSequence);

        if (mNetwork->iteration() % mUpdateStep == 0)
            emit update(totalError, mNetwork->iteration());

        msleep(mDelay);

	}

    qDebug() << QString("Learn end:\nerror:%1, iteration:%2").arg(QString::number(totalError)).arg(QString::number(mNetwork->iteration()));

    if (!mIsExit)
        emit update(totalError, mNetwork->iteration());
}

void CWorker::start()
{
    if (mSequence.empty())
        return;

	mIsStopped = false;
    mPauseCond.wakeAll();
}

void CWorker::stop()
{
	mIsStopped = true;
}

void CWorker::exit()
{
    mIsExit = true;

    if (mIsStopped)
        start();
}

void CWorker::learn(const QVector<double> &sequence)
{
    mSequence = sequence;
}


void CWorker::msleep(unsigned long msecs)
{
    if (msecs == 0)
        return;

    QMutex mutex;
    mutex.lock();

    QWaitCondition waitCondition;
    waitCondition.wait(&mutex, msecs);

    mutex.unlock();
}


QVector<double> CWorker::predict(const QVector<double> &sequence, int count) const
{
    return mNetwork->predict(sequence, count);
}
