#ifndef CIMAGE_H
#define CIMAGE_H

#include <armadillo>


typedef arma::mat Matrix2DF;

class CNeuroPixel;
class QImage;

class CImage
{
public:
    CImage(const Matrix2DF &matrix);
    CImage(const CImage &copy);
    CImage& operator=(const CImage &other);

    static CImage fromImage(QImage &from);
    int rows() const { return mMatrix.n_rows; }
    int cols()  const{ return mMatrix.n_cols; }
    CNeuroPixel color(int x, int y) const;


    //QImage toImage();

private:
    CImage();
    static double encodeColor(int color);

    Matrix2DF mMatrix;
};

#endif // CIMAGE_H
