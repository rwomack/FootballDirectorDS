//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmmatrix.h
// Description : Matrix math class
// Notes       :
//
//  Template matrix class for NxM matrices. Where N = rows and M = columns.
//
//  1x3 = [ x,y,z ]
//
//  3x1 = | x |
//        | y |
//        | z |
//
//  NxM = a(i,j) = | a11 ... a1M |
//                 | .        .  |
//                 | .        .  |
//                 | .        .  |
//                 | aN1 ... aNM |
//
//  We use row vectors (1xM) throughout this class, so a translation matrix would be:
//
//  | 1  0  0  0 |
//  | 0  1  0  0 |
//  | 0  0  1  0 |
//  | tx ty tz 0 |
//
//  To use column vectors you can Transpose() the matrix, or transpose the vector.
//
//  Concatentation Notes
//  --------------------
//
//  We have one generic matrix multiplication and two special cases for 1x3 * 4x4
//  and 1x2 * 3x3, which requires an overload for the 1x3 and 1x2 versions of
//  lmMatrix. Mul() is therefore a static member to cope with the specialisation of
//  these two cases. If Mul() were not static the only differences in the function
//  signatures would be the return value, which is not enough, and the special
//  versions would be compiled for all NxM, not just cases for 1x3 and 1x2.
//
//  eg: lmMatrix<1,3> Mul( const lmMatrix<4,4> & ) const
//  vs: static lmMatrix<1,3> Mul( const lmMatrix<3,1> &, const lmMatrix<4,4> & )
//
//  SIMD Notes
//  ----------
//
//  The SIMD specialisation is limited to specific function due to PS2 GNU C++ compiler (2.9-ee-991111)
//  not liking certain template referencing template constructs. eg:
//
//  template< unsigned int N, unsigned int M, typename T, typename TSIMD > struct lmMatrix
//  {
//    template<unsigned int P> inline static lmMatrix<N,P,T> Mul(const lmMatrix<N,M,T> &a, const lmMatrix<M,P,T> &b)
//    {
//      TSIMD::MatrixMul<P>( r.Data(), a.Data(), b.Data() )
//    }
//  }
//
//  Allowing:
//
//  lmMatrix< 4,4,float, SIMD_VU0<4,4,float> > myMatrix1;
//  lmMatrix< 4,4,float, SIMD_SSE<4,4,float> > myMatrix2;
//
//  And specialise members within VU0 or SSE to that the target achitecture can accelerate.
//
//  Instead we define a specific set of common operations and have lmMatrix specialise
//  for the common operations to call the SIMD class. This is not necessarily a bad thing.
//
//  There is an accompanying helper program called GenMatrixCode.cpp which can create the appropriate
//  specialised matrix code for different multiplications to make the job easier and less error prone.
//  see genmatrixcode.cpp
//  and lmsimd.h
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMMATRIX_H_INCLUDED
#define LMMATRIX_H_INCLUDED

#include "lmath.h"
#include "./optimisations/lbase.h"
#include "lmrand.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lmMatrix - Matrix math structure class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
// lmMatrix template structure class (NxM - N rows, M columns)
template <unsigned int N, unsigned int M, typename T,typename TSIMD = lSIMD_Default> struct lmMatrix : public MatrixElements<N,M,T,TSIMD>
{
public:
  // Removed due to ps2 gcc 2.95 problem (ConstIdentity is always zero)
  // Note: Unit test also commented out.
  //static const lmMatrix ConstIdentity;

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  lmMatrix()
  {
    MatrixElements<N,M,T,TSIMD>::InitElements();
  }

  lmMatrix(bool bIdentity)
  {
    MatrixElements<N,M,T,TSIMD>::InitElements();
    if( bIdentity ) Identity();
  }

  lmMatrix(const lmMatrix &m)
  {
    Copy( *this, m );
  }

  inline lmMatrix &operator= (const lmMatrix &m)
  {
    //if (&m != this)
    //{
    Copy( *this, m );
    //}
    return *this;
  }

  //##
	// cast / copy constructor
  // Copy from different T (eg, a float to a double) and different matrix sizes.
  // Vector are zero extended, and matrices are identity extended.
  // Special case for a homogenous R4 vector <x,y,z,w> so w==1 if the vector is extended.

  template<unsigned int P, unsigned int Q, typename R, typename S>
    lmMatrix( const lmMatrix<P,Q,R,S> &m )
		{
      MatrixElements<N,M,T,TSIMD>::InitElements();
      Convert( (*this), m );
    }

  template<unsigned int P, unsigned int Q, typename R, typename S>
  inline static void Convert( lmMatrix<N,M,T,TSIMD> &r, const lmMatrix<P,Q,R,S> &m )
  {
    unsigned int nn = N; if (nn > P) nn = P;
    unsigned int mm = M; if (mm > Q) mm = Q;

    for(unsigned int j=0 ; j< mm ; j++)
      for(unsigned int i=0 ; i< nn ; i++)
        r(i,j) = (T)m(i,j);

    // extend bottom

    for(unsigned int j=0 ; j<M ; j++)
      for(unsigned int i=P ; i<N ; i++)
      {
        if( i==j ) r(i,j) = (T)1;
        else r(i,j) = (T)0;
      }

    // extend right

    for(unsigned int j=Q ; j<M ; j++)
      for(unsigned int i=0 ; i<nn ; i++)
      {
        if( i==j ) r(i,j) = (T)1;
        else r(i,j) = (T)0;
      }

    if( N == 1 && M == 4 && M>P ) r(0,3) = (T)1;
  }

  inline static void Convert( lmMatrix<1,3,float> &r, const lmMatrix<1,4,float> &m )
  {
    TSIMD::Convert1x4To1x3( r.Data(), m.Data() );
  }

  inline static void Convert( lmMatrix<1,4,float> &r, const lmMatrix<1,3,float> &m )
  {
    TSIMD::Convert1x3To1x4( r.Data(), m.Data() );
  }

  inline static void Convert( lmMatrix<3,3,float> &r, const lmMatrix<4,4,float> &m )
  {
    TSIMD::Convert4x4To3x3( r.Data(), m.Data() );
  }

  inline static void Convert( lmMatrix<4,4,float> &r, const lmMatrix<3,3,float> &m )
  {
    TSIMD::Convert3x3To4x4( r.Data(), m.Data() );
  }

  inline static void Copy(lmMatrix<1,3,float> &r, const lmMatrix<1,3,float> &m )
  {
    TSIMD::Vector1x3Copy( r.Data(), m.Data() );
  }

  inline static void Copy(lmMatrix<1,4,float> &r, const lmMatrix<1,4,float> &m )
  {
    TSIMD::Vector1x4Copy( r.Data(), m.Data() );
  }

  inline static void Copy(lmMatrix<3,3,float> &r, const lmMatrix<3,3,float> &m )
  {
    TSIMD::Matrix3x3Copy( r.Data(), m.Data() );
  }

  inline static void Copy(lmMatrix<4,4,float> &r, const lmMatrix<4,4,float> &m )
  {
    TSIMD::Matrix4x4Copy( r.Data(), m.Data() );
  }

  template<unsigned int P, unsigned int O>
  inline static void Copy(lmMatrix<P,O,T,TSIMD> &r, const lmMatrix<P,O,T,TSIMD> &m)
  {
    for(unsigned int i = 0; i<P; i++ )
      for(unsigned int j = 0; j<O; j++ )
        r.At(i,j) = m.At(i,j);
  }

	// value constructors

  lmMatrix(const T &x, const T &y)                         { MatrixElements<N,M,T,TSIMD>::InitElements(); Init(x,y);     }
  lmMatrix(const T &x, const T &y, const T &z)             { MatrixElements<N,M,T,TSIMD>::InitElements(); Init(x,y,z);   }
  lmMatrix(const T &x, const T &y, const T &z, const T &w) { MatrixElements<N,M,T,TSIMD>::InitElements(); Init(x,y,z,w); }

  lmMatrix(const T &a00, const T &a10, const T &a20,
           const T &a01, const T &a11, const T &a21,
           const T &a02, const T &a12, const T &a22 )
  {
    MatrixElements<N,M,T,TSIMD>::InitElements();
    Init(a00,a10,a20, a01,a11,a21, a02,a12,a22 );
  }

  lmMatrix( const T &a00, const T &a10, const T &a20, const T &a30,
            const T &a01, const T &a11, const T &a21, const T &a31,
            const T &a02, const T &a12, const T &a22, const T &a32,
            const T &a03, const T &a13, const T &a23, const T &a33 )
  {
    MatrixElements<N,M,T,TSIMD>::InitElements();
    Init(a00, a10, a20, a30,  a01, a11, a21, a31,  a02, a12, a22, a32,  a03, a13, a23, a33 );
  }

  lmMatrix( const lmMatrix<1,3,T> &rx, const lmMatrix<1,3,T> &ry, const lmMatrix<1,3,T> &rz )
	{
    MatrixElements<N,M,T,TSIMD>::InitElements();
    Init(rx, ry, rz);
	}

  lmMatrix( const lmMatrix<1,3,T> &rx, const lmMatrix<1,3,T> &ry, const lmMatrix<1,3,T> &rz, const lmMatrix<1,3,T> &rw )
	{
    MatrixElements<N,M,T,TSIMD>::InitElements();
    Init(rx, ry, rz, rw);
	}

  inline lmMatrix( const lmMatrix<1,3,T> &pos, const lmMatrix<3,3,T> &rot )
  {
    MatrixElements<N,M,T,TSIMD>::InitElements();
    Init( pos, rot );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Initialisers
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
	// Initialisers for R2,3 and 4 vectors

  inline void Init(const T &x, const T &y)                          { lTAssert( M == 2 && N == 1 ); (*this)(0) = x; (*this)(1) = y; }
  inline void Init(const T &x, const T &y, const T &z)              { lTAssert( M == 3 && N == 1 ); (*this)(0) = x; (*this)(1) = y; (*this)(2) = z; }
  inline void Init(const T &x, const T &y, const T &z, const T &w)
  {
    lTAssert( (M == 4 && N == 1) || (N == 2 && M == 2) );

    if( M == 4 && N == 1 )
    {
      // R4 vector init

      (*this)(0) = x; (*this)(1) = y; (*this)(2) = z; (*this)(3) = w;
    }
    else
    {
      // R2 matrix init

      (*this)(0,0) = x; (*this)(0,1) = y;
      (*this)(1,0) = z; (*this)(1,1) = w;
    }
  }

  //##
	// Initialisers for R2,3 and 4 matrices

  inline void Init( const T &a00, const T &a01, const T &a02,
                    const T &a10, const T &a11, const T &a12,
                    const T &a20, const T &a21, const T &a22 )
  {
    lTAssert( N == 3 && M == 3 );
    (*this)(0,0) = a00; (*this)(0,1) = a01; (*this)(0,2) = a02;
    (*this)(1,0) = a10; (*this)(1,1) = a11; (*this)(1,2) = a12;
    (*this)(2,0) = a20; (*this)(2,1) = a21; (*this)(2,2) = a22;
  }

  inline void Init( const T &a00, const T &a01, const T &a02, const T &a03,
                    const T &a10, const T &a11, const T &a12, const T &a13,
                    const T &a20, const T &a21, const T &a22, const T &a23,
                    const T &a30, const T &a31, const T &a32, const T &a33 )
  {
    lTAssert( N == 4 && M == 4 );
    (*this)(0,0) = a00; (*this)(0,1) = a01; (*this)(0,2) = a02; (*this)(0,3) = a03;
    (*this)(1,0) = a10; (*this)(1,1) = a11; (*this)(1,2) = a12; (*this)(1,3) = a13;
    (*this)(2,0) = a20; (*this)(2,1) = a21; (*this)(2,2) = a22; (*this)(2,3) = a23;
    (*this)(3,0) = a30; (*this)(3,1) = a31; (*this)(3,2) = a32; (*this)(3,3) = a33;
  }

  //##
  // Initialise the sub 3x3 of any matrix with three R3 row vectors, the rest is identity.

  inline void Init( const lmMatrix<1,3,T> &rx, const lmMatrix<1,3,T> &ry, const lmMatrix<1,3,T> &rz )
	{
    lTAssert(N>=3 && M>=3);
    Identity(); // the compiler should to optimise out the redundant writes
    (*this)(0,0) = rx(0); (*this)(0,1) = rx(1); (*this)(0,2) = rx(2);
    (*this)(1,0) = ry(0); (*this)(1,1) = ry(1); (*this)(1,2) = ry(2);
    (*this)(2,0) = rz(0); (*this)(2,1) = rz(1); (*this)(2,2) = rz(2);
	}

  //##
	// Initialise the sub 4x4 any matrix with four R3 row vectors, the rest is identity.

  inline void Init( const lmMatrix<1,3,T> &rx, const lmMatrix<1,3,T> &ry, const lmMatrix<1,3,T> &rz, const lmMatrix<1,3,T> &rw )
	{
    lTAssert(N>=4 && M>=3);
    Identity(); // the compiler should to optimise out the redundant writes
    (*this)(0,0) = rx(0); (*this)(0,1) = rx(1); (*this)(0,2) = rx(2);
    (*this)(1,0) = ry(0); (*this)(1,1) = ry(1); (*this)(1,2) = ry(2);
    (*this)(2,0) = rz(0); (*this)(2,1) = rz(1); (*this)(2,2) = rz(2);
    (*this)(3,0) = rw(0); (*this)(3,1) = rw(1); (*this)(3,2) = rw(2);
	}

  //##
  // Initialise the sub 4x3 of any matrix with one R1 row vector and one R3 matrix

  inline void Init( const lmMatrix<1,3,T> &pos, const lmMatrix<3,3,T> &rot )
  {
    lTAssert(N>=4 && M>=3);
    if( N==4 && M==3 )
    {
      // special case
      memcpy( Data(), rot.Data(), sizeof(T)*9 );
      (*this)(3,0) = pos(0);
      (*this)(3,1) = pos(1);
      (*this)(3,2) = pos(2);
    }
    else
    {
      // set to identity since the destination is not a 4x3
      // the compiler should *hopefully* optimise out the redundant writes
      Identity();
      SetTranslate( pos );
      Set3x3( rot );
    }
  }

  //##
  // Clear a matrix

  inline void Zero()
  {
    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) = (T)0;
  }

  //##
  // Make a matrix identity

  inline void Identity()
  {
    if( N == 3 && M == 3 )
    {
      TSIMD::Matrix3x3Identity( this->Data() );
    }
    else if( N == 4 && M == 4 )
    {
      TSIMD::Matrix4x4Identity( this->Data() );
    }
    else
    {
      for( unsigned int j=0; j<M; j++)
      {
        for (unsigned int i=0; i<N; i++ )
        {
          if( i==j ) (*this)(i,j) = (T)1;
          else (*this)(i,j) = (T)0;
        }
      }
    }
  }

  //##
  // Get an identity matrix

  static const lmMatrix GetIdentity()
  {
    #ifdef L_PLATFORM_PS2
    lmMatrix m(true);
    return m;
    #else
    static lmMatrix m(true);
    return m;
    #endif
  }

  //##
  // Returns a 32bit hash value for this matrix
  inline int GetHash()
  {
    int hashVal = 0, shiftStep = 32/M*N, curshift = 0;
    for( unsigned int j=0; j<M; j++)
    {
      for (unsigned int i=0; i<N; i++ )
      {
        hashVal |= (int)(*this)(i,j) << curshift;
        curshift += shiftStep;
      }
    }
    return hashVal;
  }

  //##
  // Function to quickly Orthonomal matrix Inverse the current matrix
  // and return the translation vector, useful for getting view position from
  // a view matrix for example
  inline lmMatrix<1,3,T> GetTranslateFromViewMatrix() const
  {
    lmMatrix<1,3,T> pos;

    // compute inverse of translation vector

    T t0 = -(*this)(3,0);
    T t1 = -(*this)(3,1);
    T t2 = -(*this)(3,2);

    pos(0) = (t0 * (*this)(0,0)) + (t1 * (*this)(1,0)) + (t2 * (*this)(2,0));
    pos(1) = (t0 * (*this)(0,1)) + (t1 * (*this)(1,1)) + (t2 * (*this)(2,1));
    pos(2) = (t0 * (*this)(0,2)) + (t1 * (*this)(1,2)) + (t2 * (*this)(2,2));

    return pos;
	}


  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Utility logging
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Print a matrix to the log

  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( N == 1  &&  str!=NULL ) lLogFmt("%s: ", str );
    else if( str!=NULL ) lLogFmt("%s:\n", str );
    for(unsigned int i=0;i<N;i++)
    {
      for(unsigned int j=0;j<M;j++)
      {
        lLogFmt("%6.3f ", (double)(*this)(i,j) );
      }
      lLogFmt("\n");
    }
  }
  #endif

  inline void Logf(const char * str = NULL) const
  {
    if( N == 1  &&  str!=NULL ) lLogFmt("%s: ", str );
    else if( str!=NULL ) lLogFmt("%s:\n", str );
    for(unsigned int i=0;i<N;i++)
    {
      for(unsigned int j=0;j<M;j++)
      {
        lLogFmt("%6.3f ", (double)(*this)(i,j) );
      }
      lLogFmt("\n");
    }
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Matrix size

  inline unsigned int Rows()    const { return N; }
  inline unsigned int Columns() const { return M; }
  inline const T *    Data()    const { return this->m_element; }
  inline T *          Data()          { return this->m_element; }

  //##
  // Element references (row,column)

  inline const T &operator () (const unsigned int i, const unsigned int j) const { lAssert(i<N && j<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i,j); }
  inline T       &operator () (const unsigned int i, const unsigned int j)       { lAssert(i<N && j<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i,j); }
  inline const T & At(const unsigned int i, const unsigned int j) const { lAssert(i<N && j<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i,j); }
  inline T       & At(const unsigned int i, const unsigned int j)       { lAssert(i<N && j<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i,j); }

  //##
  // Element references (row) for vectors only

  inline const T &operator () (const unsigned int i) const { lTAssert(N==1); lAssert(i<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i); }
  inline T       &operator () (const unsigned int i)       { lTAssert(N==1); lAssert(i<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i); }
  inline const T & At(const unsigned int i) const { lTAssert(N==1); lAssert(i<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i); }
  inline T       & At(const unsigned int i)       { lTAssert(N==1); lAssert(i<M); return MatrixElements<N,M,T,TSIMD>::ElementRef(i); }

	// Element references for vectors

  inline const  T     &x() const { lTAssert(M > 0 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(0); }
  inline T      &x()             { lTAssert(M > 0 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(0); }
  inline const  T     &y() const { lTAssert(M > 1 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(1); }
  inline T      &y()             { lTAssert(M > 1 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(1); }
  inline const  T     &z() const { lTAssert(M > 2 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(2); }
  inline T      &z()             { lTAssert(M > 2 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(2); }
  inline const  T     &w() const { lTAssert(M > 3 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(3); }
  inline T      &w()             { lTAssert(M > 3 && N == 1); return MatrixElements<N,M,T,TSIMD>::ElementRef(3); }

  //##
  // Template row accessors for matrices, these are sometimes convenient if you don't want to initialise the whole matrix.

	template<unsigned int R>
  inline lmMatrix<1,M,T> GetRow() const
  {
    lTAssert(R<N);
    lmMatrix<1,M,T> r;

    if( N==3 && M==3 )
    {
      if( R == 0 ) TSIMD::Get3x3Row0( r.Data(), (*this).Data() );
      else if( R == 1 ) TSIMD::Get3x3Row1( r.Data(), (*this).Data() );
      else if( R == 2 ) TSIMD::Get3x3Row2( r.Data(), (*this).Data() );
    }
    else if( N == 4 && M == 4 )
    {
      if( R == 0 ) TSIMD::Get4x4Row0( r.Data(), (*this).Data() );
      else if( R == 1 ) TSIMD::Get4x4Row1( r.Data(), (*this).Data() );
      else if( R == 2 ) TSIMD::Get4x4Row2( r.Data(), (*this).Data() );
      else if( R == 3 ) TSIMD::Get4x4Row3( r.Data(), (*this).Data() );
    }
    else
    {
      for( unsigned int i=0; i<M; i++) r(i) = (*this)(R,i);
    }

    return r;
  }

	template<unsigned int R>
    inline void SetRow( const lmMatrix<1,M,T> &v )
		{
      lTAssert(R<N);
      for( unsigned int i=0; i<M; i++) (*this)(R,i) = v(i);
		}

  //##
  // Direct row accessors, these provide run-time bounds checking in debug mode using lAssert.

  inline lmMatrix<1,M,T> GetRowDirect(unsigned int row) const
	{
    lAssert(row<N);
    lmMatrix<1,M,T> r;
    for( unsigned int i=0; i<M; i++) r(i) = (*this)(row,i);
		return r;
	}

  inline void SetRowDirect( unsigned int row, const lmMatrix<1,M,T> &v )
	{
    lAssert(row<N);
    for( unsigned int i=0; i<M; i++) (*this)(row,i) = v(i);
  }

  //##
  // Template column accessors for matrices

  template <unsigned int C>
    inline lmMatrix<N,1,T> GetColumn() const
    {
      lTAssert(C<M);
      lmMatrix<N,1,T> r;
      for( unsigned int i=0; i<N; i++) r(i,0) = (*this)(i,C);
			return r;
    }

  template <unsigned int C>
    inline lmMatrix<1,N,T> GetColumnAsRow() const
    {
      lTAssert(C<M);
      lmMatrix<1,N,T> r;
      for( unsigned int i=0; i<N; i++) r(i) = (*this)(i,C);
			return r;
    }

	template<unsigned int C>
    inline void SetColumn( const lmMatrix<N,1,T> &v )
		{
      lTAssert(C<M);
      for( unsigned int i=0; i<N; i++) (*this)(i,C) = v(i,0);
		}

	template<unsigned int C>
    inline void SetColumnFromRow( const lmMatrix<1,N,T> &v )
		{
      lTAssert(C<M);
      for( unsigned int i=0; i<N; i++) (*this)(i,C) = v(i);
		}


  //##
  // Direct column accessors, these provide run-time bounds checking in debug mode using lAssert.

  inline lmMatrix<N,1,T> GetColumnDirect(unsigned int column) const
	{
    lAssert(column<M);
    lmMatrix<N,1,T> r;
    for( unsigned int i=0; i<N; i++) r(i,0) = (*this)(i,column);
		return r;
	}

  inline lmMatrix<1,N,T> GetColumnAsRowDirect(unsigned int column) const
	{
    lAssert(column<M);
    lmMatrix<1,N,T> r;
    for( unsigned int i=0; i<N; i++) r(i) = (*this)(i,column);
		return r;
	}

  inline void SetColumnDirect( unsigned int column, const lmMatrix<N,1,T> &v )
	{
    lAssert(column<M);
    for( unsigned int i=0; i<N; i++) (*this)(i,column) = v(i,0);
  }

  inline void SetColumnFromRowDirect( unsigned int column, const lmMatrix<1,N,T> &v )
	{
    lAssert(column<M);
    for( unsigned int i=0; i<N; i++) (*this)(i,column) = v(i);
  }


  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Equality operators
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Compare two matrices

  inline bool EqualTo(const lmMatrix &other) const
  {
    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) if( this->At(i,j) != other.At(i,j) ) return false;
    return true;
  }

  //##
  // Compare two matrices using an epsilon value

  inline bool AlmostEqualTo(const lmMatrix &other, const T &tol) const
  {
    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) if( ! lmAlmostEqualTo( this->At(i,j),other.At(i,j),tol ) ) return false;
    return true;
  }

  //##
  // Checks if elements of matrix are finite values or not
  inline bool IsFinite() const
  {
    for( unsigned int i=0;i<N;i++ )
    {
      for(unsigned int j = 0; j<M; j++ )
      {
        if( !lmIsFinite( this->At(i,j) ) )
        {
          return false;
        }
      }
    }
    return true;
  }

  //##
  // Returns true if this matrix is (or is close to) identity
  bool IsIdentity( T tol = LM_EPSILON )
  {
    for( unsigned int j=0; j<M; j++)
    {
      for (unsigned int i=0; i<N; i++ )
      {
        if( i==j )
        {
          if( !lmAlmostEqualTo( this->At(i,j), T(1.0), tol ) ) return false;
        }
        else
        {
          if( !lmAlmostEqualTo( this->At(i,j), T(0.0), tol ) ) return false;
        }
      }
    }

    return true;
  }

  //##
  // Returns true if matrix is symmetric (satisfies Atranspose = A)
  bool IsSymmetric( T tol = LM_EPSILON )
  {
    for( unsigned int j=0; j<M; j++)
    {
      for (unsigned int i=0; i<N; i++ )
      {
        if( i!=j )
        {
          if ( lmFabs( (*this)(i,j) - (*this)(j,i) ) > tol ) return false;
        }
      }
    }

    return true;
  }

  //##
  // Returns true if matrix is diagonal
  bool IsDiagonal( T tol = LM_EPSILON )
  {
    for( unsigned int j=0; j<M; j++)
    {
      for (unsigned int i=0; i<N; i++ )
      {
        if( i!=j )
        {
          if ( lmFabs( (*this)(i,j) ) > tol ) return false;
        }
      }
    }

    return true;
  }

  //##
	// Equality operators

  inline const bool operator== (const lmMatrix& m) const { return EqualTo(m); }
  inline const bool operator!= (const lmMatrix& m) const { return !EqualTo(m); }
  inline const bool operator<  (const lmMatrix& m) const { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ )  if( this->At(i,j) >= m.At(i,j) ) return false; return true; }
  inline const bool operator<= (const lmMatrix& m) const { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ )  if( this->At(i,j) >  m.At(i,j) ) return false; return true; }
  inline const bool operator>  (const lmMatrix& m) const { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ )  if( this->At(i,j) <= m.At(i,j) ) return false; return true; }
  inline const bool operator>= (const lmMatrix& m) const { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ )  if( this->At(i,j) <  m.At(i,j) ) return false; return true; }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Arithmetic
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°


  inline static void Add(lmMatrix<1,3,float,TSIMD> &r, const lmMatrix<1,3,float,TSIMD> &a, const lmMatrix<1,3,float,TSIMD> &b )            { TSIMD::Vector1x3Add( r.Data(), a.Data(), b.Data() ); }
  inline static void Sub(lmMatrix<1,3,float,TSIMD> &r, const lmMatrix<1,3,float,TSIMD> &a, const lmMatrix<1,3,float,TSIMD> &b )            { TSIMD::Vector1x3Sub( r.Data(), a.Data(), b.Data() ); }
  inline static void ScalarScale(lmMatrix<1,3,float,TSIMD> &r, const lmMatrix<1,3,float,TSIMD> &a, float scalar )                          { TSIMD::Vector1x3ScalarScale( r.Data(), a.Data(), scalar ); }
  inline static void ComponentScale(lmMatrix<1,3,float,TSIMD> &r, const lmMatrix<1,3,float,TSIMD> &a, const lmMatrix<1,3,float,TSIMD> &b ) { TSIMD::Vector1x3ComponentScale( r.Data(), a.Data(), b.Data() ); }

  inline static void Add(lmMatrix<1,4,float,TSIMD> &r, const lmMatrix<1,4,float,TSIMD> &a, const lmMatrix<1,4,float,TSIMD> &b )            { TSIMD::Vector1x4Add( r.Data(), a.Data(), b.Data() ); }
  inline static void Sub(lmMatrix<1,4,float,TSIMD> &r, const lmMatrix<1,4,float,TSIMD> &a, const lmMatrix<1,4,float,TSIMD> &b )            { TSIMD::Vector1x4Sub( r.Data(), a.Data(), b.Data() ); }
  inline static void ScalarScale(lmMatrix<1,4,float,TSIMD> &r, const lmMatrix<1,4,float,TSIMD> &a, float scalar )                          { TSIMD::Vector1x4ScalarScale( r.Data(), a.Data(), scalar ); }
  inline static void ComponentScale(lmMatrix<1,4,float,TSIMD> &r, const lmMatrix<1,4,float,TSIMD> &a, const lmMatrix<1,4,float,TSIMD> &b ) { TSIMD::Vector1x4ComponentScale( r.Data(), a.Data(), b.Data() ); }

  template<unsigned int P, unsigned int O>
  inline static void Add(lmMatrix<P,O,T,TSIMD> &r, const lmMatrix<P,O,T,TSIMD> &a, const lmMatrix<P,O,T,TSIMD> &b)
  {
    for( unsigned int i=0;i<P;i++ ) for(unsigned int j = 0; j<O; j++ ) r.At(i,j) = a.At(i,j) + b.At(i,j);
  }

  template<unsigned int P, unsigned int O>
  inline static void Sub(lmMatrix<P,O,T,TSIMD> &r, const lmMatrix<P,O,T,TSIMD> &a, const lmMatrix<P,O,T,TSIMD> &b)
  {
    for( unsigned int i=0;i<P;i++ ) for(unsigned int j = 0; j<O; j++ ) r.At(i,j) = a.At(i,j) - b.At(i,j);
  }

  template<unsigned int P, unsigned int O>
  inline static void ScalarScale(lmMatrix<P,O,T,TSIMD> &r, const lmMatrix<P,O,T,TSIMD> &a, T scalar)
  {
    for( unsigned int i=0;i<P;i++ ) for(unsigned int j = 0; j<O; j++ ) r.At(i,j) = a.At(i,j) * scalar;
  }

  template<unsigned int P, unsigned int O>
  inline static void ComponentScale(lmMatrix<P,O,T,TSIMD> &r, const lmMatrix<P,O,T,TSIMD> &a, const lmMatrix<P,O,T,TSIMD> &b)
  {
    for( unsigned int i=0;i<P;i++ ) for(unsigned int j = 0; j<O; j++ ) r.At(i,j) = a.At(i,j) * b.At(i,j);
  }


  inline const lmMatrix operator+ (const lmMatrix& m)  const { lmMatrix r; Add(r,*this,m); return r; }
  inline const lmMatrix operator+ (const T& v)         const { lmMatrix r; for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) r.At(i,j) = this->At(i,j) + v; return r; }
  inline const lmMatrix operator- (const lmMatrix& m)  const { lmMatrix r; Sub(r,*this,m); return r; }
  inline const lmMatrix operator- (const T& v)         const { lmMatrix r; for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) r.At(i,j) = this->At(i,j) - v; return r; }
  inline const lmMatrix operator* (const T& v)         const { lmMatrix r; ScalarScale( r, *this, v ); return r; }
  inline const lmMatrix operator- ()                   const { lmMatrix r; for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) r.At(i,j) = -this->At(i,j); return r; }
  inline const lmMatrix operator/ (const lmMatrix& m)  const { lmMatrix r; for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) r.At(i,j) = this->At(i,j) / m.At(i,j); return r; }
  inline const lmMatrix operator/ (const T &v)         const { lmMatrix r; for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) r.At(i,j) = this->At(i,j) / v; return r; }

  inline const lmMatrix operator+= (const lmMatrix& m)  { Add(*this,*this,m); return (*this); }
  inline const lmMatrix operator+= (const T& v)         { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) += v; return (*this); }
  inline const lmMatrix operator-= (const lmMatrix& m)  { Sub(*this,*this,m); return (*this); }
  inline const lmMatrix operator-= (const T& v)         { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) -= v; return (*this); }
  inline const lmMatrix operator*= (const T& v)         { ScalarScale( *this, *this, v ); return (*this); }
  inline const lmMatrix operator/= (const lmMatrix& m)  { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) /= m.At(i,j); return (*this); }
  inline const lmMatrix operator/= (const T &v)         { for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) /= v; return (*this); }

  inline const lmMatrix Scale(const lmMatrix& m) const { lmMatrix r; ComponentScale( r, *this, m ); return r; }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Vector operations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Make values of matrix absolute
  inline void Abs()
  {
    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) = lmAbs(this->At(i,j));
  }

  //##
  // Make values of matrix absolute (using fabsf)
  inline void Fabs()
  {
    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) = lmFabs(this->At(i,j));
  }

  //##
  // Dot product functions

  template<unsigned int P, unsigned int O>
  inline static T Dot(const lmMatrix<P,O,T,TSIMD> &a, const lmMatrix<P,O,T,TSIMD> &b)
  {
    T r = (T)0;
    for( unsigned int i=0; i<O; i++ ) r += a.At(i) * b.At(i);
    return r;
  }

  inline static float Dot(const lmMatrix<1,3,float> &a, const lmMatrix<1,3,float> &b ) { return TSIMD::Vector1x3Dot( a.Data(), b.Data() ); }
  inline static float Dot(const lmMatrix<1,4,float> &a, const lmMatrix<1,4,float> &b ) { return TSIMD::Vector1x4Dot( a.Data(), b.Data() ); }

  inline const T Dot(const lmMatrix<1,M,T> &other) const
  {
    return Dot(*this,other);
  }

  //##
  // Normalize matrix / vector, returns length

  template<unsigned int P, unsigned int O>
  inline static T Normalize(lmMatrix<P,O,T,TSIMD> &r, T len )
  {
    T l = r.Length();
    if( l == 0 ) return l;
    T ooLen = len / l;
    for(unsigned int i = 0; i<O; i++ ) r.At(i) *= ooLen;
    return l;
  }

  template<unsigned int P, unsigned int O>
  inline static T Normalize(lmMatrix<P,O,T,TSIMD> &r )
  {
    T l = r.Length();
    if( l == 0 ) return l;
    T ooLen = 1.0f / l;
    for(unsigned int i = 0; i<O; i++ ) r.At(i) *= ooLen;
    return l;
  }

  inline static float Normalize(lmMatrix<1,3,float> &r, float len ) { return TSIMD::Vector1x3NormalizeScale( r.Data(), len ); }
  inline static float Normalize(lmMatrix<1,4,float> &r, float len ) { return TSIMD::Vector1x4NormalizeScale( r.Data(), len ); }

  inline static float Normalize(lmMatrix<1,3,float> &r ) { return TSIMD::Vector1x3Normalize( r.Data() ); }
  inline static float Normalize(lmMatrix<1,4,float> &r ) { return TSIMD::Vector1x4Normalize( r.Data() ); }

  inline const T Normalize(const T &len)
  {
    return Normalize(*this,len);
  }

  inline const T Normalize(void)
  {
    return Normalize(*this);
  }

  inline void Truncate( const T &max )
  {
    if( Length() > max )
    {
      Normalize( max );
    }
  }


  //##
  // Return squared length of vector
  inline const T SqrLength() const
  {
    return Dot((*this));
  }

  //##
  // Return length of vector
  inline const T Length() const
  {
    return lmSqrt(SqrLength());
  }

  //##
  // Return squared distance between this vector and v
  inline const T SqrDistance(const lmMatrix<1,M,T> &v) const
  {
    lmMatrix<1,M,T> temp = (*this)-v;
    return temp.SqrLength();
  }

  //##
  // Return distance between this vector and v
  inline const T Distance(const lmMatrix<1,M,T> &v) const
  {
    lmMatrix<1,M,T> temp = (*this)-v;
    return temp.Length();
  }

  //##
  // Return major axis of this vector
  inline int GetMajorAxis() const
  {
    lTAssert( N == 1 );

    int majorAxis = 0;
    T majorValue = lmFabs( (*this)(0) );

    for (int i = 1; i < M; i++)
    {
      T value = lmFabs( (*this)(i) );
      if ( value > majorValue )
      {
        majorValue = value;
	      majorAxis = i;
	    }
    }

    return majorAxis;
  }

  //##
  // Return minor axis of this vector
  inline int GetMinorAxis() const
  {
    lTAssert( N == 1 );

    int minorAxis = 0;
    T minorValue = lmFabs( (*this)(0) );

    for (int i = 1; i < M; i++)
    {
      T value = lmFabs( (*this)(i) );
      if ( value < minorValue )
      {
        minorValue = value;
	      minorAxis = i;
	    }
    }

    return minorAxis;
  }

  //##
  // Returns angle between two normalized vectors in degrees
  inline T AngleBetween( const lmMatrix<1,M,T> &other )
  {
    return lmAcos( other.Dot( (*this) ) );
  }

  //##
  // Do lerp between 'a' and 'b'
  inline void Lerp( const T &t, const lmMatrix<1,M,T> &a, const lmMatrix<1,M,T> &b )
  {
    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ ) this->At(i,j) = a.At(i,j) + ((b.At(i,j) - a.At(i,j)) * t );
  }

  //##
  // Clamp elements between minBound and maxBound
  inline void Clamp( const T& minBound, const T& maxBound )
  {
    for(unsigned int j = 0; j<M; j++ )
    {
      if( this->At(j) < minBound ) this->At(j) = minBound;
      if( this->At(j) > maxBound ) this->At(j) = maxBound;
    }
  }

  inline void Clamp( const lmMatrix<1,M,T> & minBound, const lmMatrix<1,M,T> & maxBound )
  {
    for(unsigned int j = 0; j<M; j++ )
    {
      if( this->At(j) < minBound(j) ) this->At(j) = minBound(j);
      if( this->At(j) > maxBound(j) ) this->At(j) = maxBound(j);
    }

  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Matrix multiplication
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Multiply (concatenate) two matrices
  // In:
  //   r - resultant matrix
  //   a - first matrix
  //   b - second matrix
  //
  // Matrix multiplication rules:
  //
  // NxM * MxP = NxP
	//
  template<unsigned int P> inline const lmMatrix<N,P,T> operator*  (const lmMatrix<M,P,T>& m) const { lmMatrix<N,P,T> r; lmMatrix<N,M,T>::Mul(r,*this,m); return r; }
  template<unsigned int P> inline void                   operator*= (const lmMatrix<M,P,T>& m)       { lmMatrix<N,P,T> r; lmMatrix<N,M,T>::Mul(r,*this,m); (*this) = r; }

  #if( !defined(_MSC_VER) || (_MSC_VER >= 1300) )

  template<unsigned int P> inline static void Mul(lmMatrix<N,P,T> &r, const lmMatrix<N,M,T> &a, const lmMatrix<M,P,T> &b)
  {
    r.Zero();
    for (unsigned int i = 0; i < N; i++)
      for (unsigned int j = 0; j < M; j++)
        for (unsigned int k = 0; k < P; k++)
          r(i,k) += a(i,j) * b(j,k);
  }

  //##
  // Concatenate a NxM with the *sub* MxP of any matrix
  // NxP = NxM * MxP
  //
  // Specify P as the template parameter.
  // Example, to concatenate a 1x3 with the sub 3x3 of a 4x4 matrix:
  // lmMatrix<4,4,float> m;
  // lmMatrix<1,3,float> p.SubMulT<3>( m );
  //
  // This is currently causing problems on gcc 2.95.
  //
  //template<unsigned int P, unsigned int R, unsigned int S>
  //inline lmMatrix<N,P,T> SubMulT(const lmMatrix<R,S,T> &b) const
  //{
  //  lTAssert( R >= M );
  //  lTAssert( S >= P );
  //
  //  const lmMatrix<N,M,T> &a = (*this);
  //
  //  lmMatrix<N,P,T> r;
  //  r.Zero();
  //  for (unsigned int i = 0; i < N; i++)
  //    for (unsigned int j = 0; j < M; j++)
  //      for (unsigned int k = 0; k < P; k++)
  //        r(i,k) += (*this)(i,j) * b(j,k);
  //
  //  return r;
  //}

	#endif

  //##
  // Concatenate a NxM with the *sub* 3x3 of any matrix
  // Nx3 = NxM * Mx3
  //
  // Example, to concatenate a 1x3 with the sub 3x3 of a 4x4 matrix:
  // lmMatrix<4,4,float> m;
  // lmMatrix<1,3,float> p.SubMul3( m );
  //
  template<unsigned int R, unsigned int S>
  inline lmMatrix<N,3,T> SubMul3(const lmMatrix<R,S,T> &b) const
  {
    lTAssert( R >= M );
    lTAssert( S >= 3 );

    lmMatrix<N,3,T> r;
    r.Zero();
    for (unsigned int i = 0; i < N; i++)
      for (unsigned int j = 0; j < M; j++)
        for (unsigned int k = 0; k < 3; k++)
          r(i,k) += (*this)(i,j) * b(j,k);

    return r;
  }

  //##
  // Concatenate a NxM with the *sub* MxM of any matrix
  // NxM = NxM * MxM
  //
  // Example, to concatenate a 1x3 with the sub 3x3 of a 4x4 matrix:
  // lmMatrix<4,4,float> m;
  // lmMatrix<1,3,float> p.SubMul( m );
  //
  template<unsigned int R, unsigned int S>
  inline lmMatrix<N,M,T> SubMulM(const lmMatrix<R,S,T> &b) const
  {
    lTAssert( R >= M );
    lTAssert( S >= M );

    lmMatrix<N,M,T> r;
    r.Zero();
    for (unsigned int i = 0; i < N; i++)
      for (unsigned int j = 0; j < M; j++)
        for (unsigned int k = 0; k < M; k++)
          r(i,k) += (*this)(i,j) * b(j,k);

    return r;
  }

  // VC7 has a problem with the following specialisations, producing a C4933 warning that
	// causes the functions to be ignored when searching for a specialised version of the function.
  // We can rename the function, write specialised version of Mul() for P (as the VC6 version did)
	// or add an extra parameter to these definitions to remove the C4933 warning.
	// I've opted for the latter as I expect this problem to be fixed in the next VC7 SP.

  #if( _MSC_VER >= 1300 )
  #define _VC7_WORKAROUND , int special=1
  #else
	#define _VC7_WORKAROUND
  #endif

  //##
  // Specialized Vector3 x Matrix4x3 concatenation
  // operator* and operator*= for this function is defined after this class
  // special case of: 1x3 = 1x4 * 4x3 (with imaginary w=1 on incoming vector)

  inline static void Mul(lmMatrix<1,3,float> &r, const lmMatrix<1,3,float> &a, const lmMatrix<4,3,float> &b _VC7_WORKAROUND )
  {
   #if( _MSC_VER >= 1300 )
    L_UNUSED_PARAM( special );          // vc7 workaround, special parameter is unused
   #endif

    TSIMD::Vector1x3MulMatrix4x3( r.Data(), a.Data(), b.Data() );
  }

  //##
  // Specialized Vector3 x Matrix4x4 concatenation
  // operator* and operator*= for this function is defined after this class
  // special case of: 1x3 = 1x4 * 4x3 (with imaginary w=1 on incoming vector)

  inline static void Mul(lmMatrix<1,3,float> &r, const lmMatrix<1,3,float> &a, const lmMatrix<4,4,float> &b _VC7_WORKAROUND )
  {
   #if( _MSC_VER >= 1300 )
    L_UNUSED_PARAM( special );          // vc7 workaround, special parameter is unused
   #endif

    TSIMD::Vector1x3MulMatrix4x4( r.Data(), a.Data(), b.Data() );
  }

  //##
  // Specialized Vector2 Matrix3x3 concatenation
	// operator* and operator*= for this function is defined after this class
  // special case of: 1x2 = 1x3 * 3x2 (with imaginary z=1 on incoming vector)

  inline static void Mul(lmMatrix<1,2,T> &r, const lmMatrix<1,2,T> &a, const lmMatrix<3,3,T> &b _VC7_WORKAROUND )
  {
   #if( _MSC_VER >= 1300 )
    L_UNUSED_PARAM( special );          // vc7 workaround, special parameter is unused
   #endif

    r(0) = (a(0) * b(0,0)) + (a(1) * b(1,0)) + b(2,0);
    r(1) = (a(0) * b(0,1)) + (a(1) * b(1,1)) + b(2,1);
  }

  #undef _VC7_WORKAROUND

  // Specialised SIMD Muls

  //##
  // 4x3 = 4x3 * 4x3
  // operator* and operator*= for this function is defined after this class
  // Special case, treats 4x3's as 4x4's with 0,0,0,1 in the last column
  //
  inline static void Mul(lmMatrix<4,3,float> &r, const lmMatrix<4,3,float> &a, const lmMatrix<4,3,float> &b )
  {
    TSIMD::Matrix4x3Mul( r.Data(), a.Data(), b.Data() );
  }

  //##
  // 3x3 = 3x3 * 3x3
  //
  inline static void Mul(lmMatrix<3,3,float> &r, const lmMatrix<3,3,float> &a, const lmMatrix<3,3,float> &b )
  {
    TSIMD::Matrix3x3Mul( r.Data(), a.Data(), b.Data() );
  }

  //##
  // 4x4 = 4x4 * 4x4
  //
  inline static void Mul(lmMatrix<4,4,float> &r, const lmMatrix<4,4,float> &a, const lmMatrix<4,4,float> &b )
  {
    TSIMD::Matrix4x4Mul( r.Data(), a.Data(), b.Data() );
  }

  //##
  // 1x3 = 1x3 * 3x3
  //
  inline static void Mul(lmMatrix<1,3,float> &r, const lmMatrix<1,3,float> &a, const lmMatrix<3,3,float> &b )
  {
    TSIMD::Vector1x3MulMatrix3x3( r.Data(), a.Data(), b.Data() );
  }

  //##
  // 1x4 = 1x4 * 4x4
  //
  inline static void Mul(lmMatrix<1,4,float> &r, const lmMatrix<1,4,float> &a, const lmMatrix<4,4,float> &b )
  {
    TSIMD::Vector1x4MulMatrix4x4( r.Data(), a.Data(), b.Data() );
  }

  #if( defined(_MSC_VER) )
  #if ( _MSC_VER < 1300 )
  // Visual C/C++ 6 has problems with the general form of the Mul() template, it causes an internal compiler error.
  // We don't support VC6 anymore.
  #error "Microsoft VC6 not supported"
	#endif
  #endif

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Matrix operations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return transpose of a matrix
  //

  inline static void Transpose( lmMatrix<3,3,float> &r, const lmMatrix<3,3,float> &m )
  {
    TSIMD::Matrix3x3Transpose( r.Data(), m.Data() );
  }

  inline static void Transpose(lmMatrix<4,4,float> &r, const lmMatrix<4,4,float> &m )
  {
    TSIMD::Matrix4x4Transpose( r.Data(), m.Data() );
  }

  template<unsigned int P, unsigned int O>
  inline static void Transpose(lmMatrix<P,O,T,TSIMD> &r, const lmMatrix<P,O,T,TSIMD> &m )
  {
    for( unsigned int j=0; j<O; j++)
    {
      for( unsigned int i=0; i<P; i++)
      {
        r(j,i) = m(i,j);
      }
    }
  }

  inline lmMatrix<M,N,T> Transpose() const
  {
    lmMatrix<M,N,T> r;
    Transpose( r, *this );
    return r;
  }

  //##
  // Get the determinant of a matrix
  //
  inline const T Determinant() const
  {
    if( N == 3 && M == 3 )
    {
      const lmMatrix &m = (*this);

      return (T) ( m(0,0) * ( m(1,1)*m(2,2) - m(1,2)*m(2,1) ) +
                   m(0,1) * ( m(1,2)*m(2,0) - m(1,0)*m(2,2) ) +
                   m(0,2) * ( m(1,0)*m(2,1) - m(1,1)*m(2,0) ) );
    }
    else if( N == 4 && M == 4 )
    {
      const lmMatrix &m = (*this);

      return ((m(0,0) * m(1,1) - m(1,0) * m(0,1)) * (m(2,2) * m(3,3) - m(3,2) * m(2,3)) -
              (m(0,0) * m(2,1) - m(2,0) * m(0,1)) * (m(1,2) * m(3,3) - m(3,2) * m(1,3)) +
              (m(0,0) * m(3,1) - m(3,0) * m(0,1)) * (m(1,2) * m(2,3) - m(2,2) * m(1,3)) +
              (m(1,0) * m(2,1) - m(2,0) * m(1,1)) * (m(0,2) * m(3,3) - m(3,2) * m(0,3)) -
              (m(1,0) * m(3,1) - m(3,0) * m(1,1)) * (m(0,2) * m(2,3) - m(2,2) * m(0,3)) +
              (m(2,0) * m(3,1) - m(3,0) * m(2,1)) * (m(0,2) * m(1,3) - m(1,2) * m(0,3)));
    }
    else if( N == 4 && M == 3 )
    {
      const lmMatrix &m = (*this);

      // special case, treat 4x3 as a 4x4 with the last colunm as 0,0,0,1

      return ((m(0,0) * m(1,1) - m(1,0) * m(0,1)) * m(2,2) -
              (m(0,0) * m(2,1) - m(2,0) * m(0,1)) * m(1,2) +
              (m(0,0) * m(3,1) - m(3,0) * m(0,1)) +
              (m(1,0) * m(2,1) - m(2,0) * m(1,1)) * m(0,2) -
              (m(1,0) * m(3,1) - m(3,0) * m(1,1)) +
              (m(2,0) * m(3,1) - m(3,0) * m(2,1)));
    }
    else
    {
      lmMatrix<N,M,T> r;
      T det = GaussianInverse(r);
      const T epsilon = T(1e-12f);
      if( det > -epsilon && det < epsilon ) return 0; // if determinant is nearly zero
      else return det;
    }
  }

  //##
  // Get the Inverse of general 3x3 or 4x4 matrix
  // Out:
  //   r     - out inverted matrix
  //   true  - matrix inverse returned
  //   false - matrix inverse doesn't exist
	//
  inline const bool Inverse(lmMatrix<N,M,T> &r) const
  {
    const lmMatrix &m = (*this);

    if( N == 3 && M == 3 )
    {
      // inverse matrix is cofactor matrix divided by determinant,
      // if the determinant is nonzero.

      T det = Determinant();
      if( det == 0 ) return false; // cannot invert

      // scale it by 1/det

      det = (T(1.0) / det);

      // cofactor matrix for 3x3

      r(0,0) = det * ((m(1,1)*m(2,2))-(m(1,2)*m(2,1)));
      r(1,0) = det * ((m(2,0)*m(1,2))-(m(1,0)*m(2,2)));
      r(2,0) = det * ((m(1,0)*m(2,1))-(m(2,0)*m(1,1)));

      r(0,1) = det * ((m(2,1)*m(0,2))-(m(0,1)*m(2,2)));
      r(1,1) = det * ((m(0,0)*m(2,2))-(m(0,2)*m(2,0)));
      r(2,1) = det * ((m(2,0)*m(0,1))-(m(0,0)*m(2,1)));

      r(0,2) = det * ((m(0,1)*m(1,2))-(m(1,1)*m(0,2)));
      r(1,2) = det * ((m(1,0)*m(0,2))-(m(0,0)*m(1,2)));
      r(2,2) = det * ((m(0,0)*m(1,1))-(m(1,0)*m(0,1)));

      return true;
    }
    else if( N == 4 && M == 4 )
    {
      // inverse matrix is cofactor matrix divided by determinant,
      // if the determinant is nonzero.

      T det = Determinant();
      if( det == 0 ) return false; // cannot invert

      // scale it by 1/det

      det = (T(1.0) / det);

      // cofactor matrix for 4x4 (not exactly nice code)..

      r(0,0) = det * (m(1,1) * (m(2,2) * m(3,3) - m(3,2) * m(2,3)) + m(2,1) * (m(3,2) * m(1,3) - m(1,2) * m(3,3)) + m(3,1) * (m(1,2) * m(2,3) - m(2,2) * m(1,3)));
      r(1,0) = det * (m(1,2) * (m(2,0) * m(3,3) - m(3,0) * m(2,3)) + m(2,2) * (m(3,0) * m(1,3) - m(1,0) * m(3,3)) + m(3,2) * (m(1,0) * m(2,3) - m(2,0) * m(1,3)));
      r(2,0) = det * (m(1,3) * (m(2,0) * m(3,1) - m(3,0) * m(2,1)) + m(2,3) * (m(3,0) * m(1,1) - m(1,0) * m(3,1)) + m(3,3) * (m(1,0) * m(2,1) - m(2,0) * m(1,1)));
      r(3,0) = det * (m(1,0) * (m(3,1) * m(2,2) - m(2,1) * m(3,2)) + m(2,0) * (m(1,1) * m(3,2) - m(3,1) * m(1,2)) + m(3,0) * (m(2,1) * m(1,2) - m(1,1) * m(2,2)));
      r(0,1) = det * (m(2,1) * (m(0,2) * m(3,3) - m(3,2) * m(0,3)) + m(3,1) * (m(2,2) * m(0,3) - m(0,2) * m(2,3)) + m(0,1) * (m(3,2) * m(2,3) - m(2,2) * m(3,3)));
      r(1,1) = det * (m(2,2) * (m(0,0) * m(3,3) - m(3,0) * m(0,3)) + m(3,2) * (m(2,0) * m(0,3) - m(0,0) * m(2,3)) + m(0,2) * (m(3,0) * m(2,3) - m(2,0) * m(3,3)));
      r(2,1) = det * (m(2,3) * (m(0,0) * m(3,1) - m(3,0) * m(0,1)) + m(3,3) * (m(2,0) * m(0,1) - m(0,0) * m(2,1)) + m(0,3) * (m(3,0) * m(2,1) - m(2,0) * m(3,1)));
      r(3,1) = det * (m(2,0) * (m(3,1) * m(0,2) - m(0,1) * m(3,2)) + m(3,0) * (m(0,1) * m(2,2) - m(2,1) * m(0,2)) + m(0,0) * (m(2,1) * m(3,2) - m(3,1) * m(2,2)));
      r(0,2) = det * (m(3,1) * (m(0,2) * m(1,3) - m(1,2) * m(0,3)) + m(0,1) * (m(1,2) * m(3,3) - m(3,2) * m(1,3)) + m(1,1) * (m(3,2) * m(0,3) - m(0,2) * m(3,3)));
      r(1,2) = det * (m(3,2) * (m(0,0) * m(1,3) - m(1,0) * m(0,3)) + m(0,2) * (m(1,0) * m(3,3) - m(3,0) * m(1,3)) + m(1,2) * (m(3,0) * m(0,3) - m(0,0) * m(3,3)));
      r(2,2) = det * (m(3,3) * (m(0,0) * m(1,1) - m(1,0) * m(0,1)) + m(0,3) * (m(1,0) * m(3,1) - m(3,0) * m(1,1)) + m(1,3) * (m(3,0) * m(0,1) - m(0,0) * m(3,1)));
      r(3,2) = det * (m(3,0) * (m(1,1) * m(0,2) - m(0,1) * m(1,2)) + m(0,0) * (m(3,1) * m(1,2) - m(1,1) * m(3,2)) + m(1,0) * (m(0,1) * m(3,2) - m(3,1) * m(0,2)));
      r(0,3) = det * (m(0,1) * (m(2,2) * m(1,3) - m(1,2) * m(2,3)) + m(1,1) * (m(0,2) * m(2,3) - m(2,2) * m(0,3)) + m(2,1) * (m(1,2) * m(0,3) - m(0,2) * m(1,3)));
      r(1,3) = det * (m(0,2) * (m(2,0) * m(1,3) - m(1,0) * m(2,3)) + m(1,2) * (m(0,0) * m(2,3) - m(2,0) * m(0,3)) + m(2,2) * (m(1,0) * m(0,3) - m(0,0) * m(1,3)));
      r(2,3) = det * (m(0,3) * (m(2,0) * m(1,1) - m(1,0) * m(2,1)) + m(1,3) * (m(0,0) * m(2,1) - m(2,0) * m(0,1)) + m(2,3) * (m(1,0) * m(0,1) - m(0,0) * m(1,1)));
      r(3,3) = det * (m(0,0) * (m(1,1) * m(2,2) - m(2,1) * m(1,2)) + m(1,0) * (m(2,1) * m(0,2) - m(0,1) * m(2,2)) + m(2,0) * (m(0,1) * m(1,2) - m(1,1) * m(0,2)));

      return true;
    }
    else if( N == 4 && M == 3 )
    {
      // special case, treat 4x3 as a 4x4 with the last colunm as 0,0,0,1

      // inverse matrix is cofactor matrix divided by determinant,
      // if the determinant is nonzero.

      T det = Determinant();
      if( det == 0 ) return false; // cannot invert

      // scale it by 1/det

      det = (T(1.0) / det);

      // cofactor matrix for 4x4 (not exactly nice code)..

      r(0,0) = det * (m(1,1) * m(2,2)  + m(2,1) * -m(1,2) + m(3,1));
      r(1,0) = det * (m(1,2) * m(2,0)  + m(2,2) * -m(1,0) + m(3,2));
      r(2,0) = det * (m(1,0) * m(2,1)  - m(2,0) * m(1,1));
      r(3,0) = det * (m(1,0) * (m(3,1) * m(2,2) - m(2,1) * m(3,2)) + m(2,0) * (m(1,1) * m(3,2) - m(3,1) * m(1,2)) + m(3,0) * (m(2,1) * m(1,2) - m(1,1) * m(2,2)));
      r(0,1) = det * (m(2,1) * m(0,2)  + m(0,1) * -m(2,2));
      r(1,1) = det * (m(2,2) * m(0,0)  + m(0,2) * -m(2,0));
      r(2,1) = det * (m(2,0) * m(0,1)  - m(0,0) * m(2,1));
      r(3,1) = det * (m(2,0) * (m(3,1) * m(0,2) - m(0,1) * m(3,2)) + m(3,0) * (m(0,1) * m(2,2) - m(2,1) * m(0,2)) + m(0,0) * (m(2,1) * m(3,2) - m(3,1) * m(2,2)));
      r(0,2) = det * (m(0,1) * m(1,2)  + m(1,1) * -m(0,2));
      r(1,2) = det * (m(0,2) * m(1,0)  + m(1,2) * -m(0,0));
      r(2,2) = det * (m(0,0) * m(1,1)  - m(1,0) * m(0,1));
      r(3,2) = det * (m(3,0) * (m(1,1) * m(0,2) - m(0,1) * m(1,2)) + m(0,0) * (m(3,1) * m(1,2) - m(1,1) * m(3,2)) + m(1,0) * (m(0,1) * m(3,2) - m(3,1) * m(0,2)));

      return true;
    }
    else
    {
      // Fall back to guassian inverse
      T det = GaussianInverse(r);
      const T epsilon = T(1e-12f);
      if( det > -epsilon && det < epsilon ) return false;  // if determinant is nearly zero, inversion failed
      return true;
    }
  }

  //##
  // Return the inverse 3x3 or 4x4 matrix, or a zero matrix if inverse is not possible.
	//
  inline lmMatrix Inverse() const
  {
    lmMatrix r;
    if( !Inverse(r) )
    {
      r.Zero();
    }
    return r;
  }

  //##
  // Invert an orthonormal 3x3 or 4x4 matrix
  // In:
  //   r - resultant matrix
	//
  // Note:
  //   In the case of a 4x4 the resulting matrix will have its
  //   end column filled in with (0,0,0,1) regardless of the input
  inline void OrthoInverse(lmMatrix &r) const
  {
    //lTAssert( (N==3 && M==3) || (N==4 && M==4) );

    const lmMatrix &m = (*this);

    if( N == 3 && M == 3 )
    {
      // inverse matrix of an orthonormal 3*3 matrix is just it's transpose
      TSIMD::Matrix3x3OrthoInverse( r.Data(), m.Data() );
    }
    else if( N == 4 && M == 4 )
    {
      TSIMD::Matrix4x4OrthoInverse( r.Data(), m.Data() );
    }
    else if( N == 4 && M == 3 )
    {
      T t0, t1, t2;

      // inverse matrix of an orthonormal 4x4 matrix is just it's transpose
      // in the 3x3 section

      r(0,0)=m(0,0); r(1,0)=m(0,1); r(2,0)=m(0,2);
      r(0,1)=m(1,0); r(1,1)=m(1,1); r(2,1)=m(1,2);
      r(0,2)=m(2,0); r(1,2)=m(2,1); r(2,2)=m(2,2);

      // compute inverse of translation vector

      t0 = -m(3,0); t1 = -m(3,1); t2 = -m(3,2);

      r(3,0) = (t0 * r(0,0)) + (t1 * r(1,0)) + (t2 * r(2,0));
      r(3,1) = (t0 * r(0,1)) + (t1 * r(1,1)) + (t2 * r(2,1));
      r(3,2) = (t0 * r(0,2)) + (t1 * r(1,2)) + (t2 * r(2,2));
    }
    else
    {
      Inverse(r);
    }
  }

  //##
  // Return the inverse of an orthonormal 3x3 or 4x4 matrix
	//
  // Note:
  //   See notes on OrthoInverse( lmMatrix &r ) const
  inline lmMatrix OrthoInverse() const
  {
    lmMatrix r;
    OrthoInverse(r);
    return r;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Axis/Angle conversion operations for 3x3
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //lmMatrix<3,3,T> Slerp(T t, const lmMatrix<3,3,T>& rkR1) const
  //{
  //  lTAssert( N==3 && M==3 );
  //  const lmMatrix<3,3,T>& rkR0 = (*this);
  //
  //  lmMatrix<1,3,T> kAxis;
  //  T fAngle;
  //  lmMatrix<3,3,T> kProd = rkR0.Transpose() * rkR1;
  //  kProd.ToAxisAngle(kAxis,fAngle);
  //
  //  lmMatrix<3,3,T> kR;
  //  kR.FromAxisAngle(kAxis,fT*fAngle);
  //
  //  return kR;
  //}

  //##
  // Convert sub 3x3 to axis/angle
  //
  void ToAxisAngle( lmMatrix<1,3,T>& rkAxis, T & rfRadians ) const
  {
    lTAssert( N>=3 && M>=3 );

    // Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
    // The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
    // I is the identity and
    //
    //       +-        -+
    //   P = |  0 -z +y |
    //       | +z  0 -x |
    //       | -y +x  0 |
    //       +-        -+
    //
    // If A > 0, R represents a counterclockwise rotation about the axis in
    // the sense of looking from the tip of the axis vector towards the
    // origin.  Some algebra will show that
    //
    //   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
    //
    // In the event that A = pi, R-R^t = 0 which prevents us from extracting
    // the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
    // P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
    // z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
    // it does not matter which sign you choose on the square roots.

    const lmMatrix &m_aafEntry = (*this);

    T fTrace = m_aafEntry(0,0) + m_aafEntry(1,1) + m_aafEntry(2,2);
    T fCos = 0.5f*(fTrace-T(1.0));
    rfRadians = lmAcos(fCos);  // in [0,PI]

    if ( rfRadians > T(0.0) )
    {
      if ( rfRadians < LM_PI )
      {
        rkAxis(0) = m_aafEntry(2,1)-m_aafEntry(1,2);
        rkAxis(1) = m_aafEntry(0,2)-m_aafEntry(2,0);
        rkAxis(2) = m_aafEntry(1,0)-m_aafEntry(0,1);
        rkAxis.Normalize();
      }
      else
      {
        // angle is PI
        T fHalfInverse;
        if ( m_aafEntry(0,0) >= m_aafEntry(1,1) )
        {
          // r00 >= r11
          if ( m_aafEntry(0,0) >= m_aafEntry(2,2) )
          {
            // r00 is maximum diagonal term
            rkAxis(0) = 0.5f*lmSqrt(m_aafEntry(0,0) - m_aafEntry(1,1) - m_aafEntry(2,2) + T(1.0));
            fHalfInverse = 0.5f/rkAxis(0);
            rkAxis(1) = fHalfInverse*m_aafEntry(0,1);
            rkAxis(2) = fHalfInverse*m_aafEntry(0,2);
          }
          else
          {
            // r22 is maximum diagonal term
            rkAxis(2) = 0.5f*lmSqrt(m_aafEntry(2,2) - m_aafEntry(0,0) - m_aafEntry(1,1) + T(1.0));
            fHalfInverse = 0.5f/rkAxis(2);
            rkAxis(0) = fHalfInverse*m_aafEntry(0,2);
            rkAxis(1) = fHalfInverse*m_aafEntry(1,2);
          }
        }
        else
        {
          // r11 > r00
          if ( m_aafEntry(1,1) >= m_aafEntry(2,2) )
          {
            // r11 is maximum diagonal term
            rkAxis(1) = 0.5f*lmSqrt(m_aafEntry(1,1) - m_aafEntry(0,0) - m_aafEntry(2,2) + T(1.0));
            fHalfInverse  = 0.5f/rkAxis(1);
            rkAxis(0) = fHalfInverse*m_aafEntry(0,1);
            rkAxis(2) = fHalfInverse*m_aafEntry(1,2);
          }
          else
          {
            // r22 is maximum diagonal term
            rkAxis(2) = 0.5f*lmSqrt(m_aafEntry(2,2) - m_aafEntry(0,0) - m_aafEntry(1,1) + T(1.0));
            fHalfInverse = 0.5f/rkAxis(2);
            rkAxis(0) = fHalfInverse*m_aafEntry(0,2);
            rkAxis(1) = fHalfInverse*m_aafEntry(1,2);
          }
        }
      }
    }
    else
    {
      // The angle is 0 and the matrix is the identity.  Any axis will
      // work, so just use the x-axis.
      rkAxis(0) = T(1.0);
      rkAxis(1) = T(0.0);
      rkAxis(2) = T(0.0);
    }
  }

  //##
  // Set sub 3x3 to rotation matrix based on axis/angle
  //
  void FromAxisAngle( const lmMatrix<1,3,T>& rkAxis, T fRadians)
  {
    lTAssert( N>=3 && M>=3 );
    lmMatrix &m_aafEntry = (*this);

    T fCos = lmCos(fRadians);
    T fSin = lmSin(fRadians);
    T fOneMinusCos = T(1.0)-fCos;
    T fX2 = rkAxis(0)*rkAxis(0);
    T fY2 = rkAxis(1)*rkAxis(1);
    T fZ2 = rkAxis(2)*rkAxis(2);
    T fXYM = rkAxis(0)*rkAxis(1)*fOneMinusCos;
    T fXZM = rkAxis(0)*rkAxis(2)*fOneMinusCos;
    T fYZM = rkAxis(1)*rkAxis(2)*fOneMinusCos;
    T fXSin = rkAxis(0)*fSin;
    T fYSin = rkAxis(1)*fSin;
    T fZSin = rkAxis(2)*fSin;

    m_aafEntry(0,0) = fX2*fOneMinusCos+fCos;
    m_aafEntry(0,1) = fXYM-fZSin;
    m_aafEntry(0,2) = fXZM+fYSin;
    m_aafEntry(1,0) = fXYM+fZSin;
    m_aafEntry(1,1) = fY2*fOneMinusCos+fCos;
    m_aafEntry(1,2) = fYZM-fXSin;
    m_aafEntry(2,0) = fXZM-fYSin;
    m_aafEntry(2,1) = fYZM+fXSin;
    m_aafEntry(2,2) = fZ2*fOneMinusCos+fCos;
  }

  //##
  // Set sub 3x3 to rotation matrix based on 2 axes
  //
  void From2Axes ( const lmMatrix<1,3,T>& a, const lmMatrix<1,3,T>& b )
  {
    lTAssert( N>=3 && M>=3 );
    lmMatrix &R = (*this);
    lmMatrix<1,3,T> temp1, temp2;

    T l,k;

    l = a.Length();
    if (l <= T(0.0))
    {
      return;
    }

    l = T(1.0) / l;
    temp1 = a * l;

    k = temp1.Dot( b );
    temp2 = b - temp1*k;

    l = temp2.Length();
    if (l <= T(0.0))
    {
      return;
    }

    l = T(1.0) / l;
    temp2 *= l;

    R(0,0) = temp1(0);
    R(0,1) = temp1(1);
    R(0,2) = temp1(2);
    R(1,0) = temp2(0);
    R(1,1) = temp2(1);
    R(1,2) = temp2(2);
    R(2,0) = - temp2(1)*temp1(2) + temp1(1)*temp2(2);
    R(2,1) = - temp2(2)*temp1(0) + temp1(2)*temp2(0);
    R(2,2) = - temp2(0)*temp1(1) + temp1(0)*temp2(1);
  }


  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Euler Conversion operations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Set matrix rotation components from euler (radians)
  inline void FromEuler( const T &pitch, const T &yaw, const T &roll )
  {
    lTAssert( M >= 3 && N >= 3 );
    lmMatrix &R = (*this);

    T cosX, sinX, cosY, sinY, cosZ, sinZ;

    lmSinCos( pitch, sinX, cosX );
    lmSinCos( yaw,   sinY, cosY );
    lmSinCos( roll,  sinZ, cosZ );

    T cosXsinY = cosX * sinY;
    T sinXsinY = sinX * sinY;

    R(0,0) =  cosY * cosZ;
    R(0,1) =  sinXsinY * cosZ + cosX * sinZ;
    R(0,2) = -cosXsinY * cosZ + sinX * sinZ;

    R(1,0) = -cosY * sinZ;
    R(1,1) = -sinXsinY * sinZ + cosX * cosZ;
    R(1,2) =  cosXsinY * sinZ + sinX * cosZ;

    R(2,0) =  sinY;
    R(2,1) = -sinX * cosY;
    R(2,2) =  cosX * cosY;
  }

  //##
  // Construct from euler angles
  // In:
  //  v - Vector containing pitch, yaw, roll
  inline void FromEuler( const lmMatrix<1,3,T> &v )
  {
    FromEuler( v(0), v(1), v(2) );
  }

  //##
  // Convert matrix rotation to angles (radians)
  inline void ToEuler( T &pitch, T &yaw, T &roll ) const
  {
    lTAssert( M >= 3 && N >= 3 );

    T sp = lmClamp( (*this)(0, 2), T(-1.0), T(1.0) );
    T theta = -lmAsin( sp );
    T cp = lmCos( theta );

    if ( cp > LM_EPSILON )
    {
      yaw    = theta;
      roll   = lmAtan2( (*this)(0,1), (*this)(0,0) );
      pitch  = lmAtan2( (*this)(1,2), (*this)(2,2) );
    } else
    {
      yaw    = theta;
      roll   = -lmAtan2( (*this)(1,0), (*this)(1,1) );
      pitch  = 0;
    }
  }

  //##
  // Convert matrix rotation to angles (radians)
  inline lmMatrix<1,3,T> ToEuler() const
  {
    lmMatrix<1,3,T> v;
    ToEuler( v(0), v(1), v(2) );
    return v;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // R3 Vector operations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returned this vector reflected about another
  //
  inline lmMatrix<1,3,T> Reflect( const lmMatrix<1,3,T> &n) const
  {
    lTAssert( M == 3 && N == 1 );

    // r = 2n (n dot v) - v

    return ((n*2) * n.Dot((*this))) - (*this);
  }

  //##
  // Return the cross product of this vector and another vector
  //
  inline lmMatrix<1,3,T> Cross(const lmMatrix<1,3,T> &v) const
  {
    lTAssert( M == 3 && N == 1 );

    lmMatrix<1,3,T> r;

    TSIMD::Vector1x3Cross( r.Data(), this->Data(), v.Data() );
    return r;
  }

  inline lmMatrix<1,4,T> Cross(const lmMatrix<1,4,T> &v) const
  {
    lTAssert( M == 4 && N == 1 );

    lmMatrix<1,4,T> r;
    TSIMD::Vector1x4Cross( r.Data(), this->Data(), v.Data() );
    return r;
  }

  //##
	// Rotate a pair of vectors forming a coordinate frame
	//
  // v1    - [in/out] first vector
  // v2    - [in/out] second vector
	// theta - angle
	//
  inline static void RotateBaseVectors( lmMatrix <1,3,T> &v1, lmMatrix <1,3,T> &v2, T theta )
  {
    T s = lmSin(theta),c = lmCos(theta);
    lmMatrix <1,3,T> u1,u2;

    // u1 = v1costheta - v2sintheta
    // u2 = v1sintheta + v2costheta

    u1 = (v1 * c) - (v2 * s);
    u2 = (v1 * s) + (v2 * c);
    v1 = u1;
    v2 = u2;
  }

  //##
  // Return the 3x3 cross product matrix of an R3 vector
	//
  inline lmMatrix<3,3,T> Star()
  {
    lTAssert( M == 3 && N == 1 );

    lmMatrix<3,3,T> r;
    r(0,0) =  0;           r(0,1) =  (*this)(2);  r(0,2) = -(*this)(1);
    r(1,0) = -(*this)(2);  r(1,1) =  0;           r(1,2) =  (*this)(0);
    r(2,0) =  (*this)(1);  r(2,1) = -(*this)(0);  r(2,2) = 0;

    return r;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Matrix operations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return the matirx with the sub 3x3 orthonormalized
  //
  inline lmMatrix<N,M,T> Orthonormalize()
  {
    lTAssert( N>=3 && M>=3 );

    lmMatrix<1,3,T> X( (*this)(0,0), (*this)(0,1), (*this)(0,2) );  // row 0
    lmMatrix<1,3,T> Y( (*this)(1,0), (*this)(1,1), (*this)(1,2) );  // row 1
    lmMatrix<1,3,T> Z;

    X.Normalize();
    Z = X.Cross(Y);
    Z.Normalize();
    Y = Z.Cross(X);
    Y.Normalize();

    lmMatrix<N,M,T> r;
    r = (*this);

    r(0,0) = X(0); r(1,0) = Y(0); r(2,0) = Z(0);
    r(0,1) = X(1); r(1,1) = Y(1); r(2,1) = Z(1);
    r(0,2) = X(2); r(1,2) = Y(2); r(2,2) = Z(2);

    return r;
  }

  //##
  // Translate a matrix (sub 4x3)
	//
  inline void Translate( const lmMatrix<1,3,T> &a )
  {
    lTAssert( N>=4 && M>=3 );

    (*this)(3,0) += a(0);
    (*this)(3,1) += a(1);
    (*this)(3,2) += a(2);
  }

  //##
  // Return the translate part of a matrix (sub 4x3)
	//
  inline lmMatrix<1,3,T> GetTranslate() const
  {
    lTAssert( N>=4 && M>=3 );
    lmMatrix<1,3,T> v;
    if( N == 4 && M == 4 )
    {
      TSIMD::Get4x4Translate( v.Data(), (*this).Data() );
    }
    else
    {
      v(0)=(*this)(3,0);
      v(1)=(*this)(3,1);
      v(2)=(*this)(3,2);
    }
    return v;
  }

  //##
  // Set translate part of a matrix (sub 4x3)
  // In:
  //   v - vector to set translation of matrix
  inline void SetTranslate( const lmMatrix<1,3,T> &v )
  {
    lTAssert( N>=4 && M>=3 );
    (*this)(3,0)=v(0);
    (*this)(3,1)=v(1);
    (*this)(3,2)=v(2);
  }

  //##
  // Scale the sub 3x3 axes of a matrix
  // In:
  //   s - vector to scale axes by
  inline void AxisScale3x3( const lmMatrix<1,3,T> &s )
  {
    lTAssert( N>=3 && M>=3 );
    for( unsigned int i=0; i<3; i++) (*this)(i,i) *= s(i);
  }

  //##
  // Return the absolute axis scale part of a sub 3x3 matrix
  //
  // Notes:
  //   this returns the absolute axis scale,
  //   to see if the matrix flips coordinates use Parity().
	//
  inline lmMatrix<1,3,T> GetAxisScale3x3() const
  {
    lTAssert( N>=3 && M>=3 );
    lmMatrix<1,3,T> v;

    v(0) = lmMatrix<1,3,T>( (*this)(0,0), (*this)(0,1), (*this)(0,2) ).Length();
    v(1) = lmMatrix<1,3,T>( (*this)(1,0), (*this)(1,1), (*this)(1,2) ).Length();
    v(2) = lmMatrix<1,3,T>( (*this)(2,0), (*this)(2,1), (*this)(2,2) ).Length();

    return v;
  }

  //##
  // Get scale parity of a sub 3x3 matrix
  //   Returns true for odd parity otherwise false
	//
  inline bool Parity3x3() const
  {
    const lmMatrix &m = (*this);

    lTAssert( N>=3 && M>=3 );

    T parity;
    lmMatrix<1,3,T> a;

    // parity = ((row0 X row1) dot row2) < 0

    a(0) = (m(0,1) * m(1,2)) - (m(0,2) * m(1,1));
    a(1) = (m(0,2) * m(1,0)) - (m(0,0) * m(1,2));
    a(2) = (m(0,0) * m(1,1)) - (m(0,1) * m(1,0));

    parity = (a(0) * m(2,0) + a(1) * m(2,1) + a(2) * m(2,2));

    if( parity < 0.0 ) return true;

    return false;
  }

  //##
  // Rotate a sub 3x3 matrix of any matrix, or a specific 1x3 vector.
  // Left-handed co-ordinate system, rotations appear clockwise when looking
  // in the same direction as the axis of rotation.
	//
  // Pitch, Yaw, Roll
  inline void Rotate( const lmMatrix<1,3,T> &a )
  {
    lTAssert( (M==3 && N==1) || (N>=3 && M>=3) );

    // For a 1x3 vector

    if( N == 1 && M == 3 )
		{
    	T sx,cx;
    	T sy,cy;
    	T sz,cz;
    	T i,j,k;

      sx=lmSin(a(0));
      cx=lmCos(a(0));

      sy=lmSin(a(1));
      cy=lmCos(a(1));

      sz=lmSin(a(2));
      cz=lmCos(a(2));

      // X rotation

      i = (*this)(0,0);
      j = (((*this)(0,1) * cx) - ((*this)(0,2) * sx)) ;
      k = (((*this)(0,1) * sx) + ((*this)(0,2) * cx)) ;

      // Y rotation

      (*this)(0,0) = ((k * sy) + (i * cy)) ;
      (*this)(0,2) = ((k * cy) - (i * sy)) ;

      // Z rotation

      i = ( ((*this)(0,0) * cz) - ((*this)(0,1) * sz)) ;
      j = ( ((*this)(0,0) * sz) + ((*this)(0,1) * cz)) ;

      (*this)(0,0) = i;
      (*this)(0,1) = j;
		}

		// Otherwise we're rotating the sub 3x3
    // we can't do a lAssert here, as it will fire when we're rotating a vector
    // so we handle it with a lAssertAlways which will be optimised out for any
		// supported case.

		else if( N>=3 && M>=3 )
		{
      lmMatrix<1,3,T> row0( (*this)(0,0), (*this)(1,0), (*this)(2,0) );
      lmMatrix<1,3,T> row1( (*this)(0,1), (*this)(1,1), (*this)(2,1) );
      lmMatrix<1,3,T> row2( (*this)(0,2), (*this)(1,2), (*this)(2,2) );

      // Rotate X vectors

      RotateBaseVectors(row1,row2,a(0));

      // Rotate Y vectors

      RotateBaseVectors(row2,row0,a(1));

    	// Rotate Z vectors

      RotateBaseVectors(row0,row1,a(2));

      (*this)(0,0) = row0(0); (*this)(1,0) = row0(1); (*this)(2,0) = row0(2);
      (*this)(0,1) = row1(0); (*this)(1,1) = row1(1); (*this)(2,1) = row1(2);
      (*this)(0,2) = row2(0); (*this)(1,2) = row2(1); (*this)(2,2) = row2(2);
		}
  }

  //##
  // Return 3x3 sub matrix
  //
  inline lmMatrix<3,3,T> Get3x3() const
  {
    lTAssert( N>=3 && M>=3 );
    lmMatrix<3,3,T> r;

    for( unsigned int j=0; j<3; j++)
    {
      for( unsigned int i=0; i<3; i++)
      {
        r(i,j) = (*this)(i,j);
      }
    }
    return r;
  }

  //##
  // Apply a vector scale to this matrix (3x3 or 4x4)
  // This operation is equivalent to the post concatanation of a scale matrix with the diagonal being (scalex, scaley, scalez, 1)
  void ApplyScale( const lmMatrix<1,3,T> &scale )
  {
    lTAssert( (N==3 && M==3) || (N==4 && M==4) );

    if( N == 3 && M == 3 )
    {
      TSIMD::Matrix3x3ApplyScale( this->Data(), scale.Data() );
    }
    else if( N == 4 && M == 4 )
    {
      TSIMD::Matrix4x4ApplyScale( this->Data(), scale.Data() );
    }
  }

  //##
  // Set 3x3 sub matrix
  //
  inline void Set3x3( const lmMatrix<3,3,T> &m )
  {
    lTAssert( N>=3 && M>=3 );

    for( unsigned int j=0; j<3; j++)
    {
      for( unsigned int i=0; i<3; i++)
      {
        (*this)(i,j) = m(i,j);
      }
    }
  }

  //##
  // Return the 4x4 part of a matrix
  //
  inline lmMatrix<4,4,T> Get4x4() const
  {
    lTAssert( N>=4 && M>=4 );
    lmMatrix<4,4,T> r;

    for( unsigned int j=0; j<4; j++)
    {
      for( unsigned int i=0; i<4; i++)
      {
        r(i,j) = (*this)(i,j);
      }
    }
    return r;
  }

  //##
  // Set 4x4 sub matrix
  // In:
  //   r - resultant matrix
  //   m - 4x4 matrix
  //
  inline void Set4x4( const lmMatrix<4,4,T> &m )
  {
    lTAssert( N>=4 && M>=4 );

    for( unsigned int j=0; j<4; j++)
    {
      for( unsigned int i=0; i<4; i++)
      {
        (*this)(i,j) = m(i,j);
      }
    }
  }

  //##
  // Generate a look at matrix in the sub 3x3 of any matrix, the rest is identity
  // In:
	//   VUP    - the up vector for the view
	//   target - target position to look at, relative to the origin.
  //
  inline void LookAt( const lmMatrix<1,3,T> &VUP, const lmMatrix<1,3,T> &target)
  {
    lTAssert( N>=3 && M>=3 );
    lmMatrix<1,3,T> rx,ry,rz;

    // Calcualate the 3 principle axis vectors for the orthogonal rotation matrix R.

		// rz is the vector extending from the origin to target
		// rz =  target / |target|

    rz = target;
    rz.Normalize();

    // rx is perpendicular to the plane of target and VUP
    // rx = VUP X target / |VUP X target|
    // (Note: VUP is a normal not a vector)

    rx = VUP.Cross(target);
    rx.Normalize();

    // ry is perpendicular to the vectors rz and rx
    // ry = rz X rx / |rz X rx|
    ry = rz.Cross(rx);
    ry.Normalize();

    // Make the rotation matrix

    Init( rx, ry, rz );
  }

  //##
  // Generate a look at matrix in the sub 4x4 of any matrix, the rest is identity
  // In:
  //   VRP    - view reference point, the position of the view
  //   VUP    - the up vector for the view
  //   target - the centre of projection, point to look at
  //
  inline void LookAt(const lmMatrix<1,3,T> &VRP, const lmMatrix<1,3,T> &VUP, const lmMatrix<1,3,T> &target)
  {
    lTAssert( N>=4 && M>=4 );
    LookAt( VUP, lmMatrix<1,3,T>(target-VRP) );
    SetTranslate( VRP );
  }

/*
  //NOTE: 26/12/06 - Function needs testing, seems broken

  //##
  // Generate an approx look at matrix based on vup 0,1,0 in the sub 3x3 of any matrix, the rest is identity
  // In:
	//   target - target position to look at, relative to the origin.
  //
  inline void LookAtApprox( const lmMatrix<1,3,T> &target )
  {
    lTAssert( N>=3 && M>=3 );
    lmMatrix<1,3,T> rx,ry,rz;

    T tempVal = (T(1.0) / lmSqrt5( target.SqrLength() ));

    // Set rz to target and normalize.

    rz(0)  = target(0) * tempVal;
    rz(1)  = target(1) * tempVal;
    rz(2)  = target(2) * tempVal;

    // rx is perpendicular to the plane of target and VUP
    // rx = VUP X target / |VUP X target|
    // (Note: VUP is a normal not a vector)

    // set rx to VUP.Cross( target ), our VUP is 0, 1, 0 - cancels a lot out
    // we also take advantage of that in the Dot for the normalize

    rx(0) = rz(2);
    rx(1) = 0;
    rx(2) = -rz(0);

    tempVal  = rx(0) * rx(0);
    tempVal += rx(2) * rx(2);
    tempVal  = T(1.0) / lmSqrt5( tempVal );

    rx(0) *= tempVal;
    //rx(1) *= tempVal;                 // 0 * x = 0 (doh!)
    rx(2) *= tempVal;

    // ry is perpendicular to the vectors rz and rx
    // ry = rz X rx / |rz X rx|

    // Another cross product, take advantage of rx(1) being 0.. full Dot this time and normalize

    ry(0) = (rz(1)  * rx(2));
    ry(1) = (rz(2) * rx(0)) - (rz(0) * rx(2));
    ry(2) = -(rz(1) * rx(0));

    tempVal  = rx(0) * rx(0);
    tempVal += rx(1) * rx(1);
    tempVal += rx(2) * rx(2);
    tempVal = T(1.0) / lmSqrt5( tempVal );

    ry(0) *= tempVal;
    ry(1) *= tempVal;
    ry(2) *= tempVal;

    // Make the rotation matrix

    Init( rx, ry, rz );
  }
 */

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Interpolation (4x4)
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  lmMatrix<4,4,T> Interp( T fTime, const lmMatrix<4,4,T> &other );
  // define in lmquaternion.h due to dependancy on lmQuaternionT<>

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Singular Value Decomposition (Magic Software)
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  static inline const T GetSVDEpsilon()    { return (T)1e-04f; }  // SVD epsilon
  static const int      m_maxSVDIterations = 32;                  // Maximum SVD iterations

  //##
  // Singular Value Decomposition

  void SingularValueDecomposition3x3( lmMatrix<3,3,T>& kL, lmMatrix<1,3,T>& kS, lmMatrix<3,3,T>& kR) const
  {
    lTAssert( N == 3 && M == 3 );

    int iRow, iCol;

    lmMatrix<3,3,T> kA = (*this);

    Bidiagonalize(kA,kL,kR);

    for (int i = 0; i < m_maxSVDIterations; i++)
    {
      T fTmp, fTmp0, fTmp1;
      T fSin0, fCos0, fTan0;
      T fSin1, fCos1, fTan1;

      bool bTest1 = (lmFabs(kA(0,1)) <= GetSVDEpsilon()*(lmFabs(kA(0,0))+lmFabs(kA(1,1))));
      bool bTest2 = (lmFabs(kA(1,2)) <= GetSVDEpsilon()*(lmFabs(kA(1,1))+lmFabs(kA(2,2))));
      if ( bTest1 )
      {
        if ( bTest2 )
        {
          kS(0) = kA(0,0);
          kS(1) = kA(1,1);
          kS(2) = kA(2,2);
          break;
        }
        else
        {
          // 2x2 closed form factorization
          fTmp = (kA(1,1)*kA(1,1) - kA(2,2)*kA(2,2) + kA(1,2)*kA(1,2))/(kA(1,2)*kA(2,2));
          fTan0 = 0.5f*(fTmp+lmSqrt(fTmp*fTmp + 4.0f));
          fCos0 = lmOOSqrt(T(1.0)+fTan0*fTan0);
          fSin0 = fTan0*fCos0;

          for (iCol = 0; iCol < 3; iCol++)
          {
            fTmp0 = kL(iCol,1);
            fTmp1 = kL(iCol,2);
            kL(iCol,1) = fCos0*fTmp0-fSin0*fTmp1;
            kL(iCol,2) = fSin0*fTmp0+fCos0*fTmp1;
          }

          fTan1 = (kA(1,2)-kA(2,2)*fTan0)/kA(1,1);
          fCos1 = lmOOSqrt(T(1.0)+fTan1*fTan1);
          fSin1 = -fTan1*fCos1;

          for (iRow = 0; iRow < 3; iRow++)
          {
            fTmp0 = kR(1,iRow);
            fTmp1 = kR(2,iRow);
            kR(1,iRow) = fCos1*fTmp0-fSin1*fTmp1;
            kR(2,iRow) = fSin1*fTmp0+fCos1*fTmp1;
          }

          kS(0) = kA(0,0);
          kS(1) = fCos0*fCos1*kA(1,1) - fSin1*(fCos0*kA(1,2)-fSin0*kA(2,2));
          kS(2) = fSin0*fSin1*kA(1,1) + fCos1*(fSin0*kA(1,2)+fCos0*kA(2,2));
          break;
        }
      }
      else
      {
        if ( bTest2 )
        {
          // 2x2 closed form factorization
          fTmp = (kA(0,0)*kA(0,0) + kA(1,1)*kA(1,1) - kA(0,1)*kA(0,1))/(kA(0,1)*kA(1,1));
          fTan0 = 0.5f*(-fTmp+lmSqrt(fTmp*fTmp + 4.0f));
          fCos0 = lmOOSqrt(T(1.0)+fTan0*fTan0);
          fSin0 = fTan0*fCos0;

          for (iCol = 0; iCol < 3; iCol++)
          {
            fTmp0 = kL(iCol,0);
            fTmp1 = kL(iCol,1);
            kL(iCol,0) = fCos0*fTmp0-fSin0*fTmp1;
            kL(iCol,1) = fSin0*fTmp0+fCos0*fTmp1;
          }

          fTan1 = (kA(0,1)-kA(1,1)*fTan0)/kA(0,0);
          fCos1 = lmOOSqrt(T(1.0)+fTan1*fTan1);
          fSin1 = -fTan1*fCos1;

          for (iRow = 0; iRow < 3; iRow++)
          {
            fTmp0 = kR(0,iRow);
            fTmp1 = kR(1,iRow);
            kR(0,iRow) = fCos1*fTmp0-fSin1*fTmp1;
            kR(1,iRow) = fSin1*fTmp0+fCos1*fTmp1;
          }

          kS(0) = fCos0*fCos1*kA(0,0) - fSin1*(fCos0*kA(0,1)-fSin0*kA(1,1));
          kS(1) = fSin0*fSin1*kA(0,0) + fCos1*(fSin0*kA(0,1)+fCos0*kA(1,1));
          kS(2) = kA(2,2);
          break;
        }
        else
        {
          GolubKahanStep(kA,kL,kR);
        }
      }
    }

    // positize diagonal
    for (iRow = 0; iRow < 3; iRow++)
    {
      if ( kS(iRow) < T(0.0) )
      {
        kS(iRow) = -kS(iRow);
        for (iCol = 0; iCol < 3; iCol++)
            kR(iRow,iCol) = -kR(iRow,iCol);
      }
    }
  }

  void SingularValueComposition3x3(const lmMatrix<3,3,T>& kL, const lmMatrix<1,3,T>& kS, const lmMatrix<3,3,T>& kR)
  {
    lTAssert( N == 3 && M == 3 );

    int iRow, iCol;
    lmMatrix<3,3,T> kTmp;

    // product S*R
    for( iRow = 0; iRow < 3; iRow++)
    {
      for (iCol = 0; iCol < 3; iCol++)
          kTmp(iRow,iCol) = kS(iRow)*kR(iRow,iCol);
    }

    // product L*S*R
    for( iRow = 0; iRow < 3; iRow++)
    {
      for( iCol = 0; iCol < 3; iCol++)
      {
        (*this)(iRow,iCol) = T(0.0);
        for (int iMid = 0; iMid < 3; iMid++)
            (*this)(iRow,iCol) += kL(iRow,iMid)*kTmp(iMid,iCol);
      }
    }
  }

  // Support routines for SingularValueDecomposition3x3.
  //
  static void Bidiagonalize(lmMatrix<3,3,T>& kA, lmMatrix<3,3,T>& kL, lmMatrix<3,3,T>& kR)
  {
    T afV[3], afW[3];
    T fLength, fSign, fT1, fInvT1, fT2;
    bool bIdentity;

    // map first column to (*,0,0)
    fLength = lmSqrt(kA(0,0)*kA(0,0) + kA(1,0)*kA(1,0) + kA(2,0)*kA(2,0));

    if( fLength > T(0.0) )
    {
      fSign = (kA(0,0) > T(0.0) ? T(1.0) : -T(1.0));
      fT1 = kA(0,0) + fSign*fLength;
      fInvT1 = T(1.0)/fT1;
      afV[1] = kA(1,0)*fInvT1;
      afV[2] = kA(2,0)*fInvT1;

      fT2 = -2.0f/(T(1.0)+afV[1]*afV[1]+afV[2]*afV[2]);
      afW[0] = fT2*(kA(0,0)+kA(1,0)*afV[1]+kA(2,0)*afV[2]);
      afW[1] = fT2*(kA(0,1)+kA(1,1)*afV[1]+kA(2,1)*afV[2]);
      afW[2] = fT2*(kA(0,2)+kA(1,2)*afV[1]+kA(2,2)*afV[2]);
      kA(0,0) += afW[0];
      kA(0,1) += afW[1];
      kA(0,2) += afW[2];
      kA(1,1) += afV[1]*afW[1];
      kA(1,2) += afV[1]*afW[2];
      kA(2,1) += afV[2]*afW[1];
      kA(2,2) += afV[2]*afW[2];

      kL(0,0) = T(1.0)+fT2;
      kL(0,1) = kL(1,0) = fT2*afV[1];
      kL(0,2) = kL(2,0) = fT2*afV[2];
      kL(1,1) = T(1.0)+fT2*afV[1]*afV[1];
      kL(1,2) = kL(2,1) = fT2*afV[1]*afV[2];
      kL(2,2) = T(1.0)+fT2*afV[2]*afV[2];
      bIdentity = false;
    }
    else
    {
      kL.Identity();
      bIdentity = true;
    }

    // map first row to (*,*,0)
    fLength = lmSqrt(kA(0,1)*kA(0,1)+kA(0,2)*kA(0,2));
    if ( fLength > T(0.0) )
    {
      fSign = (kA(0,1) > T(0.0) ? T(1.0) : -T(1.0));
      fT1 = kA(0,1) + fSign*fLength;
      afV[2] = kA(0,2)/fT1;

      fT2 = -2.0f/(T(1.0)+afV[2]*afV[2]);
      afW[0] = fT2*(kA(0,1)+kA(0,2)*afV[2]);
      afW[1] = fT2*(kA(1,1)+kA(1,2)*afV[2]);
      afW[2] = fT2*(kA(2,1)+kA(2,2)*afV[2]);
      kA(0,1) += afW[0];
      kA(1,1) += afW[1];
      kA(1,2) += afW[1]*afV[2];
      kA(2,1) += afW[2];
      kA(2,2) += afW[2]*afV[2];

      kR(0,0) = T(1.0);
      kR(0,1) = kR(1,0) = T(0.0);
      kR(0,2) = kR(2,0) = T(0.0);
      kR(1,1) = T(1.0)+fT2;
      kR(1,2) = kR(2,1) = fT2*afV[2];
      kR(2,2) = T(1.0)+fT2*afV[2]*afV[2];
    }
    else
    {
      kR.Identity();
    }

    // map second column to (*,*,0)

    fLength = lmSqrt(kA(1,1)*kA(1,1)+kA(2,1)*kA(2,1));

    if( fLength > T(0.0) )
    {
      fSign = (kA(1,1) > T(0.0) ? T(1.0) : -T(1.0));
      fT1 = kA(1,1) + fSign*fLength;
      afV[2] = kA(2,1)/fT1;

      fT2 = -2.0f/(T(1.0)+afV[2]*afV[2]);
      afW[1] = fT2*(kA(1,1)+kA(2,1)*afV[2]);
      afW[2] = fT2*(kA(1,2)+kA(2,2)*afV[2]);
      kA(1,1) += afW[1];
      kA(1,2) += afW[2];
      kA(2,2) += afV[2]*afW[2];

      T fA = T(1.0)+fT2;
      T fB = fT2*afV[2];
      T fC = T(1.0)+fB*afV[2];

      if ( bIdentity )
      {
        kL(0,0) = T(1.0);
        kL(0,1) = kL(1,0) = T(0.0);
        kL(0,2) = kL(2,0) = T(0.0);
        kL(1,1) = fA;
        kL(1,2) = kL(2,1) = fB;
        kL(2,2) = fC;
      }
      else
      {
        for (int iRow = 0; iRow < 3; iRow++)
        {
          T fTmp0 = kL(iRow,1);
          T fTmp1 = kL(iRow,2);
          kL(iRow,1) = fA*fTmp0+fB*fTmp1;
          kL(iRow,2) = fB*fTmp0+fC*fTmp1;
        }
      }
    }
  }

  static void GolubKahanStep( lmMatrix<3,3,T>& kA, lmMatrix<3,3,T>& kL, lmMatrix<3,3,T>& kR )
  {
    T fT11 = kA(0,1)*kA(0,1)+kA(1,1)*kA(1,1);
    T fT22 = kA(1,2)*kA(1,2)+kA(2,2)*kA(2,2);
    T fT12 = kA(1,1)*kA(1,2);
    T fTrace = fT11+fT22;
    T fDiff = fT11-fT22;
    T fDiscr = lmSqrt(fDiff*fDiff+4.0f*fT12*fT12);
    T fRoot1 = 0.5f*(fTrace+fDiscr);
    T fRoot2 = 0.5f*(fTrace-fDiscr);

    // adjust right
    T fY = kA(0,0) - (lmFabs(fRoot1-fT22) <= lmFabs(fRoot2-fT22) ? fRoot1 : fRoot2);
    T fZ = kA(0,1);
    T fInvLength = lmOOSqrt(fY*fY+fZ*fZ);
    T fSin = fZ*fInvLength;
    T fCos = -fY*fInvLength;

    T fTmp0 = kA(0,0);
    T fTmp1 = kA(0,1);
    kA(0,0) = fCos*fTmp0-fSin*fTmp1;
    kA(0,1) = fSin*fTmp0+fCos*fTmp1;
    kA(1,0) = -fSin*kA(1,1);
    kA(1,1) *= fCos;

    int iRow;
    for (iRow = 0; iRow < 3; iRow++)
    {
      fTmp0 = kR(0,iRow);
      fTmp1 = kR(1,iRow);
      kR(0,iRow) = fCos*fTmp0-fSin*fTmp1;
      kR(1,iRow) = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust left
    fY = kA(0,0);
    fZ = kA(1,0);
    fInvLength = lmOOSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    kA(0,0) = fCos*kA(0,0)-fSin*kA(1,0);
    fTmp0 = kA(0,1);
    fTmp1 = kA(1,1);
    kA(0,1) = fCos*fTmp0-fSin*fTmp1;
    kA(1,1) = fSin*fTmp0+fCos*fTmp1;
    kA(0,2) = -fSin*kA(1,2);
    kA(1,2) *= fCos;

    int iCol;
    for (iCol = 0; iCol < 3; iCol++)
    {
      fTmp0 = kL(iCol,0);
      fTmp1 = kL(iCol,1);
      kL(iCol,0) = fCos*fTmp0-fSin*fTmp1;
      kL(iCol,1) = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust right
    fY = kA(0,1);
    fZ = kA(0,2);
    fInvLength = lmOOSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    kA(0,1) = fCos*kA(0,1)-fSin*kA(0,2);
    fTmp0 = kA(1,1);
    fTmp1 = kA(1,2);
    kA(1,1) = fCos*fTmp0-fSin*fTmp1;
    kA(1,2) = fSin*fTmp0+fCos*fTmp1;
    kA(2,1) = -fSin*kA(2,2);
    kA(2,2) *= fCos;

    for (iRow = 0; iRow < 3; iRow++)
    {
      fTmp0 = kR(1,iRow);
      fTmp1 = kR(2,iRow);
      kR(1,iRow) = fCos*fTmp0-fSin*fTmp1;
      kR(2,iRow) = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust left
    fY = kA(1,1);
    fZ = kA(2,1);
    fInvLength = lmOOSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    kA(1,1) = fCos*kA(1,1)-fSin*kA(2,1);
    fTmp0 = kA(1,2);
    fTmp1 = kA(2,2);
    kA(1,2) = fCos*fTmp0-fSin*fTmp1;
    kA(2,2) = fSin*fTmp0+fCos*fTmp1;

    for (iCol = 0; iCol < 3; iCol++)
    {
      fTmp0 = kL(iCol,1);
      fTmp1 = kL(iCol,2);
      kL(iCol,1) = fCos*fTmp0-fSin*fTmp1;
      kL(iCol,2) = fSin*fTmp0+fCos*fTmp1;
    }
  }

  // Returns determinant of a, and b=a inverse.
  // If matrix is singular, returns 0 and leaves trash in b.
  //
  // Uses Gaussian elimination with partial pivoting.

  #define SWAP(a, b, t) {t = a; a = b; b = t;}
  T GaussianInverse(lmMatrix<N,M,T> &B) const
  {
    lTAssert( N == M || (N==4&&M==3) /* keep the compiler happy for 4x3 case*/ );
    if( N == 4 && M == 3 ) return T(0.0);

    lmMatrix<N,M,T> A = (*this);
    unsigned int i, j = 0;
    unsigned int k;
    T max, t, det, pivot;

    /*---------- forward elimination ----------*/

    for (i=0; i<N; i++)                 // put identity matrix in B
        for (j=0; j<N; j++)
            B(i,j) = (float)(i==j);

    det = float(T(1.0));
    for (i=0; i<N; i++) {               // eliminate in column i, below diag
        max = float(-T(1.0));
        for (k=i; k<N; k++)             // find pivot for column i
            if (lmFabs(A(k,i)) > max) {
                max = lmFabs(A(k,i));
                j = k;
            }
        if (max<=float(0)) return float(0);         // if no nonzero pivot, PUNT
        if (j!=i) {                     // swap rows i and j
            for (k=i; k<N; k++)
                SWAP(A(i,k), A(j,k), t);
            for (k=0; k<N; k++)
                SWAP(B(i,k), B(j,k), t);
            det = -det;
        }
        pivot = A(i,i);
        det *= pivot;
        for (k=i+1; k<N; k++)           // only do elems to right of pivot
            A(i,k) /= pivot;
        for (k=0; k<N; k++)
            B(i,k) /= pivot;
        // we know that A(i, i) will be set to 1, so don't bother to do it

        for (j=i+1; j<N; j++) {         // eliminate in rows below i
            t = A(j,i);                 // we're gonna zero this guy
            for (k=i+1; k<N; k++)       // subtract scaled row i from row j
                A(j,k) -= A(i,k)*t;     // (ignore k<=i, we know they're 0)
            for (k=0; k<N; k++)
                B(j,k) -= B(i,k)*t;
        }
    }

    /*---------- backward elimination ----------*/

    for (i=N-1; i>0; i--) {             // eliminate in column i, above diag
        for (j=0; j<i; j++) {           // eliminate in rows above i
            t = A(j,i);                 // we're gonna zero this guy
            for (k=0; k<N; k++)         // subtract scaled row i from row j
                B(j,k) -= B(i,k)*t;
        }
    }

    return det;
  }
  #undef SWAP

  //##
  // Get a gaussian scalar

  static T GaussianScalar()
  {
    static bool second=false;
    static T x2;

    if( second )
    {
      second = false;
      return x2;
    }

    T x1,w;

    do
    {
      x1 = T(2.0) * (T)lRandFloatMersenne() - T(1.0);
      x2 = T(2.0) * (T)lRandFloatMersenne() - T(1.0);
      w = x1*x1 + x2*x2;
    }
    while( w >= T(1.0) );

    w = lmSqrt((T(-2.0)*lmLog(w))/w);
    x1*=w;
    x2*=w;
    second=true;
    return x1;
  }

  //##
  // Get a gaussian vector or matrix

  static lmMatrix<N,M,T> Gaussian()
  {
    lmMatrix<N,M,T> m;

    for( unsigned int i=0;i<N;i++ )
    {
      for(unsigned int j = 0; j<M; j++ )
      {
        m.At(i,j) = GaussianScalar();
      }
    }

    return m;
  }

  //##
  // Get a random vector of length len using rejection sampling

  static lmMatrix<N,M,T> RandomBetween( T lo, T hi )
  {
    lmMatrix<N,M,T> m;

    for( unsigned int i=0;i<N;i++ )
    {
      for( unsigned int j = 0; j<M; j++ )
      {
        m.At(i,j) = (T)lRandFloatMersenneBetween( lo, hi );
      }
    }

    return m;
  }

  //##
  // Get a random vector of length len using rejection sampling

  static lmMatrix<N,M,T> Random( T len )
  {
    lTAssert(N==1);
    lmMatrix<1,M,T> x;
    T w;

    do
    {
      for( int i=0;i<M;i++ ) x(i) = T(2.0) * ( (T)lRandFloatMersenne() ) - T(1.0);
      w = x.Dot(x);
    }
    while( w >= T(1.0) );

    x.Normalize( len );
    return x;
  }

  //##
  // Get a random unit vector using rejection sampling

  static lmMatrix<N,M,T> RandomUnit()
  {
    return Random( T( 1.0 ) );
  }

  //##
  // Return minimum values from two matrices
  static lmMatrix<N,M,T> Minimum( const lmMatrix<N,M,T> &mat1, const lmMatrix<N,M,T> &mat2 )
  {
    lmMatrix<N,M,T> m;

    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ )
    {
      m.At(i,j) = lmMin( mat1.At(i,j), mat2.At(i,j) );
    }

    return m;
  }

  //##
  // Return minimum values from two matrices
  static lmMatrix<N,M,T> Maximum( const lmMatrix<N,M,T> &mat1, const lmMatrix<N,M,T> &mat2 )
  {
    lmMatrix<N,M,T> m;

    for( unsigned int i=0;i<N;i++ ) for(unsigned int j = 0; j<M; j++ )
    {
      m.At(i,j) = lmMax( mat1.At(i,j), mat2.At(i,j) );
    }

    return m;
  }
};

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Global operator overloads
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// Special operator for 4x3 * 4x3

template<typename T, typename TSIMD>
  inline lmMatrix<4,3,T,TSIMD> operator* (const lmMatrix<4,3,T,TSIMD> &m,const lmMatrix<4,3,T,TSIMD> &a)
  {
    lmMatrix<4,3,T,TSIMD> r ; lmMatrix<4,3,T,TSIMD>::Mul(r,m,a); return r;
  }

template<typename T, typename TSIMD>
  inline void operator*= (lmMatrix<4,3,T,TSIMD> &m,const lmMatrix<4,3,T,TSIMD> &a)
  {
    lmMatrix<4,3,T,TSIMD> r; lmMatrix<4,3,T,TSIMD>::Mul(r,m,a); m=r;
  }


// We want special operators for 1x3 * 4x3, 1x3 * 4x4 and 2x1 * 3x3, which requires an overload
// for just 1x3 and 1x2 versions of lmMatrix.

template<typename T, typename TSIMD>
  inline lmMatrix<1,3,T,TSIMD> operator* (const lmMatrix<1,3,T,TSIMD> &m,const lmMatrix<4,3,T,TSIMD> &a)
  {
    lmMatrix<1,3,T,TSIMD> r ; lmMatrix<1,3,T,TSIMD>::Mul(r,m,a); return r;
  }

template<typename T, typename TSIMD>
  inline void operator*= (lmMatrix<1,3,T,TSIMD> &m,const lmMatrix<4,3,T,TSIMD> &a)
  {
    lmMatrix<1,3,T,TSIMD> r; lmMatrix<1,3,T,TSIMD>::Mul(r,m,a); m=r;
  }

template<typename T, typename TSIMD>
  inline lmMatrix<1,3,T,TSIMD> operator* (const lmMatrix<1,3,T,TSIMD> &m,const lmMatrix<4,4,T,TSIMD> &a)
  {
    lmMatrix<1,3,T,TSIMD> r ; lmMatrix<1,3,T,TSIMD>::Mul(r,m,a); return r;
  }

template<typename T, typename TSIMD>
  inline void operator*= (lmMatrix<1,3,T,TSIMD> &m,const lmMatrix<4,4,T,TSIMD> &a)
  {
    lmMatrix<1,3,T,TSIMD> r; lmMatrix<1,3,T,TSIMD>::Mul(r,m,a); m=r;
  }

template<typename T, typename TSIMD>
  inline lmMatrix<1,2,T,TSIMD> operator* (const lmMatrix<1,2,T,TSIMD> &m,const lmMatrix<3,3,T,TSIMD> &a)
	{
    lmMatrix<1,2,T,TSIMD> r ; lmMatrix<1,2,T,TSIMD>::Mul(r,m,a); return r;
	}

template<typename T, typename TSIMD>
  inline void operator*= (lmMatrix<1,2,T,TSIMD> &m,const lmMatrix<3,3,T,TSIMD> &a)
	{
    lmMatrix<1,2,T,TSIMD> r ; lmMatrix<1,2,T,TSIMD>::Mul(r,m,a); m=r;
	}

// Vector * Vector

template<typename T, typename TSIMD>
  inline lmMatrix<1,2,T,TSIMD> operator* (const lmMatrix<1,2,T,TSIMD> &m,const lmMatrix<1,2,T,TSIMD> &a)
  {
    lmMatrix<1,2,T,TSIMD> r; lmMatrix<1,2,T,TSIMD>::ComponentScale(r,m,a); return r;
  }

template<typename T, typename TSIMD>
  inline lmMatrix<1,3,T,TSIMD> operator* (const lmMatrix<1,3,T,TSIMD> &m,const lmMatrix<1,3,T,TSIMD> &a)
  {
    lmMatrix<1,3,T,TSIMD> r; lmMatrix<1,3,T,TSIMD>::ComponentScale(r,m,a); return r;
  }

template<typename T, typename TSIMD>
  inline lmMatrix<1,4,T,TSIMD> operator* (const lmMatrix<1,4,T,TSIMD> &m,const lmMatrix<1,4,T,TSIMD> &a)
  {
    lmMatrix<1,4,T,TSIMD> r; lmMatrix<1,4,T,TSIMD>::ComponentScale(r,m,a); return r;
  }

template<typename T, typename TSIMD>
  inline void operator*= (lmMatrix<1,2,T,TSIMD> &m,const lmMatrix<1,2,T,TSIMD> &a)
  {
    lmMatrix<1,2,T,TSIMD> r; lmMatrix<1,2,T,TSIMD>::ComponentScale(r,m,a); m=r;
  }

template<typename T, typename TSIMD>
  inline void operator*= (lmMatrix<1,3,T,TSIMD> &m,const lmMatrix<1,3,T,TSIMD> &a)
  {
    lmMatrix<1,3,T,TSIMD> r; lmMatrix<1,3,T,TSIMD>::ComponentScale(r,m,a); m=r;
  }

template<typename T, typename TSIMD>
  inline void operator*= (lmMatrix<1,4,T,TSIMD> &m,const lmMatrix<1,4,T,TSIMD> &a)
  {
    lmMatrix<1,4,T,TSIMD> r; lmMatrix<1,4,T,TSIMD>::ComponentScale(r,m,a); m=r;
  }


// Serialize to a lStream object

template <unsigned int N, unsigned int M, typename T, typename TSIMD>
  inline lStream& operator<< ( lStream& ar, lmMatrix<N,M,T,TSIMD> &m )
  {
    //ar.Serialize( m.Data(), sizeof(T)*(N*M) );

#ifndef L_PLATFORM_WII
    //[(i*M)+j]
    for( unsigned int i=0;i<N;i++ )
    {
      ar.Serialize( &m.At(i,0), sizeof(T)*(M) );
    }
#else
//    lLogFmt("%d x %d T = %d\n", N, M, sizeof(T));
//
    for( unsigned int i=0;i<N;i++ )
    {
      for( unsigned int j=0;j<M;j++ )
      {
        T val;
        ar.Serialize( &val, sizeof(T) );

        uint32 tmp = lEndian::Little32( *(uint32*)&val );
        val = *(float*)&tmp;

        m(i,j) = val;
      }
    }
#endif

    //if( N==1 )
    //{
    //  ar.Serialize( m.Data(), sizeof(T)*(M) );
    //}
    //else
    //{
    //  for( unsigned int i=0;i<M;i++ )
    //  {
    //    ar.Serialize( &m.At(0,i), sizeof(T)*(N) );
    //  }
    //}

    return ar;
  }

// Static const identity
//template <unsigned int N, unsigned int M, typename T, typename TSIMD>
//const lmMatrix<N,M,T,TSIMD> lmMatrix<N,M,T,TSIMD>::ConstIdentity( true );

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Typedefs
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef lmMatrix<4,4,float> lmMatrix4;
typedef lmMatrix<3,3,float> lmMatrix3;
typedef lmMatrix<2,2,float> lmMatrix2;

typedef lmMatrix<1,3,float> lmVector3;
typedef lmMatrix<1,4,float> lmVector4;
typedef lmMatrix<1,2,float> lmVector2;

typedef lmMatrix<4,4,double> lmMatrix4l;
typedef lmMatrix<3,3,double> lmMatrix3l;
typedef lmMatrix<2,2,double> lmMatrix2l;

typedef lmMatrix<1,3,double> lmVector3l;
typedef lmMatrix<1,4,double> lmVector4l;
typedef lmMatrix<1,2,double> lmVector2l;

typedef lmMatrix<4,3,float> lmMatrix43;
typedef lmMatrix<4,3,double> lmMatrix43l;

typedef lmMatrix<1,3,float,lSIMD_Base> lmVector3Packed;
typedef lmMatrix<3,3,float,lSIMD_Base> lmMatrix3Packed;


END_L_NAMESPACE
#endif
