//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmray.h
// Description : Ray class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMRAY_H_INCLUDED
#define LMRAY_H_INCLUDED

#include "lmath.h"
#include "lmmatrix.h"
#include "lmplane.h"
#include "lmsphere.h"
#include "lmtri.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lmRay - Ray math
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Templated ray class
template< unsigned int N, typename T >
struct lmRay
{
  lmMatrix<1,N,T> m_origin;
  lmMatrix<1,N,T> m_normal;
  T               m_length;

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Default constructor no initialisation
  lmRay() {}

  //##
  // Construct ray from origin normal and length
  // In:
  //   origin - origin
  //   normal - normal
  //   len    - length

  lmRay( const lmMatrix<1,N,T> &origin, const lmMatrix<1,N,T> &normal, const T len )
    : m_origin(origin), m_normal(normal), m_length(len)
  {}

  //##
  // Construct ray from origin and vector
  // In:
  //   origin - origin
  //   vector - vector of ray

  lmRay( const lmMatrix<1,N,T> &origin, const lmMatrix<1,N,T> &vector )
    : m_origin(origin), m_normal(vector)
  {
    m_length = m_normal.Normalize();
  }

  //##
  // Cast / copy
  template<typename U>
    lmRay( const lmRay<N,U> &r )
    {
      m_origin = r.m_origin;
      m_normal = r.m_normal;
      m_length = r.m_length;
    }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Serialize the ray to an lStream
  friend inline lStream& operator<< ( lStream& ar, lmRay &r )
  {
    return ar << r.m_origin << r.m_normal << r.m_length;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Utility logging
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Log / Print a ray

  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_origin(i) );
    lLogFmt(" : ");
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_normal(i) );
    lLogFmt(" : %6.3f\n",m_length);
  }
  #endif

  //##
  // Log / Print a ray to lLogFmt

  inline void Logf(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_origin(i) );
    lLogFmt(" : ");
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_normal(i) );
    lLogFmt(" : %6.3f\n",m_length);
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Accessors

  inline const lmMatrix<1,N,T>   GetVector() const                     { return m_normal * m_length; }
  inline void                    SetVector(const lmMatrix<1,N,T> &v)   { m_normal = v; m_length = m_normal.Normalize(); }
  inline const lmMatrix<1,N,T>   End() const                           { return m_origin + (m_normal * m_length); }
  inline const lmMatrix<1,N,T>   End(T t) const                        { return m_origin + (m_normal * t); }
  inline void                    Clip( float tnear, float tfar )       { m_origin += (m_normal * tnear); m_length = tfar - tnear; }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Distances / Closest
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns closest distance on ray to p
  inline T                       DistanceToRay(const lmMatrix<1,N,T> &p) { return ClosestOnRay(p).Distance(p); }

  //##
  // Returns closest distance along segment to p
  inline T                       DistanceToSeg(const lmMatrix<1,N,T> &p) { return ClosestOnSeg(p).Distance(p); }

  //##
  // Returns closest point along ray to p
  inline lmMatrix<1,N,T>         ClosestOnRay(const lmMatrix<1,N,T> &p)  { return m_origin + (m_normal * m_normal.Dot(p - m_origin)); }

  //##
  // Return closest point on segment
  inline lmMatrix<1,N,T>         ClosestOnSeg(const lmMatrix<1,N,T> &p)  { bool edge; return ClosestOnSeg(p,edge); }

  //##
  // Return closest point on segment
  // In:
  //   p - Point to find our closest point to
  //   edge - returned true if this is not the start or end of segment
  // Out:
  //   returns point closest on segment to p
  // todo: assumes that m_normal is normalised
  inline lmMatrix<1,N,T>         ClosestOnSeg(const lmMatrix<1,N,T> &p, bool &edge)
  {
    edge = false;
    T t = m_normal.Dot(p - m_origin);
    if (t < T(0)) return m_origin;
    if (t > m_length) return End();
    edge = true;
    return m_origin + (m_normal * t);
  }

  //##
  // Compute nearest points on each ray
  // In:
  //   other - other ray to find closest point on
  //   s     - distance along this ray of closest point
  //   t     - distance along 'other' ray of closest point
  //   c1    - closest point on this ray
  //   c2    - closest point on 'other' ray
  // Out:
  //   Returns squared distance between c1 and c2
  inline float ClosestPtSegmentSegment( lmRay &other, float &s, float &t, lmMatrix<1,N,T> &c1, lmMatrix<1,N,T> &c2 ) const
  {
    const lmMatrix<1,N,T> &p1 = m_origin;
    const lmMatrix<1,N,T> q1 = End();

    const lmMatrix<1,N,T> &p2 = other.m_origin;
    const lmMatrix<1,N,T> q2 = other.End();

    lmMatrix<1,N,T> d1 = q1 - p1; // Direction vector of segment S1
    lmMatrix<1,N,T> d2 = q2 - p2; // Direction vector of segment S2
    lmMatrix<1,N,T> r = p1 - p2;

    float a = d1.Dot(d1); // Squared length of segment S1, always nonnegative
    float e = d2.Dot(d2); // Squared length of segment S2, always nonnegative
    float f = d2.Dot(r);

    // Check if either or both segments degenerate into points
    if (a <= LM_EPSILON && e <= LM_EPSILON)
    {
      // Both segments degenerate into points
      s = t = 0.0f;
      c1 = p1;
      c2 = p2;
      return (c1 - c2).SqrLength();
    }

    if (a <= LM_EPSILON)
    {
      // First segment degenerates into a point
      s = 0.0f;
      t = f / e; // s = 0 => t = (b*s + f) / e = f / e
      t = lmClamp<T>(t, 0.0f, 1.0f);
    } else
    {
      float c = d1.Dot(r);

      if (e <= LM_EPSILON)
      {
        // Second segment degenerates into a point
        t = 0.0f;
        s = lmClamp<T>(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
      } else
      {
        // The general nondegenerate case starts here
        float b = d1.Dot(d2);
        float denom = a*e-b*b; // Always nonnegative

        // If segments not parallel, compute closest point on L1 to L2, and
        // clamp to segment S1. Else pick arbitrary s (here 0)
        if (denom != 0.0f)
        {
          s = lmClamp<T>((b*f - c*e) / denom, 0.0f, 1.0f);
        } else
        {
          s = 0.0f;
        }

        float tnom = b*s + f;
        if (tnom < 0.0f)
        {
          t = 0.0f;
          s = lmClamp<T>(-c / a, 0.0f, 1.0f);
        } else if (tnom > e)
        {
          t = 1.0f;
          s = lmClamp<T>((b - c) / a, 0.0f, 1.0f);
        } else
        {
          t = tnom / e;
        }
      }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return (c1-c2).SqrLength();
  }

  //##
  // Test if two 2D rays overlap, if they do return intersection
  // In:
  //   other - other ray to find closest point on
  //   s     - distance along this ray of closest point
  //   t     - distance along 'other' ray of closest point
  //   c1    - closest point on this ray
  //   c2    - closest point on 'other' ray
  // Out:
  //   Returns squared distance between c1 and c2
  // Notes:
  //   This function will use elements 0 and 1 for the 2d calculation

  // Returns 2 times the signed triangle area. The result is positive if
  // abc is ccw, negative if abc is cw, zero if abc is degenerate.
  inline float Signed2DTriArea( const lmMatrix<1,N,T> &a, const lmMatrix<1,N,T> &b, const lmMatrix<1,N,T> &c ) const
  {
    return (a(0) - c(0)) * (b(1) - c(1)) - (a(1) - c(1)) * (b(0) - c(0));
  }

  inline bool Test2DRayRay( lmRay &other, float &t, lmMatrix<1,N,T> &p ) const
  {
    const lmMatrix<1,N,T> &a = m_origin;
    const lmMatrix<1,N,T> b = End();

    const lmMatrix<1,N,T> &c = other.m_origin;
    const lmMatrix<1,N,T> d = other.End();

    // Sign of areas correspond to which side of ab points c and d are
    float a1 = Signed2DTriArea(a, b, d); // Compute winding of abd (+ or -)
    float a2 = Signed2DTriArea(a, b, c); // To intersect, must have sign opposite of a1

    // If c and d are on different sides of ab, areas have different signs
    if (a1 * a2 < T(0))
    {
      // Compute signs for a and b with respect to segment cd
      float a3 = Signed2DTriArea(c, d, a); // Compute winding of cda (+ or -)

      // Since area is constant a1-a2 = a3-a4, or a4=a3+a2-a1
      float a4 = a3 + a2 - a1;

      // Points a and b on different sides of cd if areas have different signs
      if (a3 * a4 < T(0))
      {
        // Segments intersect. Find intersection point along L(t)=a+t*(b-a).
        // Given height h1 of a over cd and height h2 of b over cd,
        // t = h1 / (h1 - h2) = (b*h1/2) / (b*h1/2 - b*h2/2) = a3 / (a3 - a4),
        // where b (the base of the triangles cda and cdb, i.e., the length
        // of cd) cancels out.
        t = a3 / (a3 - a4);
        p = a + t * (b - a);
        return true;
      }
    }

    // Segments not intersecting (or collinear)
    return false;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Transformations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Ray transformation

  template<unsigned int P,unsigned int Q> inline void Transform( const lmMatrix<P,Q,T> &m )
  {
    m_origin *= m;
    SetVector( GetVector().SubMul3(m) );
  }

  template<unsigned int P,unsigned int Q> inline const lmRay operator*  (const lmMatrix<P,Q,T>& m) const { lmRay<N,T> r = (*this); r.Transform(m); return r;  }
  template<unsigned int P,unsigned int Q> inline void        operator*= (const lmMatrix<P,Q,T>& m)       { Transform(m); }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Intersections
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  // plane intersections

  //##
  // Intersection between ray + plane made up of three points
  // In:
  //   a - Vector a of plane to construct
  //   b - Vector b of plane to construct
  //   c - Vector c of plane to construct
  //   t - Reference to value which have 't' along ray set
  inline const bool Intersect( const lmMatrix<1,N,T> &a, const lmMatrix<1,N,T> &b, const lmMatrix<1,N,T> &c, T &t ) const
  {
    lmPlane<N,T> plane;
    plane.FromPoints( a, b, c );
    return Intersect( plane, t );
  }

  //##
  // Intersection between ray + plane
  // In:
  //   plane - Plane to intersect with ray
  //   t   - Reference to value which have 't' along ray set
  inline const bool Intersect( const lmPlane<N,T> &plane, T &t ) const
  {
    t = (T)0;
    T v = plane.m_normal.Dot( m_normal );
    if( v == (T)0 ) return false;
    t = -(( plane.m_normal.Dot(m_origin) + plane.m_d ) / v);
    if( t<(T)0 ) return false; // not within ray
    return true;
  }

  //##
  // Intersection between segment + plane
  // In:
  //   plane - Plane to intersect with segment
  //   t   - Reference to value which have 't' along ray set
  inline const bool IntersectSegment( const lmPlane<N,T> &plane, T &t ) const
  {
    t = (T)0;
    T v = plane.m_normal.Dot( m_normal );
    if( v == (T)0 ) return false;
    t = -(( plane.m_normal.Dot(m_origin) + plane.m_d ) / v);
    if( t<(T)0 || t>m_length ) return false; // not within segment
    return true;
  }

  //##
  // Returns true if ray intersects sphere
  bool Intersect( const lmSphere<N,T> &sphere ) const
  {
    lmMatrix<1,N,T> m = m_origin - sphere.m_origin;
    float c = m.SqrLength() - sphere.GetSqrRadius();

    // if there is at least one root, must be an intersection
    if( c <= T(0) ) return true;
    float b = m.Dot( m_normal );

    // early exit if origin outside sphere and normal pointing away
    if( b > T(0) ) return false;

    float discriminant = b * b - c;

    // Does normal miss sphere?

    if( discriminant < T(0) ) return false;

    return true;
  }

  //##
  // Returns true if ray intersects sphere
  // In:
  //   sphere - sphere to intersect
  //   t      - reference to value which will have 't' along ray set
  bool Intersect( const lmSphere<N,T> &sphere, T &t ) const
  {
    lmMatrix<1,N,T> m = m_origin - sphere.m_origin;
    float b = m.Dot( m_normal );
    float c = m.SqrLength() - sphere.GetSqrRadius();

    // exit if ray origin is outside sphere and normal pointing away
    if( c > T(0) && b > T(0) ) return false;
    float discriminant = b * b - c;

    // Does normal miss sphere?
    if( discriminant < T(0) ) return false;

    // ray found to intersect, get smallest t
    t = -b - lmSqrt( discriminant );

    // if t is negative then ray started inside sphere so clamp
    if( t < T(0) ) t = T(0);

    return true;
  }

  //##
  // Intersect ray against box
  // In:
  //   box - aabox
  //   tmin - t of the first intersection along line
  // Out:
  //   Returns true on hit

  // This doesn't work -- TimJ

  bool Intersect( const lmAABox<N,T> &box, float &tmin, float *tval2 = NULL ) const
  {
    tmin = T(0);
    float tmax = m_length;

    // For all three slabs
    for (int i = 0; i < N; i++)
    {
      if (lmFabs( m_normal(i) ) < LM_EPSILON)
      {
        // Ray is parallel to slab. No hit if origin not within slab
        if (m_origin(i) < box.m_lower(i) || m_origin(i) > box.m_upper(i))
        {
          return false;
        }
      } else
      {
        // Compute intersection t value of ray with near and far plane of slab
        float ood = 1.0f / m_normal(i);
        float t1 = (box.m_lower(i) - m_origin(i)) * ood;
        float t2 = (box.m_upper(i) - m_origin(i)) * ood;

        // Make t1 be intersection with near plane, t2 with far plane
        if (t1 > t2) lSwap(t1, t2);

        // Compute the intersection of slab intersections intervals
        if (t1 > tmin) tmin = t1;
        if (t2 > tmax) tmax = t2;

        // Exit with no collision as soon as slab intersection becomes empty
        if (tmin > tmax)
        {
          return false;
        }
      }
    }

    // if tval2 pointer passed in then
    if( tval2 )
    {
      *tval2 = tmax;
    }

    // Ray intersects all 3 slabs
    return true;
  }

  //##
  // Intersect ray against triangle
  // In:
  //   tri - triangle
  //   t - t of the intersection along line
  //   u - u barycentric coordinate
  //   v - v barycentric coordinate
  // Out:
  //   Returns true on hit
  inline bool Intersect( const lmTri<T> &tri, T &t, T &u, T &v ) const
  {
    lmVector3 edge1, edge2;

    edge1 = tri.b - tri.a;
    edge2 = tri.c - tri.a;

    lmVector3 p = m_normal.Cross( edge2 );

    //determinant
    T determinant = p.Dot( edge1 );

    if ( determinant > -LM_EPSILON && determinant < LM_EPSILON )
    {
      //determinant close to 0
      return false;
    }

    lmVector3 s = m_origin - tri.a;
    lmVector3 q;

    T uTimesDeterminant = s.Dot( p );
    T vTimesDeterminant;

    if ( determinant > T(0.0f) )
    {
      //positive determinant
      if ( uTimesDeterminant < T(0.0f)  ||  uTimesDeterminant > determinant )
      {
        return false;
      }

      q = s.Cross( edge1 );
      vTimesDeterminant = m_normal.Dot( q );

      if ( vTimesDeterminant < T(0.0f)  || ( uTimesDeterminant + vTimesDeterminant ) > determinant )
      {
        return false;
      }
    }
    else
    {
      //negative determinant
      if ( uTimesDeterminant < determinant || uTimesDeterminant > T(0.0f) )
      {
        return false;
      }

      q = s.Cross( edge1 );
      vTimesDeterminant = m_normal.Dot( q );

      if ( ( uTimesDeterminant + vTimesDeterminant ) < determinant || vTimesDeterminant > T(0.0f) )
      {
        return false;
      }
    }

    T invDeterminant = T( 1.0 ) / determinant;

    T distAlongLine = invDeterminant * edge2.Dot( q );

    if ( distAlongLine >= 0.0f )//TODO: Segment&& distAlongLine < m_length )
    {
      u = uTimesDeterminant * invDeterminant;
      v = vTimesDeterminant * invDeterminant;
      t = distAlongLine;
      return true;
    }
    else
    {
      return false;
    }
  }

  //##
  // Intersect ray against triangle (doesn't care about backfacing tris)
  // In:
  //   tri - triangle
  //   t - t of the intersection along line
  //   u - u barycentric coordinate
  //   v - v barycentric coordinate
  // Out:
  //   Returns true on hit
  inline bool IntersectNoBackface( const lmTri<T> &tri, T &t, T &u, T &v ) const
  {
    lmVector3 edge1, edge2;

    edge1 = tri.b - tri.a;
    edge2 = tri.c - tri.a;

    lmVector3 p = m_normal.Cross( edge2 );

    T determinant = p.Dot( edge1 );

    if ( determinant > -LM_EPSILON && determinant < LM_EPSILON )
    {
      // determinant close to 0
      return false;
    }


    float inv_det = T(1.0) / determinant;

    lmVector3 tvec = m_origin-tri.a;

    u = tvec.Dot( p ) * inv_det;
    if( u < 0.0f || u > 1.0f )
    {
      return false;
    }

    lmVector3 qvec = tvec.Cross(edge1);

    v = m_normal.Dot(qvec) * inv_det;
    if( v < 0.0f || u + v > 1.0f )
    {
      return false;
    }

    t = edge2.Dot(qvec) * inv_det;

    if( t >= 0.0f )
    {
      return true;
    }

    return false;
  }
};

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Typedefs
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef lmRay<3,float> lmRay3;
typedef lmRay<2,float> lmRay2;

typedef lmRay<3,double> lmRay3l;
typedef lmRay<2,double> lmRay2l;

END_L_NAMESPACE
#endif

