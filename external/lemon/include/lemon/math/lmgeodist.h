//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmgeodist.h
// Description : Geometric primitive distances
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMGEODIST_H_INCLUDED
#define LMGEODIST_H_INCLUDED

#include "lmath.h"
#include "lmmatrix.h"

BEGIN_L_NAMESPACE

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// SqrDistance from point to line
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
inline float lmSqrDistancePointLine(const lmVector3 &rkPoint, const lmVector3 &lineOrigin, const lmVector3 &lineDirection,
                                     float *pfParam)
{
  lmVector3 kDiff = rkPoint - lineOrigin;
  float fSqrLen = lineDirection.SqrLength();
  float fT = kDiff.Dot( lineDirection ) / fSqrLen;
  kDiff -= lineDirection * fT;

  if ( pfParam != NULL )
  {
    *pfParam = fT;
  }

  return kDiff.SqrLength();
}


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// SqrDistance from line to line (closet set of points)
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

inline float lmSqrDistanceLineLine(const lmVector3 &line0Origin, const lmVector3 &line0Direction,
                                   const lmVector3 &line1Origin, const lmVector3 &line1Direction,
                                   float* pfLinP0, float* pfLinP1)
{
  lmVector3 kDiff = line0Origin - line1Origin;
  float fA00 = line0Direction.SqrLength();
  float fA01 = -line0Direction.Dot(line1Direction);
  float fA11 = line1Direction.SqrLength();
  float fB0 = kDiff.Dot(line0Direction);
  float fC = kDiff.SqrLength();
  float fDet = lmFabs(fA00*fA11-fA01*fA01);
  float fB1, fS, fT, fSqrDist;

  if ( fDet >= LM_EPSILON )
  {
    // lines are not parallel
    fB1 = -kDiff.Dot(line1Direction);
    float fInvDet = 1.0f/fDet;
    fS = (fA01*fB1-fA11*fB0)*fInvDet;
    fT = (fA01*fB0-fA00*fB1)*fInvDet;
    fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) + fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
  }
  else
  {
    // lines are parallel, select any closest pair of points
    fS = -fB0/fA00;
    fT = 0.0f;
    fSqrDist = fB0*fS+fC;
  }

  if ( pfLinP0 )
    *pfLinP0 = fS;

  if ( pfLinP1 )
    *pfLinP1 = fT;

  return lmFabs(fSqrDist);
}



//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// SqrDistance from point to disk (closest set)
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

//inline float lmSqrDistancePointDisk(const lmVector3 &rkPoint, const lmVector3 &diskCentre, const lmVector3 &diskNormal, float diskRadius, lmVector3 &kClosest)
//{
//  //signed distance from point to plane of rkDisk
//  lmVector3 kDiff0 = rkPoint - diskCentre;
//  float fDist = kDiff0.Dot(diskNormal);
//
//  // projection of P-C onto plane is Q-C = P-C - (fDist)*N
//  lmVector3 kDiff1 = kDiff0 - diskNormal * fDist;
//  float fSqrLen = kDiff1.SqrLength();
//  float fSqrDist;
//
//  if ( fSqrLen <= diskRadius*diskRadius )
//  {
//    // projected point Q is in disk and is closest to P
//    kClosest = diskCentre + kDiff1;
//    fSqrDist = fDist*fDist;
//  }
//  else
//  {
//    // projected point Q is outside disk, closest point is on circle
//    kClosest = diskCentre   +   kDiff1  *  ( diskRadius / lmSqrt( fSqrLen ) );
//    lmVector3 kDiff2 = rkPoint - kClosest;
//    fSqrDist = kDiff2.SqrLength();
//  }
//
//  return fSqrDist;
////}




inline lmVector3 ProjectVectorOntoPlane(const lmVector3 &v, const lmVector3 &normal)
{
  float vDotNormal = v.Dot( normal );
  return v  -  normal * vDotNormal;
}



inline bool GetLeadingCylinderSegment(lmVector3 &a, lmVector3 &b, const lmVector3 &axis, const lmVector3 &cylOrigin, const lmVector3 &cylAxis, float cylRadius, float halfCylLength)
{
  lmVector3 radialVector = ProjectVectorOntoPlane( axis, cylAxis );

  float sqrLenRadialVector = radialVector.SqrLength();

  if ( sqrLenRadialVector < LM_EPSILON )
  {
    return false;
  }
  else
  {
    //normalize and multiply by radius
    radialVector *= cylRadius / lmSqrt( sqrLenRadialVector );

    //generate points
    lmVector3 centre = cylOrigin + radialVector;
    lmVector3 capVector = cylAxis * halfCylLength;
    a = centre - capVector;
    b = centre + capVector;

    return true;
  }
}

inline bool GetTrailingCylinderSegment(lmVector3 &a, lmVector3 &b, const lmVector3 &axis, const lmVector3 &cylOrigin, const lmVector3 &cylAxis, float cylRadius, float halfCylLength)
{
  return GetLeadingCylinderSegment( a, b, -axis, cylOrigin, cylAxis, cylRadius, halfCylLength );
}




inline bool ClipSegmentToBox(const lmVector3 &_a, const lmVector3 &_b, const lmVector3 &boxP, const lmVector3 &halfBoxSize, const lmMatrix3 &boxR, int excludeAxis, float &t0, float &t1)
{
  t0 = 0.0f,
  t1 = 1.0f;

  lmVector3 a = _a - boxP;
  lmVector3 b = _b - boxP;

  for (int axisIndex = 0; axisIndex < 3; axisIndex++)
  {
    if ( axisIndex != excludeAxis )
    {
      lmVector3 boxAxis;
      boxAxis = boxR.GetRowDirect( axisIndex );

      float aA = a.Dot( boxAxis );
      float aB = b.Dot( boxAxis );
      float deltaA = aB - aA;
      float aP = aA  +  deltaA * t0;
      float aQ = aA  +  deltaA * t1;

      if ( aB  >  ( aA + LM_EPSILON ) )
      {
        if ( aQ < -halfBoxSize(axisIndex)  ||  aP > halfBoxSize(axisIndex) )
        {
          return false;
        }
        else
        {
          float oneOverDeltaA = 1.0f  /  deltaA;
          float tL = ( -halfBoxSize(axisIndex) - aA )  *  oneOverDeltaA;
          float tU = ( halfBoxSize(axisIndex) - aA )  *  oneOverDeltaA;
          t0 = lmMax( t0, tL );
          t1 = lmMin( t1, tU );
        }
      }
      else if ( aB  <  ( aA - LM_EPSILON ) )
      {
        if ( aP < -halfBoxSize(axisIndex)  ||  aQ > halfBoxSize(axisIndex) )
        {
          return false;
        }
        else
        {
          float oneOverDeltaA = 1.0f  /  deltaA;
          float tL = ( halfBoxSize(axisIndex) - aA )  *  oneOverDeltaA;
          float tU = ( -halfBoxSize(axisIndex) - aA )  *  oneOverDeltaA;
          t0 = lmMax( t0, tL );
          t1 = lmMin( t1, tU );
        }
      }
      else
      {
        if ( aP < -halfBoxSize(axisIndex)  ||  aP > halfBoxSize(axisIndex) )
        {
          return false;
        }
      }
    }
  }

  return true;
}

inline void SegToBoxClosestPoint(const lmVector3 &p1, const lmVector3 &p2,
                                 const lmVector3 &boxP, const lmVector3 &boxSize, const lmMatrix3 &boxR,
                                 lmVector3 &pSeg, lmVector3 &pBox)
{
  int i;

  // compute the start and delta of the line p1-p2 relative to the box.
  // we will do all subsequent computations in this box-relative coordinate
  // system. we have to do a translation and rotation for each point.

  lmVector3 tmp;

  lmVector3 s = ( p1 - boxP ) * boxR.Transpose();

  lmVector3 v = ( p2 - p1 ) * boxR.Transpose();

  // mirror the line so that v has all components >= 0
  lmVector3 sign;
  for (i=0; i<3; i++)
  {
    if (v(i) < 0)
    {
      s(i) = -s(i);
      v(i) = -v(i);
      sign(i) = -1;
    }
    else sign(i) = 1;
  }

  lmVector3 halfBoxSize = boxSize * 0.5f;

  // compute v^2
  lmVector3 v2 = v.Scale(v);

  // region is -1,0,+1 depending on which side of the box planes each
  // coordinate is on. tanchor is the next t value at which there is a
  // transition, or the last one if there are no more.
  int region[3];
  lmVector3 tanchor;

  // find the region and tanchor values for p1
  for (i=0; i<3; i++)
  {
    if (v(i) > 0)
    {
      if (s(i) < -halfBoxSize(i))
      {
        region[i] = -1;
        tanchor(i) = (-halfBoxSize(i)-s(i))/v(i);
      }
      else
      {
        region[i] = (s(i) > halfBoxSize(i));
        tanchor(i) = (halfBoxSize(i)-s(i))/v(i);
      }
    }
    else
    {
      region[i] = 0;
      tanchor(i) = 2;   // this will never be a valid tanchor
    }
  }

  // compute d|d|^2/dt for t=0. if it's >= 0 then p1 is the closest point
  float t=0;
  float dd2dt = 0;

  for (i=0; i<3; i++)
  {
    dd2dt -= (region[i] ? v2(i) : 0) * tanchor(i);
  }

  if (dd2dt >= 0) goto got_answer;

  do
  {
    // find the point on the line that is at the next clip plane boundary
    float next_t = 1;
    for (i=0; i<3; i++)
    {
      if (tanchor(i) > t && tanchor(i) < 1 && tanchor(i) < next_t)
        next_t = tanchor(i);
    }

    // compute d|d|^2/dt for the next t
    float next_dd2dt = 0;
    for (i=0; i<3; i++)
    {
      next_dd2dt += (region[i] ? v2(i) : 0) * (next_t - tanchor(i));
    }

    // if the sign of d|d|^2/dt has changed, solution = the crossover point
    if (next_dd2dt >= 0)
    {
      float m = (next_dd2dt-dd2dt)/(next_t - t);
      t -= dd2dt/m;
      goto got_answer;
    }

    // advance to the next anchor point / region
    for (i=0; i<3; i++)
    {
      if (tanchor(i) == next_t)
      {
        tanchor(i) = (halfBoxSize(i)-s(i))/v(i);
        region[i]++;
      }
    }
    t = next_t;
    dd2dt = next_dd2dt;
  }
  while (t < 1);
  t = 1;

  got_answer:

  // compute closest point on the line
  pSeg  =  p1  +  ( p2 - p1 ) * t;

  // compute closest point on the box
  for (i=0; i<3; i++)
  {
    tmp(i) = sign(i) * (s(i) + t*v(i));

    tmp(i) = lmClamp( tmp(i), -halfBoxSize(i), halfBoxSize(i) );
  }

  s = tmp * boxR;

  pBox = s + boxP;
}

inline float lmSqrDistancePointSeg(const lmVector3 &rkPoint, const lmVector3 &segOrigin, const lmVector3 &segDirection,
                                    float *pfParam)
{
  lmVector3 kDiff = rkPoint - segOrigin;
  float fT = kDiff.Dot( segDirection );

  if ( fT <= 0.0f )
  {
    fT = 0.0f;
  }
  else
  {
    float fSqrLen= segDirection.SqrLength();
    if ( fT >= fSqrLen )
    {
      fT = 1.0f;
      kDiff -= segDirection;
    }
    else
    {
      fT /= fSqrLen;
      kDiff -= segDirection * fT;
    }
  }

  if ( pfParam )
  {
    *pfParam = fT;
  }

  return kDiff.SqrLength();
}


inline float lmSqrDistancePointTri(const lmVector3 &rkPoint,
                                    const lmVector3 &triV0, const lmVector3 &triV1, const lmVector3 &triV2,
                                    float* pfSParam, float* pfTParam )
{
  lmVector3 triOrigin = triV0;
  lmVector3 triEdge0  = triV1-triV0;
  lmVector3 triEdge1  = triV2-triV0;

  lmVector3 kDiff = triOrigin - rkPoint;
  float fA00       = triEdge0.SqrLength();
  float fA01       = triEdge0.Dot(triEdge1);
  float fA11       = triEdge1.SqrLength();
  float fB0        = kDiff.Dot(triEdge0);
  float fB1        = kDiff.Dot(triEdge1);
  float fC         = kDiff.SqrLength();
  float fDet       = lmFabsf(fA00*fA11-fA01*fA01);
  float fS         = fA01*fB1-fA11*fB0;
  float fT         = fA01*fB0-fA00*fB1;
  float fSqrDist;

  if ( fS + fT <= fDet )
  {
      if ( fS < (float)0.0 )
      {
          if ( fT < (float)0.0 )  // region 4
          {
              if ( fB0 < (float)0.0 )
              {
                  fT = (float)0.0;
                  if ( -fB0 >= fA00 )
                  {
                      fS = (float)1.0;
                      fSqrDist = fA00+((float)2.0)*fB0+fC;
                  }
                  else
                  {
                      fS = -fB0/fA00;
                      fSqrDist = fB0*fS+fC;
                  }
              }
              else
              {
                  fS = (float)0.0;
                  if ( fB1 >= (float)0.0 )
                  {
                      fT = (float)0.0;
                      fSqrDist = fC;
                  }
                  else if ( -fB1 >= fA11 )
                  {
                      fT = (float)1.0;
                      fSqrDist = fA11+((float)2.0)*fB1+fC;
                  }
                  else
                  {
                      fT = -fB1/fA11;
                      fSqrDist = fB1*fT+fC;
                  }
              }
          }
          else  // region 3
          {
              fS = (float)0.0;
              if ( fB1 >= (float)0.0 )
              {
                  fT = (float)0.0;
                  fSqrDist = fC;
              }
              else if ( -fB1 >= fA11 )
              {
                  fT = (float)1.0;
                  fSqrDist = fA11+((float)2.0)*fB1+fC;
              }
              else
              {
                  fT = -fB1/fA11;
                  fSqrDist = fB1*fT+fC;
              }
          }
      }
      else if ( fT < (float)0.0 )  // region 5
      {
          fT = (float)0.0;
          if ( fB0 >= (float)0.0 )
          {
              fS = (float)0.0;
              fSqrDist = fC;
          }
          else if ( -fB0 >= fA00 )
          {
              fS = (float)1.0;
              fSqrDist = fA00+((float)2.0)*fB0+fC;
          }
          else
          {
              fS = -fB0/fA00;
              fSqrDist = fB0*fS+fC;
          }
      }
      else  // region 0
      {
          // minimum at interior point
          float fInvDet = ((float)1.0)/fDet;
          fS *= fInvDet;
          fT *= fInvDet;
          fSqrDist = fS*(fA00*fS+fA01*fT+((float)2.0)*fB0) +
              fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
      }
  }
  else
  {
      float fTmp0, fTmp1, fNumer, fDenom;

      if ( fS < (float)0.0 )  // region 2
      {
          fTmp0 = fA01 + fB0;
          fTmp1 = fA11 + fB1;
          if ( fTmp1 > fTmp0 )
          {
              fNumer = fTmp1 - fTmp0;
              fDenom = fA00-2.0f*fA01+fA11;
              if ( fNumer >= fDenom )
              {
                  fS = (float)1.0;
                  fT = (float)0.0;
                  fSqrDist = fA00+((float)2.0)*fB0+fC;
              }
              else
              {
                  fS = fNumer/fDenom;
                  fT = (float)1.0 - fS;
                  fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                      fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
              }
          }
          else
          {
              fS = (float)0.0;
              if ( fTmp1 <= (float)0.0 )
              {
                  fT = (float)1.0;
                  fSqrDist = fA11+((float)2.0)*fB1+fC;
              }
              else if ( fB1 >= (float)0.0 )
              {
                  fT = (float)0.0;
                  fSqrDist = fC;
              }
              else
              {
                  fT = -fB1/fA11;
                  fSqrDist = fB1*fT+fC;
              }
          }
      }
      else if ( fT < (float)0.0 )  // region 6
      {
          fTmp0 = fA01 + fB1;
          fTmp1 = fA00 + fB0;
          if ( fTmp1 > fTmp0 )
          {
              fNumer = fTmp1 - fTmp0;
              fDenom = fA00-((float)2.0)*fA01+fA11;
              if ( fNumer >= fDenom )
              {
                  fT = (float)1.0;
                  fS = (float)0.0;
                  fSqrDist = fA11+((float)2.0)*fB1+fC;
              }
              else
              {
                  fT = fNumer/fDenom;
                  fS = (float)1.0 - fT;
                  fSqrDist = fS*(fA00*fS+fA01*fT+((float)2.0)*fB0) +
                      fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
              }
          }
          else
          {
              fT = (float)0.0;
              if ( fTmp1 <= (float)0.0 )
              {
                  fS = (float)1.0;
                  fSqrDist = fA00+((float)2.0)*fB0+fC;
              }
              else if ( fB0 >= (float)0.0 )
              {
                  fS = (float)0.0;
                  fSqrDist = fC;
              }
              else
              {
                  fS = -fB0/fA00;
                  fSqrDist = fB0*fS+fC;
              }
          }
      }
      else  // region 1
      {
          fNumer = fA11 + fB1 - fA01 - fB0;
          if ( fNumer <= (float)0.0 )
          {
              fS = (float)0.0;
              fT = (float)1.0;
              fSqrDist = fA11+((float)2.0)*fB1+fC;
          }
          else
          {
              fDenom = fA00-2.0f*fA01+fA11;
              if ( fNumer >= fDenom )
              {
                  fS = (float)1.0;
                  fT = (float)0.0;
                  fSqrDist = fA00+((float)2.0)*fB0+fC;
              }
              else
              {
                  fS = fNumer/fDenom;
                  fT = (float)1.0 - fS;
                  fSqrDist = fS*(fA00*fS+fA01*fT+((float)2.0)*fB0) +
                      fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
              }
          }
      }
  }

  if ( pfSParam )
      *pfSParam = fS;

  if ( pfTParam )
      *pfTParam = fT;

  return lmFabsf(fSqrDist);


  return 0;
}



inline float lmSqrDistanceSegSeg(const lmVector3 &seg1Origin, const lmVector3 &seg1Direction,
                                  const lmVector3 &seg2Origin, const lmVector3 &seg2Direction,
                                  float* pfSegP0, float* pfSegP1)
{
  lmVector3 kDiff = seg1Origin - seg2Origin;
  float fA00 = seg1Direction.SqrLength();
  float fA01 = -seg1Direction.Dot(seg2Direction);
  float fA11 = seg2Direction.SqrLength();
  float fB0 = kDiff.Dot(seg1Direction);
  float fC = kDiff.SqrLength();
  float fDet = lmFabsf(fA00*fA11-fA01*fA01);
  float fB1, fS, fT, fSqrDist, fTmp;

  if ( fDet >= LM_FLOAT_EPSILON )
  {
      // line segments are not parallel
      fB1 = -kDiff.Dot(seg2Direction);
      fS = fA01*fB1-fA11*fB0;
      fT = fA01*fB0-fA00*fB1;

      if ( fS >= (float)0.0 )
      {
          if ( fS <= fDet )
          {
              if ( fT >= (float)0.0 )
              {
                  if ( fT <= fDet )  // region 0 (interior)
                  {
                      // minimum at two interior points of 3D lines
                      float fInvDet = ((float)1.0)/fDet;
                      fS *= fInvDet;
                      fT *= fInvDet;
                      fSqrDist = fS*(fA00*fS+fA01*fT+((float)2.0)*fB0) +
                          fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
                  }
                  else  // region 3 (side)
                  {
                      fT = (float)1.0;
                      fTmp = fA01+fB0;
                      if ( fTmp >= (float)0.0 )
                      {
                          fS = (float)0.0;
                          fSqrDist = fA11+((float)2.0)*fB1+fC;
                      }
                      else if ( -fTmp >= fA00 )
                      {
                          fS = (float)1.0;
                          fSqrDist = fA00+fA11+fC+((float)2.0)*(fB1+fTmp);
                      }
                      else
                      {
                          fS = -fTmp/fA00;
                          fSqrDist = fTmp*fS+fA11+((float)2.0)*fB1+fC;
                      }
                  }
              }
              else  // region 7 (side)
              {
                  fT = (float)0.0;
                  if ( fB0 >= (float)0.0 )
                  {
                      fS = (float)0.0;
                      fSqrDist = fC;
                  }
                  else if ( -fB0 >= fA00 )
                  {
                      fS = (float)1.0;
                      fSqrDist = fA00+((float)2.0)*fB0+fC;
                  }
                  else
                  {
                      fS = -fB0/fA00;
                      fSqrDist = fB0*fS+fC;
                  }
              }
          }
          else
          {
              if ( fT >= (float)0.0 )
              {
                  if ( fT <= fDet )  // region 1 (side)
                  {
                      fS = (float)1.0;
                      fTmp = fA01+fB1;
                      if ( fTmp >= (float)0.0 )
                      {
                          fT = (float)0.0;
                          fSqrDist = fA00+((float)2.0)*fB0+fC;
                      }
                      else if ( -fTmp >= fA11 )
                      {
                          fT = (float)1.0;
                          fSqrDist = fA00+fA11+fC+((float)2.0)*(fB0+fTmp);
                      }
                      else
                      {
                          fT = -fTmp/fA11;
                          fSqrDist = fTmp*fT+fA00+((float)2.0)*fB0+fC;
                      }
                  }
                  else  // region 2 (corner)
                  {
                      fTmp = fA01+fB0;
                      if ( -fTmp <= fA00 )
                      {
                          fT = (float)1.0;
                          if ( fTmp >= (float)0.0 )
                          {
                              fS = (float)0.0;
                              fSqrDist = fA11+((float)2.0)*fB1+fC;
                          }
                          else
                          {
                               fS = -fTmp/fA00;
                               fSqrDist = fTmp*fS+fA11+((float)2.0)*fB1+fC;
                          }
                      }
                      else
                      {
                          fS = (float)1.0;
                          fTmp = fA01+fB1;
                          if ( fTmp >= (float)0.0 )
                          {
                              fT = (float)0.0;
                              fSqrDist = fA00+((float)2.0)*fB0+fC;
                          }
                          else if ( -fTmp >= fA11 )
                          {
                              fT = (float)1.0;
                              fSqrDist = fA00+fA11+fC+
                                  ((float)2.0)*(fB0+fTmp);
                          }
                          else
                          {
                              fT = -fTmp/fA11;
                              fSqrDist = fTmp*fT+fA00+((float)2.0)*fB0+fC;
                          }
                      }
                  }
              }
              else  // region 8 (corner)
              {
                  if ( -fB0 < fA00 )
                  {
                      fT = (float)0.0;
                      if ( fB0 >= (float)0.0 )
                      {
                          fS = (float)0.0;
                          fSqrDist = fC;
                      }
                      else
                      {
                          fS = -fB0/fA00;
                          fSqrDist = fB0*fS+fC;
                      }
                  }
                  else
                  {
                      fS = (float)1.0;
                      fTmp = fA01+fB1;
                      if ( fTmp >= (float)0.0 )
                      {
                          fT = (float)0.0;
                          fSqrDist = fA00+((float)2.0)*fB0+fC;
                      }
                      else if ( -fTmp >= fA11 )
                      {
                          fT = (float)1.0;
                          fSqrDist = fA00+fA11+fC+((float)2.0)*(fB0+fTmp);
                      }
                      else
                      {
                          fT = -fTmp/fA11;
                          fSqrDist = fTmp*fT+fA00+((float)2.0)*fB0+fC;
                      }
                  }
              }
          }
      }
      else
      {
          if ( fT >= (float)0.0 )
          {
              if ( fT <= fDet )  // region 5 (side)
              {
                  fS = (float)0.0;
                  if ( fB1 >= (float)0.0 )
                  {
                      fT = (float)0.0;
                      fSqrDist = fC;
                  }
                  else if ( -fB1 >= fA11 )
                  {
                      fT = (float)1.0;
                      fSqrDist = fA11+((float)2.0)*fB1+fC;
                  }
                  else
                  {
                      fT = -fB1/fA11;
                      fSqrDist = fB1*fT+fC;
                  }
              }
              else  // region 4 (corner)
              {
                  fTmp = fA01+fB0;
                  if ( fTmp < (float)0.0 )
                  {
                      fT = (float)1.0;
                      if ( -fTmp >= fA00 )
                      {
                          fS = (float)1.0;
                          fSqrDist = fA00+fA11+fC+((float)2.0)*(fB1+fTmp);
                      }
                      else
                      {
                          fS = -fTmp/fA00;
                          fSqrDist = fTmp*fS+fA11+((float)2.0)*fB1+fC;
                      }
                  }
                  else
                  {
                      fS = (float)0.0;
                      if ( fB1 >= (float)0.0 )
                      {
                          fT = (float)0.0;
                          fSqrDist = fC;
                      }
                      else if ( -fB1 >= fA11 )
                      {
                          fT = (float)1.0;
                          fSqrDist = fA11+((float)2.0)*fB1+fC;
                      }
                      else
                      {
                          fT = -fB1/fA11;
                          fSqrDist = fB1*fT+fC;
                      }
                  }
              }
          }
          else   // region 6 (corner)
          {
              if ( fB0 < (float)0.0 )
              {
                  fT = (float)0.0;
                  if ( -fB0 >= fA00 )
                  {
                      fS = (float)1.0;
                      fSqrDist = fA00+((float)2.0)*fB0+fC;
                  }
                  else
                  {
                      fS = -fB0/fA00;
                      fSqrDist = fB0*fS+fC;
                  }
              }
              else
              {
                  fS = (float)0.0;
                  if ( fB1 >= (float)0.0 )
                  {
                      fT = (float)0.0;
                      fSqrDist = fC;
                  }
                  else if ( -fB1 >= fA11 )
                  {
                      fT = (float)1.0;
                      fSqrDist = fA11+((float)2.0)*fB1+fC;
                  }
                  else
                  {
                      fT = -fB1/fA11;
                      fSqrDist = fB1*fT+fC;
                  }
              }
          }
      }
  }
  else
  {
      // line segments are parallel
      if ( fA01 > (float)0.0 )
      {
          // direction vectors form an obtuse angle
          if ( fB0 >= (float)0.0 )
          {
              fS = (float)0.0;
              fT = (float)0.0;
              fSqrDist = fC;
          }
          else if ( -fB0 <= fA00 )
          {
              fS = -fB0/fA00;
              fT = (float)0.0;
              fSqrDist = fB0*fS+fC;
          }
          else
          {
              fB1 = -kDiff.Dot(seg2Direction);
              fS = (float)1.0;
              fTmp = fA00+fB0;
              if ( -fTmp >= fA01 )
              {
                  fT = (float)1.0;
                  fSqrDist = fA00+fA11+fC+((float)2.0)*(fA01+fB0+fB1);
              }
              else
              {
                  fT = -fTmp/fA01;
                  fSqrDist = fA00+((float)2.0)*fB0+fC+fT*(fA11*fT+
                      ((float)2.0)*(fA01+fB1));
              }
          }
      }
      else
      {
          // direction vectors form an acute angle
          if ( -fB0 >= fA00 )
          {
              fS = (float)1.0;
              fT = (float)0.0;
              fSqrDist = fA00+((float)2.0)*fB0+fC;
          }
          else if ( fB0 <= (float)0.0 )
          {
              fS = -fB0/fA00;
              fT = (float)0.0;
              fSqrDist = fB0*fS+fC;
          }
          else
          {
              fB1 = -kDiff.Dot(seg2Direction);
              fS = (float)0.0;
              if ( fB0 >= -fA01 )
              {
                  fT = (float)1.0;
                  fSqrDist = fA11+((float)2.0)*fB1+fC;
              }
              else
              {
                  fT = -fB0/fA01;
                  fSqrDist = fC+fT*(((float)2.0)*fB1+fA11*fT);
              }
          }
      }
  }

  if ( pfSegP0 )
      *pfSegP0 = fS;

  if ( pfSegP1 )
      *pfSegP1 = fT;

  return lmFabsf(fSqrDist);
}


//----------------------------------------------------------------------------
inline float lmSqrDistanceSegTri(const lmVector3 &segOrigin, const lmVector3 &segDirection,
                                  const lmVector3 &triV0, const lmVector3 &triV1, const lmVector3 &triV2,
                                  float* pfSegP, float* pfTriP0, float* pfTriP1)
{
  lmVector3 triOrigin = triV0;
  lmVector3 triEdge0  = triV1-triV0;
  lmVector3 triEdge1  = triV2-triV0;

  lmVector3 kDiff = triOrigin - segOrigin;
  float fA00 = segDirection.SqrLength();
  float fA01 = -segDirection.Dot(triEdge0);
  float fA02 = -segDirection.Dot(triEdge1);
  float fA11 = triEdge0.SqrLength();
  float fA12 = triEdge0.Dot(triEdge1);
  float fA22 = triEdge1.Dot(triEdge1);
  float fB0  = -kDiff.Dot(segDirection);
  float fB1  = kDiff.Dot(triEdge0);
  float fB2  = kDiff.Dot(triEdge1);

  lmVector3 triSegOrigin;
  lmVector3 triSegDirection;

  lmVector3 kPt;
  float fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

  // Set up for a relative error test on the angle between ray direction
  // and triangle normal to determine parallel/nonparallel status.

  lmVector3 kN = triEdge0.Cross(triEdge1);
  float fNSqrLen = kN.SqrLength();
  float fDot = segDirection.Dot(kN);
  bool bNotParallel = (fDot*fDot >= LM_FLOAT_EPSILON*fA00*fNSqrLen);

  if ( bNotParallel )
  {
      float fCof00 = fA11*fA22-fA12*fA12;
      float fCof01 = fA02*fA12-fA01*fA22;
      float fCof02 = fA01*fA12-fA02*fA11;
      float fCof11 = fA00*fA22-fA02*fA02;
      float fCof12 = fA02*fA01-fA00*fA12;
      float fCof22 = fA00*fA11-fA01*fA01;
      float fInvDet = ((float)1.0)/(fA00*fCof00+fA01*fCof01+fA02*fCof02);
      float fRhs0 = -fB0*fInvDet;
      float fRhs1 = -fB1*fInvDet;
      float fRhs2 = -fB2*fInvDet;

      fR = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;
      fS = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
      fT = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;

      if ( fR < (float)0.0 )
      {
          if ( fS+fT <= (float)1.0 )
          {
              if ( fS < (float)0.0 )
              {
                  if ( fT < (float)0.0 )  // region 4m
                  {
                      // min on face s=0 or t=0 or r=0
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge1;
                      fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                      fS = (float)0.0;
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge0;
                      fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fS0);
                      fT0 = (float)0.0;
                      if ( fSqrDist0 < fSqrDist )
                      {
                          fSqrDist = fSqrDist0;
                          fR = fR0;
                          fS = fS0;
                          fT = fT0;
                      }
                      fSqrDist0 = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS0,&fT0);
                      fR0 = (float)0.0;
                      if ( fSqrDist0 < fSqrDist )
                      {
                          fSqrDist = fSqrDist0;
                          fR = fR0;
                          fS = fS0;
                          fT = fT0;
                      }
                  }
                  else  // region 3m
                  {
                      // min on face s=0 or r=0
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge1;
                      fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                      fS = (float)0.0;
                      fSqrDist0 = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS0,
                          &fT0);
                      fR0 = (float)0.0;
                      if ( fSqrDist0 < fSqrDist )
                      {
                          fSqrDist = fSqrDist0;
                          fR = fR0;
                          fS = fS0;
                          fT = fT0;
                      }
                  }
              }
              else if ( fT < (float)0.0 )  // region 5m
              {
                  // min on face t=0 or r=0
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fS);
                  fT = (float)0.0;
                  fSqrDist0 = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)0.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else  // region 0m
              {
                  // min on face r=0
                  fSqrDist = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS,&fT);
                  fR = (float)0.0;
              }
          }
          else
          {
              if ( fS < (float)0.0 )  // region 2m
              {
                  // min on face s=0 or s+t=1 or r=0
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge1;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                  fS = (float)0.0;
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
                  fS0 = (float)1.0-fT0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
                  fSqrDist0 = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)0.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else if ( fT < (float)0.0 )  // region 6m
              {
                  // min on face t=0 or s+t=1 or r=0
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fS);
                  fT = (float)0.0;
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
                  fS0 = (float)1.0-fT0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
                  fSqrDist0 = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)0.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else  // region 1m
              {
                  // min on face s+t=1 or r=0
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                  fS = (float)1.0-fT;
                  fSqrDist0 = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)0.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
          }
      }
      else if ( fR <= (float)1.0 )
      {
          if ( fS+fT <= (float)1.0 )
          {
              if ( fS < (float)0.0 )
              {
                  if ( fT < (float)0.0 )  // region 4
                  {
                      // min on face s=0 or t=0
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge1;
                      fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                      fS = (float)0.0;
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge0;
                      fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fS0);
                      fT0 = (float)0.0;
                      if ( fSqrDist0 < fSqrDist )
                      {
                          fSqrDist = fSqrDist0;
                          fR = fR0;
                          fS = fS0;
                          fT = fT0;
                      }
                  }
                  else  // region 3
                  {
                      // min on face s=0
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge1;
                      fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                      fS = (float)0.0;
                  }
              }
              else if ( fT < (float)0.0 )  // region 5
              {
                  // min on face t=0
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fS);
                  fT = (float)0.0;
              }
              else  // region 0
              {
                  // global minimum is interior, done
                  fSqrDist = (float)0.0;
              }
          }
          else
          {
              if ( fS < (float)0.0 )  // region 2
              {
                  // min on face s=0 or s+t=1
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge1;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                  fS = (float)0.0;
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
                  fS0 = (float)1.0-fT0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else if ( fT < (float)0.0 )  // region 6
              {
                  // min on face t=0 or s+t=1
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fS);
                  fT = (float)0.0;
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
                  fS0 = (float)1.0-fT0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else  // region 1
              {
                  // min on face s+t=1
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                  fS = (float)1.0-fT;
              }
          }
      }
      else  // fR > 1
      {
          if ( fS+fT <= (float)1.0 )
          {
              if ( fS < (float)0.0 )
              {
                  if ( fT < (float)0.0 )  // region 4p
                  {
                      // min on face s=0 or t=0 or r=1
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge1;
                      fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                      fS = (float)0.0;
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge0;
                      fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fS0);
                      fT0 = (float)0.0;
                      if ( fSqrDist0 < fSqrDist )
                      {
                          fSqrDist = fSqrDist0;
                          fR = fR0;
                          fS = fS0;
                          fT = fT0;
                      }
                      kPt = segOrigin+segDirection;
                      fSqrDist0 = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS0,&fT0);
                      fR0 = (float)1.0;
                      if ( fSqrDist0 < fSqrDist )
                      {
                          fSqrDist = fSqrDist0;
                          fR = fR0;
                          fS = fS0;
                          fT = fT0;
                      }
                  }
                  else  // region 3p
                  {
                      // min on face s=0 or r=1
                      triSegOrigin = triOrigin;
                      triSegDirection = triEdge1;
                      fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                      fS = (float)0.0;
                      kPt = segOrigin+segDirection;
                      fSqrDist0 = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS0,&fT0);
                      fR0 = (float)1.0;
                      if ( fSqrDist0 < fSqrDist )
                      {
                          fSqrDist = fSqrDist0;
                          fR = fR0;
                          fS = fS0;
                          fT = fT0;
                      }
                  }
              }
              else if ( fT < (float)0.0 )  // region 5p
              {
                  // min on face t=0 or r=1
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fS);
                  fT = (float)0.0;
                  kPt = segOrigin+segDirection;
                  fSqrDist0 = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)1.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else  // region 0p
              {
                  // min face on r=1
                  kPt = segOrigin+segDirection;
                  fSqrDist = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS,&fT);
                  fR = (float)1.0;
              }
          }
          else
          {
              if ( fS < (float)0.0 )  // region 2p
              {
                  // min on face s=0 or s+t=1 or r=1
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge1;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                  fS = (float)0.0;
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
                  fS0 = (float)1.0-fT0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
                  kPt = segOrigin+segDirection;
                  fSqrDist0 = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)1.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else if ( fT < (float)0.0 )  // region 6p
              {
                  // min on face t=0 or s+t=1 or r=1
                  triSegOrigin = triOrigin;
                  triSegDirection = triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fS);
                  fT = (float)0.0;
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
                  fS0 = (float)1.0-fT0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
                  kPt = segOrigin+segDirection;
                  fSqrDist0 = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)1.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
              else  // region 1p
              {
                  // min on face s+t=1 or r=1
                  triSegOrigin = triOrigin+triEdge0;
                  triSegDirection = triEdge1-triEdge0;
                  fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fT);
                  fS = (float)1.0-fT;
                  kPt = segOrigin+segDirection;
                  fSqrDist0 = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS0,&fT0);
                  fR0 = (float)1.0;
                  if ( fSqrDist0 < fSqrDist )
                  {
                      fSqrDist = fSqrDist0;
                      fR = fR0;
                      fS = fS0;
                      fT = fT0;
                  }
              }
          }
      }
  }
  else
  {
      // segment and triangle are parallel
      triSegOrigin = triOrigin;
      triSegDirection = triEdge0;
      fSqrDist = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR,&fS);
      fT = (float)0.0;

      triSegDirection = triEdge1;
      fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
      fS0 = (float)0.0;
      if ( fSqrDist0 < fSqrDist )
      {
          fSqrDist = fSqrDist0;
          fR = fR0;
          fS = fS0;
          fT = fT0;
      }

      triSegOrigin = triOrigin + triEdge0;
      triSegDirection = triEdge1 - triEdge0;
      fSqrDist0 = lmSqrDistanceSegSeg(segOrigin,segDirection, triSegOrigin, triSegDirection,&fR0,&fT0);
      fS0 = (float)1.0-fT0;
      if ( fSqrDist0 < fSqrDist )
      {
          fSqrDist = fSqrDist0;
          fR = fR0;
          fS = fS0;
          fT = fT0;
      }

      fSqrDist0 = lmSqrDistancePointTri(segOrigin, triV0, triV1, triV2,&fS0,&fT0);
      fR0 = (float)0.0;
      if ( fSqrDist0 < fSqrDist )
      {
          fSqrDist = fSqrDist0;
          fR = fR0;
          fS = fS0;
          fT = fT0;
      }

      kPt = segOrigin+segDirection;
      fSqrDist0 = lmSqrDistancePointTri(kPt,triV0, triV1, triV2,&fS0,&fT0);
      fR0 = (float)1.0;
      if ( fSqrDist0 < fSqrDist )
      {
          fSqrDist = fSqrDist0;
          fR = fR0;
          fS = fS0;
          fT = fT0;
      }
  }

  if ( pfSegP )
      *pfSegP = fR;

  if ( pfTriP0 )
      *pfTriP0 = fS;

  if ( pfTriP1 )
      *pfTriP1 = fT;

  return fSqrDist;
}



inline float lmSqrDistancePointDisk(const lmVector3 &rkPoint, const lmVector3 &diskCentre, const lmVector3 &diskNormal, float diskRadius, lmVector3 &kClosest)
{
  //signed distance from point to plane of rkDisk
  lmVector3 kDiff0 = rkPoint - diskCentre;
  float fDist = kDiff0.Dot(diskNormal);

  // projection of P-C onto plane is Q-C = P-C - (fDist)*N
  lmVector3 kDiff1 = kDiff0 - diskNormal * fDist;
  float fSqrLen = kDiff1.SqrLength();
  float fSqrDist;

  if ( fSqrLen <= diskRadius*diskRadius )
  {
    // projected point Q is in disk and is closest to P
    kClosest = diskCentre + kDiff1;
    fSqrDist = fDist*fDist;
  }
  else
  {
    // projected point Q is outside disk, closest point is on circle
    kClosest = diskCentre   +   kDiff1  *  ( diskRadius / lmSqrt( fSqrLen ) );
    lmVector3 kDiff2 = rkPoint - kClosest;
    fSqrDist = kDiff2.SqrLength();
  }

  return fSqrDist;
}


END_L_NAMESPACE


#endif


