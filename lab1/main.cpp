#include <QApplication>
#include "mainwindow.h"

#include <iostream>
#include <armadillo>
#include "array2d.h"
#include "image.h"

using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    mat A = randu<mat>(4,5);
    mat B = randu<mat>(4,5);
    A.n_rows;

    A(1,1) = 1;

    cout << A;;

    QImage img("/mnt/D/Programming/PROJECTS/VS/kbe/sources/plugins/scs/media/icons/error.png");

    CImage image =  CImage::fromImage(img);

    MainWindow w;
    w.show();
    
    return a.exec();
}
