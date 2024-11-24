//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmCollider.h
// Description : Collider / Intersect functions
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMCOLLIDERS_H_INCLUDED
#define LMCOLLIDERS_H_INCLUDED

#include "lmath.h"

#include "lmgeom.h"

BEGIN_L_NAMESPACE

template<unsigned int N, typename T>
class lmAABox;

template <unsigned int N, typename T>
class lmSphere;

template< unsigned int N, typename T >
struct lmPlane;

template< unsigned int N, typename T >
struct lmRay;

struct lmCapsule;

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lmCollide - Collides two primitives and returns the number of contacts
//             added to the lmGeomContactSet
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

// lmSphere<3,float> collisions

int lmCollide( lmGeomContactSet &contactSet, const lmSphere<3,float> &sphere1, const lmSphere<3,float> &sphere2,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

int lmCollide( lmGeomContactSet &contactSet, const lmSphere<3,float> &sphere, const lmAABox<3,float> &box,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

int lmCollide( lmGeomContactSet &contactSet, const lmSphere<3,float> &sphere, const lmPlane<3,float> &plane,
                                             const lmMatrix4 &m1, const lmMatrix4 &m2 );

int lmCollide( lmGeomContactSet &contactSet, const lmSphere<3,float> &sphere, const lmPlane<3,float> &p );

int lmCollide( lmGeomContactSet &contactSet, const lmSphere<3,float> &sphere, const lmRay<3,float> &ray,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

// box collisions

int lmCollide( lmGeomContactSet &contactSet, const lmAABox<3,float> &box1, const lmAABox<3,float> &box2,
                                             const lmMatrix4 &m1, const lmMatrix4 &m2 );

int lmCollide( lmGeomContactSet &contactSet, const lmAABox<3,float> &box1, const lmAABox<3,float> &box2 );

int lmCollide( lmGeomContactSet &contactSet, const lmAABox<3,float> &box, const lmPlane<3,float> &plane,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

int lmCollide( lmGeomContactSet &contactSet, const lmRay<3,float> &ray, const lmAABox<3,float> &box,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

// capsule collisions

int lmCollide( lmGeomContactSet &contactSet, const lmCapsule &cap1, const lmCapsule &cap2,
                                             const lmMatrix4 &m1, const lmMatrix4 &m2 );

int lmCollide( lmGeomContactSet &contactSet, const lmCapsule &cap1, const lmCapsule &cap2 );

int lmCollide( lmGeomContactSet &contactSet, const lmCapsule &cap, const lmPlane<3,float> &plane,
                                             const lmMatrix4 &m1, const lmMatrix4 &m2 );

int lmCollide( lmGeomContactSet &contactSet, const lmCapsule &cap, const lmPlane<3,float> &plane );

int lmCollide( lmGeomContactSet &contactSet, const lmCapsule &cap, const lmAABox<3,float> &box,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

int lmCollide( lmGeomContactSet &contactSet, const lmCapsule &cap, const lmSphere<3,float> &sphere,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

int lmCollide( lmGeomContactSet &contactSet, const lmCapsule &cap, const lmRay<3,float> &ray,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

// plane collisions

int lmCollide( lmGeomContactSet &contactSet, const lmRay<3,float> &ray, const lmPlane<3,float> &plane,
                                             const lmMatrix4 &m1 = lmMatrix4::GetIdentity(),
                                             const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lmIntersects - Checks if two primitives intersect
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

bool lmIntersects( const lmSphere<3,float> &sphere1, const lmMatrix4 &m1, const lmSphere<3,float> &sphere2, const lmMatrix4 &m2 );


END_L_NAMESPACE
#endif

//int lmCollide( lmGeomContactSet &contactSet, const lmSphere<3,float> &sphere1, const lmSphere<3,float> &sphere2,
//               const lmMatrix4 &m1 = lmMatrix4::GetIdentity(), const lmMatrix4 &m2 = lmMatrix4::GetIdentity() );
