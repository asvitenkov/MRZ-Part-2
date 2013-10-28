#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QEventLoop>
#include <QWaitCondition>
#include <QMutex>

#include "neuralnetwork.h"


class CWorker : public QObject
{
    Q_OBJECT
public:
    explicit CWorker(CNeuralNetwork *network, QVector<Segment*>* vector,QObject *parent = 0);
    
signals:
    void errorValue(double error);
    
public slots:
    void process();
    void stop();
    void start();

private:
    QMutex mSync;
    QWaitCondition mPauseCond;
    bool mIsStopped;
    CNeuralNetwork *mNetwork;
    QVector<Segment*>* mSegmantArray;
    QEventLoop mEventLoop;
};

#endif // WORKER_H
