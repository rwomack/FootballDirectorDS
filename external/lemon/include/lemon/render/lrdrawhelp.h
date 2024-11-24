//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrdrawhelp.h
// Description : Draw helpers
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrender.h"

#ifndef __LRDRAWHELP__
#define __LRDRAWHELP__

BEGIN_L_NAMESPACE

//##
// This class contains a set of helper functions for drawing bitmaps, rectangles and particles
//
// Note: All coordinates are normalized coordinates in the range of 0..1 that
//       are mapped to the current screen resolution. For consoles, these range
//       is mapped to the safe area of the screen, unless the NOSAFEAREA flag
//       is specified. All draws are batched, to flush a batch call Flush()
//       or to avoid batching specify the NOBATCH flag.
//       Batches are automatically flushed on frame swaps.
//
class LRENDER_API lrDrawHelp
{
public:

  enum
  {
    NOBATCH         = 0x001,   //## don't batch draws
    NOSAFEAREA      = 0x002,   //## don't apply safe area to coordinate mapping
    POINTSAMPLE     = 0x004,   //## use point sampling
    PIXELCOORDS     = 0x008,   //## coordinates are in pixel coordinates, not normalised (implies NOSAFEAREA)
    PROJECTSIZE     = 0x010,   //## project particle size from 3d to 2d
    CENTRE          = 0x020,   //## will override any origin position and centre the draw call (particles are always centred)
    SIZEASSCALE     = 0x040,   //## treat w/h parameters as a scale rather than a pixel / normalized size (doesnt apply to particles)
    CORRECTASPECT   = 0x080,   //## automatically correct aspect on this image (along y)
    USERSHADER      = 0x100,   //## don't set the shader (use the users) and don't batch
    NOALPHA         = 0x200,   //## disable alpha blending
    WRAP            = 0x400,   //## wrap uv coords
    TEXELTOPIXEL    = 0x800,   //## correct mapping from texels to pixels on DrawBitmap / DrawBitmapRotated calls (NOTE this does not affect DrawSubBitmap calls)
  };

  static float GetSafeBorder();

  static float GetSafeSurfaceOriginX();
  static float GetSafeSurfaceOriginY();
  static float GetSafeSurfaceXRes();
  static float GetSafeSurfaceYRes();

  //##
  // This set of routines convert from normalised coordinates to pixel coordinates and back
  static float NormalToPixelX( float x );
  static float NormalToPixelY( float y );
  static float PixelToNormalX( float x );
  static float PixelToNormalY( float y );

  //##
  // Flush any queued up lrDrawHelp calls, this will be done in the lRender::BufferSwap
  static void Flush();

  //##
  // Draw the bitmap 'texture' at x, y on the screen with the specified colour (ARGB) applied to the rectangle
  static void DrawBitmap( unsigned int flags, RcPtr<lrTextureResource> texture, float x, float y, unsigned int colour = L_ARGB(255,255,255,255) );

  //##
  // Draw the bitmap 'texture' at x, y on the screen scaled by w,h with the specified colour (ARGB) applied to the rectangle
  static void DrawBitmap( unsigned int flags, RcPtr<lrTextureResource> texture, float x, float y, float w, float h, unsigned int colour = L_ARGB(255,255,255,255) );

  //##
  // Draw the bitmap 'texture' at x, y on the screen with the specified colour (ARGB) applied to the rectangle and rotated by 'degrees'
  static void DrawBitmapRotated( unsigned int flags, RcPtr<lrTextureResource> texture, float x, float y, float degrees, unsigned int colour = L_ARGB(255,255,255,255), float originU = 0.0f, float originV = 0.0f );

  //##
  // Draw the bitmap 'texture' at x, y on the screen scaled by w,h with the specified colour (ARGB) applied to the rectangle and rotated by 'degrees'
  static void DrawBitmapRotated( unsigned int flags, RcPtr<lrTextureResource> texture, float x, float y, float w, float h, float degrees, unsigned int colour = L_ARGB(255,255,255,255), float originU = 0.0f, float originV = 0.0f );

  //##
  // Draw a subbitmap from 'texture' at x,y on the screen with the specified colour (ARGB). u0,v0,u1,v1 specify the sub bitmap to draw
  static void DrawSubBitmap( unsigned int flags, RcPtr<lrTextureResource> texture, float x, float y, float u0, float v0, float u1, float v1, unsigned int colour = L_ARGB(255,255,255,255) );

  //##
  // Draw a subbitmap from 'texture' at x,y on the screen scaled by w,h with the specified colour (ARGB). u0,v0,u1,v1 specify the sub bitmap to draw
  static void DrawSubBitmap( unsigned int flags, RcPtr<lrTextureResource> texture, float x, float y, float w, float h, float u0, float v0, float u1, float v1, unsigned int colour = L_ARGB(255,255,255,255) );

  //##
  // Draw a subbitmap from 'texture' at the coordinates specified by x0-3, y0-3 with the texture coodinates u0-3, v0-3 and the colours at each corner
  // of the rectange as specified by colour0-3
  static void DrawSubBitmap( unsigned int flags, RcPtr<lrTextureResource> texture, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
                                                                                   float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                                                                                   unsigned int colour0 = L_ARGB(255,255,255,255),
                                                                                   unsigned int colour1 = L_ARGB(255,255,255,255),
                                                                                   unsigned int colour2 = L_ARGB(255,255,255,255),
                                                                                   unsigned int colour3 = L_ARGB(255,255,255,255) );

  //##
  // Draw a subbitmap from 'texture' at the coordinates specified by x0-3, y0-3 with the texture coodinates u0-3, v0-3 and the colours at each corner
  // of the rectange as specified by colour0-3. Specify z value to write pixels of our 2D bitmap with
  static void DrawSubBitmap( unsigned int flags, RcPtr<lrTextureResource> texture, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
                                                                                   float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                                                                                   unsigned int colour0, unsigned int colour1, unsigned int colour2, unsigned int colour3,
                                                                                   float z );

  //##
  // Draw a particle with 'texture' at 3d point p with size 'size' and the colour specified (ARGB)
  static void DrawParticle( unsigned int flags, RcPtr<lrTextureResource> texture, const lmVector3 &p, float size, unsigned int colour = L_ARGB(255,255,255,255) );

  //##
  // Draw a particle with 'texture' at 3d point p with size 'size' and the colour specified (ARGB). The particle will be rotated by 'degrees'
  static void DrawParticleRotated( unsigned int flags, RcPtr<lrTextureResource> texture, const lmVector3 &p, float size, float degrees, unsigned int colour = L_ARGB(255,255,255,255) );

  //##
  // Draw a rectangle at x,y of size w,h and the specified colour (ARGB)
  static void DrawRect( unsigned int flags, float x, float y, float w, float h, unsigned int colour = L_ARGB(255,255,255,255) );

  //##
  // Draw a rectangle at x,y of size w,h with specified colours (ARGB) at each corner
  static void DrawRect( unsigned int flags, float x, float y, float w, float h,
                                unsigned int colour0, unsigned int colour1, unsigned int colour2, unsigned int colour3 );


  //##
  // Project a 3d point 'p' and return the 2d point and resulting z coordinate
  static lmVector2 ProjectPoint( const lmVector3 &p, float &z );
};

END_L_NAMESPACE
#endif
