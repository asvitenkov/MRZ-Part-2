#include "neuralnetwork.h"


#include <QDebug>

CNeuralNetwork::CNeuralNetwork(int wSize, int imgNumber, double lCoef, double maxError, int maxIter, QObject *parent)
    : QObject(parent)
    , mWindowSize(wSize)
    , mImageNumber(imgNumber)
    , mLearningCoefficient(lCoef)
    , mMaxError(maxError)
    , mMaxIterations(maxIter)
    , mIterations(0)
{
    initialize();
}



void CNeuralNetwork::initialize()
{
    mWeightMatrix1 = arma::randu<CMatrix>(mImageNumber, mWindowSize + mImageNumber) * 2 - 1;
    mWeightMatrix2 = arma::randu<CMatrix>(1, mImageNumber) * 2 - 1;
    mContextMatrix = CMatrix(1, mImageNumber);
    mContextMatrix.fill(0);
}



QSharedPointer< QVector<CMatrix> > CNeuralNetwork::createLearningMatrix(const QVector<double> &sequence) const
{
    Q_ASSERT(sequence.size() >= mWindowSize + mImageNumber -1 );

    QVector<CMatrix> *result = new QVector<CMatrix>();

    for(int i=0; i < mImageNumber; ++i)
    {
        CMatrix mat(1, mWindowSize);

        for(int j=0; j < mWindowSize; j++)
            mat(0,j) = sequence[i+j];

        result->push_back(mat);
    }

    return QSharedPointer< QVector<CMatrix> >(result);
}


void CNeuralNetwork::learn(const QVector<double> &sequence)
{
    QSharedPointer< QVector<CMatrix> > lVector = createLearningMatrix(sequence);
    QVector<double> eVector = sequence;
    eVector.resize(mWindowSize + mImageNumber);


    double totalError = std::numeric_limits<double>::max();

    while(totalError > mMaxError)
    {
        learnStep(*lVector.data(), eVector, mContextMatrix, mWeightMatrix1, mWeightMatrix2, mLearningCoefficient);

        mIterations++;

        totalError = error(*lVector.data(), eVector, mContextMatrix, mWeightMatrix1, mWeightMatrix2);

    }

    qDebug() << totalError;
    qDebug() << mIterations;
}


void CNeuralNetwork::learnStep(const QVector<CMatrix> &learn, const QVector<double> &etalons, CMatrix &contexMatrix, CMatrix &wM1, CMatrix &wM2, double lCoef) const
{

    Q_ASSERT(learn.size() <= etalons.size());

    for(int i=0; i<learn.size(); i++)
    {
        CMatrix X = arma::join_rows(learn[i], contexMatrix);

        double norm = arma::norm(X,2);

        normalizeMatrix(X);

        CMatrix S1 = X * wM1.t();
        CMatrix Y1 = S1; // Переделать на Y1 = F(S1)
        CMatrix S2 = Y1 * wM2.t();
        CMatrix Y2 = S2; // Переделать на Y1 = F(S1)

        CMatrix gamma2 = Y2 * norm - etalons[i];
        CMatrix gamma1 = gamma2 * wM2;

        CMatrix dW1 = lCoef * gamma1.t() * X;
        CMatrix dW2 = lCoef * gamma2.t() * Y1;

        wM1 -= dW1;
        wM2 -= dW2;

        contexMatrix = Y1;
    }
}



void CNeuralNetwork::normalizeMatrix(CMatrix &matrix) const
{

    double sum;
    for(uint i=0; i<matrix.n_rows; i++)
    {

        sum = arma::norm(matrix.row(i),2);

        matrix.row(i)/=sum;

    }
}



double CNeuralNetwork::error(const QVector<CMatrix> &learn, const QVector<double> &etalons, CMatrix &contexMatrix, CMatrix &wM1, CMatrix &wM2) const
{
    Q_ASSERT(learn.size() <= etalons.size());

    double error = 0;

    for(int i=0; i<learn.size(); i++)
    {
        CMatrix X = arma::join_rows(learn[i], contexMatrix);

        double norm = arma::norm(X,2);
        normalizeMatrix(X);

        CMatrix S1 = X * wM1.t();
        CMatrix Y1 = S1; // Переделать на Y1 = F(S1)
        CMatrix S2 = Y1 * wM2.t();
        CMatrix Y2 = S2; // Переделать на Y1 = F(S1)

        CMatrix gamma2 = Y2 * norm - etalons[i];

        error += pow(gamma2(0,0),2);
    }

    return error;

}





