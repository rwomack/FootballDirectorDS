//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrfont.h
// Description :
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrender.h"

#ifndef __LRFONT__
#define __LRFONT__

#include <lemon/math/lmmatrix.h>

BEGIN_L_NAMESPACE

class lIOStreamSeekable;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrFont - font class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// This class contains basic font routines
//
// Either load a font using Load or LoadIFF or Serialize. Or on Windows use Win32GenFont to obtain a font using Win32 system calls.
//
// font = lrFont::Win32GenFont( fontName, fontHeightPixels );
// font->Print(render, lrFont::AlignCenter, lrFont::AlignCenter, "Hello world");
class LRENDER_API lrFont
{
  public:
  lrFont();
  virtual ~lrFont();

  // Print

  void SimplePrint( float x, float y, const char * str );
  void Print( float x, float y, const char * fmt, ... );
  void Print3D( int flags, lmMatrix4 m, const char* str);

  //##
  // Set the color of the font for the next Prints performed, this colour is (ARGB8888)
  void  SetColour( unsigned int colour );
  unsigned int GetColour();

  //##
  // Set the scale of the font for the next Prints performed - same scale applied to x / y
  void  SetScale( float scale );
  float GetScale();

  //##
  // Set the scale of the font for the next Prints performed (seperate x / y scales)
  void  SetScaleXY( float scaleX, float scaleY );
  float GetScaleX();
  float GetScaleY();

  //##
  // Set the scale of the font for the next Prints performed using the resolution passed in to adjust the scale for the current
  // screen resolution
  void  SetScaleRes( float scale, int xRes, int yRes );
  static void  SetScaleResGlobal( float scale, int xRes, int yRes );

  void  SetCharSpacingOffset( float offset );
  float GetCharSpacingOffset();

  // Get information on the font size

  const int  DefaultPixelHeight() const { return _defaultHeight; }
  const int  DefaultPixelWidth() const  { return _defaultWidth; }

  const int  ChrPixelWidth(char c) const  { return asciiMap[c].w; }
  const int  ChrPixelHeight(char c) const { return asciiMap[c].h; }

  const float ChrWidth(char c) const;
  const float ChrHeight(char c) const;

  const void GetStringPixelBound( int &outX, int &outY, const char * fmt, ... );
  const void GetStringBound( float &outX, float &outY, const char * fmt, ... );

  // the following enumerations can be substituted for x and y parameters

  enum
  {
    // since coordinates are in normalised space 0..1, then these values
    // are way out of range of any possible rendering value

    AlignLeft    = -101,
    AlignRight   = -102,
    AlignTop     = -101,
    AlignBottom  = -102,
    AlignCenter  = -103,
  };

  enum
  {
    PRINT3D_ENABLE_DEPTH_TEST = 1,
    PRINT3D_ENABLE_DEPTH_WRITE = 2,
  };

#if defined(L_PLATFORM_WINDOWS)
  static lrFont * Win32GenFont(const char * typeFace, unsigned int xRes, lIMAGE_TYPE imgType = L_IMAGE_ARGB_8888, const char *charList = NULL, bool bItalic = false, bool bBold = false );  // TODO: was DXT5
#endif

  static lrFont *GetBlankFont();

  // Import

  static lrFont * ImportImage( lImage *img, unsigned int chrW, unsigned chrH, unsigned int spaceWidth, unsigned int defaultHeight, lIMAGE_TYPE imgType, const char *fontStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", bool bCalcAlpha = true, bool bDupeUpperCaseToLowerCase = true );
  static lrFont * ImportImage( const char *fileName, unsigned int chrW, unsigned chrH, unsigned int spaceWidth, unsigned int defaultHeight, lIMAGE_TYPE imgType, const char *fontStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", bool bCalcAlpha = true, bool bDupeUpperCaseToLowerCase = true );

  // I/O routines

  static lrFont * LoadIFF( lIFFContext *ctxt );
  void            SaveIFF( lIFFContext *ctxt );

  static lrFont * Load( lIOStreamSeekable & );
  void            Save( lIOStreamSeekable & );

  static lrFont * Load( const char * );
  bool            Save( const char * );

  virtual void Serialize( lStream & ar );

  void            SetFixedCharWidth( float v );

  // lrFont implementation

  private:
    struct chrInfo
    {
      unsigned short x,y;
      unsigned short w,h;
      friend lStream& operator<< ( lStream& ar, struct chrInfo &c )  { ar << c.x << c.y << c.w << c.h; return ar; }
    };

    int _defaultWidth;
    int _defaultHeight;

    float _scalex;
    float _scaley;

    float _pageWidth;
    float _pageHeight;

    chrInfo                   asciiMap[256];
    RcPtr<lrTextureResource>  _tex;
    unsigned int              _colour;

    float                     _offset;

    float                     _fixedCharWidth;

    static bool     GetChrPos( char * usedMap, int mapWidth, int mapHeight, lrFont::chrInfo * chr );
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrFontServer - Global font server class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// lrFontServer
//   global static font server class
// usage: lrFont * font = lrFontServer::GetFont( name, yres )
//
class LRENDER_API lrFontServer
{
public:
  static lrFont * GetFont(const char * name, int yRes)
  {
    for(int i=0;i<fontList.Size();i++)
    {
      if( fontList(i).yRes == yRes &&
          strcmpi( fontList(i).name, name ) == 0 )
      {
        return fontList(i).font;
      }
    }

#if defined(L_PLATFORM_WINDOWS)
    lrFont * font = lrFont::Win32GenFont(name,yRes);
#else
    lrFont *font = lrFont::GetBlankFont();
#endif

    if( font == NULL )
    {
      #ifndef L_PLATFORM_XBOX
      lLogSys("FONT","Failed to create font %s(%d)\n", name, yRes );
      #endif
      return NULL;
    }

    int index = fontList.Add();
    fontList(index).font = font;
    strcpy(fontList(index).name,name);
    fontList(index).yRes = yRes;
    return font;
  }

private:
  struct FontInfo
  {
    lrFont * font;
    int      yRes;
    char     name[256];
    //HWND     handle;
  };

  static lArray<FontInfo,TRenderArrayTrait> fontList;
};

END_L_NAMESPACE

#endif
