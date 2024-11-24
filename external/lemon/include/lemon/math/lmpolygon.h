//����������������������������������������������������������������������������
// File        : lmpolygon.h
// Description : Polygon geometry class
// Notes       :
//
// C:\src\sort\Physics Research\gjk\tri
// inpoly, triangulate, clipping, area, normal, plane, bbox
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMPOLYGON_H_INCLUDED
#define LMPOLYGON_H_INCLUDED

#include "lmath.h"
#include "lmplane.h"
#include "lmray.h"

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// lmPolygon - Polygon
//����������������������������������������������������������������������������

template <typename T>
class lmPolygon
{
public:

  lArray< lmMatrix<1,3,T> > points;

  //����������������������������������������������������������������������������
  // Constructors
  //����������������������������������������������������������������������������

  //##
  // Default constructor with no initialisation
  inline lmPolygon()
  {
  }

  //����������������������������������������������������������������������������
  // Accessors
  //����������������������������������������������������������������������������

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

//����������������������������������������������������������������������������
// Typedefs
//����������������������������������������������������������������������������

typedef lmPolygon<float>  lmPolygon3;
typedef lmPolygon<double> lmPolygon3l;

END_L_NAMESPACE


#endif

