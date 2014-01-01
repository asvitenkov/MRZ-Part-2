#include "worker.h"
#include "neuralnetwork.h"

#include <QDebug>


CWorker::CWorker(int wSize, int imgNumber, double lCoef, double maxError, quint64 maxIter, QObject *parent /* = 0 */)
	: QObject(parent)
	, mIsStopped(true)
	, mIsExit(false)
	, mMaxError(maxError)
	, mMaxIterations(maxIter)
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

        if(mNetwork->iteration() % 50000 == 0)
            qDebug() << totalError;
	}

    qDebug() << QString("Learn end:\nerror:%1, iteration:%2").arg(QString::number(totalError)).arg(QString::number(mNetwork->iteration()));
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
