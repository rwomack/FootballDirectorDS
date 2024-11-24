//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmpolygon.h
// Description : Polygon geometry class
// Notes       :
//
// C:\src\sort\Physics Research\gjk\tri
// inpoly, triangulate, clipping, area, normal, plane, bbox
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMPOLYGON_H_INCLUDED
#define LMPOLYGON_H_INCLUDED

#include "lmath.h"
#include "lmplane.h"
#include "lmray.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmPolygon - Polygon
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T>
class lmPolygon
{
public:

  lArray< lmMatrix<1,3,T> > points;

  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
  // Constructors
  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

  //##
  // Default constructor with no initialisation
  inline lmPolygon()
  {
  }

  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
  // Accessors
  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

  //##
  // Get normal of polygon
  inline lmMatrix<1,3,T> GetNormal() const
  {
    lAssert( points.Size() >= 3 );
    lmVector3 n = lmVector3( points(1) - points( 0 ) ).Cross( lmVector3( points(2) - points(0 ) ) );
    return n;
  }

  //##
  // Get unit normal of polygon
  inline lmMatrix<1,3,T> GetUnitNormal() const
  {
    lAssert( points.Size() >= 3 );
    lmVector3 n = lmVector3( points(1) - points( 0 ) ).Cross( lmVector3( points(2) - points(0 ) ) );
    n.Normalize();
    return n;
  }

  //##
  // Return plane derived from polygon
  inline lmPlane<3,T> GetPlane() const
  {
    lAssert( points.Size() >= 3 );
    lmPlane<3,T> result;
    result.FromPoints( points(0), points(1), points(2) );
    return result;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Typedefs
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

typedef lmPolygon<float>  lmPolygon3;
typedef lmPolygon<double> lmPolygon3l;

END_L_NAMESPACE


#endif

