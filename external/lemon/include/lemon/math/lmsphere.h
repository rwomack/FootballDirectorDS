//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmsphere.h
// Description : Sphere class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMSPHERE_H_INCLUDED
#define LMSPHERE_H_INCLUDED

#include "../containers/larray.h"
#include "lmath.h"
#include "lmside.h"
#include "lmmatrix.h"
#include "lmplane.h"

BEGIN_L_NAMESPACE

template<unsigned int N, typename T>
class lmAABox;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lmSphere -   sphere
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Templated sphere class
template <unsigned int N, typename T>
class lmSphere
{
public:
  lmMatrix<1,N,T> m_origin;
  T               m_radius;

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct empty sphere
  inline lmSphere()
  {
    Empty();
  }

  //##
  // Construct sphere from initial origin + radius
  // In:
  //   origin - Origin for sphere
  //   radius - Radius of sphere
  inline lmSphere( const lmMatrix<1,N,T> &origin, T radius )
  {
    m_origin = origin;
    m_radius = radius;
  }

  //##
  // Construct sphere from initial origin and point on sphere surface
  // In:
  //   origin - origin of sphere
  //   pointOnSphere - a point on the sphere
  inline lmSphere( const lmMatrix<1,N,T> &origin, const lmMatrix<1,N,T> &pointOnSphere )
  {
    m_origin = origin;
    m_radius = m_origin.Distance( pointOnSphere );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Static Construction methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct lmSphere to fit around an AABox
  // In:
  //   box - lmAABox to construct sphere around
  static lmSphere<N,T> AroundBox( const lmAABox<N,T> &box );

  //##
  // Construct lmSphere from array of points
  // In:
  //   points - Array of points to construct lmAABox from
  static lmSphere<N,T> AroundPoints( const lArray< lmMatrix<1,N,T> > &points )
	{
		if(points.Size())
		{
      return FromPointArray( (void*)points.Data(), points.ElementSize(), points.Size() );
    }

    lmSphere<N,T> sph;
    return sph;
	}

  //##
  // Construct lmAABox from array of points
  // In:
  //   points - Array of points to construct lmAABox from
  static lmSphere<N,T> AroundPoints( const void *data, int stride, int numPoints )
	{
    if(data != NULL && numPoints > 0)
		{
      return FromPointArray( data, stride, numPoints );
    }

    lmSphere<N,T> sph;
    return sph;
	}

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Serialize sphere to an lStream
  friend lStream & operator<<(lStream &ar, lmSphere<N,T> &sphere)
  {
    ar << sphere.m_origin << sphere.m_radius;
    return ar;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns the origin of the sphere
  inline const lmMatrix<1,N,T> & GetOrigin() const
  {
    return m_origin;
  }

  //##
  // Returns the radius of the sphere
  inline const T & GetRadius() const
  {
    return m_radius;
  }

  //##
  // Returns the squared radius of the sphere
  inline T GetSqrRadius() const
  {
    return (m_radius * m_radius);
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Utility Logging
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Print a sphere to the log
  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_origin(i) );
    lLogFmt(" : ");
    lLogFmt("%6.3f\n", (double)m_radius );
  }
  #endif

  //##
  // Print a sphere to lLogFmt
  inline void Logf(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_origin(i) );
    lLogFmt(" : ");
    lLogFmt("%6.3f\n", (double)m_radius );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Extending the sphere
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Empties the sphere (clears radius to 0.0)
  inline void Empty()
  {
    m_radius = T( 0.0 );
  }

  //##
  // Returns true if sphere is empty
  inline bool IsEmpty() const
  {
    return m_radius == T( 0.0 );
  }

  //##
  // Extend sphere to contain another sphere
  // In:
  //   sphere - Other sphere to extend around
  inline void Extend( const lmSphere<N,T> &sphere )
	{
    if ( IsEmpty() )
		{
      m_origin = sphere.m_origin;
      m_radius = sphere.m_radius;
    }
    else
    {
      lmMatrix<1,N,T> originDiff = sphere.m_origin - m_origin;
      T               originDiffSqrLen = originDiff.SqrLength();

      T               radiusDiff = sphere.m_radius - m_radius;
      T               radiusDiffSqr = radiusDiff * radiusDiff;

      if( radiusDiffSqr >= originDiffSqrLen )
      {
        if( radiusDiff >= T(0.0) )
        {
          m_origin = sphere.m_origin;
          m_radius = sphere.m_radius;
          return;
        }
      }

      T originDiffLen = lmSqrt( originDiffSqrLen );
      if( originDiffLen > LM_EPSILON )
      {
        T coeff = (originDiffLen + radiusDiff) / (((T)2.0) * originDiffLen);
        m_origin = m_origin + originDiff * coeff;
      }

      m_radius = ((T)0.5) * (originDiffLen + m_radius + sphere.m_radius);
    }
  }

  //##
  // Extend sphere to contain box
  void Extend( const lmAABox<N,T> &box );

  //##
  // Extend sphere to contain point p
  inline void Extend( const lmMatrix<1,N,T> &p )
	{
    if ( IsEmpty() )
		{
      m_origin = p;
      m_radius = T(0.0);
    }
    else
    {
      lmMatrix<1,N,T> orig2p = (p - m_origin);

      float dist2 = orig2p.SqrLength();

      if( dist2 > (m_radius * m_radius) )
      {
        float dist = lmSqrt( dist2 );
        float newRadius = (m_radius + dist) * 0.5f;
        float k = (newRadius - m_radius) / dist;
        m_radius = newRadius;
        // test this
        m_origin += orig2p * k;
      }
    }
	}

  //##
  // Create sphere to contain array of points (fast version) TODO

  // An Efficient Bounding Sphere by Jack Ritter from "Graphics Gems", Academic Press, 1990
  // Routine to calculate tight bounding sphere over  a set of points in 3D
  // Code written by Jack Ritter and Lyle Rains.
  inline static lmSphere<N,T> FromPointArray( void *data, int stride, int numPoints )
  {
    lmSphere <N,T> result;
    lmVector3 xmin,xmax,ymin,ymax,zmin,zmax,dia1,dia2;
    float radius2;

    char *pointdata = (char*)data;

    // FIRST PASS: find 6 minima/maxima points
    xmin.Init( LM_FLOAT_MAX, LM_FLOAT_MAX, LM_FLOAT_MAX);
    xmax.Init(-LM_FLOAT_MAX,-LM_FLOAT_MAX,-LM_FLOAT_MAX);
    ymin.Init( LM_FLOAT_MAX, LM_FLOAT_MAX, LM_FLOAT_MAX);
    ymax.Init(-LM_FLOAT_MAX,-LM_FLOAT_MAX,-LM_FLOAT_MAX);
    zmin.Init( LM_FLOAT_MAX, LM_FLOAT_MAX, LM_FLOAT_MAX);
    zmax.Init(-LM_FLOAT_MAX,-LM_FLOAT_MAX,-LM_FLOAT_MAX);

    for (int i = 0; i < numPoints; i++)
    {
      lmVector3 *caller_p = (lmVector3 *)pointdata;

      if ((*caller_p)(0) < xmin(0))  xmin = (*caller_p); // New xminimum point
      if ((*caller_p)(0) > xmax(0))  xmax = (*caller_p);
      if ((*caller_p)(1) < ymin(1))  ymin = (*caller_p);
      if ((*caller_p)(1) > ymax(1))  ymax = (*caller_p);
      if ((*caller_p)(2) < zmin(2))  zmin = (*caller_p);
      if ((*caller_p)(2) > zmax(2))  zmax = (*caller_p);

      pointdata+=stride;
    }

    // Set xspan = distance between the 2 points xmin & xmax (squared)
    lmVector3 d;
    d = xmax - xmin;
    float xspan = d.SqrLength();

    // Same for y & z spans
    d = ymax - ymin;
    float yspan = d.SqrLength();

    d = zmax - zmin;
    float zspan = d.SqrLength();

    // Set points dia1 & dia2 to the maximally separated pair
    dia1 = xmin;
    dia2 = xmax; // assume xspan biggest
    float maxspan = xspan;

    if (yspan > maxspan)
    {
      maxspan = yspan;
      dia1 = ymin;
      dia2 = ymax;
    }

    if (zspan > maxspan)
    {
      dia1 = zmin;
      dia2 = zmax;
    }

    // dia1,dia2 is a diameter of initial sphere
    // calc initial centre
    result.m_origin.Init( (dia1(0)+dia2(0))*0.5f,
                          (dia1(1)+dia2(1))*0.5f,
                          (dia1(2)+dia2(2))*0.5f );

    // calculate initial radius**2 and radius
    d = dia2 - result.m_origin;
    radius2 = d.SqrLength();
    result.m_radius = (float)lmSqrt( radius2 );

    // SECOND PASS: increment current sphere
    pointdata = (char*)data;

    for (int i = 0; i < numPoints; i++)
    {
      lmVector3 *caller_p = (lmVector3 *)pointdata;

      d = (*caller_p) - result.m_origin;

      float old_to_p_sq = d.SqrLength();

      if (old_to_p_sq > radius2)   // do r**2 test first
      {
        // this point is outside of current sphere
        float old_to_p = (float)lmSqrt( old_to_p_sq );

        // calc radius of new sphere
        result.m_radius = (result.m_radius + old_to_p) * 0.5f;
        radius2 = result.m_radius*result.m_radius;   /* for next r**2 compare */

        float old_to_new = old_to_p - result.m_radius;
        // calc centre of new sphere
        float recip = 1.0f / old_to_p;

        result.m_origin(0) = (result.m_radius * result.m_origin(0) + old_to_new * (*caller_p)(0)) * recip;
        result.m_origin(1) = (result.m_radius * result.m_origin(1) + old_to_new * (*caller_p)(1)) * recip;
        result.m_origin(2) = (result.m_radius * result.m_origin(2) + old_to_new * (*caller_p)(2)) * recip;
      }

      pointdata+=stride;
    }

    return result;
  }

  //##
  // Extend sphere to contain array of points FAST version
  inline void ExtendArray( const lArray< lmMatrix<1,N,T> > &points )
  {
    // create sphere from points
    lmSphere<N,T> sphere = FromPointArray( points );

    // now extend using that sphere
    Extend( sphere );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Comparisons
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Compare two spheres
  // In:
  //   other - Sphere to compare with
  inline bool EqualTo( const lmSphere<N,T> &other ) const
  {
    if( m_origin != other.m_origin ) return false;
    if( m_radius != other.m_radius ) return false;
    return true;
  }

  //##
  // Compare two spheres using tolerance epsilon
  // In:
  //   other - Sphere to compare with
  //   tol   - Tolerance to use in comparison
  inline bool AlmostEqualTo(const lmSphere<N,T> &other, const T &tol) const
  {
    if( !m_origin.AlmostEqualTo( other.m_origin, tol ))  return false;
    if( !lmAlmostEqualTo(m_radius, other.m_radius, tol)) return false;
    return true;
  }

  //##
  //comparison operators

  inline bool operator==( const lmSphere<N,T> &other ) const
  {
    return EqualTo( other );
  }

  inline bool operator!=( const lmSphere<N,T> &other ) const
  {
    return !EqualTo( other );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Containment / Intersection
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns true if the sphere contains the box specified by boxCentre + halfBoxExtents
  inline bool ContainsBox( const lmMatrix<1,N,T> &boxCentre, const lmMatrix<1,N,T> &halfBoxExtents )
  {
    lmMatrix<1,N,T> sphereToBox = boxCentre - m_origin;

    lmMatrix<1,N,T> leadingVector;
    for (unsigned int i = 0; i < N; i++)
    {
      leadingVector(i) = sphereToBox(i) >= T(0.0)  ?  halfBoxExtents(i)  :  -halfBoxExtents(i);
    }

    return ( sphereToBox + leadingVector ).SqrLength()  <=  ( m_radius * m_radius );
  }

  //##
  // Get side of plane that sphere is on
  inline lmSide GetSide(const lmPlane<N,T> &plane)
	{
    T distance = plane.Distance( m_origin );

    if ( distance > m_radius )
		{
      return LMSIDE_POSITIVE;
		}
    else if ( distance < -m_radius )
		{
      return LMSIDE_NEGATIVE;
		}
		else
		{
      return LMSIDE_ON;
		}
	}

  // Get distance from plane to sphere
  inline T GetDistance(const lmPlane<N,T> &plane)
	{
    T distance = plane.Distance( m_origin );

    if ( distance > m_radius )
		{
      return distance - m_radius;
		}
    else if ( distance < -m_radius )
		{
      return distance + m_radius;
		}

    return T(0.0);
	}

  //##
  // Calculates intersection between box + sphere
  bool Intersect(const lmAABox<N,T> &box, T &distToOriginSquared) const;

  //##
  // Intersect 2 moving spheres
  // In:
  //   velA    - velocity of 'this'
  //   sphereB - other sphere
  //   velB    - velocity of other sphere
  //   t0      - normalized in t
  //   t1      - normalized out t
  // Out:
  //   Returns bool on whether there is an intersection
  inline bool IntersectSweptSphere(const lmVector3 &velA, const lmSphere &sphereB, const lmVector3 &velB, T &t0, T &t1)
  {
    lmVector3 velAB( velB - velA );
    lmVector3 AB( sphereB.m_origin - m_origin );
    T rAB = m_radius + sphereB.m_radius;

    T ABlen2 = AB.SqrLength();
    T rAB2   = rAB * rAB;

    // check if spheres are currently overlapping...
    if (ABlen2 <= rAB2)
    {
      t0 = T(0.0);
      t1 = T(0.0);
      return true;
    } else
    {
      // check if they hit each other
      T a = velAB.SqrLength();

      if ((a<-LM_EPSILON) || (a>+LM_EPSILON))
      {
        // if a is '0' then the objects don't move relative to each other
        T b = (velAB.Dot( AB )) * T(2.0);
        T c = ABlen2 - rAB2;
        T q = b*b - T(4.0)*a*c;

        if (q >= 0.0f)
        {
          // 1 or 2 contacts
          T sq = (T)lmSqrt(q);
          T d  = T(1.0) / (T(2.0)*a);
          T r1 = (-b + sq) * d;
          T r2 = (-b - sq) * d;

          if (r1 < r2)
          {
            t0 = r1;
            t1 = r2;
          } else
          {
            t0 = r2;
            t1 = r1;
          }

          return true;
        }
      }
    }

    return false;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Transformations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Performs an orthogonal transform on the sphere
  template <unsigned int M> inline void OrthoTransform(const lmMatrix<M,M,T> &m)
  {
    m_origin = m_origin * m;
    T dSquared = 0.0;
    for (int i = 0; i < N; i++)
    {
      dSquared += lmSq( m(i,0) );
    }

    float scaleFactor = lmSqrt( dSquared );
    m_radius *= scaleFactor;
  }

  //##
  // Translate sphere by v
  inline void Translate(const lmMatrix<1,N,T> &v)
	{
    m_origin += v;
	}

  //##
  // Scale sphere by s
  inline void Scale( const T &s )
	{
    m_radius *= s;
	}
};

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Typedefs
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef lmSphere<2,float> lmSphere2;
typedef lmSphere<3,float> lmSphere3;

typedef lmSphere<2,double> lmSphere2l;
typedef lmSphere<3,double> lmSphere3l;

END_L_NAMESPACE
#endif

#include "lmsphere.inl"
