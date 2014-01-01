#include <QApplication>
#include <QThread>
#include "mainwindow.h"

#include <cblas.h>

//delete
#include <armadillo>
#include "worker.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    goto_set_num_threads(1);

//    arma::mat A(3,3);
//    A << 1 << 2<<3<<arma::endr<<5<<arma::endr<<7<<8<<9 <<arma::endr;

//    std::cout <<A;



//    QVector<double> seq;
//    for(int i=0; i<10; i++)
//        seq << i+1;

//    //seq << 1 << 2 << 3 << 5 << 8 << 13 << 21 << 34 << 55 << 89;

//    CWorker worker(3, 6, 0.001, 0.1, 10000000000000);

//    QThread thread;

//    connect(&thread, SIGNAL(started()), &worker, SLOT(process()));
//    thread.wait(100);
//    worker.learn(seq);

    MainWindow w;
    w.show();
    
    return a.exec();
}
