//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmcapsule.h
// Description : Capsule math class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMCAPSULE_H_INCLUDED
#define LMCAPSULE_H_INCLUDED

#include "lmmatrix.h"
#include "./optimisations/lbase.h"

BEGIN_L_NAMESPACE

//##
//  Capsule geometry class
struct lmCapsule
{
  lmVector3 m_a, m_b;
  float m_radius, m_sqrRadius;

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  inline lmCapsule()
  {
    m_a = lmVector3( 0.0f, 0.0f, 0.0f );
    m_b = lmVector3( 0.0f, 0.0f, 0.0f );
    m_radius = 0.0f;
    m_sqrRadius = 0.0f;
  }

  //##
  // Construct capsule from two end points and radius
  inline lmCapsule( const lmVector3 &a, const lmVector3 &b, float radius )
  {
    m_a = a;
    m_b = b;
    m_radius = radius;
    m_sqrRadius = radius * radius;
  }

  //##
  // Construct capsule from radius + length along z axis
  inline lmCapsule( float radius, float length )
  {
    float halflen = length * 0.5f;
    m_a = lmVector3( 0, 0, -halflen );
    m_b = lmVector3( 0, 0,  halflen );
    m_radius = radius;
    m_sqrRadius = radius * radius;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Construct from
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

//  inline void ExtendArray(const lArray< lmMatrix<1,N,T> > &points)

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return true if capsule is effectively a sphere
  bool IsSphere() const
  {
    return m_a.SqrDistance( m_b )  <  LM_SQR_EPSILON;
  }

  //##
  // Get one end of the capsule
  const lmVector3 & GetA() const
  {
    return m_a;
  }

  //##
  // Get one end of the capsule
  const lmVector3 & GetB() const
  {
    return m_b;
  }

  //##
  // Get distance between ends of capsule
  float GetLength() const
  {
    return m_a.Distance( m_b );
  }

  //##
  // Get radius of capsule
  float GetRadius() const
  {
    return m_radius;
  }

  //##
  // Get squared radius of capsule
  float GetSqrRadius() const
  {
    return m_radius*m_radius;
  }

  //##
  // Get centre of capsule
  lmVector3 GetCentre() const
  {
    return ( m_a + m_b )  *  0.5f;
  }

  //##
  // Get direction of capsule
  lmVector3 GetDirection() const
  {
    return m_b - m_a;
  }

  //##
  // Transform capsule by matrix
  friend inline lmCapsule & operator*=(lmCapsule &cap, const lmMatrix4 &m)
  {
    cap.m_a *= m;
    cap.m_b *= m;
    return cap;
  }

  /*


// Returns the squared distance between point c and segment ab
float SqDistPointSegment(Point a, Point b, Point c)
{
    Vector ab = b – a, ac = c – a, bc = c – b;
    float e = Dot(ac, ab);
    // Handle cases where c projects outside ab
    if (e <= 0.0f) return Dot(ac, ac);
    float f = Dot(ab, ab);
    if (e >= f) return Dot(bc, bc);
    // Handle case where c projects onto ab
    return Dot(ac, ac) – e * e / f;
}


// Clamp n to lie within the range [min, max]
float Clamp(float n, float min, float max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
float ClosestPtSegmentSegment(Point p1, Point q1, Point p2, Point q2,
                              float &s, float &t, Point &c1, Point &c2)
{
    Vector d1 = q1 - p1; // Direction vector of segment S1
    Vector d2 = q2 - p2; // Direction vector of segment S2
    Vector r = p1 - p2;
    float a = Dot(d1, d1); // Squared length of segment S1, always nonnegative
    float e = Dot(d2, d2); // Squared length of segment S2, always nonnegative
    float f = Dot(d2, r);

    // Check if either or both segments degenerate into points
    if (a <= EPSILON && e <= EPSILON) {
        // Both segments degenerate into points
        s = t = 0.0f;
        c1 = p1;
        c2 = p2;
        return Dot(c1 - c2, c1 - c2);
    }
    if (a <= EPSILON) {
        // First segment degenerates into a point
        s = 0.0f;
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = Clamp(t, 0.0f, 1.0f);
    } else {
        float c = Dot(d1, r);
        if (e <= EPSILON) {
            // Second segment degenerates into a point
            t = 0.0f;
            s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
        } else {
            // The general nondegenerate case starts here
            float b = Dot(d1, d2);
            float denom = a*e-b*b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2, and
            // clamp to segment S1. Else pick arbitrary s (here 0)
            if (denom != 0.0f) {
                s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
            } else s = 0.0f;

            // Compute point on L2 closest to S1(s) using
            // t = Dot((P1+D1*s)-P2,D2) / Dot(D2,D2) = (b*s + f) / e
            t = (b*s + f) / e;

            // If t in [0,1] done. Else clamp t, recompute s for the new value
            // of t using s = Dot((P2+D2*t)-P1,D1) / Dot(D1,D1)= (t*b - c) / a
            // and clamp s to [0, 1]
            if (t < 0.0f) {
                t = 0.0f;
                s = Clamp(-c / a, 0.0f, 1.0f);
            } else if (t > 1.0f) {
                t = 1.0f;
                s = Clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return Dot(c1 - c2, c1 - c2);
}


   int TestSphereCapsule(Sphere s, Capsule capsule)
  {
      // Compute (squared) distance between sphere center and capsule line segment
      float dist2 = SqDistPointSegment(capsule.a, capsule.b, s.c);

      // If (squared) distance smaller than (squared) sum of radii, they collide
      float radius = s.r + capsule.r;
      return dist2 <= radius * radius;
  }

  int TestCapsuleCapsule(Capsule capsule1, Capsule capsule2)
  {
      // Compute (squared) distance between the inner structures of the capsules
      float s, t;
      Point c1, c2;
      float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b,
                                            capsule2.a, capsule2.b, s, t, c1, c2);

      // If (squared) distance smaller than (squared) sum of radii, they collide
      float radius = capsule1.r + capsule2.r;
      return dist2 <= radius * radius;
  }*/


};


END_L_NAMESPACE
#endif

