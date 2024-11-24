//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : font.h
// Description : Font class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LFONT_H
#define __LFONT_H

#include <lemon/platform/lcore.h>
#include <lemon/object/lsharedptr.h>
#include <lemon/math/lmmatrix.h>

BEGIN_L_NAMESPACE

#define FONT_INFO_POINT_CHR '$'
#define FONT_CROSS_CHR '#'

// Pre-declared classes

class lSpriteBank;
struct lSpriteBankSprite;
class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Font
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lFont
{
public:
  lFont( lSharedPtr<lSpriteBank>, const char*);
  ~lFont();

  static void SetAutoShadow( bool bEnable );
  static void SetInfoPointSprite( lSharedPtr<lSprite> );

  void            SetColour( unsigned int colour, bool bNoAutoshadow = false );
  unsigned int    GetColour();

  //NOTE: Shadow doesn't apply to Print3D
  void            SetShadow( bool b, unsigned int colour = 0 );
  bool            IsShadowed();

  void            SetScale( float scale );
  void            SetScaleX( float scale );
  void            SetScaleY( float scale );
  void            SetScaleXY( float scaleX, float scaleY );

  void            GetStringBound( int& outWidth, int& outHeight, const char* text );
  void            Print( int x, int y, const char* text, int flipFlag = 0, float rotation = 0 );
  void            PrintCenterX( int y, const char* text );

  void            GetStringBound( float& outWidth, float& outHeight, const char* text );
  void            PrintCenterX( float y, const char* text );
  void            Print( float x, float y, const char* text );
  void            Print( lmMatrix2 matrix, const char* text );
  void            Print3D( int flags, lmMatrix4 m, const char* text, int flipFlag = 0, float rotation = 0);

  void            SetScaleRes( float scale, int xRes, int yRes );
  static void     SetScaleResGlobal( float scale, int xRes, int yRes );

  const int       DefaultPixelHeight() const { return m_maxHeight; }
  const int       DefaultPixelWidth() const  { return m_maxWidth; }

  void            SetFixedCharWidth( int offset );
  int             GetFixedCharWidth();

  int             ChrWidth( char c );
  int             ChrHeight( char c );

  void            SetPixelSpacingReduction(int x, int y);

  void            Tick(float delta);

private:

  lSharedPtr<lSpriteBank>       m_spriteBank;
  const lSpriteBankSprite*      m_sprite;
  unsigned int                  m_colour;
  int                           m_maxWidth;
  int                           m_maxHeight;
  int                           m_numChars;
  char*                         m_charList;
  float                         m_scaleX;
  float                         m_scaleY;
  int                           m_asciiMap[256];
  int                           m_reduceSpacingPixelsX;
  int                           m_reduceSpacingPixelsY;
  int                           m_fixedCharWidth;

  bool                          m_bShadow;
  int                           m_shadowColour;

  static lSharedPtr<lSprite>           m_infoSprite;
};


END_L_NAMESPACE
#endif
