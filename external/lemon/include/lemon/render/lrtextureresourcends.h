//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrtextureresourcends.h
// Description : NDS texture resource
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifndef LRTEXTURERESOURCENDS_H_INCLUDED
#define LRTEXTURERESOURCENDS_H_INCLUDED

#include "./lrtextureresourcecommon.h"
#include "./lrstreamresourcends.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrTextureResourceNDS - Texture for NDS
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrTextureResourceNDS : public lResource
{
public:
  lrTextureResourceNDS();
  ~lrTextureResourceNDS();

  // Common interface

  static lrTextureResourceNDS * CreateTexture( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type );
  static lrTextureResourceNDS * CreateTexture( lImage * img );

  bool         SetHeader( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type, unsigned int data );
  unsigned int GetNumLevels();
  bool         GetMipMapLevel( unsigned int level, lrTextureResourceLevel & );
  void *       GetMipMapData( unsigned int level );
  unsigned int GetLevelDataSize( unsigned int level );
  unsigned int CalculateDataSize();
  unsigned int CalculateTableSize();

  lImage *    CreateLevelImage(int level);
  lImage *    CreateImage();

  void         Dirty();

  unsigned int format;
  unsigned int data;
  lrCacheIdNDS cacheId;
  
  struct NDSTextureFormat
  {
    unsigned int type:8;         // sce type
    unsigned int pageType:2;     // page type, 0=32bit, 1=16bit, 2=8bit, 3=4bit
    unsigned int numMipMaps:4;   // number of mip maps
    unsigned int usize:4;        // log2 width
    unsigned int vsize:4;        // log2 height
    unsigned int swizzled:1;     // swizzled, 0=none (Gu::TEXBUF_NORMAL), 1=swizzled (Gu::TEXBUF_FAST)
    unsigned int specialtype:2;  // special type, 0=none
    unsigned int spare:7;        // spare
  };

  friend lResourceFixer & operator << (lResourceFixer &fixer, lrTextureResourceNDS &texture )
  {
    fixer.FixData( (void *&)texture.data, texture.CalculateDataSize() + texture.CalculateTableSize() );
    return fixer;
  }
};


END_L_NAMESPACE

#endif
