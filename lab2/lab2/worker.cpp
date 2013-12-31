#include "worker.h"
#include "neuralnetwork.h"


CWorker::CWorker(int wSize, int imgNumber, double lCoef, double maxError, int maxIter, QObject *parent /* = 0 */)
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
	
		//totalError = mNetwork->error()

	}
}

void CWorker::start()
{
	mIsStopped = false;
}

void CWorker::stop()
{
	mIsStopped = true;
	mPauseCond.wakeAll();
}

void CWorker::exit()
{

}

void CWorker::learn(const QVector<double> &sequence)
{
	mSequence = sequence;
	start();
}