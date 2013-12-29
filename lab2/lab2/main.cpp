#include <QApplication>
#include "mainwindow.h"

#include <cblas.h>

//delete
#include <armadillo>
#include "neuralnetwork.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    goto_set_num_threads(1);

//    arma::mat A(3,3);
//    A << 1 << 2<<3<<arma::endr<<5<<arma::endr<<7<<8<<9 <<arma::endr;

//    std::cout <<A;


    CNeuralNetwork network(2, 6, 0.01, 0.0005, 10000);

    QVector<double> seq;
    //for(int i=0; i<10; i++)
    //    seq << i+1;

    seq << 1 << 2 << 3 << 5 << 8 << 13 << 21;

    network.learn(seq);

    MainWindow w;
    w.show();
    
    return a.exec();
}
