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
    CWorker(int wSize, int imgNumber, double lCoef, double maxError, quint64 maxIter, QObject *parent = 0);
	virtual ~CWorker();

	void learn(const QVector<double> &sequence);

    QVector<double> predict(const QVector<double> &sequence, int count) const;

    inline bool isStopped() const { return mIsStopped; }
    inline bool isExit() const { return mIsExit; }
    inline void setDelay(uint delay) { mDelay = delay; }
    inline void setUpdateStep(int step) { mUpdateStep = step; }

signals:
    void error(double error);
    void iteration(int iteration);
    void update(double error, int iteration);

private:
    void msleep(unsigned long msecs);

	bool mIsStopped;
	bool mIsExit;
	QMutex mSync;
	QWaitCondition mPauseCond;
	CNeuralNetwork* mNetwork;
	double mMaxError;
    quint64 mMaxIterations;
	QVector<double> mSequence;
    uint mDelay;
    int mUpdateStep;

public slots:
	void process();
	void stop();
	void start();
	void exit();
};

#endif // WORKER_H
