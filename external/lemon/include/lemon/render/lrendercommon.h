//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrendercommon.h
// Description : Common rendering interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrenderapi.h"

#ifndef LRENDERCOMMON_H_INCLUDED
#define LRENDERCOMMON_H_INCLUDED

#include "./lrendertypes.h"

BEGIN_L_NAMESPACE

class lImage;

// Common interface
//
// These define a common interface, but not default implementation is supplied.
// The platform should implement the base members, as well as deriving and extended
// if required.

class lrVertexDescCommon;       // Common vertex description interface
class lrShaderCommon;           // Common shader interface
class lrSurfaceDescCommon;      // Common surface description
class lrSurfaceCommon;          // Common surface interface
class lrSurfaceTextureCommon;   // Common surface-texture interface
class lrRenderStateCommon;      // Common render state data
class lrRenderStatisticsCommon; // Common render statistics data


// Pre-declaration for platform specific derived interfaces

class lrVertexDesc;          // Platform vertex description interface
class lrShader;              // Platform shader interface
class lrSurfaceDesc;         // Platform surface description
class lrSurface;             // Platform surface interface
class lrSurfaceTexture;      // Platform surface texture interface
class lrRenderState;         // Platform render state data
class lrRenderStatistics;    // Platform render statistics data


// Pre-declared platform specifics
//
// These should have been defined by previously
// including "lrtextureresource.h"
// and       "lrstreamresource.h"
//
//
//class lrTextureResource;     // Platform texture resource
//class lrCacheId;             // Platform cache id
//
//template<class T, int S = sizeof(T)> class lrVertexStream; // Platform vertex stream
//template<class T, int S = sizeof(T)> class lrIndexStream;  // Platform index stream
//

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexDescCommon - Common vertex format description interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrVertexDescCommon
{
public:

  enum
  {
    TYPE_FLOAT1 = 0,                    // 32 bit float
    TYPE_FLOAT2 = 1,                    // 2 x 32 bit float
    TYPE_FLOAT3 = 2,                    // 3 x 32 bit float
    TYPE_FLOAT4 = 3,                    // 4 x 32 bit float
    TYPE_COLOUR = 4,                    // Packed ARGB colour (8888)
    TYPE_UBYTE4 = 5,                    // 4 x 8 bit unsigned integer
    #ifdef L_PLATFORM_XBOX
    TYPE_NORMPACKED3   = 6,
    #endif
    TYPE_SHORT2 = 7,                    // 2 x 16-bit signed integer
    TYPE_SHORT4 = 8,                    // 4 x 16-bit signed integer

    // NOTE: PSP types - may be temporarily here
    TYPE_UBYTE3  = 9,                   // 3 x 8-bit unsigned integer
    TYPE_USHORT3 = 10,                  // 3 x 16-bit unsigned integer
    TYPE_COLOUR_BGR565 = 11,            // 16bit BGR565
    TYPE_COLOUR_ABGR1555 = 12,          // 16bit BGR5551
    TYPE_COLOUR_ABGR4444 = 13,          // 16bit ABGR4444
    TYPE_COLOUR_ABGR8888 = 14,          // 32bit ABGR8888
    TYPE_BYTE3       = 15,              // 3x 8-bit signed integer
    TYPE_SHORT3      = 16,              // 3x 16-bit signed integer
    TYPE_UBYTE        = 17,             // 8bit unsigned integer
    TYPE_USHORT       = 18,             // 16bit unsigned integer
    TYPE_UBYTE2       = 19,             // 2 x 8bit unsigned integer
    TYPE_SHORT2USHORT1= 20,             // 3 x 16bit integers (signed X, signed Y, unsigned Z)
    TYPE_USHORT2      = 21,             // 2 x 16-bit unsigned integer
  };

  enum
  {
    USAGE_POSITION     = 0,  // position
    USAGE_POSITIONT    = 11, // position transformed
    USAGE_BLENDWEIGHT  = 1,  // blend weights
    USAGE_BLENDINDICES = 2,  // blend indices
    USAGE_NORMAL       = 3,  // normal
    USAGE_PSIZE        = 4,  // point-sprite size
    USAGE_TEXCOORD     = 5,  // tex coord
    USAGE_TANGENT      = 6,  // tangent
    USAGE_BINORMAL     = 7,  // binormal
    USAGE_COLOUR       = 8,  // colour
    USAGE_FOG          = 9,  // pixel fog
    USAGE_DEPTH        = 10, // depth value
    USAGE_MAX          = 12,
  };

  // Generic

  static unsigned int TypeSize(unsigned int type);

  // Platform specific interface should conform to the following:

  // Reg
  // Add a stream register
  //
  // stream     - stream index (4 bit)
  // offset     - bytes offset in stream (8 bit)
  // type       - register type (8 bit)
  // usage      - usage type (4 bit)
  // usageIndex - usage index (4 bit)
  //
  //void Reg(unsigned int stream, unsigned int offset, unsigned int type, unsigned int usage, unsigned int usageIndex );
  //void Empty();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrShaderCommon - Common geometry shader interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrShaderCommon
{
public:
  virtual ~lrShaderCommon() {}
  virtual bool Select( const lrRenderState & ) = 0;
  virtual bool UpdateTransform( const lrRenderState & ) = 0;
  virtual bool ProcessGeometry( const lrRenderState &, lrPrimitiveType, unsigned int startIndex, unsigned int count ) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurfaceDescCommon - Surface description
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurfaceDescCommon
{
public:
  lrSurfaceDescCommon();

  unsigned int xRes;          // x resolution of surface
  unsigned int yRes;          // y resolution of surface
  unsigned int colourBits;    // colour buffer bits
  unsigned int alphaBits;     // alpha bits
  unsigned int depthBits;     // depth bits
  unsigned int stencilBits;   // stencil bits
  unsigned int caps;          // surface caps (note, platform specific caps start at bit 16)
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurfaceCommon - Common surface interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurfaceCommon
{
public:
  const lrSurfaceDesc &  GetDesc() const;
  float                  GetAspectRatio() const;
  void                   Select();

protected:
  lrSurfaceCommon();
  ~lrSurfaceCommon();
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurfaceTextureCommon - Common surface texture interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurfaceTextureCommon
{
public:
  lrSurface                 * GetSurface() const;
  RcPtr<lrTextureResource>    GetBackBufferTexture() const;
  RcPtr<lrTextureResource>    GetDepthStencilTexture() const;

  void    Invalidate();
  void    Validate();
  bool    IsValid();

  //##
  // Dispose the contents of this surface texture as the data contained within it is no longer needed
  // The surface texture will regain memory for itself when it is next selected using SurfaceSelect
  // it may not be used as a texture and this command will fail if it is currently referenced as an
  // active texture.
  bool                        Dispose();

protected:
  lrSurfaceTextureCommon();
  ~lrSurfaceTextureCommon();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrRenderStateCommon - Common render state data
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrRenderStateCommon
{
public:

  // Primary matrices

  lmMatrix4 worldMatrix;
  lmMatrix4 viewMatrix;
  lmMatrix4 projectionMatrix;
  lmMatrix4 screenMatrix;

  // Auxiliary matrices

  lmMatrix4 WVPMatrix;       // local to projection ( worldMatrix * viewMatrix * projectionMatrix )
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrRenderStatisticsCommon - Common render statistics
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrRenderStatisticsCommon
{
public:
  // calls made

  int numSurfaceSelect;                 // number of SurfaceSelect calls

  int numTransformSet;                  // number of TransformSet calls
  int numTextureSet;                    // number of SetTextureResource calls
  int numTextureSetNonNULL;             // number of non-NULL SetTextureResource calls (substat of numTextureSet)

  int numVertexDescSet;                 // number of SetVertexDesc calls
  int numVertexStreamSet;               // number of SetVertexStream calls
  int numVertexStreamSetCached;         // number of SetVertexStream calls with cacheId (substat of numVertexStreamSet)
  int numIndexStreamSet;                // number of SetIndexStream calls
  int numIndexStreamSetCached;          // number of SetIndexStream calls with cacheId (substat of numIndexStreamSet)

  int numShaderSet;                     // number of SetShader calls

  // geometry rendered

  int numPosts;                         // number of PostGeometry calls
  int numTris;                          // number of triangles (trilists, tri-strips, trifans) passed to PostGeometry
  int numIndices;                       // number of indices passed to PostGeometry
  int numVerts;                         // number of vertices passed to rendering API (NOTE: Harder to get unless we just use vertex stream info?)

  // index / vertex data amounts

  int totalVertexStreamBytesSet;        // number of bytes of vertex stream data set to be used
  int totalIndexStreamBytesSet;         // number of bytes of index stream data set to be used
  int totalTextureBytesSet;             // number of bytes of texture data set to be used

  // advanced stats

  //NOTE: Could identify textures set multiple times not in a batch etc
};

// #defines for lrRenderStatistics
#if defined(L_DEBUG) || defined(LR_FORCE_ENABLE_STATISTICS)  // if in debug or devrelease build (or statistics forced on)

// render statistics enabled
#define LR_STATISTICS_ENABLED
#define LR_STATISTICS_SET( x ) x

#else

// render statistics disabled
#define LR_STATISTICS_SET( x )

#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRender - Common static render interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  LR_STATISTICS_CURRENT = 0x0001,
  LR_STATISTICS_MIN     = 0x0002,
  LR_STATISTICS_MAX     = 0x0004,
  LR_STATISTICS_AVERAGE = 0x0008,
  LR_STATISTICS_TOTAL   = 0x0010,
  LR_STATISTICS_ALL     = LR_STATISTICS_CURRENT | LR_STATISTICS_AVERAGE | LR_STATISTICS_MIN | LR_STATISTICS_MAX | LR_STATISTICS_TOTAL,
};

//##
//  Base render class
class LRENDER_API lRender
{
public:

  // initialisation

  static void                   Init();
  static void                   Deinit();
  static bool                   IsReady();

  // surface interface

  static const lrSurfaceDesc  * SurfaceGetAvail(unsigned int *);
  static lrSurface            * SurfaceCreate(const lrSurfaceDesc &);
  static bool                   SurfaceChange(lrSurface *, const lrSurfaceDesc &);
  static void                   SurfaceDestroy(lrSurface *);
  static bool                   SurfaceSelect(lrSurface *);
  static void                   SurfaceEnable(lrSurface *, bool );
  static bool                   SurfaceSaveTGA( char * );
  static lImage               * SurfaceCreateImage();
  static lrSurface            * SurfaceGetDefault();
  static lrSurface            * SurfaceGetCurrent();
  static void                   SurfacePrintStats();

  static lrSurfaceTexture     * SurfaceTextureCreate(const lrSurfaceDesc &);
  static void                   SurfaceTextureDestroy(lrSurfaceTexture *);

  // buffer interface

  static void                   BufferSwap();

  //##
  // Clear the colour buffer, stencil and z buffer. The colour buffer will be cleared as specified (ARGB8888)
  static void                   BufferClear( unsigned int colour );
  static void                   BufferClearDepth();

  //##
  // Perform a Buffer Swap and clear the colour buffer, stencil and z buffer. The colour buffer will be cleared as specified (ARGB8888)
  static void                   BufferSwapClear( unsigned int colour );

  //static void                   BufferClear( unsigned int clearFlags, unsigned int colour, float z,unsigned int stencil);
  static bool                   BufferSetViewport( int x, int y, int width, int height );
  static bool                   BufferGetViewport( int &x, int &y, int &width, int &height );
  static void                   BufferDefaultViewport();

  // transform interface

  static void                   TransformSet( const lrTransformType, const lmMatrix4& );
  static void                   TransformGet( const lrTransformType, lmMatrix4& );

  // texture interface

  static void                   SetTextureResource( int index, const RcPtr<lrTextureResource> & );

  // vertex + index stream interface

  static void                   SetVertexDesc( lrVertexDesc * );
  static void                   SetShader( lrShader * );
  static lrShader             * GetShader();

  static lrShader             * GetDefaultShader();
  static void                   SetDefaultShader();

  //##
  // Set the vertex stream data up for the following render calls
  // NOTE:
  //   On platforms such as PSP the user should ensure that the data passed in will be valid, i.e. that the data if written dynamically
  //   was written using an uncached pointer or the data cache has been flushed
  static void                   SetVertexStream( unsigned int stream, const void * data, unsigned int stride, unsigned int numBytes, lrCacheId * );

  //##
  // Set the index stream data up for the following render calls
  // NOTE:
  //   On platforms such as PSP the user should ensure that the data passed in will be valid, i.e. that the data if written dynamically
  //   was written using an uncached pointer or the data cache has been flushed
  static void                   SetIndexStream( const void * data, unsigned int stride, unsigned int numBytes, lrCacheId * );

  static void                   SetNullVertexStream( unsigned int stream );
  static void                   SetNullIndexStream();

  template<class T, int S> static void SetVertexStream( unsigned int stream, lrVertexStream<T,S> & );
  template<class T, int S> static void SetIndexStream( lrIndexStream<T,S> & );

  // render interface

  static void PostGeometryAdv( lrPrimitiveType p, int startIndex, int count, const char *file, const char *function, int line );
  static void PrintPostInfo();
  static void ClearPostInfo();

  // stats interface

  static void ResetStatistics( int statSelect = LR_STATISTICS_ALL );
  static bool GetStatistics( lrRenderStatistics &stats, int statSelect = LR_STATISTICS_CURRENT );
  static void LogStatistics( int statSelect = LR_STATISTICS_CURRENT );
  static void DiffStatistics( lrRenderStatistics &diff, lrRenderStatistics &prev, lrRenderStatistics &comp );
  static void UpdateStatistics();
};

#define PostGeometry( a, b, c ) PostGeometryAdv( a, b, c, __FILE__, __FUNCTION__, __LINE__ )

template<class T, int S>
inline void lRender::SetVertexStream( unsigned int streamIndex, lrVertexStream<T,S> &stream )
{
  SetVertexStream( streamIndex, stream.Data(), stream.Stride(), stream.NumBytes(), &stream.CacheId() );
}

template<class T, int S>
inline void lRender::SetIndexStream( lrIndexStream<T,S> &stream )
{
  SetIndexStream( stream.Data(), stream.Stride(), stream.NumBytes(), &stream.CacheId() );
}

END_L_NAMESPACE
#endif
