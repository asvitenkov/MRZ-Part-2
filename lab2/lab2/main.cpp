#include <QApplication>
#include <QThread>
#include "mainwindow.h"

#include <cblas.h>

//delete
#include <QDebug>
#include <QTime>
#include "neuralnetwork.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    goto_set_num_threads(1);

    MainWindow w;

    w.setWindowTitle("MRZ lab2");

    w.show();
    
    return a.exec();

//    int p = 4;
//    int m = 4;

//    QVector<double> vec;
//    for(int i =0; i< m+p; i++)
//        vec << i+1;

//    quint64 it = 0;

//    CNeuralNetwork network(p,m,0.1);

//    QTime start = QTime::currentTime();

//    network.initSequences(vec);

//    while(it<200000)
//    {
//        it++;
//        network.learn();
//        network.error();
//    }
//    QTime end = QTime::currentTime();

//    qDebug() << start.msecsTo(end);

//    return 0;

}
