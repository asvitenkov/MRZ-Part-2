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
	, mZeroingLearnContextNeuronsType(None)
	, mZeroingPredictContextNeuronsType(None)
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
    QVector<double> eVector ;//= sequence;
    //eVector.resize(mWindowSize + mImageNumber);
    for(int i=0; i<mImageNumber; i++)
        eVector << sequence[i+mWindowSize];



    double totalError = std::numeric_limits<double>::max();

    double mOldError = totalError;

    while(totalError > mMaxError)
    {
        learn(*lVector.data(), eVector, mContextMatrix, mWeightMatrix1, mWeightMatrix2, mLearningCoefficient);

        mIterations++;

        totalError = error(*lVector.data(), eVector, mContextMatrix, mWeightMatrix1, mWeightMatrix2);



        if(mIterations % 10000 == 0)
        {
            mOldError = totalError;

//            if(qIsNull(mOldError - totalError))
//                mContextMatrix.fill(0);
        }

        if(mIterations % 100000 == 0)
        {
            qDebug() << totalError;
            qDebug() << mIterations;
        }
    }

    qDebug() << totalError;
    qDebug() << mIterations;

    qDebug() << predict(sequence,4);
}


void CNeuralNetwork::learn(const QVector<CMatrix> &learn, const QVector<double> &etalons, CMatrix &contexMatrix, CMatrix &wM1, CMatrix &wM2, double lCoef) const
{

    Q_ASSERT(learn.size() <= etalons.size());


	switch (mZeroingLearnContextNeuronsType)
	{
	case ZeroingFirst:
		if (mIterations == 0)
			contexMatrix.fill(0);
		break;

	case ZeroingAlways:
		contexMatrix.fill(0);
		break;
	}

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



QVector<double> CNeuralNetwork::predict(const QVector<double> &sequence, int count) const
{
    CVector sVector(sequence.size());

    for(int i=0; i<sequence.size(); i++)
        sVector[i] = sequence[i];


    CVector predictVec =  predict(sVector, mContextMatrix, mWeightMatrix1, mWeightMatrix2, mWindowSize, count);

    QVector<double> result;
    for(uint i=0; i<predictVec.n_elem; i++)
        result << predictVec[i];

    return result;
}


CVector CNeuralNetwork::predict(const CVector &sequence, const CMatrix &contexMatrix, const CMatrix &wM1, const CMatrix &wM2, int wSize, int count) const
{
    CVector predict(count);
	predict.fill(0);

	CMatrix cntxtMtr = contexMatrix.t();


    for(int i=0; i<count; i++)
    {

		switch (mZeroingPredictContextNeuronsType)
		{
		case ZeroingFirst:
			if (i)
				cntxtMtr = contexMatrix.t();
			else 
				cntxtMtr.fill(0);
			break;

		case ZeroingAlways:
			cntxtMtr.fill(0);
			break;
		}

        CVector image(wSize);

        if (wSize - i > 0)
        {
            CVector v1 = sequence.subvec(sequence.n_elem - wSize + i, sequence.n_elem-1);
            v1.set_size(wSize - i);

            CVector v2;
			if(i)
				v2 = predict.subvec(0,i-1);

            image = arma::join_cols(v1,v2);
        }
        else
        {
            image = predict.subvec(i - wSize, i-1);
        }

        CMatrix X = arma::join_cols(image, cntxtMtr);

        CMatrix Y1 = wM1 * X;
        CMatrix Y2 = wM2 * Y1;

        predict[i] = Y2(0,0);

    }

    return predict;
}

