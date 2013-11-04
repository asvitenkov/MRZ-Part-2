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

    CCalculateError *error;

    for(int i=0; i< THREAD_COUNT; i++)
    {
        error = new CCalculateError(&mFirstLayerMatrix, &mSecondLayerMatrix);
        error->setAutoDelete(false);
        mThreatArray.push_back(error);
    }
}



double CNeuralNetwork::learn(const QVector<Segment *> &vector)
{

    int size = vector.size();
    Segment* pSegment;
    Matrix2DF Y;
    Matrix2DF Xs;
    Matrix2DF deltaX;

    for(int i=0; i<size; i++)
    {
        pSegment = vector.at(i);
        Matrix2DF &X = *pSegment->mMatrix;
        Y = X * mFirstLayerMatrix;
        Xs = Y * mSecondLayerMatrix;
        deltaX = Xs - X;

        mFirstLayerMatrix = mFirstLayerMatrix - mAlpha * ( X.t() * deltaX * ( mSecondLayerMatrix.t() ) );

        mSecondLayerMatrix = mSecondLayerMatrix - mAlpha * ( Y.t() * deltaX );

        normalizeMatrix(mFirstLayerMatrix);
        normalizeMatrix(mSecondLayerMatrix);

//        std::cout << mFirstLayerMatrix;
//        std::cout << mSecondLayerMatrix;
    }

    mStep++;

//    int part = size / THREAD_COUNT;


//    CCalculateError *pItem;
//    QThreadPool *threadPool = QThreadPool::globalInstance();

//    for(int i=0; i<THREAD_COUNT; i++)
//    {
//        pItem = mThreatArray.at(i);
//        pItem->mFrom = part * i;
//        pItem->mTo = (i+1) * part;
//        //if(i == THREAD_COUNT -1 )
//        //    pItem->mTo
//        pItem->mSegmentArray = &vector;
//        threadPool->start(pItem);
//    }

//    threadPool->waitForDone();

//    for(int i=0; i<THREAD_COUNT; i++)
//    {
//        error = mThreatArray.at(i)->mError;
//    }


//    for(int i=0; i<size; i++)
//    {
//        pSegment = vector.at(i);
//        Matrix2DF &X = *pSegment->mMatrix;
//        deltaX = (X * mFirstLayerMatrix) * mSecondLayerMatrix - X;
//        error += arma::accu(arma::pow(deltaX,2));
//    }

    //qDebug() << "Error: " <<error;

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


CCalculateError::CCalculateError(Matrix2DF *fMatrix, Matrix2DF *sMatrix)
    : mFrom(0)
    , mTo(0)
    , mSegmentArray(0)
    , mFirstMatrix(fMatrix)
    , mSecondMatrix(sMatrix)
{

}



CCalculateError::~CCalculateError()
{

}

void CCalculateError::run()
{
    mError = 0;
    Matrix2DF deltaX;
    Matrix2DF *X;
    Segment* pSegment;
    for(int i=mFrom; i<mTo; i++)
    {
        pSegment = mSegmentArray->at(i);
        X = pSegment->mMatrix;
        deltaX = (*X * *mFirstMatrix) * *mSecondMatrix - *X;
        mError += arma::accu(arma::pow(deltaX,2));
    }
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
         sum = arma::accu(dMat.col(i));
         for(int j=0; j<matrix.n_rows; j++)
         {
             matrix(j,i)/=sqrt(sum);
         }
    }
}
