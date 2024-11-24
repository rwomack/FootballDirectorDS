//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmsphere.inl
// Description : lmSphere inline functions
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LMSPHERE_INL
#define __LMSPHERE_INL

#include "lmsphere.h"
#include "lmaabox.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmSphere functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Construct lmSphere to fit around an AABox
// In:
//   box - lmAABox to construct sphere around
template <unsigned int N, typename T>
lmSphere<N,T> lmSphere<N,T>::AroundBox( const lmAABox<N,T> &box )
{
  lmSphere<N,T> sph;
  sph.Extend( box );
  return sph;
}

//##
// Extend sphere to fit around box
template <unsigned int N, typename T>
void lmSphere<N,T>::Extend( const lmAABox<N,T> &box )
{
  lmSphere<N,T> sphere( box.GetCentre(), box.GetRadius() );

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
// Checks for a sphere intersecting a box
// Returns true if box intersects sphere and the distance to the origin (squared)
template <unsigned int N, typename T>
bool lmSphere<N,T>::Intersect( const lmAABox<N,T> &box, T &distToOriginSquared ) const
{
  // start the closest point on the surface of the box at the origin of the
  // sphere relative to the box centre
  lmMatrix<1,N,T> closestPointOnBox = GetOrigin() - box.GetCentre();
  lmMatrix<1,N,T> halfSize = box.GetHalfSize();

  // project test point onto box, and accumulate the square of the distance as we go
  // for each axis, move the test point onto the closest point
  // on the surface of the box

  T distanceSquared = 0.0f;

  for (int i = 0; i < N; i++)
  {
    if ( closestPointOnBox(i) < -halfSize(i) )
    {
      T delta = closestPointOnBox(i) + halfSize(i);
      distanceSquared += delta * delta;
      closestPointOnBox(i) = -halfSize(i);
    }
    else if ( closestPointOnBox(i) > halfSize(i) )
    {
      T delta = closestPointOnBox(i) - halfSize(i);
      distanceSquared += delta * delta;
      closestPointOnBox(i) = halfSize(i);
    }
  }

  // output the distance squared
  if ( distanceSquared <= lmSq<T>( GetRadius() ) )
  {
    distToOriginSquared = distanceSquared;
    return true;
  }
  else
  {
    return false;
  }
}

END_L_NAMESPACE
#endif
