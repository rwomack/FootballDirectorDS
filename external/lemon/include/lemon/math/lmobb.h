//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmobb.h
// Description : Oriented bounding box
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMOBB_H_INCLUDED
#define LMOBB_H_INCLUDED

#include "lmath.h"
#include "lmmatrix.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lmOBB - Oriented bounding box
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

template<unsigned int N, typename T>
class lmOBB
{
public:
  lmMatrix<1,N,T>   m_centre;                  // centre
  lmMatrix<1,N,T>   m_axis[N];                 // axis of obb
  lmMatrix<1,N,T>   m_extents;                 // extents

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Default constructor to empty box
  inline lmOBB()
	{
    Empty();
	}

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Serialize to a lStream object
  friend inline lStream &operator<<( lStream &ar, lmOBB &b )
  {
    ar << b.m_centre;
    for(int i = 0; i < N; i++)
    {
      ar << b.m_axis[i];
    }
    ar << b.m_extents;
    return ar;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Empty / Extending
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Empty OBB
  inline void Empty()
  {
		for (unsigned int i = 0; i < N; i++)
		{
      m_centre(i) = m_extents(i) = T(0.0);
    }

		for (unsigned int i = 0; i < N; i++)
		{
      for (unsigned int j = 0; j < N; j++)
      {
        m_axis[i](j) = T(0.0);
      }
    }
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Intersection test
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Test isect with another OBB
  // TODO: put calc of R and AbsR in with the calcs
  static inline bool TestOBBOBB( lmOBB &a, lmOBB &b )
  {
    float ra, rb;
    lmMatrix3 R, AbsR;

    // Compute rotation matrix expressing b in a's coordinate frame
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            R(i,j) = a.m_axis[i].Dot(b.m_axis[j]);

    // Compute translation vector t
    lmVector3 t = b.m_centre - a.m_centre;
    // Bring translation into a's coordinate frame
    t = lmVector3(t.Dot(a.m_axis[0]), t.Dot(a.m_axis[1]), t.Dot(a.m_axis[2]) );

    // Compute common subexpressions. Add in an epsilon term to
    // counteract arithmetic errors when two edges are parallel and
    // their cross product is (near) null (see text for details)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            AbsR(i,j) = lmFabsf(R(i,j)) + LM_EPSILON;

    // Test axes L = A0, L = A1, L = A2
    for (int i = 0; i < 3; i++)
    {
        ra = a.m_extents(i);
        rb = b.m_extents(0) * AbsR(i,0) + b.m_extents(1) * AbsR(i,1) + b.m_extents(2) * AbsR(i,2);
        if (lmFabsf(t(i)) > ra + rb) return false;
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++)
    {
        ra = a.m_extents(0) * AbsR(0,i) + a.m_extents(1) * AbsR(1,i) + a.m_extents(2) * AbsR(2,i);
        rb = b.m_extents(i);
        if (lmFabsf(t(0) * R(0,i) + t(1) * R(1,i) + t(2) * R(2,i)) > ra + rb) return false;
    }

    // Test axis L = A0 x B0
    ra = a.m_extents(1) * AbsR(2,0) + a.m_extents(2) * AbsR(1,0);
    rb = b.m_extents(1) * AbsR(0,2) + b.m_extents(2) * AbsR(0,1);
    if (lmFabsf(t(2) * R(1,0) - t(1) * R(2,0)) > ra + rb) return false;

    // Test axis L = A0 x B1
    ra = a.m_extents(1) * AbsR(2,1) + a.m_extents(2) * AbsR(1,1);
    rb = b.m_extents(0) * AbsR(0,2) + b.m_extents(2) * AbsR(0,0);
    if (lmFabsf(t(2) * R(1,1) - t(1) * R(2,1)) > ra + rb) return false;

    // Test axis L = A0 x B2
    ra = a.m_extents(1) * AbsR(2,2) + a.m_extents(2) * AbsR(1,2);
    rb = b.m_extents(0) * AbsR(0,1) + b.m_extents(1) * AbsR(0,0);
    if (lmFabsf(t(2) * R(1,2) - t(1) * R(2,2)) > ra + rb) return false;

    // Test axis L = A1 x B0
    ra = a.m_extents(0) * AbsR(2,0) + a.m_extents(2) * AbsR(0,0);
    rb = b.m_extents(1) * AbsR(1,2) + b.m_extents(2) * AbsR(1,1);
    if (lmFabsf(t(0) * R(2,0) - t(2) * R(0,0)) > ra + rb) return false;

    // Test axis L = A1 x B1
    ra = a.m_extents(0) * AbsR(2,1) + a.m_extents(2) * AbsR(0,1);
    rb = b.m_extents(0) * AbsR(1,2) + b.m_extents(2) * AbsR(1,0);
    if (lmFabsf(t(0) * R(2,1) - t(2) * R(0,1)) > ra + rb) return false;

    // Test axis L = A1 x B2
    ra = a.m_extents(0) * AbsR(2,2) + a.m_extents(2) * AbsR(0,2);
    rb = b.m_extents(0) * AbsR(1,1) + b.m_extents(1) * AbsR(1,0);
    if (lmFabsf(t(0) * R(2,2) - t(2) * R(0,2)) > ra + rb) return false;

    // Test axis L = A2 x B0
    ra = a.m_extents(0) * AbsR(1,0) + a.m_extents(1) * AbsR(0,0);
    rb = b.m_extents(1) * AbsR(2,2) + b.m_extents(2) * AbsR(2,1);
    if (lmFabsf(t(1) * R(0,0) - t(0) * R(1,0)) > ra + rb) return false;

    // Test axis L = A2 x B1
    ra = a.m_extents(0) * AbsR(1,1) + a.m_extents(1) * AbsR(0,1);
    rb = b.m_extents(0) * AbsR(2,2) + b.m_extents(2) * AbsR(2,0);
    if (lmFabsf(t(1) * R(0,1) - t(0) * R(1,1)) > ra + rb) return false;

    // Test axis L = A2 x B2
    ra = a.m_extents(0) * AbsR(1,2) + a.m_extents(1) * AbsR(0,2);
    rb = b.m_extents(0) * AbsR(2,1) + b.m_extents(1) * AbsR(2,0);
    if (lmFabsf(t(1) * R(0,2) - t(0) * R(1,2)) > ra + rb) return false;

    // Since no separating axis found, the OBBs must be intersecting
    return true;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Closest point
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Get closest point on box to point
  inline lmMatrix<1,N,T> ClosestPoint( const lmMatrix<1,N,T> &point ) const
  {
    lmMatrix<1,N,T> d = point - m_centre;
    lmMatrix<1,N,T> q = m_centre;

    for (unsigned int i = 0; i < N; i++)
    {
      // Project d into axis, get distance along axis
      float dist = d.Dot( m_axis(i) );

      // clamp to box if distance greater than extents
      if( dist >  m_extents(i) ) dist =  m_extents(i);
      if( dist < -m_extents(i) ) dist = -m_extents(i);

      q += dist * m_axis(i);
    }
  }

  //##
  // Get closest square distance and closest point from box to point
  inline T SqrDistanceToClosestPoint( const lmMatrix<1,N,T> &point, lmMatrix<1,N,T> &closestPoint ) const
  {
    closestPoint = ClosestPoint( point );
    T sqDist = closestPoint.SqrDistance( point );
    return sqDist;
  }

  //##
  // Get closest distance and closest point from box to point
  inline T DistanceToClosestPoint( const lmMatrix<1,N,T> &point, lmMatrix<1,N,T> &closestPoint ) const
  {
    return lmSqrt( SqrDistanceToClosestPoint( point, closestPoint ) );
  }

  //compute the intersection of the faces of @this with an outgoing ray
  void ComputeOutgoingRayIntersection(const lmVector3 &rayOrigin, const lmVector3 &rayVector,
                                      T &t, unsigned int &faceIndex) const
  {
    lAssert( Contains( rayOrigin ) );

    faceIndex = 0;
    bool bFound = false;

    for (unsigned int i = 0; i < N; i++)
    {
      T r = rayVector(i);

      if ( r != T(0.0) )
      {
        T thisT;
        unsigned int faceSign;

        if ( r < T(0.0) )
        {
          thisT = ( m_lower(i) - rayOrigin(i) )  /  r;
          faceSign = 0;
        }
        else if ( r > T(0.0) )
        {
          thisT = ( m_upper(i) - rayOrigin(i) )  /  r;
          faceSign = 1;
        }

        if ( !bFound  ||  thisT < t )
        {
          t = thisT;
          faceIndex = i * 2  +  faceSign;
          bFound = true;
        }
      }
    }
  }
};

typedef lmOBB<2,float> lmOBB2;
typedef lmOBB<3,float> lmOBB3;

typedef lmOBB<2,double> lmOBB2l;
typedef lmOBB<3,double> lmOBB3l;

END_L_NAMESPACE
#endif
