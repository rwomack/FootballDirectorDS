//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrtextureresourcepc.h
// Description : Win32 PC texture resource
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifndef LRTEXTURERESOURCEPC_H_INCLUDED
#define LRTEXTURERESOURCEPC_H_INCLUDED

#include "./lrtextureresourcecommon.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrTextureResourcePC - Texture for Win32 PC
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrTextureResourcePC : public lResource
{
public:
  lrTextureResourcePC();
  ~lrTextureResourcePC();

  // Common interface

  static lrTextureResourcePC * CreateTexture( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type );
  static lrTextureResourcePC * CreateTexture( lImage * img );
  static lrTextureResourcePC * CreateDynamicTexture( unsigned int width, unsigned int height, lIMAGE_TYPE type );

  bool         SetHeader( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type, unsigned int data, bool bDynamic );
  unsigned int GetNumLevels();
  bool         GetMipMapLevel( unsigned int level, lrTextureResourceLevel & );
  void *       GetMipMapData( unsigned int level );
  unsigned int GetLevelDataSize(unsigned int level );
  unsigned int CalculateDataSize();
  unsigned int CalculateTableSize();

  lImage *    CreateLevelImage(int level);
  lImage *    CreateImage();

  void         Dirty();

  // PC Specific interface

  bool         IsSurface();
  void         SetSurfaceHeader();

  // Resource

  friend lResourceFixer & operator << (lResourceFixer &fixer, lrTextureResourcePC &texture )
  {
    texture.d3dTexture = NULL;
    fixer.FixData( (void *&)texture.data, texture.CalculateDataSize() + texture.CalculateTableSize() );
    return fixer;
  }

  // texture format, 8-bits (a limage format)
  // mipmaps, 4 bits (number of mip maps)
  // usize,   4 bits (log2 of size)
  // vsize,   4 bits (log2 of size)
  // psize,   4 bits (log2 of size)
  // surface, 1 bit (if the texture is a system render surface)
  // dynamic, 1 bit (if the texture is dynamic)
  // 6 bits spare, to store, cubemap flag, number of dimensions etc.

  struct PCTextureFmt
  {
    unsigned int type:8;
    unsigned int numMipMaps:4;
    unsigned int usize:4;
    unsigned int vsize:4;
    unsigned int psize:4;
    unsigned int surface:1;
    unsigned int dynamic:1;
    unsigned int spare:6;
  };

  unsigned int format;
  unsigned int data;

  bool         bDynamicDirty;                  // if dynamic texture is dirty

  // For the driver
  void * d3dTexture;
  void * d3dDevice;  // only for comparison, does not keep a reference.
};

// Save lrTextureResourcePC as images.
// Note that saving muliple references to the same texture into an archive will actually
// save multple copies.

//inline lStream & operator << (lStream &ar, RcPtr<lrTextureResourcePC> &texture )
//{
//  if( ar.IsLoading() )
//  {
//    lImage * img;
//    ar << img;
//    if(img)
//    {
//      texture = lrTextureResourcePC::CreateTexture( img );
//    }
//    else
//    {
//      texture = NULL;
//    }
//    delete img;
//  }
//  else
//  {
//    lImage * img = NULL;
//    if( texture != NULL ) img = texture->CreateImage();
//    ar << img;
//    if( img ) delete img;
//  }
//
//  return ar;
//}

END_L_NAMESPACE

#endif
