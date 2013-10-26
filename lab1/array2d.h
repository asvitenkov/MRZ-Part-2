#ifndef ARRAY2D_H
#define ARRAY2D_H

template <class T>
class CArray2D
{
public:
    CArray2D()
        : mData(0)
        , mX(0)
        , mY(0)
    {
    }


    CArray2D(const int &x, const int &y)
    {
        mData = 0;
        allocate( x, y );
    }


    ~CArray2D()
    {
        deallocate();
    }


    T get( const int& x, const int& y )
    {
        return mData[y][x];
    }


    void set( const T& t, const int& x, const int& y )
    {
        mData[y][x] = t;
    }

    void setAll( const T& t )
    {
        for ( int y = 0; y < mX; y++ )
        {
            for ( int x = 0; x < mY; x++ )
            {
                mData[x][y] = t;
            }
        }
    }


    T& operator ()(const int &x, const int &y)
    {
        return mData[x][y];
    }

private:
    void allocate( const int& x, const int& y )
    {
        // Remember dimensions.
        mX = x;
        mY = y;

        // Allocate.
        mData = new T*[mX];
        for ( int i = 0; i < mX; i++ )
        {
            mData[i] = new T[mY];
        }
    }


    void deallocate()
    {
        if ( 0 == mData )
        {
            // Nothing to do.
            return;
        }

        // Free the memory.
        for ( int i = 0; i < mX; i++ )
        {
            delete[] mData[i];
        }
        delete[] mData;

        // Reset.
        mY = 0;
        mX = 0;
        mData = 0;
    }

private:


    int mX;
    int mY;
    T** mData;
};

#endif // ARRAY2D_H
