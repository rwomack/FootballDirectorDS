//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmfrustum.h
// Description : R3 view frustum math class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMFRUSTUM_H_INCLUDED
#define LMFRUSTUM_H_INCLUDED

#include "lmath.h"
#include "./optimisations/lbase.h"
#include "lmmatrix.h"
#include "lmaabox.h"
#include "lmsphere.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//##
// R3 view frustum
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
class LMATH_API lmFrustum
{
public:
  float      m_LBound;                  // left bound
  float      m_UBound;                  // upper bound
  float      m_DMin;                    // direction min
  float      m_DMax;                    // direction max
  float      m_DRatio;                  // direction ration (max / min)
  lmVector3 m_Origin;                   // eye position (origin)
  lmVector3 m_LVector;                  // left vector
  lmVector3 m_UVector;                  // up vector
  lmVector3 m_DVector;                  // face vector (direction)

  lmVector4 m_planes[6];

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  lmFrustum() {}

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Serialize to a lStream object
  inline friend lStream& operator<< ( lStream& ar, lmFrustum &f )
  {
    return ar << f.m_LBound
              << f.m_UBound
              << f.m_DMin
              << f.m_DMax
              << f.m_DRatio
              << f.m_Origin
              << f.m_LVector
              << f.m_UVector
              << f.m_DVector;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Convertors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Convert from vectors to frustum
  void FromVectors( const lmVector3 &origin, const lmVector3 &left, const lmVector3 &up, const lmVector3 &dir, float dMin, float dMax )
  {
    m_Origin  = origin;
    m_LVector = left;
    m_UVector = up;
    m_DVector = dir;
    m_LBound  = m_LVector.Normalize();
    m_UBound  = m_UVector.Normalize();
    m_DVector.Normalize();
    m_DMin    = dMin;
    m_DMax    = dMax;
    m_DRatio  = m_DMax/m_DMin;
  }

  //##
  // Convert from view projection matrix to frustum
  void FromViewProjection( const lmMatrix4 &viewMatrix, const lmMatrix4 &projectionMatrix )
  {
    lmMatrix4 invViewMatrix = viewMatrix.OrthoInverse();

    m_DMin    = (-(projectionMatrix(3,2)/projectionMatrix(2,2)));
    m_DMax    = m_DMin+(m_DMin/(projectionMatrix(2,2)-projectionMatrix(2,3)));
    m_DRatio  = m_DMax/m_DMin;
    m_LBound  = m_DMin / projectionMatrix(0,0);
    m_UBound  = m_DMin / projectionMatrix(1,1);

    m_Origin  = invViewMatrix.GetTranslate();
    m_LVector = invViewMatrix.GetRow<0>();
    m_UVector = invViewMatrix.GetRow<1>();
    m_DVector = invViewMatrix.GetRow<2>();

    // Get combined matrix
    lmMatrix4 matComb = viewMatrix * projectionMatrix;

    // Left clipping plane
    m_planes[0](0) = (matComb(0,3) + matComb(0,0));
    m_planes[0](1) = (matComb(1,3) + matComb(1,0));
    m_planes[0](2) = (matComb(2,3) + matComb(2,0));
    m_planes[0](3) = (matComb(3,3) + matComb(3,0));

    // Right clipping plane
    m_planes[1](0) = (matComb(0,3) - matComb(0,0));
    m_planes[1](1) = (matComb(1,3) - matComb(1,0));
    m_planes[1](2) = (matComb(2,3) - matComb(2,0));
    m_planes[1](3) = (matComb(3,3) - matComb(3,0));

    // Near clipping plane
    m_planes[2](0) = (matComb(0,3) + matComb(0,2));
    m_planes[2](1) = (matComb(1,3) + matComb(1,2));
    m_planes[2](2) = (matComb(2,3) + matComb(2,2));
    m_planes[2](3) = (matComb(3,3) + matComb(3,2));
    // was changed to: m_planes[2](3) = matComb(3,2); //This is wrong

    // Top clipping plane
    m_planes[3](0) = (matComb(0,3) - matComb(0,1));
    m_planes[3](1) = (matComb(1,3) - matComb(1,1));
    m_planes[3](2) = (matComb(2,3) - matComb(2,1));
    m_planes[3](3) = (matComb(3,3) - matComb(3,1));

    // Bottom clipping plane
    m_planes[4](0) = (matComb(0,3) + matComb(0,1));
    m_planes[4](1) = (matComb(1,3) + matComb(1,1));
    m_planes[4](2) = (matComb(2,3) + matComb(2,1));
    m_planes[4](3) = (matComb(3,3) + matComb(3,1));

    // Far clipping plane
    m_planes[5](0) = (matComb(0,3) - matComb(0,2));
    m_planes[5](1) = (matComb(1,3) - matComb(1,2));
    m_planes[5](2) = (matComb(2,3) - matComb(2,2));
    m_planes[5](3) = (matComb(3,3) - matComb(3,2));

    for(int i = 0; i < 6; i++)
    {
      lmVector3 vec( m_planes[i](0), m_planes[i](1), m_planes[i](2) );
      float oolen = lmOOSqrt( vec.SqrLength() );
      m_planes[i] *= oolen;
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  inline float GetMTwoLF () const
  {
    return -2.0f*m_LBound*m_DMax;
  }

  inline float GetMTwoUF () const
  {
    return -2.0f*m_UBound*m_DMax;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Utility functions
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  // distance/closest

  inline float      Distance( const lmVector3  &point ) const { return lmSqrt(SqrDistance(point)); }
  inline lmVector3  Closest( const lmVector3 &point ) const   { lmVector3 r; SqrDistance(point,&r); return r; }
  inline float      SqrDistance( const lmVector3& rkPoint, lmVector3* pkClosest = NULL ) const;

  //##
  // Create bounding box to surround this frustum
  inline lmAABox3 CreateBoundingBox() const
  {
    lmVector3 DScaled = m_DVector*m_DMin;
    lmVector3 LScaled = m_LVector*m_LBound;
    lmVector3 UScaled = m_UVector*m_UBound;

    lmVector3 point1 = DScaled - LScaled + UScaled;
    lmVector3 point2 = DScaled + LScaled - UScaled;

    lmVector3 farpoint1 = m_Origin + point1 * m_DRatio;
    lmVector3 farpoint2 = m_Origin + point2 * m_DRatio;

    point1 += m_Origin;

    lmAABox3 result;

    result.Extend( farpoint1 );
    result.Extend( farpoint2 );
    result.Extend( point1 );

    return result;
  }

  //##
  // Create bounding sphere to surround this frustum
  inline lmSphere3 CreateBoundingSphere() const
  {
    return lmSphere3::AroundBox( CreateBoundingBox() );;
  }

  //##
  // Create 8 points of frustum for rendering
  inline void CreateVectors( lmVector3 *points )
  {
    lmVector3 DScaled = m_DVector*m_DMin;
    lmVector3 LScaled = m_LVector*m_LBound;
    lmVector3 UScaled = m_UVector*m_UBound;

    points[0] = DScaled - LScaled + UScaled;
    points[1] = DScaled - LScaled - UScaled;
    points[2] = DScaled + LScaled + UScaled;
    points[3] = DScaled + LScaled - UScaled;

    for (int i = 0, ip = 4; i < 4; i++, ip++)
    {
      points[ip] = m_Origin + points[i] * m_DRatio;
      points[i] += m_Origin;
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Intersections
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //inline const bool IntersectRay( const lmRay<N,T> &ray, T &t ) const
  //inline const bool IntersectSeg( const lmRay<N,T> &ray, T &t ) const

  //inline const bool IntersectBox( const lmAABox3& box ) const;
  //inline const bool IntersectBox( const lmAABox3& box, const lmMatrix4 &transform ) const;

  inline const bool IntersectSweptSphere( const lmVector3 &c, const float r, const lmVector3 &sweepDir ) const;

  inline const bool IsSphereInside( const lmVector3 &c, const float r ) const;
  inline const bool IsPointInside( const lmVector3 &v ) const;
  inline const bool IsHullInside( const lArray< lmVector3 > &points ) const;

  //##
  // Intersect an lmAABox3 against this frustum using the planes specified by 'inClipMask'
  // In:
  //   a           - lmAABox3 to test against the frustums planes
  //   outClipMask - clipping flags with a bit for each plane which are returned indicating the planes which intersect the aabox
  //   inClipMask  - clipping mask for which of the 6 frustum planes to use
  // Out:
  //   Returns false if the box was behind one of the clipping planes and therefore can make an early out
  //   otherwise returns true with the full outClipMask information
  //
  // Notes:
  //   Need a fast way to translate the half diagonal, check the index tracking which was talked about in paper
  inline const bool intersectAABBFrustum ( const lmAABox3 &a, unsigned int& outClipMask, unsigned int inClipMask ) const
  {
    lAssert( inClipMask <= (1<<6) );    // check we're not going to overflow our plane array

    lmVector3 m = a.GetCentre(); // centre of AABB
    lmVector3 d = a.GetHalfSize(); // half-diagonal

    unsigned int mk = 1, curP = 0;
    outClipMask = 0;      // init outclip mask

    while (mk <= inClipMask)  // loop while there are active planes..
    {
      if (inClipMask & mk) // if clip plane is active...
      {
        const lmVector4 &p = m_planes[curP];

        float
        NP = d(0) * lmFabsf(p(0)) +
             d(1) * lmFabsf(p(1)) +
             d(2) * lmFabsf(p(2));

        float
        MP = m(0) * p(0) +
             m(1) * p(1) +
             m(2) * p(2) +
                    p(3);

        if ((MP+NP) < 0.0f) return false; // behind clip plane
        if ((MP-NP) < 0.0f) outClipMask |= mk;
      }

      mk+=mk;   // mk = (1<<iter)
      curP++;      // next plane
    }

    return true; // AABB intersects frustum
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Transformations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  template<unsigned int P,unsigned int Q> inline void Transform( const lmMatrix<P,Q,float> &m )
  {
    // Note: we can obviously make this a lot more efficient.

    lmMatrix4 view(true);
    view.SetRow<0>( lmVector4(m_LVector(0),m_LVector(1),m_LVector(2),0) );
    view.SetRow<1>( lmVector4(m_UVector(0),m_UVector(1),m_UVector(2),0) );
    view.SetRow<2>( lmVector4(m_DVector(0),m_DVector(1),m_DVector(2),0) );
    view.SetTranslate( m_Origin );

    view = view * m;

    m_Origin  = view.GetTranslate();
    m_LVector = view.template GetRow<0>();  // Note: GCC3.2.2 bug, must have 'template' in there.
    m_UVector = view.template GetRow<1>();
    m_DVector = view.template GetRow<2>();

    for(int i = 0; i < 6; i++)
    {
      lmPlane3 plane( m_planes[i] );
      plane.Transform( m );
      m_planes[i](0) = plane.m_normal(0);
      m_planes[i](1) = plane.m_normal(1);
      m_planes[i](2) = plane.m_normal(2);
      m_planes[i](3) = plane.m_d;
    }
  }

  template<unsigned int P,unsigned int Q> inline const lmFrustum operator*  (const lmMatrix<P,Q,float>& m) const { lmFrustum r = (*this); r.Transform(m); return r;  }
  template<unsigned int P,unsigned int Q> inline void            operator*= (const lmMatrix<P,Q,float>& m)       { Transform(m); }
};

// Inline functions

inline float lmFrustum::SqrDistance( const lmVector3& rkPoint, lmVector3* pkClosest ) const
{
  // compute coordinates of point with respect to frustum coordinate system

  lmVector3 kDiff = rkPoint - m_Origin;
  lmVector3 kTest = lmVector3(
      kDiff.Dot(m_LVector),
      kDiff.Dot(m_UVector),
      kDiff.Dot(m_DVector));

  // perform calculations in octant with nonnegative L and U coordinates

  bool bLSignChange;
  if ( kTest(0) < 0.0f )
  {
      bLSignChange = true;
      kTest(0) = -kTest(0);
  }
  else
  {
      bLSignChange = false;
  }

  bool bUSignChange;
  if ( kTest(1) < 0.0f )
  {
      bUSignChange = true;
      kTest(1) = -kTest(1);
  }
  else
  {
      bUSignChange = false;
  }

  // frustum derived parameters
  float fLMin      = m_LBound;
  float fLMax      = m_DRatio*fLMin;
  float fUMin      = m_UBound;
  float fUMax      = m_DRatio*fUMin;
  float fDMin      = m_DMin;
  float fDMax      = m_DMax;
  float fLMinSqr   = fLMin * fLMin;
  float fUMinSqr   = fUMin * fUMin;
  float fDMinSqr   = fDMin * fDMin;
  float fMinLDDot  = fLMinSqr + fDMinSqr;
  float fMinUDDot  = fUMinSqr + fDMinSqr;
  float fMinLUDDot = fLMinSqr + fMinUDDot;
  float fMaxLDDot  = m_DRatio * fMinLDDot;
  float fMaxUDDot  = m_DRatio * fMinUDDot;
  float fMaxLUDDot = m_DRatio * fMinLUDDot;

  // Algorithm computes closest point in all cases by determining in which
  // Voronoi region of the vertices, edges, and faces of the frustum that
  // the test point lives.

  lmVector3 kClosest;
  float fLDot, fUDot, fLDDot, fUDDot, fLUDDot, fLEdgeDot, fUEdgeDot, fT;

  if ( kTest(2) >= fDMax )
  {
    if ( kTest(0) <= fLMax )
    {
      if ( kTest(1) <= fUMax )
      {
        // F-face
        kClosest(0) = kTest(0);
        kClosest(1) = kTest(1);
        kClosest(2) = fDMax;
      }
      else
      {
        // UF-edge
        kClosest(0) = kTest(0);
        kClosest(1) = fUMax;
        kClosest(2) = fDMax;
      }
    }
    else
    {
      if ( kTest(1) <= fUMax )
      {
        // LF-edge
        kClosest(0) = fLMax;
        kClosest(1) = kTest(1);
        kClosest(2) = fDMax;
      }
      else
      {
        // LUF-vertex
        kClosest(0) = fLMax;
        kClosest(1) = fUMax;
        kClosest(2) = fDMax;
      }
    }
  }
  else if ( kTest(2) <= fDMin )
  {
    if ( kTest(0) <= fLMin )
    {
      if ( kTest(1) <= fUMin )
      {
        // N-face
        kClosest(0) = kTest(0);
        kClosest(1) = kTest(1);
        kClosest(2) = fDMin;
      }
      else
      {
        fUDDot = fUMin*kTest(1) + fDMin*kTest(2);
        if ( fUDDot >= fMaxUDDot )
        {
          // UF-edge
          kClosest(0) = kTest(0);
          kClosest(1) = fUMax;
          kClosest(2) = fDMax;
        }
        else if ( fUDDot >= fMinUDDot )
        {
          // U-face
          fUDot = fDMin*kTest(1) - fUMin*kTest(2);
          fT = fUDot/fMinUDDot;
          kClosest(0) = kTest(0);
          kClosest(1) = kTest(1) - fT*fDMin;
          kClosest(2) = kTest(2) + fT*fUMin;
        }
        else
        {
          // UN-edge
          kClosest(0) = kTest(0);
          kClosest(1) = fUMin;
          kClosest(2) = fDMin;
        }
      }
    }
    else
    {
      if ( kTest(1) <= fUMin )
      {
        fLDDot = fLMin*kTest(0) + fDMin*kTest(2);
        if ( fLDDot >= fMaxLDDot )
        {
          // LF-edge
          kClosest(0) = fLMax;
          kClosest(1) = kTest(1);
          kClosest(2) = fDMax;
        }
        else if ( fLDDot >= fMinLDDot )
        {
          // L-face
          fLDot = fDMin*kTest(0) - fLMin*kTest(2);
          fT = fLDot/fMinLDDot;
          kClosest(0) = kTest(0) - fT*fDMin;
          kClosest(1) = kTest(1);
          kClosest(2) = kTest(2) + fT*fLMin;
        }
        else
        {
          // LN-edge
          kClosest(0) = fLMin;
          kClosest(1) = kTest(1);
          kClosest(2) = fDMin;
        }
      }
      else
      {
        fLUDDot = fLMin*kTest(0) + fUMin*kTest(1) + fDMin*kTest(2);
        fLEdgeDot = fUMin*fLUDDot - fMinLUDDot*kTest(1);
        if ( fLEdgeDot >= 0.0f )
        {
          fLDDot = fLMin*kTest(0) + fDMin*kTest(2);
          if ( fLDDot >= fMaxLDDot )
          {
            // LF-edge
            kClosest(0) = fLMax;
            kClosest(1) = kTest(1);
            kClosest(2) = fDMax;
          }
          else if ( fLDDot >= fMinLDDot )
          {
            // L-face
            fLDot = fDMin*kTest(0) - fLMin*kTest(2);
            fT = fLDot/fMinLDDot;
            kClosest(0) = kTest(0) - fT*fDMin;
            kClosest(1) = kTest(1);
            kClosest(2) = kTest(2) + fT*fLMin;
          }
          else
          {
            // LN-edge
            kClosest(0) = fLMin;
            kClosest(1) = kTest(1);
            kClosest(2) = fDMin;
          }
        }
        else
        {
          fUEdgeDot = fLMin*fLUDDot - fMinLUDDot*kTest(0);
          if ( fUEdgeDot >= 0.0f )
          {
            fUDDot = fUMin*kTest(1) + fDMin*kTest(2);
            if ( fUDDot >= fMaxUDDot )
            {
              // UF-edge
              kClosest(0) = kTest(0);
              kClosest(1) = fUMax;
              kClosest(2) = fDMax;
            }
            else if ( fUDDot >= fMinUDDot )
            {
              // U-face
              fUDot = fDMin*kTest(1) - fUMin*kTest(2);
              fT = fUDot/fMinUDDot;
              kClosest(0) = kTest(0);
              kClosest(1) = kTest(1) - fT*fDMin;
              kClosest(2) = kTest(2) + fT*fUMin;
            }
            else
            {
              // UN-edge
              kClosest(0) = kTest(0);
              kClosest(1) = fUMin;
              kClosest(2) = fDMin;
            }
          }
          else
          {
            if ( fLUDDot >= fMaxLUDDot )
            {
              // LUF-vertex
              kClosest(0) = fLMax;
              kClosest(1) = fUMax;
              kClosest(2) = fDMax;
            }
            else if ( fLUDDot >= fMinLUDDot )
            {
              // LU-edge
              fT = fLUDDot/fMinLUDDot;
              kClosest(0) = fT*fLMin;
              kClosest(1) = fT*fUMin;
              kClosest(2) = fT*fDMin;
            }
            else
            {
              // LUN-vertex
              kClosest(0) = fLMin;
              kClosest(1) = fUMin;
              kClosest(2) = fDMin;
            }
          }
        }
      }
    }
  }
  else
  {
    fLDot = fDMin*kTest(0) - fLMin*kTest(2);
    fUDot = fDMin*kTest(1) - fUMin*kTest(2);
    if ( fLDot <= 0.0f )
    {
      if ( fUDot <= 0.0f )
      {
        // point inside frustum
        kClosest = kTest;
      }
      else
      {
        fUDDot = fUMin*kTest(1) + fDMin*kTest(2);
        if ( fUDDot >= fMaxUDDot )
        {
          // UF-edge
          kClosest(0) = kTest(0);
          kClosest(1) = fUMax;
          kClosest(2) = fDMax;
        }
        else
        {
          // U-face
          fUDot = fDMin*kTest(1) - fUMin*kTest(2);
          fT = fUDot/fMinUDDot;
          kClosest(0) = kTest(0);
          kClosest(1) = kTest(1) - fT*fDMin;
          kClosest(2) = kTest(2) + fT*fUMin;
        }
      }
    }
    else
    {
      if ( fUDot <= 0.0f )
      {
        fLDDot = fLMin*kTest(0) + fDMin*kTest(2);
        if ( fLDDot >= fMaxLDDot )
        {
          // LF-edge
          kClosest(0) = fLMax;
          kClosest(1) = kTest(1);
          kClosest(2) = fDMax;
        }
        else
        {
          // L-face
          fLDot = fDMin*kTest(0) - fLMin*kTest(2);
          fT = fLDot/fMinLDDot;
          kClosest(0) = kTest(0) - fT*fDMin;
          kClosest(1) = kTest(1);
          kClosest(2) = kTest(2) + fT*fLMin;
        }
      }
      else
      {
        fLUDDot = fLMin*kTest(0) + fUMin*kTest(1) + fDMin*kTest(2);
        fLEdgeDot = fUMin*fLUDDot - fMinLUDDot*kTest(1);
        if ( fLEdgeDot >= 0.0f )
        {
          fLDDot = fLMin*kTest(0) + fDMin*kTest(2);
          if ( fLDDot >= fMaxLDDot )
          {
            // LF-edge
            kClosest(0) = fLMax;
            kClosest(1) = kTest(1);
            kClosest(2) = fDMax;
          }
          else // assert( fLDDot >= fMinLDDot ) from geometry
          {
            // L-face
            fLDot = fDMin*kTest(0) - fLMin*kTest(2);
            fT = fLDot/fMinLDDot;
            kClosest(0) = kTest(0) - fT*fDMin;
            kClosest(1) = kTest(1);
            kClosest(2) = kTest(2) + fT*fLMin;
          }
        }
        else
        {
          fUEdgeDot = fLMin*fLUDDot - fMinLUDDot*kTest(0);
          if ( fUEdgeDot >= 0.0f )
          {
            fUDDot = fUMin*kTest(1) + fDMin*kTest(2);
            if ( fUDDot >= fMaxUDDot )
            {
              // UF-edge
              kClosest(0) = kTest(0);
              kClosest(1) = fUMax;
              kClosest(2) = fDMax;
            }
            else // assert( fUDDot >= fMinUDDot ) from geometry
            {
              // U-face
              fUDot = fDMin*kTest(1) - fUMin*kTest(2);
              fT = fUDot/fMinUDDot;
              kClosest(0) = kTest(0);
              kClosest(1) = kTest(1) - fT*fDMin;
              kClosest(2) = kTest(2) + fT*fUMin;
            }
          }
          else
          {
            if ( fLUDDot >= fMaxLUDDot )
            {
              // LUF-vertex
              kClosest(0) = fLMax;
              kClosest(1) = fUMax;
              kClosest(2) = fDMax;
            }
            else // assert( fLUDDot >= fMinLUDDot ) from geometry
            {
              // LU-edge
              fT = fLUDDot/fMinLUDDot;
              kClosest(0) = fT*fLMin;
              kClosest(1) = fT*fUMin;
              kClosest(2) = fT*fDMin;
            }
          }
        }
      }
    }
  }

  kDiff = kTest - kClosest;

  // convert back to original quadrant
  if ( bLSignChange )
      kClosest(0) = -kClosest(0);

  if ( bUSignChange )
      kClosest(1) = -kClosest(1);

  if ( pkClosest )
  {
    // caller wants closest point, convert back to world coordinates
    *pkClosest = m_Origin + m_LVector*kClosest(0) + m_UVector*kClosest(1) + m_DVector*kClosest(2);
  }

  // compute and return squared distance
  return kDiff.SqrLength();
}

////----------------------------------------------------------------------------
//inline const bool lmFrustum::IntersectBox( const lmAABox3& box ) const
//{
//  return IntersectBox( box, lmMatrix4::GetIdentity() );
//}

//##
// Check if point is inside frustum
//
// This method is faster than the old version which checked all the planes
inline const bool lmFrustum::IsPointInside( const lmVector3 &v ) const
{
  lmVector3 OP = v - m_Origin;

  float f = OP.Dot(m_DVector);         // project along direction vector
  if (f < m_DMin || m_DMax < f) return false;

  float r = OP.Dot(m_LVector);         // project along left vector
  float rLimit = m_LBound * f;
  if (r < -rLimit || rLimit < r) return false;

  float u = OP.Dot(m_UVector);         // project along up vector
  float uLimit = m_UBound * f;
  if (u < -uLimit || uLimit < u) return false;

  return true;
}

//##
//Check if sphere is inside frustm
inline const bool lmFrustum::IsSphereInside( const lmVector3 &centre, const float radius ) const
{
  lmVector3 OP = centre - m_Origin;

  float f = OP.Dot(m_DVector);             // project along direction vector
  if (f < m_DMin-radius || m_DMax+radius < f) return false;

  float l = OP.Dot(m_LVector);             // project along left vector
  float lLimit = m_LBound * f;
  float lTop = lLimit + radius;
  if (l < -lTop || lTop< l) return false;

  float u = OP.Dot(m_UVector);             // project along up vector
  float uLimit = m_UBound * f;
  float uTop = uLimit + radius;
  if (u < -uTop || uTop < u) return false;

  return true;
}

//##
//Check if point of hull are inside frustm
inline const bool lmFrustum::IsHullInside( const lArray< lmVector3 > &points ) const
{
  int notIn[6] = {0,0,0,0,0,0};

  for (int i = 0; i < points.Size(); i++)
  {
    int inCount = 0;

    lmVector3 OP = points(i) - m_Origin;

    float f = OP.Dot(m_DVector);
    float l = OP.Dot(m_LVector);  // OP dot LVector
    float u = OP.Dot(m_UVector);  // OP dot UVector

    float lLimit = m_LBound * f;
    float uLimit = m_UBound * f;

    if ( f < m_DMin ) notIn[0]++; else if (f > m_DMax) notIn[3]++; else inCount++;
    if ( l < -lLimit) notIn[1]++; else if (l > lLimit) notIn[4]++; else inCount++;
    if ( u < -uLimit) notIn[2]++; else if (u > uLimit) notIn[5]++; else inCount++;

    // this point is completely in frustum so therefore hull is in
    if (inCount == 3) return true;
  }

  // check to see if all the points were completely out on anyone part of frustum
  for(int i = 0; i < 6; i++)
  {
    if(notIn[i] == points.Size())
    {
      // all the points were out on this part, so hull isn't in
      return false;
    }
  }

  return true;
}


//
inline bool SweptSpherePlaneIntersect(float& t0, float& t1, const lmVector4 &plane, const lmVector3 &sphereC, const float sphereR,
                                      const lmVector3 &sweepDir)
{
    float b_dot_n = plane(0)*sphereC(0) + plane(1)*sphereC(1) + plane(2)*sphereC(2) + plane(3);
    float d_dot_n = plane(0)*sweepDir(0) + plane(1)*sweepDir(1) + plane(2)*sweepDir(2);

    if (d_dot_n == 0.0f)
    {
      if (b_dot_n <= sphereR)
      {
        //  effectively infinity
        t0 = 0.f;
        t1 = 1e32f;
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      float tmp0 = ( sphereR - b_dot_n) / d_dot_n;
      float tmp1 = (-sphereR - b_dot_n) / d_dot_n;
      t0 = lmMin(tmp0, tmp1);
      t1 = lmMax(tmp0, tmp1);
      return true;
    }
}

//##
// Intersect swept sphere with frustum
//TOFIX!
//inline const bool lmFrustum::IntersectSweptSphere( const lmVector3 &c, const float r, const lmVector3 &sweepDir ) const
//{
//  //  algorithm -- get all 12 intersection points of the swept sphere with the view frustum
//  //  for all points >0, displace sphere along the sweep driection.  if the displaced sphere
//  //  is inside the frustum, return TRUE.  else, return FALSE
//
//  float displacements[12];
//  int cnt = 0;
//  float a, b;
//  bool inFrustum = false;
//
//  for (int i=0; i<6; i++)
//  {
//    if (SweptSpherePlaneIntersect(a, b, m_planes[i], c, r, sweepDir))
//    {
//      if (a>=0.f) displacements[cnt++] = a;
//      if (b>=0.f) displacements[cnt++] = b;
//    }
//  }
//
//  for (int i=0; i<cnt; i++)
//  {
//    lmVector3 displacedSphereC = c + (sweepDir * displacements[i]);
//    float      displacedSphereR = r * 1.1f;  // BAROG: this looks like a hack to me (from nvidia praticalpsm code) =)
//    if(IntersectSphere( displacedSphereC, displacedSphereR )) return true;
//  }
//
//  return false;
//}


END_L_NAMESPACE
#endif

