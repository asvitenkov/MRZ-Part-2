#include "neuralnetwork.h"

#include <QDebug>

CNeuralNetwork::CNeuralNetwork(int segmantLength, int secondLayerCount)
    : mSegmentLength(segmantLength)
    , mSecondLayerCount(secondLayerCount)
{
    initialize();
}


CNeuralNetwork::~CNeuralNetwork()
{

}

void CNeuralNetwork::initialize()
{
    mFirstLayerMatrix = arma::randu<arma::mat>(mSegmentLength, mSecondLayerCount) * 2 - 1;
    mSecondLayerMatrix = arma::randu<arma::mat>(mSecondLayerCount, mSegmentLength) * 2 - 1;
    //std::cout << mFirstLayerMatrix << "\n";
    //std::cout << mSecondLayerMatrix << "\n";
}



double CNeuralNetwork::learn(const QVector<Segment *> &vector)
{

    //std::cout << mFirstLayerMatrix.n_rows << " " << mFirstLayerMatrix.n_cols << "\n";
    //std::cout << mFirstLayerMatrix << "\n\n\n";

    int size = vector.size();

    for(int i=0; i<size; i++)
    {
        Segment* pSegment = vector.at(i);
        Matrix2DF &X = *pSegment->mMatrix;
        Matrix2DF Y = X * mFirstLayerMatrix;
        Matrix2DF Xs = Y * mSecondLayerMatrix;
        Matrix2DF deltaX = Xs - X;

        double alphaFirst = 1.0 / arma::accu(arma::pow(Xs,2));
        double alphaSecond = 1.0 / arma::accu(arma::pow(Y,2));


        mFirstLayerMatrix = mFirstLayerMatrix - (alphaFirst * X.t()) * deltaX * ( mSecondLayerMatrix.t() );

        mSecondLayerMatrix = mSecondLayerMatrix - alphaSecond * Y.t() * deltaX;

    }

    double error = 0;

    for(int i=0; i<size; i++)
    {
        Segment* pSegment = vector.at(i);
        Matrix2DF &X = *pSegment->mMatrix;
        Matrix2DF deltaX = X * mFirstLayerMatrix * mSecondLayerMatrix - X;
        error+=arma::accu(arma::pow(deltaX,2));
    }

    return error;
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
