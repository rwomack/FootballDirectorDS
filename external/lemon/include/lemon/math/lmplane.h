//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmplane.h
// Description : Plane math class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMPLANE_H_INCLUDED
#define LMPLANE_H_INCLUDED

#include "lmath.h"
#include "lmside.h"
#include "lmmatrix.h"


BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lmPlane - Plane math
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Templated plane geometry class
template< unsigned int N, typename T >
struct lmPlane
{
  lmMatrix<1,N,T>  m_normal;
  T                m_d;

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Default constructor (no initialisation)
  lmPlane() {}

  //##
  // Construct plane from plane normal and offset
  // In:
  //   normal - plane normal
  //   d      - plane offset
  lmPlane( const lmMatrix<1,N,T> &normal, T d )
           : m_normal( normal ), m_d( d )
  {
  }

  //##
  // Construct plane from a vector4
  // In:
  //   normald - plane normal and offset
  lmPlane( const lmMatrix<1,4,T> &normald )
           : m_normal( normald(0), normald(1), normald(2) ), m_d( normald(3) )
  {
  }

  //##
  // Construct plane from two vectors which exists on the plane
  // In:
  //   origin - vector 1
  //   vector - vector 2
  lmPlane( const lmMatrix<1,N,T> &origin, const lmMatrix<1,N,T> &vector )
    : m_normal(vector)
  {
    m_normal.Normalize();
    m_d = -(origin.Dot(m_normal));
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Serialize the plane to an lStream
  friend inline lStream& operator<< ( lStream& ar, lmPlane &p )
  {
    return ar << p.m_normal << p.m_d;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Conversions
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Normalize the plane
  void Normalize()
  {
    float oolen = lmOOSqrt( m_normal.SqrLength() );
    m_normal(0) *= oolen;
    m_normal(1) *= oolen;
    m_normal(2) *= oolen;
    m_d         *= oolen;
  }

  //##
  // Create plane from origin + normal
  // In:
  //   origin - Plane origin (i.e. point on plane)
  //   vector - Plane normal
  void FromOriginVector( const lmMatrix<1,N,T> &origin, const lmMatrix<1,N,T> &vector )
  {
    m_normal = vector;
    m_normal.Normalize();
    m_d = -(origin.Dot(m_normal));
  }

  //##
  // Create plane from 3 points (should be noncollinear)
  // In:
  //   a - Point a
  //   b - Point b
  //   c - Point c
  // Out:
  //   Returns true if successful otherwise false
  // In:
  //   a - Point a
  //   b - Point b
  //   c - Point c
  bool FromPoints( const lmMatrix<1,N,T> &a, const lmMatrix<1,N,T> &b, const lmMatrix<1,N,T> &c )
  {
    lmMatrix<1,N,T> ab = b - a;
    lmMatrix<1,N,T> ac = c - a;

    T l1, l2;

    if( (l1 = lmSqrt( ab.Dot(ab) )) == (T)0 ) return false;
    if( (l2 = lmSqrt( ac.Dot(ac) )) == (T)0 ) return false;

    T l   = lmMax<T>(l1,l2);
    T ool = ((T)1.0)/l;

    ab *= ool;
    ac *= ool;

    m_normal = ab.Cross(ac);

    if( (l = lmSqrt( m_normal.Dot(m_normal)) ) == (T)0 ) return false;
    m_normal /= l;
    //ool = ((T)1.0)/l;
    //m_normal *= ool;

    m_d = -(a.Dot(m_normal));

    return true;
  }

  //##
  // Force through point
  // In:
  //   point  - Point we want our plane to contain
  void ForceThroughPoint( const lmMatrix<1,N,T> &point )
  {
    m_d = -( m_normal * point );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Utility logging
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Print a plane to the log

  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_normal(i) );
    lLogFmt(" : %6.3f\n",m_d);
  }
  #endif


  //##
  // Print a plane to lLogFmt

  inline void Logf(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_normal(i) );
    lLogFmt(" : %6.3f\n",m_d);
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return plane normal
  inline const lmMatrix<1,N,T> GetNormal() const
  {
    return m_normal;
  }

  //##
  // Return plane offset 'd'
  inline const T GetD() const
  {
    return m_d;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Distances / Side classification
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return distance from point to plane
  // TODO check me
  inline const T                Distance( const lmMatrix<1,N,T> &point ) const  { return point.Dot(m_normal) + m_d; }

  //##
  // Return closest point on plane to 'point'
  inline const lmMatrix<1,N,T>  Closest(const lmMatrix<1,N,T> &point ) const    { return point - (m_normal * Distance(point)); }

  //##
  // Return the side of the plane that 'point' is on
  inline const lmSide Side( const lmMatrix<1,N,T> &point, T epsilon = T(0.0) ) const
  {
    T distance = Distance( point );

    if ( distance > epsilon )
		{
      return LMSIDE_POSITIVE;
		}
    else if ( distance < epsilon )
		{
      return LMSIDE_NEGATIVE;
		}
		else
		{
      return LMSIDE_ON;
		}
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Comparisons
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Compare two planes
  // In:
  //   other - plane to compare with

  inline bool EqualTo( const lmPlane<N,T> &other ) const
  {
    if( m_normal != other.m_normal ) return false;
    if( m_d != other.m_d ) return false;
    return true;
  }

  //##
  // Compare two planes using tolerance
  // In:
  //   other - plane to compare with
  //   tol   - tolerance epsilon to use

  inline bool AlmostEqualTo(const lmPlane<N,T> &other, const T &tol) const
  {
    if( !m_normal.AlmostEqualTo( other.m_normal, tol ))  return false;
    if( !lmAlmostEqualTo(m_d, other.m_d, tol)) return false;
    return true;
  }

  //##
  // Comparison operators

  inline bool operator==(const lmPlane<N,T> &other) const
  {
    return EqualTo( other );
  }

  inline bool operator!=(const lmPlane<N,T> &other) const
  {
    return !EqualTo( other );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Transformations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Perform ortho transformation on plane
  template<unsigned int P, unsigned int Q> inline void OrthoTransform(const lmMatrix<P,Q,T> &matrix)
  {
    // Rotating the normal will rotate the plane around the origin.
    m_normal = m_normal.SubMulM( matrix );

    if ( P == ( N + 1 )  &&  Q == ( N + 1 ) )
    {
      //The matrix has an extra row and an extra column; use the extra row as the translation.
      //Now we need to take the translation that the matrix applies into account.
      //The translation can be obtained by taking the last row of the matrix (N+1 x N+1) matrix.
      //Compute the amount by which the translation will move the plane along the normal

      T dDiff = m_normal.Dot( matrix.template GetRow<N>() ); // Note: GCC3.2.2 bug, must have 'template' in there.

      //Now apply this to m_d
      m_d -= dDiff;
    }
  }

  //##
  // Transform plane by matrix
  // In:
  //   matrix - matrix to transform plane by

  template<unsigned int P,unsigned int Q> inline void Transform( const lmMatrix<P,Q,T> &matrix )
  {
    FromOriginVector( ( m_normal * -m_d ) * matrix, m_normal.SubMulM( matrix ) );
  }

  //##
  // Plane transformation operator *
  template<unsigned int P,unsigned int Q> inline const lmPlane operator*  (const lmMatrix<P,Q,T>& m) const
  {
    lmPlane<N,T> r = (*this);
    r.Transform(m);
    return r;
  }

  //##
  // Plane transformation operator *=
  template<unsigned int P,unsigned int Q> inline void          operator*= (const lmMatrix<P,Q,T>& m)
  {
    Transform(m);
  }
};

//##
// Three way intersection of planes
// In:
//  plane1 - plane 1
//  plane2 - plane 2
//  plane3 - plane 3
// Out:
//  Returns point of intersection of the planes
template< unsigned int N, typename T >
inline lmMatrix<1,3,T> IntersectThreePlanes( const lmPlane<3,T> &plane1, const lmPlane<3,T> &plane2, const lmPlane<3,T> &plane3 )
{
  // planes cant be same
  lAssert( plane1 != plane2 );
  lAssert( plane1 != plane3 );
  lAssert( plane2 != plane3 );

  lmMatrix<1,3,T> result;

  lmMatrix<1,3,T> n1n2 = plane1.m_normal.Cross( plane2.m_normal );
  lmMatrix<1,3,T> n2n3 = plane2.m_normal.Cross( plane3.m_normal );
  lmMatrix<1,3,T> n3n1 = plane3.m_normal.Cross( plane1.m_normal );

  result = (n1n2 * plane1.m_d) + (n3n1 * plane2.m_d) + (n1n2 * plane3.m_d);
  result /= n1n2.Dot( plane3.m_normal );

  return result;
}

//##
// Two way intersection of planes
// In:
//  plane1 - plane 1
//  plane2 - plane 2
//  p      - origin point of line
//  d      - line length + direction
// Out:
//  Returns point + normal of the line intersection of the planes
template< unsigned int N, typename T >
inline bool IntersectTwoPlanes( const lmPlane<3,T> &plane1, const lmPlane<3,T> &plane2, lmMatrix<1,3,T> &p, lmMatrix<1,3,T> &d )
{
  // Compute direction of intersection line
  d = plane1.m_normal.Cross( plane2.m_normal );

  // If d is (near) zero, the planes are parallel (and separated)
  // or coincident, so they’re not considered intersecting
  float denom = d.SqrLength();
  if (denom < LM_EPSILON) return false;

  // Compute point on intersection line
  p = ( lmMatrix<1,3,T>(plane1.d*plane2.n - plane2.d*plane1.n).Cross( d ) ) / denom;
  return true;
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Typedefs
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef lmPlane<3,float> lmPlane3;
typedef lmPlane<2,float> lmPlane2;

typedef lmPlane<3,double> lmPlane3l;
typedef lmPlane<2,double> lmPlane2l;


END_L_NAMESPACE
#endif


