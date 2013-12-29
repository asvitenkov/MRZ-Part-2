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
    explicit CNeuralNetwork(int wSize, int imgNumber, double lCoef, double maxError, int maxIter, QObject *parent = 0);
    
    void learn(const QVector<double> &sequence);

signals:
    
public slots:
    
private:
    void initialize();
    QSharedPointer< QVector<CMatrix> > createLearningMatrix(const QVector<double> &sequence) const;
    void learnStep(const QVector<CMatrix> &learn, const QVector<double> &etalons, CMatrix &contexMatrix, CMatrix &wM1, CMatrix &wM2, double lCoef) const;
    void normalizeMatrix(CMatrix &matrix) const;
    double error(const QVector<CMatrix> &learn, const QVector<double> &etalons, CMatrix &contexMatrix, CMatrix &wM1, CMatrix &wM2) const;

private:
    int mWindowSize;
    int mImageNumber;
    double mLearningCoefficient;
    double mMaxError;
    int mMaxIterations;
    int mIterations;
    CMatrix mWeightMatrix1;
    CMatrix mWeightMatrix2;
    CMatrix mContextMatrix;


};

#endif // NEURALNETWORK_H
