#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <defines.h>

#include <QObject>
#include <QSharedPointer>
#include <QVector>

class CNeuralNetwork : public QObject
{
    Q_OBJECT

public:
    typedef enum{
         None
        ,ZeroingFirst
        ,ZeroingAlways
    } ZeroingType;


public:
    explicit CNeuralNetwork(int wSize, int imgNumber, double lCoef, /*double maxError, int maxIter,*/ QObject *parent = 0);
    void learn(const QVector<double> &sequence);
    double error(const QVector<double> &sequence) const;
    QVector<double> predict(const QVector<double> &sequence, int count) const;

    void setZeroingLearnContextNeuronsType(ZeroingType type);
    void setZeroingPredictContextNeuronsType(ZeroingType type);

    void zeroingContextNeurons();

    inline quint64 iteration() const { return mIterations; }

signals:
    
public slots:
    
private:
    void initialize();
    QSharedPointer< QVector<CMatrix> > createLearningMatrix(const QVector<double> &sequence) const;
    QVector<double> createEtalonVector(const QVector<double> &sequence) const;
    void learn(const QVector<CMatrix> &learn, const QVector<double> &etalons, CMatrix &contexMatrix, CMatrix &wM1, CMatrix &wM2, double lCoef) const;
    void normalizeMatrix(CMatrix &matrix) const;
    double error(const QVector<CMatrix> &learn, const QVector<double> &etalons, const CMatrix &contexMatrix, const CMatrix &wM1, const CMatrix &wM2) const;
    CVector predict(const CVector &sequence, const CMatrix &contexMatrix, const CMatrix &wM1, const CMatrix &wM2, int wSize, int count) const;

private:
    int mWindowSize;
    int mImageNumber;
    double mLearningCoefficient;
    //double mMaxError;
    //int mMaxIterations;
    quint64 mIterations;
    CMatrix mWeightMatrix1;
    CMatrix mWeightMatrix2;
    CMatrix mContextMatrix;

    ZeroingType mZeroingLearnContextNeuronsType;
    ZeroingType mZeroingPredictContextNeuronsType;


};

#endif // NEURALNETWORK_H
