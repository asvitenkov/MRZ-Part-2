#include "firstreport.h"
#include "image.h"
#include <QImage>

#include <QDebug>

FirstReport::FirstReport(int n, int m, double error, double alpha, const QVector<int> &pValues, const QString &img, const QString &outFileName)
{
    mM = m;
    mN = n;
    mError = error;
    mAlpha = alpha;
    mPValues = pValues;
    mOutFileName = outFileName;
    mImgName = img;


}


FirstReport::~FirstReport()
{

}

void FirstReport::run()
{

    QImage img(mImgName);

    if (img.isNull())
        return;

    mFile.setFileName(mOutFileName);

    mFile.open(QIODevice::WriteOnly);

    QTextStream out(&mFile);

    out << "N, M, P, Alpha, Error, Z, It\n";

    for(int i=0; i<mPValues.size(); i++)
    {
        execute(mN, mM, mError, mAlpha, mPValues[i], img, out);
    }

     mFile.close();

}

void FirstReport::execute(int n, int m, double error, double alpha, int pValue, const QImage &img, QTextStream &out)
{
        CImage image = CImage::fromImage(img);
        QVector<Segment*>* vec = image.split(n,m);

        CNeuralNetwork network(n*m*3, pValue, alpha);

        double e = network.getError(*vec);

        while(e > error)
        {
            network.learn(*vec);
            e = network.getError(*vec);
            //qDebug() << e;
        }

        double z = ( (double) (m*n + vec->size()) * pValue + 2 ) / (m * n * vec->size());


        qDeleteAll(vec->begin(),vec->end());
        //vec->clear();

        delete vec;



        out << QString("%1, %2, %3, %4, %5, %6, %7\n")
                               .arg(QString::number(n))
                               .arg(QString::number(m))
                               .arg(QString::number(pValue))
                               .arg(QString::number(alpha,'f',7))
                               .arg(QString::number(e))
                               .arg(QString::number(z))
                               .arg(QString::number(network.getStep()));
        qDebug() << QString("%1, %2, %3, %4, %5, %6, %7\n")
                               .arg(QString::number(n))
                               .arg(QString::number(m))
                               .arg(QString::number(pValue))
                               .arg(QString::number(alpha,'f',7))
                               .arg(QString::number(e))
                               .arg(QString::number(z))
                               .arg(QString::number(network.getStep()));

}

