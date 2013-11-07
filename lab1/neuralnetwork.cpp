#include "neuralnetwork.h"

#include <QDebug>
#include <QThreadPool>

CNeuralNetwork::CNeuralNetwork(int segmantLength, int secondLayerCount, double alpha)
    : mSegmentLength(segmantLength)
    , mSecondLayerCount(secondLayerCount)
    , mAlpha(alpha)
    , mStep(0)
{
    Q_ASSERT(mAlpha >0 && mAlpha <= 0.01);
    initialize();
}


CNeuralNetwork::~CNeuralNetwork()
{

}

void CNeuralNetwork::initialize()
{
    mFirstLayerMatrix = arma::randu<Matrix2DF>(mSegmentLength, mSecondLayerCount) * 2 - 1;
    mSecondLayerMatrix = arma::randu<Matrix2DF>(mSecondLayerCount, mSegmentLength) * 2 - 1;

    for(int i=0; i<mFirstLayerMatrix.n_rows; i++)
        for(int j=0; j<mFirstLayerMatrix.n_cols; j++)
            mFirstLayerMatrix(i,j) = 0.02;

    mSecondLayerMatrix = mFirstLayerMatrix.t();

}



double CNeuralNetwork::learn(const QVector<Segment *> &vector)
{

    int size = vector.size();
    Segment* pSegment;
    //Matrix2DF tY = Matrix2DF(1,12);
    //Matrix2DF tXs = Matrix2DF(1,192);
    Matrix2DF &Y = mFirstLayerMatrix;
    Matrix2DF Xs = mFirstLayerMatrix;
    Matrix2DF deltaX = mFirstLayerMatrix;

    for(int i=0; i<size; i++)
    {
        pSegment = vector.at(i);
        Matrix2DF &X = *pSegment->mMatrix;
        Y = X * mFirstLayerMatrix;
        Xs = Y * mSecondLayerMatrix;
        deltaX = Xs - X;


        mFirstLayerMatrix = mFirstLayerMatrix - mAlpha * ( X.t() * deltaX * ( mSecondLayerMatrix.t() ) );

        mSecondLayerMatrix = mSecondLayerMatrix - mAlpha * ( Y.t() * deltaX );

        //qDebug() << mSecondLayerMatrix.n_rows << " " << mSecondLayerMatrix.n_cols;

        //normalizeMatrix(mFirstLayerMatrix);
        //normalizeMatrix(mSecondLayerMatrix);

    }

    mStep++;


    return 0;
}


QVector<Segment*>* CNeuralNetwork::process(const QVector<Segment *> &vector) const
{
    QVector<Segment*> *retVector = new QVector<Segment*>();

    for(int i=0; i<vector.size(); i++)
    {
        Segment &pSeg = *vector.at(i);
        Matrix2DF result = *pSeg.mMatrix * mFirstLayerMatrix * mSecondLayerMatrix;
        retVector->push_back(new Segment(pSeg, new Matrix2DF(result)));
    }

    return retVector;
}


double CNeuralNetwork::getError(const QVector<Segment *> &vector) const
{
    Segment* pSegment;
    Matrix2DF deltaX;
    double error = 0;
    int size = vector.size();
        for(int i=0; i<size; i++)
        {
            pSegment = vector.at(i);
            Matrix2DF &X = *pSegment->mMatrix;
            deltaX = (X * mFirstLayerMatrix) * mSecondLayerMatrix - X;
            error += arma::accu(arma::pow(deltaX,2));
        }

   return error;
}


void CNeuralNetwork::normalizeMatrix(Matrix2DF &matrix)
{

    Matrix2DF dMat = arma::pow(matrix,2);

    double sum;
    for(int i=0; i<matrix.n_cols; i++)
    {
         sum = sqrt(arma::accu(dMat.col(i)));
         for(int j=0; j<matrix.n_rows; j++)
         {
             matrix(j,i)/=sum;
         }
    }
}
