//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmmatrixnxm.h
// Description : Matrix NxM math class (Dynamic size)
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMMATRIXNXM_H_INCLUDED
#define LMMATRIXNXM_H_INCLUDED

#include "lmath.h"
#include "./optimisations/lbase.h"
#include "lmrand.h"
#include "lmmatrix.h"

BEGIN_L_NAMESPACE

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lmMatrixNxM
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

template<typename T>
class lmMatrixNxM
{
  unsigned int   N;
  unsigned int   M;
  T            * m_elements;

  // Perform the allocation for the dynamic matrix
  inline void Alloc(unsigned int Q, unsigned int P)
  {
    N = Q;
    M = P;
    m_elements = new T[ P * Q ];
  }

  // Perform the free for the dynamic matrix
  inline void Destroy()
  {
    if ( m_elements != NULL )
    {
      delete[] m_elements;
    }
  }


public:

  lmMatrixNxM()
  {
    M = N = 0;
    m_elements = NULL;
  }

  lmMatrixNxM(unsigned int N, unsigned int M, bool bIdentity=false)
  {
    Alloc( N, M );

    if ( bIdentity )
    {
      Identity();
    }
  }

  lmMatrixNxM(const lmMatrixNxM &m)
  {
    Alloc( m.N, m.M );
    memcpy( m_elements, m.m_elements, N*M*sizeof(T) );
  }

  // cast / copy constructor, copy from different T (eg, a float to a double)

  template<typename R> lmMatrixNxM( const lmMatrixNxM<R> &m )
  {
    Alloc( m.Rows(), m.Columns() );
    for(unsigned int j=0 ; j< M ; j++)
      for(unsigned int i=0 ; i< N ; i++)
        (*this)(i,j) = (T)m(i,j);
  }

  template <unsigned int B, unsigned int A>
    lmMatrixNxM(const lmMatrix<B,A,T> &m)
  {
    Alloc( B, A );
    FromFixedMatrix( m );
  }

  ~lmMatrixNxM()
  {
    Destroy();
  }

  inline lmMatrixNxM &operator= (const lmMatrixNxM &m)
  {
    if (&m != this)
    {
      Destroy();

	    Alloc( m.N, m.M );

      memcpy( m_elements, m.m_elements, N*M*sizeof(T) );
    }
    return *this;
  }

  template <unsigned int B, unsigned int A>
    void FromFixedMatrix(const lmMatrix<B,A,T> &m)
  {
    lAssert( B == N );
    lAssert( A == M );

    for (int y = 0; y < B; y++)
    {
      for (int x = 0; x < A; x++)
      {
        (*this)(y,x) = m(y,x);
      }
    }
  }

  template<unsigned int B, unsigned int A>
    inline void ToFixedMatrix(lmMatrix<B,A,T> &m)
  {
    lAssert( B == N );
    lAssert( A == M );

    for (int y = 0; y < B; y++)
    {
      for (int x = 0; x < A; x++)
      {
        m(y,x) = (*this)(y,x);
      }
    }
  }

  //##
  // Serialize to a lStream object
  friend lStream& operator<< ( lStream& ar, lmMatrixNxM &m )
  {
    if( ar.IsLoading() )
    {
      unsigned int N,M;
      ar << N << M;
      m.Alloc(N,M);
    }
    else
    {
      ar << m.N << m.M;
    }
    ar.Serialize( m.Data(), sizeof(T) * (m.N*m.M) );
  }

  //##
  // Clear a matrix

  inline void Zero()
  {
    for( unsigned int i=0;i<N*M;i++ ) m_elements[i] = (T)0;
  }

  //##
  // Make a matrix identity

  inline void Identity()
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

  // Get an identity matrix

  static const lmMatrixNxM GetIdentity()
  {
    lmMatrixNxM m;
    m.Identity();
    return m;
  }

  // Matrix sizes

  inline unsigned int Rows()    const { return N; }
  inline unsigned int Columns() const { return M; }
  inline const T *    Data()    const { return m_elements; }
  inline T *          Data()          { return m_elements; }

  // Element references (row,column)

  inline const T &operator () (const unsigned int i, const unsigned int j) const { lAssert(i<N && j<M); return m_elements[i+(j*N)]; }
  inline T       &operator () (const unsigned int i, const unsigned int j)       { lAssert(i<N && j<M); return m_elements[i+(j*N)]; }

  // Direct row accessors, these provide run-time bounds checking in debug mode using lAssert.

  inline lmMatrixNxM GetRow(unsigned int row) const
	{
    lAssert(row<N);
    lmMatrixNxM<T> r(1,M,false);
    for( unsigned int i=0; i<M; i++) r(i) = (*this)(row,i);
		return r;
	}

  inline void SetRow( unsigned int row, const lmMatrixNxM &v )
	{
    lAssert(row<N);
    lAssert( v.N == 1 && v.M == M );
    for( unsigned int i=0; i<M; i++) (*this)(row,i) = v(i);
  }

  //##
  // Print a matrix to the log

  inline void Log(const char * str = NULL) const
  {
    if( N == 1 ) if( str!=NULL ) lLogFmt("%s: ", str );
    else if( str!=NULL ) lLogFmt("%s:\n", str );
    for(int i=0;i<N;i++)
    {
      for(int j=0;j<M;j++)
      {
        lLogFmt("%6.3f ", (float)(*this)(i,j) );
      }
      lLogFmt("\n");
    }
  }

  inline void Logf(const char * str = NULL) const
  {
    if( N == 1 ) if( str!=NULL ) lLogFmt("%s: ", str );
    else if( str!=NULL ) lLogFmt("%s:\n", str );
    for(int i=0;i<N;i++)
    {
      for(int j=0;j<M;j++)
      {
        lLogFmt("%6.3f ", (float)(*this)(i,j) );
      }
      lLogFmt("\n");
    }
  }

  //##
  // Peform inner dot product
  inline T Dot( const lmMatrixNxM & m ) const
  {
    lAssert( N==1 );
    lAssert( m.N == 1 );
    lAssert( M == m.M );

    T r = (T)0;
    for( unsigned int i=0; i<M; i++ ) r += (*this)(0,i) * m(0,i);
    return r;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Equality operators
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Compare two matrices

  inline bool EqualTo(const lmMatrixNxM &other) const
  {
    lAssert( N == other.N );
    lAssert( M == other.M );
    for( unsigned int i=0; i<N*M; i++) if( m_elements[i] != other.m_elements[i] ) return false;
    return true;
  }

  inline bool AlmostEqualTo(const lmMatrixNxM &other, const T &tol) const
  {
    lAssert( N == other.N );
    lAssert( M == other.M );
    for( unsigned int i=0; i<N*M; i++) if( ! lmAlmostEqualTo(m_elements[i],other.m_elements[i],tol) ) return false;
    return true;
  }

  inline bool IsFinite() const
  {
    for( unsigned int i=0; i<N*M; i++)
    {
      if ( !lmIsFinite( m_elements[i] ) )
      {
        return false;
      }
    }

    return true;
  }

  //##
	// Equality operators

  inline const bool operator== (const lmMatrixNxM& m) const { return EqualTo(m); }
  inline const bool operator!= (const lmMatrixNxM& m) const { return !EqualTo(m); }
  inline const bool operator<  (const lmMatrixNxM& m) const { for( unsigned int i=0; i<N*M; i++) if( m_elements[i] >= m.m_elements[i] ) return false; return true; }
  inline const bool operator<= (const lmMatrixNxM& m) const { for( unsigned int i=0; i<N*M; i++) if( m_elements[i] >  m.m_elements[i] ) return false; return true; }
  inline const bool operator>  (const lmMatrixNxM& m) const { for( unsigned int i=0; i<N*M; i++) if( m_elements[i] <= m.m_elements[i] ) return false; return true; }
  inline const bool operator>= (const lmMatrixNxM& m) const { for( unsigned int i=0; i<N*M; i++) if( m_elements[i] <  m.m_elements[i] ) return false; return true; }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Arithmetic
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  inline const lmMatrixNxM operator+ (const lmMatrixNxM& m)  const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = m_elements[i] + m.m_elements[i]; return r; }
  inline const lmMatrixNxM operator+ (const T& v)            const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = m_elements[i] + v; return r; }
  inline const lmMatrixNxM operator- (const lmMatrixNxM& m)  const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = m_elements[i] - m.m_elements[i]; return r; }
  inline const lmMatrixNxM operator- (const T& v)            const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = m_elements[i] - v; return r; }
  inline const lmMatrixNxM operator*  (const lmMatrixNxM& m) const { lmMatrixNxM r = lmMatrixNxM::Mul(*this,m); return r;  }
  inline const lmMatrixNxM operator- ()                      const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = -m_elements[i]; return r; }
  inline const lmMatrixNxM operator/ (const lmMatrixNxM& m)  const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = m_elements[i] / m.m_elements[i]; return r; }
  inline const lmMatrixNxM operator/ (const T &v)            const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = m_elements[i] / v; return r; }

  inline const lmMatrixNxM operator+= (const lmMatrixNxM& m)  { for( unsigned int i=0; i<N*M; i++) m_elements[i] += m.m_elements[i]; return (*this); }
  inline const lmMatrixNxM operator+= (const T& v)            { for( unsigned int i=0; i<N*M; i++) m_elements[i] += v; return (*this); }
  inline const lmMatrixNxM operator-= (const lmMatrixNxM& m)  { for( unsigned int i=0; i<N*M; i++) m_elements[i] -= m.m_elements[i]; return (*this); }
  inline const lmMatrixNxM operator-= (const T& v)            { for( unsigned int i=0; i<N*M; i++) m_elements[i] -= v; return (*this); }
  inline const lmMatrixNxM operator*= (const T& v)            { for( unsigned int i=0; i<N*M; i++) m_elements[i] *= v; return (*this); }
  inline const lmMatrixNxM operator*= (const lmMatrixNxM& m)  { lmMatrixNxM r = lmMatrixNxM::Mul(*this,m); (*this) = r; return (*this); }

  inline const lmMatrixNxM operator/= (const lmMatrixNxM& m)  { for( unsigned int i=0; i<N*M; i++) m_elements[i] /= m.m_elements[i]; return (*this); }
  inline const lmMatrixNxM operator/= (const T &v)            { for( unsigned int i=0; i<N*M; i++) m_elements[i] /= v; return (*this); }

  inline const lmMatrixNxM Scale(const lmMatrixNxM& m) const { lmMatrixNxM r; for( unsigned int i=0; i<N*M; i++) r.m_elements[i] = m_elements[i] * m.m_elements[i]; return r; }

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
  inline static lmMatrixNxM Mul(const lmMatrixNxM &a, const lmMatrixNxM &b)
  {
    unsigned int N = a.N;
    unsigned int M = a.M;
    unsigned int P = b.M;
    lAssert(b.N == M );

    lmMatrixNxM r(N,P,false);
    r.Zero();
    for (unsigned int i = 0; i < N; i++)
      for (unsigned int j = 0; j < M; j++)
        for (unsigned int k = 0; k < P; k++)
          r(i,k) += a(i,j) * b(j,k);

    return r;
  }

  //##
  // Multiply (concatenate) two matrices (transpose a)
  // In:
  //   r - resultant matrix
  //   a - first matrix (to transpose)
  //   b - second matrix
  //
  // Matrix multiplication rules:
  //
  // NxM * MxP = NxP
	//
  inline static lmMatrixNxM MulTranspose(const lmMatrixNxM &a, const lmMatrixNxM &b)
  {
    unsigned int N = a.N;
    unsigned int M = a.M;
    unsigned int P = b.M;
    lAssert(b.N == M );

    lmMatrixNxM r(N,P,false);
    r.Zero();
    for (unsigned int i = 0; i < N; i++)
      for (unsigned int j = 0; j < M; j++)
        for (unsigned int k = 0; k < P; k++)
          r(i,k) += a(j,i) * b(j,k);

    return r;
  }

  //##
  // Return transpose of this matrix
  inline lmMatrixNxM<T> Transpose() const
  {
    lmMatrixNxM<T> result( M, N, false );

    for (unsigned int j = 0; j < N; j++)
    {
      for (unsigned int i = 0; i < M; i++)
      {
        result(i,j) = (*this)(j,i);
      }
    }

    return result;
  }

  //##
  // Get determinant of matrix
  inline const T Determinant( float epsilon = T(1e-12f) ) const
  {
    lmMatrixNxM r;
    T det = GaussianInverse(r);

    if( det > -epsilon && det < epsilon )
      return 0; // if determinant is nearly zero
    else
      return det;
  }


  //##
  // Get the Inverse of matrix
  // Out:
  //   r     - out inverted matrix
  //   true  - matrix inverse returned
  //   false - matrix inverse doesn't exist
  //
  inline const bool Inverse(lmMatrixNxM &r) const
  {
    float det = GaussianInverse(r);
    const float epsilon = float(1e-12);
    if( det > -epsilon && det < epsilon ) return false;  // if determinant is nearly zero, inversion failed
    return true;
  }

  //##
  // Return the inverse matrix, or a zero matrix if inverse is not possible.
	//
  inline lmMatrixNxM Inverse() const
  {
    lmMatrixNxM r;
    if( !Inverse(r) ) r.Zero();
    return r;
  }

  //##
  // Returns determinant of a, and b=a inverse.
  // If matrix is singular, returns 0 and leaves trash in b.
  //
  // Uses Gaussian elimination with partial pivoting.
  //
  #define SWAP(a, b, t) {t = a; a = b; b = t;}
  float GaussianInverse(lmMatrixNxM &B) const
  {
    lAssert( N == M );
    lmMatrixNxM A = (*this);
    unsigned int i, j, k;
    float max, t, det, pivot;

    B.Destroy();
    B.Alloc( N, M );

    /*---------- forward elimination ----------*/

    for (i=0; i<N; i++)                 // put identity matrix in B
        for (j=0; j<N; j++)
            B(i,j) = (T)(i==j);

    det = 1.0;
    for (i=0; i<N; i++)
    {               // eliminate in column i, below diag
        max = -1.;
        for (k=i; k<N; k++)             // find pivot for column i
            if (lmFabs(A(k,i)) > max)
            {
                max = lmFabs(A(k,i));
                j = k;
            }
        if (max<=0.) return 0.;         // if no nonzero pivot, PUNT
        if (j!=i)
        {                     // swap rows i and j
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

        for (j=i+1; j<N; j++)
        {         // eliminate in rows below i
            t = A(j,i);                 // we're gonna zero this guy
            for (k=i+1; k<N; k++)       // subtract scaled row i from row j
                A(j,k) -= A(i,k)*t;     // (ignore k<=i, we know they're 0)
            for (k=0; k<N; k++)
                B(j,k) -= B(i,k)*t;
        }
    }

    /*---------- backward elimination ----------*/

    for (i=N-1; i>0; i--)
    {             // eliminate in column i, above diag
        for (j=0; j<i; j++)
        {           // eliminate in rows above i
            t = A(j,i);                 // we're gonna zero this guy
            for (k=0; k<N; k++)         // subtract scaled row i from row j
                B(j,k) -= B(i,k)*t;
        }
    }

    return det;
  }
  #undef SWAP
};

END_L_NAMESPACE
#endif
