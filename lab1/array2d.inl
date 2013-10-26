#include "array2d.h"

template <class T>
CArray2D<T>::CArray2D()
    : mData(0)
    , mX(0)
    , mY(0)
{
}


template <class T>
CArray2D<T>::CArray2D(const int &x, const int &y)
{
    mData = 0;
    allocate( x, y );
}


template <class T>
CArray2D<T>::~CArray2D()
{
    deallocate();
}


template <class T>
T CArray2D<T>::get( const int& x, const int& y )
{
    return mData[y][x];
}


template <class T>
void CArray2D<T>::set( const T& t, const int& x, const int& y )
{
    mData[y][x] = t;
}

template <class T>
void CArray2D<T>::setAll( const T& t )
{
    for ( int y = 0; y < mX; y++ )
    {
        for ( int x = 0; x < mY; x++ )
        {
            mData[x][y] = t;
        }
    }
}

template <class T>
void CArray2D<T>::allocate( const int& x, const int& y )
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


template <class T>
void CArray2D<T>::deallocate()
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

template <class T>
T& CArray2D<T>::operator ()(const int &x, const int &y)
{
    return mData[x][y];
}
