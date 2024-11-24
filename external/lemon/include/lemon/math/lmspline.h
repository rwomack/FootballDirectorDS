//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmspline.h
// Description : Spline related include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMSPLINE_H_INCLUDED
#define LMSPLINE_H_INCLUDED

#include "../containers/larray.h"
#include "lmath.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Spline classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LMATH_API lmSplineEval
{
  enum Type
  {
    Hermite,
    Bezier,
    Catrom,
    BSpline,
    MaxBasis,              // after this, no more basis matrixes
    InterpolatingTSpline  = MaxBasis,
    ExterpolatingTSpline,
    MaxFunc
  };

  L_STREAM_ENUM_FRIEND( Type );

  static const lmMatrix4 & GetBasis(Type type);
  static void              CalcCoeff(Type type, float t, lmVector4 &h, lmVector4 &i);
  static lmVector3         CalcT(Type type, float t, const lmVector3 &p1,const lmVector3 &p2,const lmVector3 &p3,const lmVector3 &p4, lmVector3 *tangent = NULL);
};

struct LMATH_API lmSplineTCBKey
{
  float                 T;       // T this key belongs to
  bool                  linear;  // Will this be computed as linear interpolation?
  float                 tens;    // Tension     : How sharply does the curve bend
  float                 cont;    // Continuity  : How rapid is the change in speed and direction
  float                 bias;    // Bias        : Bias what is the direction of the curse as it passes through the keypoint
  lArray<float>         vec;     // data
  lmVector3           * fDiff;   // Forward differentiation data


  lmSplineTCBKey();
  ~lmSplineTCBKey();

  friend lStream& operator<< ( lStream& ar, lmSplineTCBKey &key )
  {
    int version=1;
    return ar << version << key.T << key.linear << key.tens << key.cont << key.bias << key.vec;
  }
};

struct LMATH_API lmSplineTCB
{
  float                   T;       // current T to compute
  lArray<lmSplineTCBKey>  keyList; // keyframe items
  lArray<float>           curVec;  // results of current spline channel
  lArray<float>           curDeriv;

  lmSplineTCB( unsigned int keys, unsigned int items );
  ~lmSplineTCB();

  void CalcTCB(float t);
  int  CalcTCBHintNoDeriv(float step, int keyHint);
  int  CalcTCBHint(float step, int keyHint);
  void CalcForwardDiff(float t);

  friend lStream& operator<< ( lStream& ar, lmSplineTCB &spline )
  {
    int version=1;
    return ar << version << spline.T << spline.keyList << spline.curVec << spline.curDeriv;
  }
};

END_L_NAMESPACE

#endif

