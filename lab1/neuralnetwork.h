#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <armadillo>
#include "image.h"


typedef arma::mat Matrix2DF;

class CNeuralNetwork
{
public:
    CNeuralNetwork(int segmantLength, int secondLayerCount);
    virtual ~CNeuralNetwork();

    double learn(const QVector<Segment*> &vector);
    QVector<Segment*>* process(const QVector<Segment*> &vector) const ;
private:
    void initialize();


    int mSegmentLength;
    int mSecondLayerCount;

    Matrix2DF mFirstLayerMatrix;
    Matrix2DF mSecondLayerMatrix;

};

#endif // NEURALNETWORK_H
