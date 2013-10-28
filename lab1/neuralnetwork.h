#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <armadillo>
#include "image.h"

#include <QRunnable>


typedef arma::mat Matrix2DF;

#define THREAD_COUNT 8

class CCalculateError: public QRunnable
{
public:
    CCalculateError(Matrix2DF *fMatrix, Matrix2DF *sMatrix);
    virtual ~CCalculateError();
    void run();

    int mFrom;
    int mTo;
    int mError;
    const QVector<Segment*>* mSegmentArray;
    Matrix2DF *mFirstMatrix;
    Matrix2DF *mSecondMatrix;
};


class CNeuralNetwork
{
public:
    CNeuralNetwork(int segmantLength, int secondLayerCount, double alpha);
    virtual ~CNeuralNetwork();

    double learn(const QVector<Segment*> &vector);
    QVector<Segment*>* process(const QVector<Segment*> &vector) const ;
    double getError(const QVector<Segment*> &vector) const ;
    int getStep() const { return mStep; }
private:
    void initialize();
    int mStep;

    int mSegmentLength;
    int mSecondLayerCount;
    double mAlpha;

    Matrix2DF mFirstLayerMatrix;
    Matrix2DF mSecondLayerMatrix;

    QVector<CCalculateError*> mThreatArray;

    //CCalculateError* mCError1;
    //CCalculateError* mCError2;
    //CCalculateError* mCError3;
    //CCalculateError* mCError4;

};




#endif // NEURALNETWORK_H
