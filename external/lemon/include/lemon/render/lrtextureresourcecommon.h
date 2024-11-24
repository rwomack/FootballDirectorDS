//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrtextureresourcecommon.h
// Description : Common texture resource include
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include "./lrenderapi.h"

#ifndef LRTEXTURERESOURCECOMMON_H_INCLUDED
#define LRTEXTURERESOURCECOMMON_H_INCLUDED

#include "../platform/lcore.h"
#include "../image/limage.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrTextureResourceLevel - describes a mip map level of a texture
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lrTextureResourceLevel
{
  void       * data;
  unsigned int dataSize;
  void       * table;
  unsigned int tableSize;
  lIMAGE_TYPE  type;
  unsigned int width;
  unsigned int height;
  unsigned int pitch; // (width * bitsPerPixel) / 8
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrTextureResourceXXX
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// All platform specific texture resources should implement the following interface.
// This is not defined a common base class, as resources are accessible across
// platform. (ie, you can access PS2 and Xbox texture resources on PC).
//
//
//class LRENDER_API lrTextureResourceXXX : public lResource
//{
//  static lrTextureResourceXXX * CreateTexture( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type );
//  static lrTextureResourceXXX * CreateTexture( lImage * img );
//
//  lImage *    CreateLevelImage(int level);
//  lImage *    CreateImage();
//
//  void         SetHeader( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type, unsigned int data );
//  unsigned int GetNumLevels();
//  bool         GetMipMapLevel( unsigned int level, lrTextureResourceLevel & );
//  void *       GetMipMapData( unsigned int level );
//  unsigned int GetLevelDataSize(unsigned int level );
//  unsigned int CalculateDataSize();
//  void         Dirty();
//
//  friend lResourceFixer & operator << (lResourceFixer &fixer, lrTextureResourceXXX &texture );
//
//};

END_L_NAMESPACE

#endif
