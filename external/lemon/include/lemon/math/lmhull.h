//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmhull.h
// Description : Convex Hull math classes
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMHULL_H_INCLUDED
#define LMHULL_H_INCLUDED

#include "..\containers\larray.h"
#include "lmath.h"
#include "lmmatrix.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Defines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmConvexHull - Base convex hull class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T>
class lmConvexHull
{
public:
  lArray< unsigned int >     m_indicesArray;
  lArray< lmMatrix<1,3,T> >  m_pointsArray;

  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
  // Constructors
  //
  //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

  lmConvexHull()
  {
  }
};

//##
// Hull creation function
LMATH_API int CreateHull( lmConvexHull<float> &result, lArray< lmMatrix<1,3,float> > &vertexList );

END_L_NAMESPACE
#endif

