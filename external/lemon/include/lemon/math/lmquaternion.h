//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmquaternion.h
// Description : Quaternion math class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMQUATERNION_H_INCLUDED
#define LMQUATERNION_H_INCLUDED

#include "lmath.h"
#include "lmmatrix.h"
#include "lmrand.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lmQuaternion - Quaternion math structure class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
// lmQuaternion template structure class
//
template <class T>
struct lmQuaternionT
{
private:
  T m_element[4]; // Definition, <x,y,z,w> (x,y,z = axis, w = angle)

public:
  //##
  // Constructors

  lmQuaternionT() {}
  lmQuaternionT(const lmQuaternionT &q) { memcpy(m_element, q.m_element, 4*sizeof(T)); }
  lmQuaternionT(T x, T y, T z, T w) { m_element[0] = x; m_element[1] = y; m_element[2] = z; m_element[3] = w; }

  //##
  // Constructors for angle&angle, sub 3x3 matrix and euler angles

  template <class VT> lmQuaternionT(const T &angle,const lmMatrix<1,3,VT> &axis)               { FromAngleAxis(angle,axis); }
  template <unsigned int N, unsigned int M, class MT> lmQuaternionT(const lmMatrix<N,M,MT> &m) { FromMatrix(m); }
  lmQuaternionT(const T &x, const T &y, const T &z)                                            { FromEuler(x,y,z); }

  // Accessors

  inline const  T     &x() const { return m_element[0]; }
  inline T      &x()             { return m_element[0]; }
  inline const  T     &y() const { return m_element[1]; }
  inline T      &y()             { return m_element[1]; }
  inline const  T     &z() const { return m_element[2]; }
  inline T      &z()             { return m_element[2]; }
  inline const  T     &w() const { return m_element[3]; }
  inline T      &w()             { return m_element[3]; }

  //##
  // Print a quaternion to the log
  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    lLogFmt("%6.3f %6.3f %6.3f %6.3f\n", m_element[0], m_element[1], m_element[2], m_element[3] );
  }
  #endif

  inline void Logf(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );

    lLogFmt("%6.3f %6.3f %6.3f %6.3f\n", m_element[0], m_element[1], m_element[2], m_element[3] );
  }

  //##
  // Clear a quaternion
  inline void Zero() { lmQuaternionT::Zero(*this); }
  inline static void Zero(lmQuaternionT &r)
  {
    r.m_element[0] = 0; r.m_element[1] = 0; r.m_element[2] = 0; r.m_element[3] = 0;
  }

  //##
  // Make a quaternion identity
  inline void Identity() { lmQuaternionT::Identity(*this); }
  inline static void Identity(lmQuaternionT &r)
  {
    r.m_element[0] = T(0.0); r.m_element[1] = T(0.0); r.m_element[2] = T(0.0); r.m_element[3] = T(1.0);
  }

  //##
  // Returns true if this quaternion is identity
  bool IsIdentity( float tol = LM_EPSILON )
  {
    if( lmAlmostEqualTo( m_element[0], T(0.0), tol ) &&
        lmAlmostEqualTo( m_element[1], T(0.0), tol ) &&
        lmAlmostEqualTo( m_element[2], T(0.0), tol ) &&
        lmAlmostEqualTo( m_element[3], T(1.0), tol ) )
    {
      return true;
    }

    return false;
  }

  //##
  // Element references
  inline const T &operator () (const unsigned int i) const { lAssert( i < 4 ); return m_element[i]; }
  inline T       &operator () (const unsigned int i)       { lAssert( i < 4 ); return m_element[i]; }

  inline const T *    Data()    const { return m_element; }
  inline T *          Data()          { return m_element; }

  //##
  // Assignment operator
  inline lmQuaternionT &operator = (const lmQuaternionT &m)
  {
    if( &m != this ) memcpy(m_element, m.m_element, 4*sizeof(T));
    return *this;
  }

  //##
  // Check quaternion is valid (not infinite / NaN)
  inline bool IsFinite() const
  {
    for( unsigned int i=0; i<4; i++)
    {
      if ( !lmIsFinite( m_element[i] ) )
      {
        return false;
      }
    }

    return true;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Operations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Make rotation go the other way
  inline void InverseRotation()
  {
    m_element[0] = -m_element[0];
    m_element[1] = -m_element[1];
    m_element[2] = -m_element[2];
  }

  //##
  // Multiply two quaternions
  // In:
  //   r - quaternion result
  //   a - quaternion a
  //   b - quaternion b
  //
  inline static void Mul(lmQuaternionT<T> &r, const lmQuaternionT<T> &q1, const lmQuaternionT<T> &q2 )
  {
    r(0) = q1(3) * q2(0) + q1(0) * q2(3) + q1(1) * q2(2) - q1(2) * q2(1);  // 4 muls, 1 sub, 2 adds
    r(1) = q1(3) * q2(1) + q1(1) * q2(3) + q1(2) * q2(0) - q1(0) * q2(2);  // 4 muls, 1 sub, 2 adds
    r(2) = q1(3) * q2(2) + q1(2) * q2(3) + q1(0) * q2(1) - q1(1) * q2(0);  // 4 muls, 1 sub, 2 adds
    r(3) = q1(3) * q2(3) - q1(0) * q2(0) - q1(1) * q2(1) - q1(2) * q2(2);  // 4 muls, 3 sub
                                                                           // 16 muls, 6 subs, 2adds = 26 ops
  }

//  inline static void Mul(ctpQuaternion<T> &r, const ctpQuaternion<T> &q1, const ctpQuaternion<T> &q2 )  // same as dQmultiply0
//  {  // r = rotate by q2, then q1
//    r(0) = q1(0) * q2(0) - q1(1) * q2(1) - q1(2) * q2(2) - q1(3) * q2(3);
//    r(1) = q1(0) * q2(1) + q1(1) * q2(0) + q1(2) * q2(3) - q1(3) * q2(2);
//    r(2) = q1(0) * q2(2) + q1(2) * q2(0) + q1(3) * q2(1) - q1(1) * q2(3);
//    r(3) = q1(0) * q2(3) + q1(3) * q2(0) + q1(1) * q2(2) - q1(2) * q2(1);
//  }

  // TODO document

  inline static void Mul1(lmQuaternionT<T> &r, const lmQuaternionT<T> &q1, const lmQuaternionT<T> &q2 )  // same as dQmultiply1
  {  // r = rotate by q2, then by inverse of q1
    r(0) = q1(3) * q2(3) + q1(0) * q2(0) + q1(1) * q2(1) + q1(2) * q2(2);
    r(1) = q1(3) * q2(0) - q1(0) * q2(3) - q1(1) * q2(2) + q1(2) * q2(1);
    r(2) = q1(3) * q2(1) - q1(1) * q2(3) - q1(2) * q2(0) + q1(0) * q2(2);
    r(3) = q1(3) * q2(2) - q1(2) * q2(3) - q1(0) * q2(1) + q1(1) * q2(0);
  }

  inline static void Mul2(lmQuaternionT<T> &r, const lmQuaternionT<T> &q1, const lmQuaternionT<T> &q2 )  // same as dQmultiply2
  {  // r = rotate by inverse of q2, then q1
    r(0) =  q1(3) * q2(3) + q1(0) * q2(0) + q1(1) * q2(1) + q1(2) * q2(2);
    r(1) = -q1(3) * q2(0) + q1(0) * q2(3) - q1(1) * q2(2) + q1(2) * q2(1);
    r(2) = -q1(3) * q2(1) + q1(1) * q2(3) - q1(2) * q2(0) + q1(0) * q2(2);
    r(3) = -q1(3) * q2(2) + q1(2) * q2(3) - q1(0) * q2(1) + q1(1) * q2(0);
  }

  inline static void Mul3(lmQuaternionT<T> &r, const lmQuaternionT<T> &q1, const lmQuaternionT<T> &q2 )  // same as dQmultiply3
  {  // r = rotate by inverse of q2, then inverse of q1
    r(0) =  q1(3) * q2(3) - q1(0) * q2(0) - q1(1) * q2(1) - q1(2) * q2(2);
    r(1) = -q1(3) * q2(0) - q1(0) * q2(3) + q1(1) * q2(2) - q1(2) * q2(1);
    r(2) = -q1(3) * q2(1) - q1(1) * q2(3) + q1(2) * q2(0) - q1(0) * q2(2);
    r(3) = -q1(3) * q2(2) - q1(2) * q2(3) + q1(0) * q2(1) - q1(1) * q2(0);
  }

  //##
  // Return conjugate of this quaternion
  inline lmQuaternionT<T> Conjugate() const
  {
    return lmQuaternionT<T>( -(*this)(0), -(*this)(1), -(*this)(2), (*this)(3) );
  }

  //##
  // Returns magnitude of quaternion
  inline T Magnitude() const
  {
    T magnitude = lmSqrt( (m_element[0] * m_element[0]) +
                          (m_element[1] * m_element[1]) +
                          (m_element[2] * m_element[2]) +
                          (m_element[3] * m_element[3]) );

    return magnitude;
  }

  //##
  // Normalize a quaternion
  //
  inline void Normalize()
  {
    Normalize(*this);
  }

  inline static void Normalize(lmQuaternionT<T> &a)
  {
    T magnitude = lmSqrt( (a.m_element[0] * a.m_element[0]) +
                          (a.m_element[1] * a.m_element[1]) +
                          (a.m_element[2] * a.m_element[2]) +
                          (a.m_element[3] * a.m_element[3]) );

    if( magnitude > T(0.0) )
    {
      magnitude = T(1.0) / magnitude;

      a.m_element[0] *= magnitude;
      a.m_element[1] *= magnitude;
      a.m_element[2] *= magnitude;
      a.m_element[3] *= magnitude;
    }
  }

  //##
  // Dot quaternion with another
  inline T Dot( const lmQuaternionT &other ) const
  {
    return m_element[0]*other.m_element[0] + m_element[1]*other.m_element[1] + m_element[2]*other.m_element[2] + m_element[3]*other.m_element[3];
  }

  //##
  // Calculate logarithm of a quaternion
  inline static lmQuaternionT<T> Log( const lmQuaternionT<T> &quat )
  {
    T a     = lmAcos( quat(2) );
    T sina  = lmSin( a );

    lmQuaternionT<T> result;
    result(2) = T(0.0);

    if( sina > T( 0.0 ) )
    {
      float oosina = T(1.0) / sina;
      result(0) = a * quat(0) * oosina;
      result(1) = a * quat(1) * oosina;
      result(2) = a * quat(2) * oosina;
    } else
    {
      result(0) = T( 0.0 );
      result(1) = T( 0.0 );
      result(2) = T( 0.0 );
    }

    return result;
  }

  //##
  // Calculate exponential of a quaternion
  inline static lmQuaternionT<T> Exp( const lmQuaternionT<T> &quat )
  {
    T sina, cosa;
    T a = lmSqrt( (quat.m_element[0] * quat.m_element[0]) +
                  (quat.m_element[1] * quat.m_element[1]) +
                  (quat.m_element[2] * quat.m_element[2]));

    lmSinCos( a, sina, cosa );

    lmQuaternionT<T> result;
    result(2) = cosa;

    if( a > T( 0.0 ) )
    {
      float ooa = T(1.0) / a;
      result(0) = sina * quat(0) * ooa;
      result(1) = sina * quat(1) * ooa;
      result(2) = sina * quat(2) * ooa;
    } else
    {
      result(0) = T( 0.0 );
      result(1) = T( 0.0 );
      result(2) = T( 0.0 );
    }

    return result;
  }

  //##
  // Spherical cubic interpolation between quaternions
  inline static lmQuaternionT<T> Squad( T t,
                                        const lmQuaternionT<T> &quat1, const lmQuaternionT<T> &quat2,
                                        const lmQuaternionT<T> &quata, const lmQuaternionT<T> &quatb )
  {
    lmQuaternionT<T> quatc, quatd;
    quatc = SlerpNoInvert( t, quat1, quat2 );
    quatd = SlerpNoInvert( t, quata, quatb );

    return SlerpNoInvert( T( 2.0 ) * t * ( 1.0 - t ), quatc, quatd );
  }

  //##
  // Calculate a control point for spline interpolation based on three quaternions (at t-1, t, t+1)
  inline static lmQuaternionT<T> Spline(const lmQuaternionT<T> &quattm1, const lmQuaternionT<T> &quatt,
                                        const lmQuaternionT<T> &quattp1 )
  {
    lmQuaternionT<T> quati;

    quati(0) = -quatt(0);
    quati(1) = -quatt(1);
    quati(2) = -quatt(2);
    quati(3) = quatt(3);

    return quatt * Exp((Log( quati * quattm1 ) + Log( quati * quattp1 )) / -4);
  }

  //##
  // Get a random vector of length len using rejection sampling
  inline static lmQuaternionT<T> Random()
  {
    float x = lRandFloatMersenneBetween( T(0.0), LM_2PI );
    float y = lRandFloatMersenneBetween( T(0.0), LM_2PI );
    float z = lRandFloatMersenneBetween( T(0.0), LM_2PI );

    lmQuaternionT<T> q;
    q.FromEuler(x, y, z);
    return q;
  }

  //##
  // Inverse
  inline lmQuaternionT<T> Inverse() const
  {
    T norm = m_element[0] * m_element[0]  +
             m_element[1] * m_element[1]  +
	           m_element[2] * m_element[2]  +
		         m_element[3] * m_element[3];

    if( norm > T(0.0) )
    {
      T invNorm = T(1.0) / norm;
      return lmQuaternionT<T>( -m_element[0] * invNorm,
                               -m_element[1] * invNorm,
			                         -m_element[2] * invNorm,
                                m_element[3] * invNorm );
    }
    else
    {
      // return an invalid result to flag the error
      return lmQuaternionT<T>( T(0.0), T(0.0), T(0.0), T(0.0) );
    }
  }

  //##
  // Multiply lmMatrix<1,3,T> by Quaternion
  inline static void MulVector( lmMatrix<1,3,T> &out, const lmQuaternionT<T> q, const lmMatrix<1,3,T> &in )
  {
    T fTx  = q.m_element[0]+q.m_element[0];
    T fTy  = q.m_element[1]+q.m_element[1];
    T fTz  = q.m_element[2]+q.m_element[2];

    T fTwx = fTx*q.m_element[3];
    T fTwy = fTy*q.m_element[3];
    T fTwz = fTz*q.m_element[3];

    T fTxx = fTx*q.m_element[0];
    T fTyy = fTy*q.m_element[1];
    T fTzz = fTz*q.m_element[2];

    T fTxy = fTy*q.m_element[0];
    T fTxz = fTz*q.m_element[0];
    T fTyz = fTz*q.m_element[1];


    out(0) = (in(0)*(T(1.0)-fTyy-fTzz)) + (in(1)*(fTxy-fTwz))        + (in(2)*(fTxz+fTwy));
    out(1) = (in(0)*(fTxy+fTwz))        + (in(1)*(T(1.0)-fTxx-fTzz)) + (in(2)*(fTyz-fTwx));
    out(2) = (in(0)*(fTxz-fTwy))        + (in(1)*(fTyz+fTwx))        + (in(2)*(T(1.0)-fTxx-fTyy));
  }

  //##
  // Multiply lmMatrix<1,3,T> by Quaternion (where quaternion is unnormalized)
  inline static void MulVectorUnnormalized( lmMatrix<1,3,T> &out, const lmQuaternionT<T> q, const lmMatrix<1,3,T> &in )
  {
    lmQuaternionT<T> qnorm = q;
    qnorm.Normalize();
    return MulVector3( out, qnorm, in );
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Interpolation operators
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Lerp beween two quaternions by 't' without invert
  // In:
  //   q1       - First quaternion
  //   q2       - Second quaternion
  //   t        - Amount to lerp by
  // Out:
  //   Returns lerped quaternion (which will need to be normalised)
  inline static lmQuaternionT<T> LerpNoInvert(const lmQuaternionT<T> &q1, const lmQuaternionT<T> &q2, T t)
  {
    return q1  +  ( q2 - q1 ) * t;
  }

  //##
  // Lerp beween two quaternions by 't'
  // In:
  //   q1       - First quaternion
  //   q2       - Second quaternion
  //   t        - Amount to lerp by
  // Out:
  //   Returns lerped quaternion (which will need to be normalised)
  inline static lmQuaternionT<T> Lerp(const lmQuaternionT<T> &q1, const lmQuaternionT<T> &q2, T t)
  {
    T cosom = q1.Dot( q2 );

    if ( cosom  <  T( 0.0 ) )
    {
      return q1  +  ( (-q2) - q1 )  *  t;
    }
    else
    {
      return q1  +  ( q2 - q1 ) * t;
    }
  }

  //##
  // Slerp No Invert (note: quaternions must be unit length)
  // This function doesnt take shortest path
  inline static lmQuaternionT<T> SlerpNoInvert(T t, const lmQuaternionT<T> &quat1, const lmQuaternionT<T> &quat2)
  {
    lmQuaternionT<T> result;
    T cosom( 0.0 ), scale0( 0.0 ), scale1( 0.0 );

    cosom = quat1.Dot( quat2 );

    if ( lmFabs(cosom) < T(0.0001) )
    {
      return quat2;
    }

    // calculate coefficients

    if ( cosom > T( -0.95 ) && cosom < T( 0.95 ) )
    {
      T omega = lmAcos(cosom);

      // standard case (slerp)
      T fInvSin = T( 1.0 ) / lmSin(omega);
      scale0 = lmSin((T(1.0) - t) * omega) * fInvSin;
      scale1 = lmSin(t * omega) * fInvSin;
    }
    else
    {
      // "from" and "to" quaternions are very close
      //  ... so we can do a linear interpolation
      scale0 = T(1.0) - t;
      scale1 = t;
    }

    // calculate final values
    result(0) = (scale0 * quat1(0) + scale1 * quat2(0));
    result(1) = (scale0 * quat1(1) + scale1 * quat2(1));
    result(2) = (scale0 * quat1(2) + scale1 * quat2(2));
    result(3) = (scale0 * quat1(3) + scale1 * quat2(3));

    return result;
  }

  //##
  // Slerp (note: quaternions must be unit length)
  // This takes shortest path
  inline static lmQuaternionT<T> Slerp(T t, const lmQuaternionT<T> &quat1, const lmQuaternionT<T> &quat2)
  {
    lmQuaternionT<T> quat1b, result;
    T cosom( 0.0 ), scale0( 0.0 ), scale1( 0.0 );

    cosom = quat1.Dot( quat2 );

    if ( lmFabs(cosom) < T(0.0001) )
    {
      return quat2;
    }

    // pick shortest interpolation route.
    if( cosom < T(0.0) )
    {
      cosom     = -cosom;
      quat1b(0) = -quat2(0);
      quat1b(1) = -quat2(1);
      quat1b(2) = -quat2(2);
      quat1b(3) = -quat2(3);
    }
    else
    {
      quat1b = quat2;
    }

    // calculate coefficients

    if ( cosom < T( 0.95 ) )
    {
      T omega = lmAcos(cosom);

      // standard case (slerp)
      T fInvSin = T( 1.0 ) / lmSin(omega);
      scale0 = lmSin((T(1.0) - t) * omega) * fInvSin;
      scale1 = lmSin(t * omega) * fInvSin;
    }
    else
    {
      // "from" and "to" quaternions are very close
      //  ... so we can do a linear interpolation
      scale0 = T(1.0) - t;
      scale1 = t;
    }

    // calculate final values
    result(0) = (scale0 * quat1(0) + scale1 * quat1b(0));
    result(1) = (scale0 * quat1(1) + scale1 * quat1b(1));
    result(2) = (scale0 * quat1(2) + scale1 * quat1b(2));
    result(3) = (scale0 * quat1(3) + scale1 * quat1b(3));

    return result;
  }

  //##
  // Fast Slerp based on article from GPG5
  // This function will take shortest path
  inline static lmQuaternionT<T> SlerpFastNoInvert(T t, const lmQuaternionT<T> &quat1, const lmQuaternionT<T> &quat2)
  {
    lmQuaternionT<T> result;
    T omega( 0.0 ), cosom( 0.0 ), scale0( 0.0 ), scale1( 0.0 );

    cosom = quat1.Dot( quat2 );

    if ( lmFabs(cosom) < T(0.0001) )
    {
      return quat2;
    }

    // calculate coefficients

    if ( cosom < T( 0.95 ) )
    {
      T mC1 = T(1.570994357)+(T(.5642929859)+(T(-.1783657717)+T(.4319949352e-1)*cosom)*cosom)*cosom;
      T mC3 = T(-.6461396382)+(T(.5945657936)+(T(.8610323953e-1)-T(.3465122928e-1)*cosom)*cosom)*cosom;
      T mC5 = T(.7949823521e-1)+(T(-.1730436931)+(T(.1079279599) -T(.1439397801e-1)*cosom)*cosom)*cosom;
      T mC7 = T(-.4354102836e-2)+(T(.1418962736e-1)+(T(-.1567189691e-1)+T(.5848706227e-2)*cosom)*cosom)*cosom;

      T mRecipOnePlusAdotB = T( 1.0 ) / ( T( 1.0 ) + cosom );

      T _T = T( 1 ) - t, _t2 = t * t, _T2 = _T * _T;
      scale0 = (mC1+(mC3+(mC5+mC7*_T2)*_T2)*_T2)*_T * mRecipOnePlusAdotB;
      scale1 = (mC1+(mC3+(mC5+mC7*_t2)*_t2)*_t2)*t * mRecipOnePlusAdotB;
    }
    else
    {
      // "from" and "to" quaternions are very close
      //  ... so we can do a linear interpolation
      scale0 = T(1.0) - t;
      scale1 = t;
    }

    // calculate final values
    result(0) = (scale0 * quat1(0) + scale1 * quat2(0));
    result(1) = (scale0 * quat1(1) + scale1 * quat2(1));
    result(2) = (scale0 * quat1(2) + scale1 * quat2(2));
    result(3) = (scale0 * quat1(3) + scale1 * quat2(3));

    return result;
  }

  //##
  // Fast Slerp based on article from GPG5
  // This function will take shortest path
  inline static lmQuaternionT<T> SlerpFast(T t, const lmQuaternionT<T> &quat1, const lmQuaternionT<T> &quat2)
  {
    lmQuaternionT<T> quat1b, result;
    T omega( 0.0 ), cosom( 0.0 ), scale0( 0.0 ), scale1( 0.0 );

    cosom = quat1.Dot( quat2 );

    if ( lmFabs(cosom) < T(0.0001) )
    {
      return quat2;
    }

    // pick shortest interpolation route.
    if( cosom < T(0.0) )
    {
      cosom     = -cosom;
      quat1b(0) = -quat2(0);
      quat1b(1) = -quat2(1);
      quat1b(2) = -quat2(2);
      quat1b(3) = -quat2(3);
    }
    else
    {
      quat1b = quat2;
    }

    // calculate coefficients

    if ( cosom < T( 0.95 ) )
    {
      T mC1 = T(1.570994357)+(T(.5642929859)+(T(-.1783657717)+T(.4319949352e-1)*cosom)*cosom)*cosom;
      T mC3 = T(-.6461396382)+(T(.5945657936)+(T(.8610323953e-1)-T(.3465122928e-1)*cosom)*cosom)*cosom;
      T mC5 = T(.7949823521e-1)+(T(-.1730436931)+(T(.1079279599) -T(.1439397801e-1)*cosom)*cosom)*cosom;
      T mC7 = T(-.4354102836e-2)+(T(.1418962736e-1)+(T(-.1567189691e-1)+T(.5848706227e-2)*cosom)*cosom)*cosom;

      T mRecipOnePlusAdotB = T( 1.0 ) / ( T( 1.0 ) + cosom );

      T _T = T( 1 ) - t, _t2 = t * t, _T2 = _T * _T;
      scale0 = (mC1+(mC3+(mC5+mC7*_T2)*_T2)*_T2)*_T * mRecipOnePlusAdotB;
      scale1 = (mC1+(mC3+(mC5+mC7*_t2)*_t2)*_t2)*t * mRecipOnePlusAdotB;
    }
    else
    {
      // "from" and "to" quaternions are very close
      //  ... so we can do a linear interpolation
      scale0 = T(1.0) - t;
      scale1 = t;
    }

    // calculate final values
    result(0) = (scale0 * quat1(0) + scale1 * quat1b(0));
    result(1) = (scale0 * quat1(1) + scale1 * quat1b(1));
    result(2) = (scale0 * quat1(2) + scale1 * quat1b(2));
    result(3) = (scale0 * quat1(3) + scale1 * quat1b(3));

    return result;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Construction from other data types
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct a angle and axis
  // In:
  //   angle -  Angle around axis
  //   axis  -  Direction axis
  inline void FromAngleAxis(const T &angle, const lmMatrix<1,3,T> &axis)
  {
    T sinx, cosx;
    lmSinCos( angle*T(0.5), sinx, cosx );
    m_element[0] = axis(0) * sinx;
    m_element[1] = axis(1) * sinx;
    m_element[2] = axis(2) * sinx;
    m_element[3] = cosx;
  }

  //##
  // Convert quaternion to an angle and axis
  // In:
  //   angle - OUT angle around axis
  //   axis  - OUT axis
  template <class VT>
  inline void ToAngleAxis( T &angle, lmMatrix<1,3,VT> &axis )
  {
    T sqrLen = m_element[0]*m_element[0]+m_element[1]*m_element[1]+m_element[2]*m_element[2];
    if ( sqrLen > T(0.0) )
    {
      angle = T(2.0)*lmAcos(m_element[3]);
      T invLen = lmOOSqrt(sqrLen);
      axis(0) = m_element[0]*invLen;
      axis(1) = m_element[1]*invLen;
      axis(2) = m_element[2]*invLen;
    }
    else
    {
      // angle is 0 (mod 2*pi), so any axis will do
      angle    = T(0.0);
      axis(0)  = T(1.0);
      axis(1)  = T(0.0);
      axis(2)  = T(0.0);
    }
  }

  //##
  // Construct from euler angles
  // In:
  //   p,y,r -  Euler angles (pitch, yaw, roll)
  inline void FromEuler(const T &p, const T &y, const T &r)
  {
    T pitch = p * 0.5f;                 // x
    T yaw   = y * 0.5f;                 // y
    T roll  = r * 0.5f;                 // z

    T cosPitch, cosYaw, cosRoll;
    T sinPitch, sinYaw, sinRoll;

    lmSinCos( pitch, sinPitch, cosPitch );
    lmSinCos( yaw, sinYaw, cosYaw );
    lmSinCos( roll, sinRoll, cosRoll );

    float cosYawsinPitch = cosYaw * sinPitch;
    float cosYawcosPitch = cosYaw * cosPitch;
    float sinYawcosPitch = sinYaw * cosPitch;
    float sinYawsinPitch = sinYaw * sinPitch;

    m_element[0] = cosYawsinPitch * cosRoll - sinYawcosPitch * sinRoll;
    m_element[1] = cosYawsinPitch * sinRoll + sinYawcosPitch * cosRoll;
    m_element[2] = cosYawcosPitch * sinRoll - sinYawsinPitch * cosRoll;
    m_element[3] = cosYawcosPitch * cosRoll + sinYawsinPitch * sinRoll;
  }

  //##
  // Construct from euler angles
  // In:
  //  v - Vector containing pitch, yaw, roll
  inline void FromEuler(const lmMatrix<1,3,T> &v)
  {
    FromEuler( v(0), v(1), v(2) );
  }

  //##
  // Convert to Euler
  inline lmMatrix<1,3,T> ToEuler() const
  {
    const lmQuaternionT<T> &q1 = (*this);

    T sqw = q1(3)*q1(3);
    T sqx = q1(0)*q1(0);
    T sqy = q1(1)*q1(1);
    T sqz = q1(2)*q1(2);

    T roll = lmAtan2( T(2.0) * (q1(0)*q1(1) + q1(3)*q1(2)), (sqw + sqx - sqy - sqz));
    T pitch = lmAtan2( T(2.0) * (q1(3)*q1(0) + q1(1)*q1(2)), (sqw - sqx - sqy + sqz));
    T yaw = T(-2.0) * (q1(0)*q1(2) - q1(3)*q1(1));

    // clamp sin to -1.0 -> 1.0
    yaw = lmClamp( yaw, T( -1.0 ), T( 1.0 ) );

    return lmMatrix<1,3,T>( pitch, lmAsin( yaw ), roll );
  }

  //##
  // Convert to Euler
  inline void ToEuler( T &pitch, T &yaw, T &roll ) const
  {
    lmMatrix<1,3,T> v = ToEuler();
    pitch = v(0);
    yaw   = v(1);
    roll  = v(2);
  }

  //##
  // Generate a quat of the shortest arc to rotate vector src to dest
  inline static lmQuaternionT<T> ShortestArc( const lmMatrix<1,3,T>& src, const lmMatrix<1,3,T>& dest )
  {
    lmQuaternionT<T> out;

    lmMatrix<1,3,T> v0 = src;
    lmMatrix<1,3,T> v1 = dest;

    v0.Normalize();
    v1.Normalize();

    lmMatrix<1,3,T> c = v0.Cross( v1 );

    float d = v0.Dot( v1 );

    if( d >= T(1.0) )
    {
      // vectors are same;
      Identity( out );
      return out;
    }

    float oos = lmOOSqrt( ( T(1.0) + d ) * T(2.0) );

    out(0) = c(0) * oos;
    out(1) = c(1) * oos;
    out(2) = c(3) * oos;
    out(3) = oos * 0.5f;

    return out;
  }


  //##
  // Generate a quaternion to look at a point
  // In:
  //   lookat - Vector to point the quaternion at
  inline static lmQuaternionT<T> LookAt( const lmMatrix<1,3,T>& lookat )
  {
    const T lengthEpsilon = T(0.001); // 1mm reasonable?
    lmQuaternionT<T> out;

    // get the y axis rotation
    T r = lmSqrt(lookat(0) * lookat(0) + lookat(2) * lookat(2));
    T ycos2 = T(1), ysin2 = T(0);
    if(r > lengthEpsilon)
    {
      T c = lookat(2) / r;
      ycos2 = lmSqrt((T(1) + c) / 2);
      ysin2 = lmSqrt((T(1) - c) / 2);
      if(lookat(0) < T(0)) ysin2 = -ysin2;
    }
    // get the (rotated frame) x axis rotation
    T d = lmSqrt(r * r + lookat(1) * lookat(1));
    T xcos2 = T(1), xsin2 = T(0);
    if(d > lengthEpsilon)
    {
      T c = r / d;
      xcos2 = lmSqrt((T(1) + c) / 2);
      xsin2 = lmSqrt((T(1) - c) / 2);
      if(lookat(1) > T(0)) xsin2 = -xsin2;
    }

    // TODO: setup the rotation manually
    out = lmQuaternionT( T(0), ysin2, T(0), ycos2 ) * lmQuaternionT( xsin2, T(0), T(0), xcos2);
    return out;
  }

  //##
  // Construct from a matrix (the sub 3x3 portion)
  //
  template <unsigned int N, unsigned int M, class MT> inline void FromMatrix(const lmMatrix<N,M,MT> &m)
  {
    lTAssert( N>=3 && M>=3 );

    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    T tr, s;

    tr = m(0,0) + m(1,1) + m(2,2);

    if(tr >= T(0.0))
    {
      s = (T)lmSqrt(tr + T(1.0));
      m_element[3] = 0.5f * s;
      s = 0.5f / s;
      m_element[0] = (m(1,2) - m(2,1)) * s;
      m_element[1] = (m(2,0) - m(0,2)) * s;
      m_element[2] = (m(0,1) - m(1,0)) * s;
    }
    else
    {
      int i = 0;

      if(m(1,1) > m(0,0)) i = 1;
      if(m(2,2) > m(i,i)) i = 2;

      switch(i)
      {
        case 0:
          s = (T)lmSqrt(m(0,0) - m(1,1) - m(2,2) + T(1.0));
          m_element[0] = 0.5f * s;
          s = 0.5f / s;
          m_element[1] = (m(0,1) + m(1,0)) * s;
          m_element[2] = (m(0,2) + m(2,0)) * s;
          m_element[3] = (m(1,2) - m(2,1)) * s;
          break;
        case 1:
          s = (T)lmSqrt(m(1,1) - m(2,2) - m(0,0) + T(1.0));
          m_element[1] = 0.5f * s;
          s = 0.5f / s;
          m_element[2] = (m(1,2) + m(2,1)) * s;
          m_element[0] = (m(1,0) + m(0,1)) * s;
          m_element[3] = (m(2,0) - m(0,2)) * s;
          break;
        case 2:
          s = (T)lmSqrt(m(2,2) - m(0,0) - m(1,1) + T(1.0));
          m_element[2] = 0.5f * s;
          s = 0.5f / s;
          m_element[0] = (m(2,0) + m(0,2)) * s;
          m_element[1] = (m(2,1) + m(1,2)) * s;
          m_element[3] = (m(0,1) - m(1,0)) * s;
          break;
      }
    }
  }

  //##
  // Set the 3x3 of a matrix to a rotation matrix from the quaternion.
  inline void SetMatrix3x3( lmMatrix<4,4,T> &m )
  {
    // 3x3 rotation:
    //
    //| (1 - 2Y^2 - 2Z^2) , (2XY + 2WZ)       , (2XZ - 2WY)       |
    //| (2XY - 2WZ)       , (1 - 2X^2 - 2Z^2 ), (2YZ + 2WX)       |
    //| (2XZ + 2WY)       , (2YZ - 2WX)       , (1 - 2X^2 - 2Y^2) |

    // factored out:

    T fTx  = m_element[0]+m_element[0];
    T fTy  = m_element[1]+m_element[1];
    T fTz  = m_element[2]+m_element[2];
    T fTwx = fTx*m_element[3];
    T fTwy = fTy*m_element[3];
    T fTwz = fTz*m_element[3];
    T fTxx = fTx*m_element[0];
    T fTxy = fTy*m_element[0];
    T fTxz = fTz*m_element[0];
    T fTyy = fTy*m_element[1];
    T fTyz = fTz*m_element[1];
    T fTzz = fTz*m_element[2];

    m(0,0) = T(1.0)-(fTyy+fTzz);
    m(1,0) = fTxy-fTwz;
    m(2,0) = fTxz+fTwy;
    m(0,1) = fTxy+fTwz;
    m(1,1) = T(1.0)-(fTxx+fTzz);
    m(2,1) = fTyz-fTwx;
    m(0,2) = fTxz-fTwy;
    m(1,2) = fTyz+fTwx;
    m(2,2) = T(1.0)-(fTxx+fTyy);
  }

  //##
  // Set the 3x3 of a matrix to a rotation matrix from the quaternion. (where quaternion is unnormalized)
  inline void SetMatrix3x3Unnormalized( lmMatrix<4,4,T> &m ) const
  {
    lmQuaternionT<T> qnorm = (*this);
    qnorm.Normalize();

    return qnorm.SetMatrix3x3( m );
  }

  //##
  // Create a 3x3 matrix from the quaternion.
  const lmMatrix<3,3,T> ToMatrix3x3(void) const
  {
    lmMatrix<3,3,T> r;

    // 3x3 rotation:
    //
    //| (1 - 2Y^2 - 2Z^2) , (2XY + 2WZ)       , (2XZ - 2WY)       |
    //| (2XY - 2WZ)       , (1 - 2X^2 - 2Z^2 ), (2YZ + 2WX)       |
    //| (2XZ + 2WY)       , (2YZ - 2WX)       , (1 - 2X^2 - 2Y^2) |

    // factored out:

    T fTx  = m_element[0]+m_element[0];
    T fTy  = m_element[1]+m_element[1];
    T fTz  = m_element[2]+m_element[2];
    T fTwx = fTx*m_element[3];
    T fTwy = fTy*m_element[3];
    T fTwz = fTz*m_element[3];
    T fTxx = fTx*m_element[0];
    T fTxy = fTy*m_element[0];
    T fTxz = fTz*m_element[0];
    T fTyy = fTy*m_element[1];
    T fTyz = fTz*m_element[1];
    T fTzz = fTz*m_element[2];

    r(0,0) = T(1.0)-(fTyy+fTzz);
    r(1,0) = fTxy-fTwz;
    r(2,0) = fTxz+fTwy;
    r(0,1) = fTxy+fTwz;
    r(1,1) = T(1.0)-(fTxx+fTzz);
    r(2,1) = fTyz-fTwx;
    r(0,2) = fTxz-fTwy;
    r(1,2) = fTyz+fTwx;
    r(2,2) = T(1.0)-(fTxx+fTyy);

    return r;
  }

  //##
  // Create a 3x3 matrix from the quaternion (where quaternion is unnormalized)
  const lmMatrix<3,3,T> ToMatrix3x3Unnormalized(void) const
  {
    lmQuaternionT<T> qnorm = (*this);
    qnorm.Normalize();

    return qnorm.ToMatrix3x3();
  }


  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Operator overloads
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Multiply quaternion with quaternion
  inline lmQuaternionT<T> operator*(const lmQuaternionT<T> &q) const
  {
    lmQuaternionT<T> r;
    lmQuaternionT<T>::Mul( r, *this, q );
    return r;
  }

  inline const lmQuaternionT<T> operator*= (const lmQuaternionT<T> & q)
  {
    lmQuaternionT<T> r;
    lmQuaternionT<T>::Mul( r, *this, q );

    m_element[0] = r.m_element[0];
    m_element[1] = r.m_element[1];
    m_element[2] = r.m_element[2];
    m_element[3] = r.m_element[3];

    return (*this);
  }

  //##
  // Add two quaternions
  inline lmQuaternionT<T> operator+(const lmQuaternionT<T> &q) const
  {
    lmQuaternionT<T> qr( m_element[0] + q.m_element[0],
                         m_element[1] + q.m_element[1],
                         m_element[2] + q.m_element[2],
                         m_element[3] + q.m_element[3] );
    return qr;
  }

  inline const lmQuaternionT<T> operator+= (const lmQuaternionT<T> & q)
  {
    m_element[0] += q.m_element[0];
    m_element[1] += q.m_element[1];
    m_element[2] += q.m_element[2];
    m_element[3] += q.m_element[3];

    return (*this);
  }

  //##
  // Subtract two quaternions
  inline lmQuaternionT<T> operator-(const lmQuaternionT<T> &q) const
  {
    return lmQuaternionT<T>( m_element[0] - q.m_element[0],
                             m_element[1] - q.m_element[1],
			                       m_element[2] - q.m_element[2],
					                   m_element[3] - q.m_element[3] );
  }

  inline const lmQuaternionT<T> operator-= (const lmQuaternionT<T> & q)
  {
    m_element[0] -= q.m_element[0];
    m_element[1] -= q.m_element[1];
    m_element[2] -= q.m_element[2];
    m_element[3] -= q.m_element[3];

    return (*this);
  }

  //##
  // Scale quaternion by scalar value
  inline lmQuaternionT<T> operator*(T fScalar) const
  {
    return lmQuaternionT<T>( m_element[0] * fScalar,
                             m_element[1] * fScalar,
			                       m_element[2] * fScalar,
					                   m_element[3] * fScalar );
  }

  inline const lmQuaternionT<T> operator*= (T fScalar)
  {
    m_element[0] *= fScalar;
    m_element[1] *= fScalar;
    m_element[2] *= fScalar;
    m_element[3] *= fScalar;

    return (*this);
  }

  //##
  // Negate elements of quaternion
  inline lmQuaternionT<T> operator-() const
  {
    return lmQuaternionT<T>( -m_element[0], -m_element[1], -m_element[2], -m_element[3] );
  }

  //##
  // Compare two quaternions
  inline bool EqualTo(const lmQuaternionT<T> &q) const
  {
    for( unsigned int i=0; i<4; i++) if( m_element[i] != q.m_element[i] ) return false;
    return true;
  }

  inline bool AlmostEqualTo(const lmQuaternionT<T> &q, const T &tol) const
  {
    for( unsigned int i=0; i<4; i++) if( ! lmAlmostEqualTo(m_element[i],q.m_element[i],tol) ) return false;
    return true;
  }

  //##
  // Comparison operators
  inline bool operator==(const lmQuaternionT<T> &q) const
  {
    return EqualTo( q );
  }

  inline bool operator!=(const lmQuaternionT<T> &q) const
  {
    return !EqualTo( q );
  }
};

typedef lmQuaternionT<float> lmQuaternion;
typedef lmQuaternionT<double> lmQuaternionl;

//##
// Serialize to a lStream object
template <class T>
inline lStream& operator<< ( lStream& ar, lmQuaternionT<T> &q )
{
  ar.Serialize( q.Data(), sizeof(T)*(4) );
  return ar;
}

//##
// Interpolate between two matrices using quaternion slerp function
template <unsigned int N, unsigned int M, typename T, typename TSIMD>
  lmMatrix<4,4,T> lmMatrix<N,M,T,TSIMD>::Interp( T fTime, const lmMatrix<4,4,T> &other )
  {
    lmMatrix<3,3,T> rkM0 = Get3x3();
    lmMatrix<1,3,T> rkT0 = GetTranslate();
    lmMatrix<3,3,T> rkM1 = other.Get3x3();
    lmMatrix<1,3,T> rkT1 = other.GetTranslate();

    if( fTime <= T(0.0) )
    {
      return (*this);
    }

    if( fTime >= T(1.0) )
    {
      return other;
    }

    // factor M0 = L0*S0*R0
    lmMatrix<3,3,T> kL0, kR0;
    lmMatrix<1,3,T> kS0;
    rkM0.SingularValueDecomposition3x3(kL0,kS0,kR0);

    // compute quaternions for L0 and R0
    lmQuaternionT<T> kQL0, kQR0;
    kQL0.FromMatrix(kL0);
    kQR0.FromMatrix(kR0);

    // factor M1 = L1*S1*R1
    lmMatrix<3,3,T> kL1, kR1;
    lmMatrix<1,3,T> kS1;
    rkM1.SingularValueDecomposition3x3(kL1,kS1,kR1);

    // compute quaternions for L1 and R1
    lmQuaternionT<T> kQL1, kQR1;
    kQL1.FromMatrix(kL1);
    kQR1.FromMatrix(kR1);

    // compute M = L*S*R
    lmMatrix<3,3,T> kL, kR;
    lmMatrix<1,3,T> kS;

    // spherical linear interpolation of quaternions for L and R
    lmQuaternionT<T> kQL = lmQuaternionT<T>::Slerp(fTime,kQL0,kQL1);
    lmQuaternionT<T> kQR = lmQuaternionT<T>::Slerp(fTime,kQR0,kQR1);

    kL = kQL.ToMatrix3x3();
    kR = kQR.ToMatrix3x3();

    // generalized geometric means, S = S0^{1-t}*S1^t
    T fOMTime = T(1.0) - fTime;
    kS(0) = lmPow(lmFabs(kS0(0)),fOMTime) * lmPow(lmFabs(kS1(0)),fTime);
    kS(1) = lmPow(lmFabs(kS0(1)),fOMTime) * lmPow(lmFabs(kS1(1)),fTime);
    kS(2) = lmPow(lmFabs(kS0(2)),fOMTime) * lmPow(lmFabs(kS1(2)),fTime);

    lmMatrix<4,4,T> r(true);

    // compute the product M = L*S*R
    lmMatrix<3,3,T> rot;
    rot.SingularValueComposition3x3(kL,kS,kR);
    r.Set3x3(rot);

    // linearly interpolate T = (1-t)*T0+t*T1
    r.SetTranslate( (rkT0*fOMTime) + (rkT1*fTime) );

    return r;
  }

//##
// Multiply lmMatrix<1,3,T> by Quaternion
template<typename T>
  inline lmMatrix<1,3,T> operator*(const lmMatrix<1,3,T> &v, const lmQuaternionT<T> &q)
  {
    lmMatrix<1,3,T> result;
    lmQuaternionT<T>::MulVector( result, q, v );
    return result;
  }

//##
// Multiply lmMatrix<1,3,T> by Quaternion
template<typename T>
  inline void operator*=(lmMatrix<1,3,T> &v, const lmQuaternionT<T> &q)
  {
    lmMatrix<1,3,T> result;
    lmQuaternionT<T>::MulVector( result, q, v );
    v = result;
  }

END_L_NAMESPACE
#endif
