#include <QApplication>
#include "mainwindow.h"

#include <iostream>
#include <QTime>
#include "neuralnetwork.h"

#include <cblas.h>

//delete
#include <QDebug>

using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    mat A = randu<mat>(2,2);
//    A.at(0,0) = 1;
//    A.at(0,1) = 2;
//    A.at(0,2) = 3;

//    A.at(1,0) = 1;
//    A.at(1,1) = 2;
//    A.at(1,2) = 3;

//    A.at(2,0) = 1;
//    A.at(2,1) = 2;
//    A.at(2,2) = 3;

//    std::cout << A / A.t() * A ;

    goto_set_num_threads(1);

    QImage img("/home/anton/projects/256.png");

    int ww = 8;
    int hh = 8;

    CImage image = CImage::fromImage(img);
    QVector<Segment*>* vec = image.split(hh,ww);

    CNeuralNetwork network(ww*hh*3, 12, 0.0000001);

    QTime start = QTime::currentTime();

    for(int i=0; i< 100; i++)
    {
        /*qDebug() << "error " << */network.learn(*vec);
        network.getError(*vec);
    }
    QTime end = QTime::currentTime();

    qDebug() << start.msecsTo(end);

    MainWindow w;
    w.show();
    
    return a.exec();
}
