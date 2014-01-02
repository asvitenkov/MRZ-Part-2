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

    MainWindow w;

    w.setWindowTitle("MRZ lab2");

    w.show();
    
    return a.exec();
}
