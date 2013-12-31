#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QVector>

class CNeuralNetwork;

class CWorker : public QObject
{
	Q_OBJECT

public:
	CWorker(int wSize, int imgNumber, double lCoef, double maxError, int maxIter, QObject *parent = 0);
	virtual ~CWorker();

	void learn(const QVector<double> &sequence);

private:
	bool mIsStopped;
	bool mIsExit;
	QMutex mSync;
	QWaitCondition mPauseCond;
	CNeuralNetwork* mNetwork;
	double mMaxError;
	int mMaxIterations;
	QVector<double> mSequence;

public slots:
	void process();
	void stop();
	void start();
	void exit();
};

#endif // WORKER_H
