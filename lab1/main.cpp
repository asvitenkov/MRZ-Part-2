#include <QApplication>
#include "mainwindow.h"

#include <iostream>
#include <armadillo>
#include "array2d.h"
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

    mat A = randu<mat>(5,5) * 2 -1;
    double ac = accu(A);
    std::cout <<  A;

    QImage img("/home/anton/projects/ns3.bmp");

    int ww = 50;
    int hh = 50;

    CImage image = CImage::fromImage(img);
    QVector<Segment*>* vec = image.split(hh,ww);

    CNeuralNetwork network(ww*hh*3, 20);

    for(int i=0; i< 200; i++)
        qDebug() << "error " << network.learn(*vec);

   QVector<Segment*>* res =  network.process(*vec);

   CImage result = CImage::unite(ww,hh, img.height(), img.width(),res);

   QImage resultImg = result.toImage();

   QLabel *pLbl = new QLabel;
   pLbl->setPixmap(QPixmap::fromImage(resultImg));


    MainWindow w;
    w.setCentralWidget(pLbl);
    w.show();
    
    return a.exec();
}
