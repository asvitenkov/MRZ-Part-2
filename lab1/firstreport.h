#ifndef FIRSTREPORT_H
#define FIRSTREPORT_H

#include "neuralnetwork.h"
#include <QFile>
#include <QTextStream>

class FirstReport
{
public:
    FirstReport(int n, int m, double error, double alpha, const QVector<int> &pValues, const QString &img,const QString &outFileName);
    ~FirstReport();

    void run();

private:

    void execute(int n, int m, double error, double alpha, int pValue, const QImage &img, QTextStream &out);



    QVector<int> mPValues;
    double mError;
    int mN;
    int mM;
    double mAlpha;
    QString mOutFileName;
    QFile mFile;
    QString mImgName;
};

#endif // FIRSTREPORT_H
