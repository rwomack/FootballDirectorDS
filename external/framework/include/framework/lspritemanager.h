//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lspritemanager.h
// Description : lSpriteManager
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LSPRITEMANAGER_H
#define __LSPRITEMANAGER_H

#include <lemon/platform/lcore.h>
#include <lemon/object/lsharedptr.h>
#include <lemon/math/lmmatrix.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSpriteBank;
struct lSpriteBankSprite;

enum
{
  PRINT3D_ENABLE_DEPTH_TEST = 1,
  PRINT3D_ENABLE_DEPTH_WRITE = 2,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SpriteFlipFlags
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum SpriteFlipFlags
{
  FLIP_NONE  = 0,
  FLIP_HORIZ = 1,
  FLIP_VERT  = 2,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SpriteState
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct SpriteState
{
  float x;
  float y;
  float w;
  float h;
  float clipX0;
  float clipY0;
  float clipX1;
  float clipY1;
  float rotation;
  uint32 flipFlags;
  uint32 frameIndex;
  uint32 colour;
  uint32 transparent;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSpriteManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lSpriteManager
{
public:
  static int GetDisplayWidth();
  static int GetDisplayHeight();
  static int GetDisplayIndex();
  static int GetNumDisplays();
  static void BufferSwap();

  #if defined(L_PLATFORM_WINDOWS)
  static void EmulateDS();
  static bool IsEmulateDS();
  #endif

  static void Render( const lSpriteBank *, const lSpriteBankSprite *, const SpriteState * );
  static void RenderFontCell( const lSpriteBank * bank, int cellIndex, float x, float y, float scaleX, float scaleY, luint32_t colour );
  static void RenderCell( const lSpriteBank * bank, int cellIndex, int x, int y, luint32_t colour );
  static void Render3D( int flags, lmMatrix4 m, const lSpriteBank * bank, const lSpriteBankSprite * sprite, const SpriteState * state );

  static void DrawRect2( luint32_t x, luint32_t y, luint32_t w, luint32_t h, luint32_t colour );
  static void DrawLine2( luint32_t x0, luint32_t y0, luint32_t x1, luint32_t y1, luint32_t colour );

  #if defined(L_PLATFORM_NDS)
  static void DrawRect( luint32_t x1, luint32_t y1, luint32_t x2, luint32_t y2, luint8_t alpha, luint16_t colour16 );
  static void DrawLine( luint32_t x1, luint32_t y1, luint32_t x2, luint32_t y2, luint8_t alpha, luint16_t colour16 );
  static void DrawGradientRect( luint32_t x1, luint32_t y1, luint32_t x2, luint32_t y2, luint8_t alpha, luint16_t colour16A, luint16_t colour16B );
  static void RenderCellDirect( int *cacheIndexPtr, void * pageData, bool dirty, int pageCellIndex, int x, int y, luint32_t inColour );
  static int GetPolys();
  static void RenderWithDropShadow( const lSpriteBank *, const lSpriteBankSprite *, const SpriteState *, luint32_t dropcolour );
  #endif

  //temp
  static void SetScreenFade( int alpha );
  static void SetScreenFade2( int alpha, int startY );


  static void SetHackScaleNTSC(bool);
  #ifndef L_PLATFORM_NDS
  static float GetHackScaleNTSCValue();
  #endif
};

END_L_NAMESPACE
#endif

