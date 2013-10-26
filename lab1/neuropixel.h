#ifndef CNEUROPIXEL_H
#define CNEUROPIXEL_H

class CNeuroPixel
{
public:
    CNeuroPixel();
    CNeuroPixel(double r, double g, double b);
    CNeuroPixel(const CNeuroPixel &copy);
    CNeuroPixel& operator=(const CNeuroPixel &pixel);

    double red() const { return mRed; }
    double green() const { return mGreen; }
    double blue() const { return mBlue; }

private:
    double mRed;
    double mGreen;
    double mBlue;
};

#endif // CNEUROPIXEL_H
