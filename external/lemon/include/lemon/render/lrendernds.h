//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrendernds.h
// Description : NDS rendering interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#if !defined(L_PLATFORM_NDS)
#error lrendernds.h should only be included on the NDS platform
#endif

#ifndef LRENDERNDS_H_INCLUDED
#define LRENDERNDS_H_INCLUDED

#include <nitro.h>

#include "../math/lmaabox.h"
#include "../math/lmfrustum.h"

BEGIN_L_NAMESPACE


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexDesc - Vertex description
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lrVertexDescArrayTrait
{
  enum { MAX_SIZE = 200 };
  uint8 m_data[MAX_SIZE];
public:
  lrVertexDescArrayTrait()
  {
  }

  ~lrVertexDescArrayTrait()
  {
  }

  template<typename T> T * Reallocate(T * existingPtr, unsigned int newSize, unsigned int )
  {
    L_UNUSED_PARAM(existingPtr);
    lAssert( newSize * sizeof(T) <= MAX_SIZE );
    if( newSize == 0 )
    {
      return NULL;
    }
    return (T*)m_data;
  }

  static unsigned int CalcReserveSize( unsigned int size )
  {
    return size;
  }
};


class LRENDER_API lrVertexDesc : public lrVertexDescCommon
{
public:
  lrVertexDesc();
  ~lrVertexDesc();

  struct Value
  {
    uint16 stream;
    uint16 offset;
    uint16 type;
    uint16 usage;
    uint16 usageIndex;
  };

  lArray<Value,lrVertexDescArrayTrait>   desc;    // value
  unsigned int                           descID;  // unique identifier for this vertex description

  static unsigned int curdescID;

  void Empty()       { desc.Empty(); descID = curdescID++; }  // gets a new ID for it if we blank it
  void Reg( unsigned int stream, unsigned int offset, unsigned int type, unsigned int usage, unsigned int usageIndex );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrShader - Geometry shader implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrShader : public lrShaderCommon
{
public:
  lrShader();
  virtual ~lrShader();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrShaderNDS - Hardware PSP shader
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrShaderNDS : public lrShader
{
public:
  lrShaderNDS();
  ~lrShaderNDS();

  bool Select(const lrRenderState &rs);
  bool ProcessGeometry( const lrRenderState &, lrPrimitiveType, unsigned int startIndex, unsigned int count );
  bool UpdateTransform( const lrRenderState & );

  static void Log( float multiplier );
  static void LogClear();

//  virtual void PreDrawCallback( const lrRenderState &rs )  = 0;
//  virtual void PostDrawCallback( const lrRenderState &rs ) = 0;

  // lrShaderPSP interface

  //lrDrawContext  m_drawContext;
};

class LRENDER_API lrShaderFF : public lrShaderNDS
{
public:
  lrShaderFF();
  virtual ~lrShaderFF();

  bool ProcessGeometry( const lrRenderState &, lrPrimitiveType, unsigned int startIndex, unsigned int count );
//  virtual void PreDrawCallback( const lrRenderState &rs );
//  virtual void PostDrawCallback( const lrRenderState &rs );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrShader2D - Fixed function shader
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrShader2D : public lrShaderNDS
{
public:
  lrShader2D();
  virtual ~lrShader2D();

  bool ProcessGeometry( const lrRenderState &, lrPrimitiveType, unsigned int startIndex, unsigned int count );
//  virtual void PreDrawCallback( const lrRenderState &rs );
//  virtual void PostDrawCallback( const lrRenderState &rs );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrDrawContext
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define LR_SURFACE_CAPS_NDS_TOP_SCREEN                    (1<<16)
#define LR_SURFACE_CAPS_NDS_BOTTOM_SCREEN                 (1<<17)

class LRENDER_API lrDrawContext
{
public:
  lrDrawContext();
  ~lrDrawContext();

  void Apply( const lrRenderState &rs );

  int flags;

  inline  bool       GetFlag( unsigned int flag ) const         { return ( flags & flag )  !=  0; }
  inline  void       SetFlag( unsigned int flag )               { flags = flags | flag; }
  inline  void       ClearFlag( unsigned int flag )             { flags = flags & ~flag; }
  inline  void       SetFlag( unsigned int flag, bool b )       { if(b) flags = flags | flag; else flags = flags & ~flag;; }

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrRenderState
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrRenderState : public lrRenderStateCommon
{
public:
  lmMatrix4 wv;
//  lmMatrix4 vp;
  lmMatrix4 ps;

  // Streams

  enum { MAX_STREAMS = 1 };
  struct VertexStream
  {
    unsigned char * pData;
    unsigned int    stride;
    uint32          cacheID;
    uint32          numVerts;
  };

  VertexStream     stream[MAX_STREAMS];
  unsigned short * pIndex;
  uint32           indexStride;
  uint32           indexCacheID;

  lrVertexDesc   * curVertexDesc;

  bool             dirtyMatrix;
  bool             dirtyVPMatrix;

  lrTextureResourceNDS * curTexture;
  bool                   curTextureUseMips;

  // Texture references

  enum { NUM_TEXTURE_STAGES = 2 };
  RcPtr<lrTextureResource> textureRef[NUM_TEXTURE_STAGES];
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurfaceDesc - Surface description
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurfaceDesc : public lrSurfaceDescCommon
{
public:
  lrSurfaceDesc();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurface : public lrSurfaceCommon
{
public:

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurfaceTexture
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurfaceTexture : public lrSurfaceTextureCommon
{
public:
  // nothing extra here
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrRenderStatistics
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrRenderStatistics : public lrRenderStatisticsCommon
{
public:

  // perform addition
  inline const lrRenderStatistics operator+= (const lrRenderStatistics& stats)
  {
    numSurfaceSelect                +=  stats.numSurfaceSelect;
    numTransformSet                 +=  stats.numTransformSet;
    numTextureSet                   +=  stats.numTextureSet;
    numTextureSetNonNULL            +=  stats.numTextureSetNonNULL;
    numVertexDescSet                +=  stats.numVertexDescSet;
    numVertexStreamSet              +=  stats.numVertexStreamSet;
    numVertexStreamSetCached        +=  stats.numVertexStreamSetCached;
    numIndexStreamSet               +=  stats.numIndexStreamSet;
    numIndexStreamSetCached         +=  stats.numIndexStreamSetCached;
    numShaderSet                    +=  stats.numShaderSet;
    numPosts                        +=  stats.numPosts;
    numTris                         +=  stats.numTris;
    numIndices                      +=  stats.numIndices;
    totalVertexStreamBytesSet       +=  stats.totalVertexStreamBytesSet;
    totalIndexStreamBytesSet        +=  stats.totalIndexStreamBytesSet;
    totalTextureBytesSet            +=  stats.totalTextureBytesSet;

    return (*this);
  }


};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRenderNDS - NDS static render interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lRenderNDS : public lRender
{
public:
};


END_L_NAMESPACE

#endif
