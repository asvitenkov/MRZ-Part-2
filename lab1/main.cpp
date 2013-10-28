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

//    mat A = randu<mat>(5,5) * 2 -1;
//    double ac = accu(A);
//    std::cout <<  A;

//    QImage img("/home/aas/ns3.bmp");

//    int ww = 8;
//    int hh = 8;

//    CImage image = CImage::fromImage(img);
//    QVector<Segment*>* vec = image.split(hh,ww);

//    CNeuralNetwork network(ww*hh*3, 3, 0.000001);

//    QTime start = QTime::currentTime();

//    for(int i=0; i< 10000; i++)
//        /*qDebug() << "error " << */network.learn(*vec);
//    QTime end = QTime::currentTime();

//    qDebug() << start.msecsTo(end);

//   QVector<Segment*>* res =  network.process(*vec);

//   CImage result = CImage::unite(ww,hh, img.height(), img.width(),res);

//   QImage resultImg = result.toImage();

//   QLabel *pLbl = new QLabel;
//   pLbl->setPixmap(QPixmap::fromImage(resultImg));


    MainWindow w;
//    w.setCentralWidget(pLbl);
    w.show();
    
    return a.exec();
}
