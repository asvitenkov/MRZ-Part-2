#include "neuropixel.h"
#include <qmath.h>


CNeuroPixel::CNeuroPixel()
    : mRed(0)
    , mGreen(0)
    , mBlue(0)
{
}

CNeuroPixel::CNeuroPixel(double r, double g, double b)
    : mRed(r)
    , mGreen(g)
    , mBlue(b)
{
    Q_ASSERT(qAbs(r) <= 1);
    Q_ASSERT(qAbs(g) <= 1);
    Q_ASSERT(qAbs(b) <= 1);
}

CNeuroPixel::CNeuroPixel(const CNeuroPixel &copy)
{
    mRed = copy.mRed;
    mGreen = copy.mGreen;
    mBlue = copy.mBlue;
}

CNeuroPixel& CNeuroPixel::operator =(const CNeuroPixel& pixel)
{
    mRed = pixel.mRed;
    mGreen = pixel.mGreen;
    mBlue = pixel.mBlue;

    return *this;
}
