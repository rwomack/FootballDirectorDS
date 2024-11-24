//����������������������������������������������������������������������������
// File        : lmhull.h
// Description : Convex Hull math classes
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMHULL_H_INCLUDED
#define LMHULL_H_INCLUDED

#include "..\containers\larray.h"
#include "lmath.h"
#include "lmmatrix.h"

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// Defines
//����������������������������������������������������������������������������

//����������������������������������������������������������������������������
// lmConvexHull - Base convex hull class
//����������������������������������������������������������������������������

template <typename T>
class lmConvexHull
{
public:
  lArray< unsigned int >     m_indicesArray;
  lArray< lmMatrix<1,3,T> >  m_pointsArray;

  //����������������������������������������������������������������������������
  // Constructors
  //
  //����������������������������������������������������������������������������

  lmConvexHull()
  {
  }
};

//##
// Hull creation function
LMATH_API int CreateHull( lmConvexHull<float> &result, lArray< lmMatrix<1,3,float> > &vertexList );

END_L_NAMESPACE
#endif

