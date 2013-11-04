#include <QApplication>
#include "mainwindow.h"

#include <iostream>
#include <armadillo>
#include <QTime>
#include "image.h"
#include "neuralnetwork.h"


//delete
#include <QLabel>
#include <QDebug>

using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    mat A = randu<mat>(2,2);
//    A.at(1,0) = 1;
//    A.at(1,1) = 2;
//    A.at(0,1) = 3;
//    A.at(0,0) = 4;



//    std::cout << A ;

//    arma::eps(A.row(0));

//    rowvec vvv = A.row(1).;
//    double sum =  arma::sum(vvv);

//    QImage img("/home/anton/projects/256.png");

//    int ww = 8;
//    int hh = 8;

//    CImage image = CImage::fromImage(img);
//    QVector<Segment*>* vec = image.split(hh,ww);

//    CNeuralNetwork network(ww*hh*3, 12, 0.0005);

//    QTime start = QTime::currentTime();

//    for(int i=0; i< 100; i++)
//    {
//        /*qDebug() << "error " << */network.learn(*vec);
//        //network.getError(*vec);
//    }
//    QTime end = QTime::currentTime();

//    qDebug() << start.msecsTo(end);

    MainWindow w;
    w.show();
    
    return a.exec();
}
