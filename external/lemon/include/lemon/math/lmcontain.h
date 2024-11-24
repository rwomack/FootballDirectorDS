//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmContain.h
// Description : Containment functions
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright 2005 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMCONTAINERS_H_INCLUDED
#define LMCONTAINERS_H_INCLUDED

#include "lmath.h"

#include "lmgeom.h"
#include "lmaabox.h"
#include "lmcapsule.h"
#include "lmplane.h"
#include "lmtri.h"
#include "lmpolygon.h"
#include "lmray.h"
#include "lmsphere.h"
#include "lmobb.h"

BEGIN_L_NAMESPACE

// Used to return intersect, contained, no intersection
// empty primitives should not be able to intersect ? (what about an empty prim inside a non-empty one?)

enum
{
  CONTAIN_OUTSIDE = 0,
  CONTAIN_INTERSECT,
  CONTAIN_INSIDE,
};

//bool        lmContain( lmSphere<T> &s, lmAABox<T> &box );

//##
// Check if sphere intersects plane
template <unsigned int N, typename T>
inline int lmContain( const lmSphere<N,T> &s, const lmPlane<N,T> &plane )
{
  T dist = plane.Distance( s.m_origin );

  if( dist < -s.m_radius )
  {
    // sphere is fully contained in negative halfspace
    return CONTAIN_INSIDE;
  }

  if( lmFabsf( dist ) <= s.m_radius )
  {
    return CONTAIN_INTERSECT;
  }

  return CONTAIN_OUTSIDE;
}

//##
// Check if obb intersects plane
template <unsigned int N, typename T>
inline int lmContain( const lmOBB<N,T> &b, const lmPlane<N,T> &plane )
{
  lTAssert( N == 3 );                   // this function is 3d right now

  // compute projection interval radius of b onto plane
  T r = b.m_extent(0) * lmFabsf( plane.m_normal.Dot( b.m_axis[0] ) ) +
        b.m_extent(1) * lmFabsf( plane.m_normal.Dot( b.m_axis[1] ) ) +
        b.m_extent(2) * lmFabsf( plane.m_normal.Dot( b.m_axis[2] ) );

  // compute distance of obb centre from plane
  T s = plane.Distance( b.m_centre );

  if( s < -r )
  {
    // obb is fully contained in negative halfspace
    return CONTAIN_INSIDE;
  }

  if( lmFabsf( s ) <= r )
  {
    return CONTAIN_INTERSECT;
  }

  return CONTAIN_OUTSIDE;
}

//##
// Check if aabox intersects plane
template <unsigned int N, typename T>
inline int lmContain( const lmAABox<N,T> &b, const lmPlane<N,T> &plane )
{
  lTAssert( N == 3 );                   // this function is 3d right now

  // compute centre + extents
  lmMatrix<1,N,T> c = b.GetCentre();
  lmMatrix<1,N,T> e = b.GetHalfSize();

  // compute project interval of box onto plane
  T r = e(0) * lmFabsf( plane.m_normal(0) ) +
        e(1) * lmFabsf( plane.m_normal(1) ) +
        e(2) * lmFabsf( plane.m_normal(2) );

  // compute distance of box centre from plane
  T s = plane.Distance( c );

  if( s < -r )
  {
    // aabox is fully contained in negative halfspace
    return CONTAIN_INSIDE;
  }

  if( lmFabsf( s ) <= r )
  {
    return CONTAIN_INTERSECT;
  }

  return CONTAIN_OUTSIDE;
}

//##
// Check if sphere intersects aabox
// In:
//   s - Sphere to check
//   b - AABox to check
//   closestPoint - pointer to a vector which will have the closest point on the AABox to the sphere centre set in it
template <unsigned int N, typename T>
inline int lmContain( const lmSphere<N,T> &s, const lmAABox<N,T> &b, lmMatrix<1,N,T> *closestPoint = NULL )
{
  lTAssert( N == 3 );                   // this function is 3d right now

  lmMatrix<1,N,T> point = b.ClosestPoint( s.m_origin );
  float           sqrRadius = s.GetSqrRadius();

  if( point.SqrDistance( s.m_origin ) > sqrRadius )
  {
    return CONTAIN_OUTSIDE;
  }

  // todo: check this
  // Should be finding point furthest from sphere centre and checking distance
  lmMatrix<1,N,T> sphereToBox = b.GetCentre() - s.m_origin;
  lmMatrix<1,N,T> leadingPoint = b.GetLeadingPoint( sphereToBox );

  if( leadingPoint.SqrDistance( s.m_origin ) <= sqrRadius )
  {
    // box is fully contained in sphere
    return CONTAIN_INSIDE;
  }

  return CONTAIN_INTERSECT;
}

//##
// Check if sphere intersects obb
// In:
//   s - Sphere to check
//   b - OBB to check
template <unsigned int N, typename T>
inline int lmContain( const lmSphere<N,T> &s, const lmOBB<N,T> &b )
{
  lTAssert( N == 3 );                   // this function is 3d right now

  lmMatrix<1,N,T> point = b.ClosestPoint( s.m_origin );

  if( point.SqrDistance( s.m_origin ) > s.GetSqrRadius() )
  {
    return CONTAIN_OUTSIDE;
  }

  // todo: check this
  // Should be finding point furthest from sphere centre and checking distance
  lmMatrix<1,N,T> sphereToBox = b.GetCentre() - s.m_origin;
  lmMatrix<1,N,T> leadingPoint = b.GetLeadingPoint( sphereToBox );

  if( leadingPoint.SqrDistance( s.m_origin )  <=  s.GetSqrRadius() )
  {
    // box is fully contained in sphere
    return CONTAIN_INSIDE;
  }

  return CONTAIN_INTERSECT;
}

//##
// Check if sphere contains point
// In:
//   s - Sphere to check
//   p - point to check
template <unsigned int N, typename T>
inline int lmContain( const lmSphere<N,T> &s, const lmMatrix<1,N,T> &p )
{
  if( p.SqrDistance( s.m_origin ) <= s.GetSqrRadius() )
  {
    return CONTAIN_INSIDE;
  }

  return CONTAIN_OUTSIDE;
}

//##
// Check if sphere contains another sphere
// In:
//   s1 - Sphere to check
//   s2 - Sphere to check
template <unsigned int N, typename T>
inline int lmContain( const lmSphere<N,T> &s1, const lmSphere<N,T> &s2 )
{
  float newrad = (s2.m_origin - s1.m_origin).SqrLength();

  if( newrad < ( ( s1.m_radius + s2.m_radius ) * ( s1.m_radius + s2.m_radius ) ) )
  {
    return CONTAIN_INSIDE;
  }

  // todo: add intersect

  return CONTAIN_OUTSIDE;
}

//##
// Check if sphere intersects triangle
// In:
//   s - Sphere to check
//   t - Triangle to check
template <unsigned int N, typename T>
inline int lmContain( const lmSphere<N,T> &s, const lmTri<T> &t )
{
  lTAssert( N == 3 );                   // this function is 3d right now

  // find closest point on tri to sphere centre
  lmMatrix<1,N,T> point = t.ClosestPtPointTriangle( s.m_origin );

  if( point.SqrDistance( s.m_origin ) > s.GetSqrRadius() )
  {
    return CONTAIN_OUTSIDE;
  }

  // check if tri contained inside
  if( t.a.SqrDistance( s.m_origin ) <= s.GetSqrRadius() &&
      t.b.SqrDistance( s.m_origin ) <= s.GetSqrRadius() &&
      t.c.SqrDistance( s.m_origin ) <= s.GetSqrRadius() )
  {
    return CONTAIN_INSIDE;
  }

  return CONTAIN_INTERSECT;
}

//##
// Check if sphere intersects polygon
// In:
//   s - Sphere to check
//   p - Polygon to check
template <unsigned int N, typename T>
inline int lmContain( const lmSphere<N,T> &s, const lmPolygon<T> &p )
{
  lTAssert( N == 3 );                   // this function is 3d right now
  lAssert( p.points.Size() >= 3 );      // check we have enough points in polygon

  // calc plane equation
  lmPlane<N,T> m = p.GetPlane();

  // no intersection if sphere doesnt intersect plane of polygon
  if( lmContain( s, m ) != CONTAIN_INTERSECT )
  {
    return CONTAIN_OUTSIDE;
  }

  //
  bool bFullyContained = true;
  bool bIntersect = false;

  // check if any of the edges pierces the sphere
  for( int k = p.points.Size(), i = 0, j = k - 1; i < k; j = i, i++ )
  {
    float t;
    lmMatrix<1,N,T> q;

    if( p.points(i).SqrDistance( s.m_origin ) > s.GetSqrRadius() )
    {
      bFullyContained = false;
    }

    // test if edge intersects s
    lmRay<N,T> testray( p.points(j), p.points(i) - p.points(j) );
    if( testray.Intersect( s, t, q ) && t <= 1.0f )
    {
      // edge intersects sphere
      bIntersect = true;
    }
  }

  // all the points in the polygon are inside the sphere
  if( bFullyContained )
  {
    return CONTAIN_INSIDE;
  }

  // did anything intersect
  return bIntersect ? CONTAIN_INTERSECT : CONTAIN_OUTSIDE;
}

// sphere
// box
// obb
// capsule
// cylinder
// cone
// frustum


END_L_NAMESPACE
#endif
