//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmdecomp.h
// Description : Decomposition related include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMDECOMP_H_INCLUDED
#define LMDECOMP_H_INCLUDED

#include "lmath.h"
#include "lmmatrix.h"
#include "lmquaternion.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Matrix decomposition
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
//  <unfinished>
//
//  A class for storing and manipulating parts of an affine transformation
class LMATH_API lmAffineParts
{
public:
  // T F R U K U'
  // U' - inverse of u.
  // T - translation matrix
  // R - rotation defined by Quaternion q.
  // U - rotates you into the coordinates system where the scaling or stretching is done
  // K - scaling matrix
  // F - either an identity matrix or negative identity matrix

  lmVector3           t; // Translation components
  lmQuaternion        q; // Essential rotation
  lmQuaternion        u; // Stretch rotation
  lmVector3           k; // Stretch factors
  float               f; // Sign of determinant

  inline lmAffineParts()
  {
    t = lmVector3( 0.0f, 0.0f, 0.0f );
    u = q = lmQuaternion( 0.0f, 0.0f, 0.0f, 1.0f );
    k = lmVector3( 1.0f, 1.0f, 1.0f );
    f = 1.0f;
  }

  //##
  //  Construct using the affine transformation parts passed in
  //
  // In:
  //  t - translation components
  //  q - essential rotation
  //  u - stretch rotation
  //  k - stretch factors
  //  f - sign of determinant
  inline lmAffineParts(const lmVector3 &t, const lmQuaternion &q, const lmQuaternion &u,
                       const lmVector3 &k, float f) : t( t ), q( q ), u( u ), k( k ), f( f )
  {
  }

  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s:\n", str );
    t.Log("t");
    q.Log("q");
    u.Log("u");
    k.Log("k");
    lLogFmt("f: %6.3f\n", f);
  }
  #endif

  //##
  //  Interpolate between two sets of affine transformation parts and return the
  //  resulting affine transformation represented in an lmAffineParts.
  //
  //  Example:
  //  <code>
  //  lmAffineParts a, b;
  //
  //  lmAffineParts result = lmAffineParts::Interpolate( a, b, 0.5f );
  //  </code>
  static lmAffineParts Interpolate(const lmAffineParts &p0, const lmAffineParts &p1, float t)
  {
    lmQuaternion iq = lmQuaternion::Slerp(t, p0.q, p1.q );
    lmQuaternion iu = lmQuaternion::Slerp(t, p0.u, p1.u );

    lmQuaternion::Normalize( iq );
    lmQuaternion::Normalize( iu );

    return lmAffineParts( p0.t  +  ( p1.t - p0.t ) * t,  iq,  iu,
                           /*p0.k  +  ( p1.k - p0.k ) * t,*/        // TIMJ-HACK: there's a problem with interpolating the scale
                           p0.k,                                    //            so we just use the scale from the source. eep.
			                     p0.f  +  ( p1.f - p0.f ) * t );
  }
};


void LMATH_API lmDecompAffine( const lmMatrix4 &m, lmAffineParts &parts, float stretchEpsilon = LM_EPSILON );
void LMATH_API lmRecompAffine( lmMatrix4 &m, const lmAffineParts &parts );

END_L_NAMESPACE

#endif


