#include <QApplication>
#include "mainwindow.h"

#include <iostream>
#include <QTime>

#include <cblas.h>

//delete
#include <QDebug>

using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    goto_set_num_threads(1);

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
