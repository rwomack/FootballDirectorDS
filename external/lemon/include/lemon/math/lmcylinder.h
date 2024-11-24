//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmcylinder.h
// Description : Cylinder math class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMCYLINDER_H_INCLUDED
#define LMCYLINDER_H_INCLUDED

#include "lmath.h"

BEGIN_L_NAMESPACE

//##
//  Cylinder geometry class
struct lmCylinder
{
  lmVector3 m_a, m_b;
  float m_radius, m_sqrRadius;

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  inline lmCylinder()
  {
    m_a = lmVector3( 0.0f, 0.0f, 0.0f );
    m_b = lmVector3( 0.0f, 0.0f, 0.0f );
    m_radius = 0.0f;
    m_sqrRadius = 0.0f;
  }

  inline lmCylinder(const lmVector3 &a, const lmVector3 &b, float radius)
  {
    m_a = a;
    m_b = b;
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
  // Get one end of the cylinder
  const lmVector3 & GetA() const
  {
    return m_a;
  }

  //##
  // Get one end of the cylinder
  const lmVector3 & GetB() const
  {
    return m_b;
  }

  //##
  // Get distance between ends of cylinder
  float GetLength() const
  {
    return m_a.Distance( m_b );
  }

  //##
  // Get radius of cylinder
  float GetRadius() const
  {
    return m_radius;
  }

  //##
  // Get centre of cylinder
  lmVector3 GetCentre() const
  {
    return ( m_a + m_b )  *  0.5f;
  }

  //##
  // Get direction of cylinder
  lmVector3 GetDirection() const
  {
    return m_b - m_a;
  }

  //##
  // Transform capsule by matrix
  friend inline lmCylinder & operator*=(lmCylinder &cyl, const lmMatrix4 &m)
  {
    cyl.m_a *= m;
    cyl.m_b *= m;
    return cyl;
  }
};


END_L_NAMESPACE
#endif

