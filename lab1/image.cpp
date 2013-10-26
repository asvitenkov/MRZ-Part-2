#include "image.h"
#include <QImage>
#include <armadillo>
#include "neuropixel.h"


CImage::CImage()
{

}

CImage::CImage(const Matrix2DF &matrix)
{
    mMatrix = matrix;
}

CImage::CImage(const CImage &copy)
{
    mMatrix = copy.mMatrix;
}

CImage& CImage::operator =(const CImage &other)
{
    mMatrix = other.mMatrix;
    return *this;
}

CNeuroPixel CImage::color(int x, int y) const
{
    return CNeuroPixel(mMatrix(x,y*3), mMatrix(x,y*3+1), mMatrix(x,y*3+2));
}


CImage CImage::fromImage(QImage &from)
{
    Q_ASSERT(!from.isNull());

    if (from.isNull())
        return CImage();

    Matrix2DF matrix(from.height(),from.width()*3);
    for(int i=0; i<from.height();i++)
        for(int j=0; j<from.width(); j++)
        {
            QRgb color = from.pixel(i,j);
            matrix(i,j*3) = encodeColor(qRed(color));
            matrix(i,j*3+1) = encodeColor(qGreen(color));
            matrix(i,j*3+2) = encodeColor(qBlue(color));
        }


    CImage img(matrix);

    return img;
}


double CImage::encodeColor(int color)
{
    //std::cout << color <<std::endl;
    Q_ASSERT(2*color/255 - 1 <=1 &&  2*color/255 - 1 >=-1);
    return 2*color/255 - 1;
}
