//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrprimdraw.h
// Description : Primitive drawing helpers
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrender.h"

#ifndef LRPRIMDRAW_H_INCLUDED
#define LRPRIMDRAW_H_INCLUDED

#include "./lrenderhelp.h" // for lrcamera.h

#include "../math/lmaabox.h"
#include "../math/lmplane.h"
#include "../math/lmray.h"
#include "../math/lmsphere.h"
#include "../math/lmtri.h"
#include "../math/lmfrustum.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrPrimDraw - Primitive drawing interface class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// This is a 3D primitive drawing class mainly intended for debugging purposes, it is by no means an optimal primitive renderer
// Note that all colours specified to functions in this class should be uint32 colour values (ARGB8888)
class LRENDER_API lrPrimDraw
{
public:
  static void DrawAABox(const lmAABox3 &box, unsigned int  colour = 0xffffffff);
  static void DrawAABox(const lmAABox3 &box, const lmMatrix4 &m, unsigned int  colour = 0xffffffff);
  static void DrawAABox(const lmVector3 &dim, unsigned int  colour = 0xffffffff);
  static void DrawAABox(const lmVector3 &dim, const lmMatrix4 &m, unsigned int  colour = 0xffffffff);

  static void DrawPlane(const lmPlane3 &plane);

  static void DrawSeg(const lmVector3 &p1, const lmVector3 &p2, unsigned int  colour = 0xffffffff );
  static void DrawSeg(const lmVector3 &p1, const lmVector3 &p2, unsigned int colour1, unsigned int colour2);

  static void DrawRay(const lmRay3 &ray, float len, unsigned int  colour = 0xffffffff);
  static void DrawRay(const lmRay3 &ray, unsigned int colour = 0xffffffff);

  static void DrawSphere(const lmVector3 &p, float r, unsigned int colour = 0xffffffff);
  static void DrawSphereOutline(const lmVector3 &p, float r, unsigned int colour = 0xffffffff);
  static void DrawSphere(const lmSphere3 &sphere, unsigned int colour = 0xffffffff);
  static void DrawSphereOutline(const lmSphere3 &sphere, unsigned int colour = 0xffffffff);

  static void DrawZHemisphereOutline(float radius, unsigned int colour = 0xffffffff);
  static void DrawZCylinderOutline(float length, float radius, unsigned int colour = 0xffffffff);
  static void DrawCylinderOutline(const lmVector3 &a, const lmVector3 &b, float radius, unsigned int colour = 0xfffffff);

  static void DrawZCapsuleOutline(float length, float radius, unsigned int colour = 0xffffffff);
  static void DrawCapsuleOutline(const lmVector3 &a, const lmVector3 &b, float radius, unsigned int colour = 0xfffffff);

  static void DrawAxis( const lmMatrix4 &m, float size=1, bool removeScale = true, bool rectify = false );
  static void DrawNormalPatch( const lmMatrix4 &m, const lmVector3 &point, const lmVector3 &normal );
  static void DrawTri( const lmVector3 &p1, const lmVector3 &p2, const lmVector3 &p3, unsigned int  colour = 0xffffffff );
  static void DrawTri(const lmTri3 &tri, unsigned int colour = 0xffffffff);
  static void DrawSolidTri( const lmVector3 &p1, const lmVector3 &p2, const lmVector3 &p3, unsigned int  colour = 0xffffffff );
  static void DrawSolidTri(const lmTri3 &tri, unsigned int colour = 0xffffffff);

  static void DrawFrustum( lrCamera &cam, unsigned int colour = 0xffffffff );
  static void DrawFrustum( const lmFrustum &frustum, unsigned int  colour = 0xffffffff );
};

END_L_NAMESPACE

#endif


