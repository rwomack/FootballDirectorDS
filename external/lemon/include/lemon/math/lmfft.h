//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmfft.h
// Description : Fast Fourier Transform
// Notes       :
//
//  There are two different FFT algorithms that produce the complex
//  conjuagates of each other (the imaginary parts are negated). The only
//  different is the 2PI term being positive or negative. By default we
//  use the positive version. To use the negative version, pass -1 as the sign
//  parameter into the appropriate routines to negate the 2PI term.
//
//  The lmFFT class precalulcates a table for performing the FFT, so it must
//  be instanciated before use. This avoids many sin/cos pairs that must
//  otherwise be performed in the main outer loop of the FFT algo.
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMFFT_H_INCLUDED
#define LMFFT_H_INCLUDED

#include "lmath.h"
#include <complex>          // this code depends on the std::complex class

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmFFT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LMATH_API lmFFT
{
public:

  lmFFT( unsigned int inPow ) : m_pow(inPow), m_size(1<<inPow)
  {
    m_powTable = new std::complex<float>[m_size];
    for (unsigned int i = 0; i<m_size; i++)
    {
      m_powTable[i] = std::complex<float>( lmCos(LM_2PI*float(i)/float(m_size)) , lmSin(LM_2PI*float(i)/float(m_size)) );
    }
  }

  ~lmFFT()
  {
    delete[] m_powTable;
  }

  // fft
  // Perform 1D Fast Fourier Transform
  // In:
  //  data   - complex array of data, of 2^m_pow
  //  stride - stride between elements
  //  sign   - sign of 2*PI*i term
  //
  // Out:
  //  Transforms the data array in-place.
  //
  void fft( std::complex<float> *data, unsigned int stride=1, int sign=1 ) const
  {
    for( int i=0; i<m_size; i++ )
    {
      int j = reverseBits( i, m_pow );
      if( i<j ) std::swap(data[i*stride], data[j*stride]);
    }

    unsigned int bd(1);      // Butterfly delta
    unsigned int bs(2);      // Butterfly step
    unsigned int wm(m_size>>1);  // Unit root multiplier

    std::complex<float> temp;
    unsigned int ci, ni; // Current index, next index in the butterfly

    for( unsigned int level=0; level<m_pow; level++ )
    {
      for( unsigned int i=0; i<m_size; i+=bs )
      {
        for( unsigned int j=0; j<bd; j++ )
        {
          ci       = stride*(i+j);
          ni       = stride*(i+j+bd);
          temp     = unitPow( (sign*j*wm)&(m_size-1) ) * data[ni];

          data[ni] = data[ci] - temp;
          data[ci] += temp;
        }
      }

      bd <<= 1;
      bs <<= 1;
      wm >>= 1;
    }
  }

private:
  std::complex<float> * m_powTable;
  unsigned int          m_pow;
  unsigned int          m_size;

  inline const std::complex<float>& unitPow(unsigned int p) const
  {
    lAssert(0<=p && p<=m_size);
    return m_powTable[p];
  }

  inline unsigned int reverseBits( unsigned int index, unsigned numBits ) const
  {
    unsigned int rev;
    for( int i=rev=0; i < numBits; i++ )
    {
      rev = (rev << 1) | (index & 1);
      index >>= 1;
    }
    return rev;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmFFT2D
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LMATH_API  lmFFT2D
{
public:
  lmFFT2D( unsigned int rowPow, unsigned int colPow ) : m_rowPow(rowPow), m_colPow(colPow), m_fftRow(rowPow), m_fftColumn(colPow)
  { }

  // fft2d
  // Perform 2D Fast Fourier Transform
  // In:
  //  data   - complex array of data, of 2^m_pow x 2^m_pow
  //  sign   - sign of 2*PI*i term
  //
  // Out:
  //  Transforms the data array in-place.
  //
  void fft2d(std::complex<float>* data, int sign=1)
  {
    unsigned int rows = 1<<m_rowPow;
    unsigned int columns = 1<<m_colPow;
    unsigned int i;

    std::complex<float>* ptr = data;

    for (i = 0; i<rows; ++i)
    {
      m_fftColumn.fft(ptr, 1, sign);
      ptr += columns;
    }

    ptr = data;
    for (i = 0; i<columns; ++i)
    {
      m_fftRow.fft(ptr, columns, sign);
      ptr++;
    }
  }

private:
  unsigned int m_rowPow;
  unsigned int m_colPow;
  lmFFT        m_fftRow;
  lmFFT        m_fftColumn;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmFFTAMD - AMD optimised FFT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __AMD__

//#include <amd3dx.h>
#include <mm3dnow.h>
#define MM64(_x) *((__m64*)&(_x))

#ifdef L_COMPILER_MSVC
 #pragma warning(disable : 4700 ) // local variable 'x' used without having been initialized
#endif

class LMATH_API lmFFT_AMD
{
public:
    lmFFT_AMD(unsigned int inPow) : m_pow(inPow)
    {
      w = new COMPLEX[1<<inPow];

      COMPLEX *pcW;
      float  cos_sin[2], w_real, w_imag, wrecur_real, wrecur_imag, wtemp_real;
      int j, le, n;

      n = 1 << inPow;
      le = n/2;

      //_sincos ((float)(PI/le), cos_sin);
      cos_sin[0] = lmCos( (float)(LM_PI/le) );
      cos_sin[1] = lmSin( (float)(LM_PI/le) );

      wrecur_real = w_real =  cos_sin[0];  // cosine
      //wrecur_imag = w_imag = -cos_sin[1];  // -sine // TimJ: we want the other sign.
      wrecur_imag = w_imag = cos_sin[1];  // -sine
      pcW = w;
      for (j = 1; j < le; j++)
      {
        pcW->real = wrecur_real;
        pcW->imag = wrecur_imag;
        pcW++;
        wtemp_real = wrecur_real*w_real - wrecur_imag*w_imag;
        wrecur_imag = wrecur_real*w_imag + wrecur_imag*w_real;
        wrecur_real = wtemp_real;
      }
    }

    ~lmFFT_AMD()
    {
      delete w;
    }

    void fft(std::complex<float>* data) const
    {
      COMPLEX *x =reinterpret_cast<COMPLEX*>(data);

      static int n = 1;
      static const float pm[] = {+1,-1};

      COMPLEX u,ux;
      COMPLEX *xi,*xip, *xj, *wptr;
      float divisor, temp;
      register int i,j;
      int k,l,le,windex;

      n = 1 << m_pow;
      le = n;
      windex = 1;

      __m64 mm6 = MM64( pm[0] );

      for (l = 0; l < m_pow; l++)
      {
        le >>= 1;

        for (i = 0; i < n; i += 2*le)
        {
          xi = &x[i];
          xip = &xi[le];

          __m64 mm0 = MM64(*xi);
          __m64 mm1 = MM64(*xip);
          MM64(*xi)  = _m_pfadd( mm0, mm1 );
          MM64(*xip) = _m_pfsub( mm0, mm1 );
        }

        wptr = &w[windex - 1];
        for (j = 1; j < le; j++)
        {
          u = *wptr;
          ux.real = wptr->imag;
          ux.imag = wptr->real;

          __m64 mm7 = MM64(ux);

          for (i = j; i < n; i += 2*le)
          {
            xi = x + i;
            xip = xi + le;

            __m64 mm0 = MM64(*xi);
            __m64 mm1 = MM64(*xip);

            MM64(*xi) = _m_pfadd( mm0, mm1 );
            mm0       = _m_pfsub( mm0, mm1 );

            __m64 mm3 = MM64(u);

            mm3 = _m_pfmul( mm3, mm0 );
            mm3 = _m_pfmul( mm3, mm6 );
            mm3 = _m_pfacc( mm3, mm3 );

            xip->real = _m_to_float(mm3);

            mm0 = _m_pfmul( mm0, mm7 );
            mm0 = _m_pfacc( mm0, mm0 );

            xip->imag = _m_to_float(mm0);
          }
          wptr += windex;
        }
        windex <<= 1;
      }

      j = 0;
      for (i = 1; i < (n-1); i++)
      {
        k = n >> 1;
        while (k <= j)
        {
          j -= k;
          k >>= 1;
        }
        j += k;
        if (i < j)
        {
          xi = &x[i];
          xj = &x[j];

          __m64 mm0 = MM64(*xi);
          __m64 mm1 = MM64(*xj);
          MM64(*xj) = mm0;
          MM64(*xi) = mm1;
        }
      }

      _m_femms();

      // correct adjust for inverse fft (JBN 990428)
      //if (forward == -1)
      //{
      //  divisor = -1.0f / (float)n;
      //  for (i = 0; i < n; i++)
      //  {
      //    temp = x[i].real*divisor;
      //    x[i].real = x[i].imag*divisor;
      //    x[i].imag = temp;
      //  }
      //}
    }


private:

    struct COMPLEX
    {
      float real;
      float imag;
    };

    int m_pow;
    COMPLEX *w;
};

class LMATH_API  lmFFT2D_AMD
{
public:
  lmFFT2D_AMD( unsigned int rowPow, unsigned int colPow ) : m_rowPow(rowPow), m_colPow(colPow), m_fftRow(rowPow), m_fftColumn(colPow)
  {
    xx = new std::complex<float>[1<<m_rowPow];
  }

  void fft2d(std::complex<float>* data, int sign=1)
  {
    unsigned int rows = 1<<m_rowPow;
    unsigned int columns = 1<<m_colPow;
    unsigned int i;

    std::complex<float>* ptr = data;

    for (i = 0; i<rows; ++i)
    {
      m_fftColumn.fft(ptr);
      ptr += columns;
    }

    ptr = data;
    for (i = 0; i<columns; ++i)
    {
      for( int j=0;j<rows;j++ )
      {
        xx[j] = ptr[j*columns];
      }

      m_fftRow.fft(xx);

      for( int j=0;j<rows;j++ )
      {
        ptr[j*columns] = xx[j];
      }

      ptr++;
    }
  }

private:
  std::complex<float> * xx;
  unsigned int m_rowPow;
  unsigned int m_colPow;
  lmFFT_AMD  m_fftRow;
  lmFFT_AMD  m_fftColumn;
};

#undef MM64
#ifdef L_COMPILER_MSVC
 #pragma warning(default : 4700 ) // local variable 'x' used without having been initialized
#endif
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmFFT_SSE - SSE FFT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#if defined(L_PLATFORM_XBOX) | defined(__SSE__)

#include <xmmintrin.h>
#ifdef L_COMPILER_MSVC
 #pragma warning(disable : 4700 ) // local variable 'x' used without having been initialized
#endif

class LMATH_API  lmFFT_SSE
{
public:
    lmFFT_SSE(unsigned int inPow) : m_pow(inPow)
    {
      w = new COMPLEX[1<<inPow];

      COMPLEX *pcW;
      float  cos_sin[2], w_real, w_imag, wrecur_real, wrecur_imag, wtemp_real;
      int j, le, n;

      n = 1 << inPow;
      le = n/2;

      //_sincos ((float)(PI/le), cos_sin);
      cos_sin[0] = lmCos( (float)(LM_PI/le) );
      cos_sin[1] = lmSin( (float)(LM_PI/le) );

      wrecur_real = w_real =  cos_sin[0];  // cosine
      //wrecur_imag = w_imag = -cos_sin[1];  // -sine // TimJ: we want the other sign.
      wrecur_imag = w_imag = cos_sin[1];  // -sine
      pcW = w;
      for (j = 1; j < le; j++)
      {
        pcW->real = wrecur_real;
        pcW->imag = wrecur_imag;
        pcW++;
        wtemp_real = wrecur_real*w_real - wrecur_imag*w_imag;
        wrecur_imag = wrecur_real*w_imag + wrecur_imag*w_real;
        wrecur_real = wtemp_real;
      }
    }

    ~lmFFT_SSE()
    {
      delete w;
    }

    void fft(std::complex<float>* data) const
    {
      COMPLEX *x =reinterpret_cast<COMPLEX*>(data);

      static int n = 1;

      __declspec(align(16)) COMPLEX u[2];
      COMPLEX *xi,*xip, *xj, *wptr;
      float divisor, temp;
      register int i,j;
      __m128 xmm0, xmm1, xmm3, xmm4, xmm7;
      int k,l,le,windex;

	  xmm0 = _mm_set_ps(0,0,0,0);
	  xmm1 = _mm_set_ps(0,0,0,0);
	  xmm3 = _mm_set_ps(0,0,0,0);

      n = 1 << m_pow;
      le = n;
      windex = 1;

      for (l = 0; l < m_pow; l++)
      {
        le >>= 1;

        for (i = 0; i < n; i += 2*le)
        {
          xi = &x[i];
          xip = &xi[le];

          xmm0 = _mm_loadl_pi( xmm0, (__m64*)xi );
          xmm1 = _mm_loadl_pi( xmm1, (__m64*)xip );
          _mm_storel_pi( (__m64*)xi, _mm_add_ps( xmm0, xmm1 ) );
          _mm_storel_pi( (__m64*)xip, _mm_sub_ps( xmm0, xmm1 ) );
        }

        wptr = &w[windex - 1];
        for (j = 1; j < le; j++)
        {
          u[0].real  = wptr->real;
          u[0].imag  = -wptr->imag;             // negate 0 in xmm3 below

          u[1].real = wptr->imag;
          u[1].imag = wptr->real;

          // u[1] was ux

          xmm7 = _mm_load_ps( (const float *)&u[0] );

          for (i = j; i < n; i += 2*le)
          {
            xi = &x[i];
            xip = &xi[le];

            xmm0 = _mm_loadl_pi( xmm0, (__m64*)xi );
            xmm1 = _mm_loadl_pi( xmm1, (__m64*)xip );

            xmm4 = _mm_add_ps( xmm0, xmm1 );
            xmm0 = _mm_sub_ps( xmm0, xmm1 );

            xmm3 = _mm_mul_ps( _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE(1,0,1,0) ), xmm7 );
            xmm3 = _mm_add_ps( xmm3, _mm_shuffle_ps( xmm3, xmm3, _MM_SHUFFLE(2,3,0,1) ) );

            _mm_storel_pi( (__m64*)xi, xmm4 );
            _mm_storel_pi( (__m64*)xip, _mm_shuffle_ps( xmm3, xmm3, _MM_SHUFFLE(0,0,2,0) ) );
          }

          wptr += windex;
        }
        windex <<= 1;
      }

      j = 0;
      for (i = 1; i < (n-1); i++)
      {
        k = n >> 1;
        while (k <= j)
        {
          j -= k;
          k >>= 1;
        }
        j += k;
        if (i < j)
        {
          xi = &x[i];
          xj = &x[j];

          xmm0 = _mm_loadl_pi( xmm0, (__m64*)xi );
          xmm1 = _mm_loadl_pi( xmm1, (__m64*)xj );
          _mm_storel_pi( (__m64*)xj, xmm0 );
          _mm_storel_pi( (__m64*)xi, xmm1 );
        }
      }

      // correct adjust for inverse fft (JBN 990428)
      //if (forward == -1)
      //{
      //  divisor = -1.0f / (float)n;
      //  for (i = 0; i < n; i++)
      //  {
      //    temp = x[i].real*divisor;
      //    x[i].real = x[i].imag*divisor;
      //    x[i].imag = temp;
      //  }
      //}
    }

private:

    struct COMPLEX
    {
      float real;
      float imag;
    };

    int m_pow;
    COMPLEX *w;
};

class LMATH_API  lmFFT2D_SSE
{
public:
  lmFFT2D_SSE( unsigned int rowPow, unsigned int colPow ) : m_rowPow(rowPow), m_colPow(colPow), m_fftRow(rowPow), m_fftColumn(colPow)
  {
    xx = new std::complex<float>[1<<m_rowPow];
  }

  void fft2d(std::complex<float>* data, int sign=1)
  {
    unsigned int rows = 1<<m_rowPow;
    unsigned int columns = 1<<m_colPow;
    unsigned int i;

    std::complex<float>* ptr = data;

    for (i = 0; i<rows; ++i)
    {
      m_fftColumn.fft(ptr);
      ptr += columns;
    }

    ptr = data;
    for (i = 0; i<columns; ++i)
    {
      for( int j=0;j<rows;j++ )
      {
        xx[j] = ptr[j*columns];
      }

      m_fftRow.fft(xx);

      for( int j=0;j<rows;j++ )
      {
        ptr[j*columns] = xx[j];
      }

      ptr++;
    }
  }

private:
  std::complex<float> * xx;
  unsigned int m_rowPow;
  unsigned int m_colPow;
  lmFFT_SSE  m_fftRow;
  lmFFT_SSE  m_fftColumn;
};

#ifdef L_COMPILER_MSVC
 #pragma warning(default : 4700 ) // local variable 'x' used without having been initialized
#endif

#endif




END_L_NAMESPACE

#endif
