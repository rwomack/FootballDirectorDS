//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmtri.h
// Description : Triangle geometry class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMTRI_H_INCLUDED
#define LMTRI_H_INCLUDED

#include "lmath.h"
#include "lmplane.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmTri - Triangle
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
//  Triangle template class
template <typename T>
class lmTri
{
public:
  lmMatrix<1,3,T> a, b, c;

  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
  // Constructors
  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

  //##
  // Default constructor with no initialisation
  inline lmTri()
  {
  }

  //##
  // Constructor from three vertices
  inline lmTri( const lmMatrix<1,3,T> &a, const lmMatrix<1,3,T> &b, const lmMatrix<1,3,T> &c )
  {
    this->a = a;
    this->b = b;
    this->c = c;
  }

  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
  // Accessors
  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

  //##
  // Get U vector ( b - a )
  inline lmMatrix<1,3,T> GetU() const
  {
    return b - a;
  }

  //##
  // Get V vector ( c - a )
  inline lmMatrix<1,3,T> GetV() const
  {
    return c - a;
  }

  //##
  // Get normal of triangle
  inline lmMatrix<1,3,T> GetNormal() const
  {
    return GetU().Cross( GetV() );
  }

  //##
  // Get unit normal of triangle
  inline lmMatrix<1,3,T> GetUnitNormal() const
  {
    lmVector3 normal = GetNormal();
    normal.Normalize();
    return normal;
  }

  //##
  // Get mid point of the triangle
  inline lmMatrix<1,3,T> GetMidPoint() const
  {
    return ((a + b + c) * (1.0f / 3.0f));
  }

  //##
  // Return plane derived from triangle
  lmPlane<3,T> GetPlane() const
  {
    lmPlane<3,T> result;
    result.FromPoints( a, b, c );
    return result;
  }

  //##
  // Compute the area of the triangle
  inline T ComputeArea() const
  {
    return GetNormal().Length();
  }

  //##
  // Compute the area^2 of the triangle
  inline T ComputeSqrArea() const
  {
    return GetNormal().SqrLength();
  }

  //##
  // Return true if the triangle is degenerate
  inline bool IsDegenerate( float epsilon = LM_EPSILON ) const
  {
    return ComputeSqrArea()  <  ( epsilon * epsilon );
  }

  //##
  // Return closest point on triangle to 'p'
  inline lmMatrix<1,3,T> ClosestPtPointTriangle( const lmMatrix<1,3,T> &p ) const
  {
    // Check if P in vertex region outside A
    lmMatrix<1,3,T> ab = b - a;
    lmMatrix<1,3,T> ac = c - a;
    lmMatrix<1,3,T> ap = p - a;

    float d1 = ab.Dot(ap);
    float d2 = ac.Dot(ap);
    if (d1 <= T(0) && d2 <= T(0)) return a; // barycentric coordinates (1,0,0)

    // Check if P in vertex region outside B
    lmMatrix<1,3,T> bp = p - b;
    float d3 = ab.Dot(bp);
    float d4 = ac.Dot(bp);
    if (d3 >= T(0) && d4 <= d3) return b; // barycentric coordinates (0,1,0)

    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1*d4 - d3*d2;
    if (vc <= T(0) && d1 >= T(0) && d3 <= T(0))
    {
        float v = d1 / (d1 - d3);
        return a + (ab * v); // barycentric coordinates (1-v,v,0)
    }

    // Check if P in vertex region outside C
    lmMatrix<1,3,T> cp = p - c;
    float d5 = ab.Dot(cp);
    float d6 = ac.Dot(cp);
    if (d6 >= T(0) && d5 <= d6) return c; // barycentric coordinates (0,0,1)

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5*d2 - d1*d6;
    if (vb <= T(0) && d2 >= T(0) && d6 <= T(0))
    {
        float w = d2 / (d2 - d6);
        return a + (ac * w); // barycentric coordinates (1-w,0,w)
    }

    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3*d6 - d5*d4;
    if (va <= T(0) && (d4 - d3) >= T(0) && (d5 - d6) >= T(0))
    {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + ((c - b) * w); // barycentric coordinates (0,1-w,w)
    }

    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    float denom = T(1) / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = T(1) - v - w
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Typedefs
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

typedef lmTri<float> lmTri3;
typedef lmTri<double> lmTri3l;

END_L_NAMESPACE


#endif

