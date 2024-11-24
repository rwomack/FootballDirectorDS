//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrenderpc.h
// Description : Win32 PC rendering interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#if !defined(L_PLATFORM_WINDOWS)
#error lrenderpc.h should only be included on pc win32 platforms
#endif

#ifndef LRENDERPC_H_INCLUDED
#define LRENDERPC_H_INCLUDED

#include "./lrenderdxcommon.h"
#include "./lrstatesystempc.h"

struct ID3DXFragmentLinker;
typedef ID3DXFragmentLinker *LPD3DXFRAGMENTLINKER;

typedef LPCSTR D3DXHANDLE;
typedef D3DXHANDLE *LPD3DXHANDLE;

#include <D3D9.H>

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexDesc - Vertex description
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrVertexDesc : public lrVertexDescCommon
{
public:
  lrVertexDesc();
  ~lrVertexDesc();

  void  Reg( unsigned int stream, unsigned int offset, unsigned int type, unsigned int usage, unsigned int usageIndex );
  void  Empty();
  const D3DVERTEXELEMENT9 *GetElements();

private:
  lArray<D3DVERTEXELEMENT9>    m_elements;
  bool                         m_bChanged;
  LPDIRECT3DVERTEXDECLARATION9 m_pD3DDeclaration;
  LPDIRECT3DDEVICE9            m_pD3DDevice;

  friend class lRender;
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
// lrShaderFF - Fixed function shader
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrShaderFF : public lrShader
{
public:
  lrShaderFF();
  virtual bool Select( const lrRenderState & );
  virtual bool UpdateTransform( const lrRenderState & );
  virtual bool ProcessGeometry( const lrRenderState &, lrPrimitiveType, unsigned int startIndex, unsigned int count );

  D3DFILLMODE m_d3dFillMode;
  lrCullMode m_cullMode;
  bool        m_alphaTest;
  bool        m_alphaBlend;
  bool        m_depthTest;
  bool        m_depthWrite;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurfaceDesc PC caps
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define LR_SURFACE_CAPS_PC_WINDOWED                    (1<<16)
#define LR_SURFACE_CAPS_PC_TEXTURESURFACE_DEPTHTEXTURE (1<<17)
#define LR_SURFACE_CAPS_PC_INTERVAL_IMMEDIATE          (1<<18)
#define LR_SURFACE_CAPS_PC_FULLSCREEN_CURSOR           (1<<19)
#define LR_SURFACE_CAPS_PC_ENABLE_ALT_ENTER            (1<<20)

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurfaceDesc - Surface description
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurfaceDesc : public lrSurfaceDescCommon
{
public:
  lrSurfaceDesc();
  HWND hWnd;  // Window for the surface
  HICON hIcon;
  HICON hIconSmall;
  char * wndTitle;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrSurface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrSurface : public lrSurfaceCommon
{
public:
  // nothing extra here
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
// lrRenderState
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrRenderState : public lrRenderStateCommon
{
public:

  enum { NUM_SAMPLER_STAGES = 16 };
  enum { NUM_TEXTURE_STAGES = 8 };
  enum { NUM_STREAMS        = 16 };

  // Vertex buffer

  unsigned int      numVertices;

  // Index buffer

  unsigned int      baseVertexIndex;
  unsigned int      startIndex;

  // DirectX9 interface

  LPDIRECT3DDEVICE9 pD3DDevice;
  D3DCAPS9          dxCaps;

  //// TODO: can we move these into the private section?

  bool            dirtyMatrix;
  lrShader      * shader;
  lrVertexDesc  * vertexDesc;
  RcPtr<lrTextureResource> textureRef[NUM_TEXTURE_STAGES];
  LPDIRECT3DINDEXBUFFER9 pIndexData;

  LPDIRECT3DPIXELSHADER9  pPixelShader;
  LPDIRECT3DVERTEXSHADER9 pVertexShader;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrRenderStatistics
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrRenderStatistics : public lrRenderStatisticsCommon
{
public:
  int numCreateVShader;                 // number of DirectX CreateVertexShader calls
  int numCreatePShader;                 // number of DirectX CreatePixelShader calls
  int numCompileShader;                 // number of DirectX shader compile functions called

  int numSetPixelShader;                // number of SetPixelShader calls, compares last set to eliminate duplication
  int numSetPixelShaderNew;             // number of DirectX SetPixelShader calls (actual sets) (substats of numSetPixelShader)
  int numSetVertexShader;               // number of SetVertexShader calls, compares last set to eliminate duplication
  int numSetVertexShaderNew;            // number of DirectX SetVertexShader calls (actual sets) (substats of numSetVertexShader)

  int numSetPixelShaderConstant;        // number of SetPixelShaderConstant calls
  int numSetVertexShaderConstant;       // number of SetVertexShaderConstant calls

  int numSetPixelShaderConstantActual;  // number of DirectX SetPixelShaderConstant calls
  int numSetVertexShaderConstantActual; // number of DirectX SetVertexShaderConstant calls

  int numUpdateTransforms;              // number of internal transform updates (involves frustum calculation, world, view, projection matrix muls)

  int numCreateTexture;                 // number of DirectX CreateTexture calls
  int numSetTextureActual;              // number of DirectX SetTexture calls
  int numTextureUploads;                // number of texture uploads due to not being cached (substat of numTextureSet)

  int numSetIndices;                    // number of DirectX SetIndices calls
  int numSetStreamSource;               // number of DirectX SetStreamSource calls

  int numIBLock;                        // number of index buffer locks
  int numVBLock;                        // number of vertex buffer locks
  int totalIBBytesLocked;               // number of index buffer bytes locked
  int totalVBBytesLocked;               // number of vertex buffer bytes locked

//  int numCreateIB;
//  int numCreateVB;
//  int numCreateSurface;

//  int numChangeRT;

//  int numRenderStateChange;

  int totalTextureBytes;                // total number of texture bytes added to locked Textures
  int totalGeometryBytes;               // total number of geometry bytes added to locked IB/VB


  //TODO: Vertex/Index cache

//* Extra render stats mode to detect inefficiences,
//                textures used multiple times in a frame but not as a batch,
//                texture flushes done,
//                large textures on small triangles.
//
//  compare against theoritical.

  // perform addition
  inline const lrRenderStatistics operator+= (const lrRenderStatistics& stats)
  {
    numSurfaceSelect                 +=  stats.numSurfaceSelect;
    numTransformSet                  +=  stats.numTransformSet;
    numTextureSet                    +=  stats.numTextureSet;
    numTextureSetNonNULL             +=  stats.numTextureSetNonNULL;
    numVertexDescSet                 +=  stats.numVertexDescSet;
    numVertexStreamSet               +=  stats.numVertexStreamSet;
    numVertexStreamSetCached         +=  stats.numVertexStreamSetCached;
    numIndexStreamSet                +=  stats.numIndexStreamSet;
    numIndexStreamSetCached          +=  stats.numIndexStreamSetCached;
    numShaderSet                     +=  stats.numShaderSet;
    numPosts                         +=  stats.numPosts;
    numTris                          +=  stats.numTris;
    numIndices                       +=  stats.numIndices;
    totalVertexStreamBytesSet        +=  stats.totalVertexStreamBytesSet;
    totalIndexStreamBytesSet         +=  stats.totalIndexStreamBytesSet;
    totalTextureBytesSet             +=  stats.totalTextureBytesSet;
    numCreateVShader                 +=  stats.numCreateVShader;
    numCreatePShader                 +=  stats.numCreatePShader;
    numCompileShader                 +=  stats.numCompileShader;
    numSetPixelShader                +=  stats.numSetPixelShader;
    numSetPixelShaderNew             +=  stats.numSetPixelShaderNew;
    numSetVertexShader               +=  stats.numSetVertexShader;
    numSetVertexShaderNew            +=  stats.numSetVertexShaderNew;
    numSetPixelShaderConstant        +=  stats.numSetPixelShaderConstant;
    numSetVertexShaderConstant       +=  stats.numSetVertexShaderConstant;
    numSetPixelShaderConstantActual  +=  stats.numSetPixelShaderConstantActual;
    numSetVertexShaderConstantActual +=  stats.numSetVertexShaderConstantActual;
    numUpdateTransforms              +=  stats.numUpdateTransforms;
    numCreateTexture                 +=  stats.numCreateTexture;
    numSetTextureActual              +=  stats.numSetTextureActual;
    numTextureUploads                +=  stats.numTextureUploads;
    numSetIndices                    +=  stats.numSetIndices;
    numSetStreamSource               +=  stats.numSetStreamSource;
    numIBLock                        +=  stats.numIBLock;
    numVBLock                        +=  stats.numVBLock;
    totalIBBytesLocked               +=  stats.totalIBBytesLocked;
    totalVBBytesLocked               +=  stats.totalVBBytesLocked;
    totalTextureBytes                +=  stats.totalTextureBytes;
    totalGeometryBytes               +=  stats.totalGeometryBytes;

    return (*this);
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexShaderCompiled
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrVertexShaderCompiled
{
public:
  lrVertexShaderCompiled( RcPtr<lrCompiledVS> shader );
  ~lrVertexShaderCompiled();

  bool Select();
  LPDIRECT3DVERTEXSHADER9 GetShader(LPDIRECT3DDEVICE9);

private:
  RcPtr<lrCompiledVS>    m_compiledShader;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrPixelShaderCompiled
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrPixelShaderCompiled
{
public:
  lrPixelShaderCompiled( RcPtr<lrCompiledPS> shader );
  ~lrPixelShaderCompiled();

  bool Select();
  LPDIRECT3DPIXELSHADER9 GetShader(LPDIRECT3DDEVICE9);

private:
  RcPtr<lrCompiledPS>    m_compiledShader;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DPIXELSHADER9  m_pPixelShader;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexShaderFile
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrVertexShaderFile
{
public:
  lrVertexShaderFile( const char * fileName );
  ~lrVertexShaderFile();

  bool Select();
  LPDIRECT3DVERTEXSHADER9 GetShader(LPDIRECT3DDEVICE9);

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
  const char *            m_fileName;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexShaderInline
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrVertexShaderInline
{
public:
  lrVertexShaderInline(const lString &source);
  ~lrVertexShaderInline();

  bool Select();
  LPDIRECT3DVERTEXSHADER9 GetShader(LPDIRECT3DDEVICE9);

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
  lString                 m_source;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexShaderInlineDebug
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrVertexShaderInlineDebug
{
public:
  lrVertexShaderInlineDebug(const lString &source);
  ~lrVertexShaderInlineDebug();

  bool Select();
  LPDIRECT3DVERTEXSHADER9 GetShader(LPDIRECT3DDEVICE9);

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
  lString                 m_source;
  char                    m_fileName[128];
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrPixelShaderFile
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrPixelShaderFile
{
public:
  lrPixelShaderFile( const char * fileName );
  ~lrPixelShaderFile();

  bool Select();
  LPDIRECT3DPIXELSHADER9 GetShader(LPDIRECT3DDEVICE9);

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DPIXELSHADER9  m_pPixelShader;
  const char *            m_fileName;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrPixelShaderInline
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrPixelShaderInline
{
public:
  lrPixelShaderInline(const lString &source, const char *hackName = "");
  ~lrPixelShaderInline();

  bool Select();
  LPDIRECT3DPIXELSHADER9 GetShader(LPDIRECT3DDEVICE9);

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DPIXELSHADER9  m_pPixelShader;
  lString                 m_source;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrHLSLVertexShaderInline
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrHLSLVertexShaderInline
{
public:
  lrHLSLVertexShaderInline( const lString &source, const char * target = "vs_1_1", const char *function = "main" );
  ~lrHLSLVertexShaderInline();

  bool Select();
  LPDIRECT3DVERTEXSHADER9 GetShader(LPDIRECT3DDEVICE9);
  void                    AssembleToFile( const char* );
  void SetTarget( const char *target )
  {
    m_target = target;
  }

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
  lString                 m_source;
  const char *            m_func;
  const char *            m_target;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrHLSLVertexShaderInlineDebug
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrHLSLVertexShaderInlineDebug
{
public:
  lrHLSLVertexShaderInlineDebug( const lString &source, const char * target = "vs_1_1", const char *function = "main" );
  ~lrHLSLVertexShaderInlineDebug();

  bool Select();
  LPDIRECT3DVERTEXSHADER9 GetShader(LPDIRECT3DDEVICE9);
  void SetTarget( const char *target )
  {
    m_target = target;
  }

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
  lString                 m_source;
  const char *            m_func;
  const char *            m_target;
  char                    m_fileName[128];
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrHLSLVertexShaderFile
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrHLSLVertexShaderFile
{
public:
  lrHLSLVertexShaderFile( const char * fileName, const char * target = "vs_1_1", const char *function = "main" );
  ~lrHLSLVertexShaderFile();

  bool Select();
  LPDIRECT3DVERTEXSHADER9 GetShader(LPDIRECT3DDEVICE9);
  void SetTarget( const char *target )
  {
    m_target = target;
  }

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
  const char *            m_func;
  const char *            m_fileName;
  const char *            m_target;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrHLSLPixelShaderInline
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrHLSLPixelShaderInline
{
public:
  lrHLSLPixelShaderInline( const lString &source, const char * target = "ps_1_1", const char *function = "main" );
  ~lrHLSLPixelShaderInline();

  bool Select();
  LPDIRECT3DPIXELSHADER9  GetShader(LPDIRECT3DDEVICE9);
  void                    AssembleToFile( const char* );
  void SetTarget( const char *target )
  {
    m_target = target;
  }

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DPIXELSHADER9  m_pPixelShader;
  lString                 m_source;
  const char *            m_func;
  const char *            m_target;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrHLSLPixelShaderFile
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrHLSLPixelShaderFile
{
public:
  lrHLSLPixelShaderFile( const char * fileName, const char * target = "ps_1_1", const char *function = "main" );
  ~lrHLSLPixelShaderFile();

  bool Select();
  LPDIRECT3DPIXELSHADER9  GetShader(LPDIRECT3DDEVICE9);
  void SetTarget( const char *target )
  {
    m_target = target;
  }

private:
  bool                    m_bAssembled;
  LPDIRECT3DDEVICE9       m_pD3DDevice;
  LPDIRECT3DPIXELSHADER9  m_pPixelShader;
  const char *            m_func;
  const char *            m_fileName;
  const char *            m_target;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrVertexShaderFragmentLibrary
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lrVertexShaderFragmentLibrary;

class LRENDER_API lrVertexShaderFragment
{
protected:
  lArray<char>   _nameData;
  lArray<int>    _nameArray;

public:

  void Add( const char *name )
  {
    int start = _nameData.Add( (int)strlen(name) + 1 );
    memcpy(&_nameData(start), name, strlen(name));
    _nameData.Last() = 0;

    _nameArray.Push( start );
  }

  lrVertexShaderFragment &lrVertexShaderFragment::operator=(const lrVertexShaderFragment &frag)
  {
    _nameData = frag._nameData;
    _nameArray = frag._nameArray;

    return *this;
  }

  friend lrVertexShaderFragmentLibrary;
};

class LRENDER_API lrVertexShaderFragmentLibrary
{
public:

  typedef struct fragmentWeld
  {
    lrVertexShaderFragment  fragments;
    LPD3DXHANDLE             fragmentHandles;
  };

  lrVertexShaderFragmentLibrary();
  ~lrVertexShaderFragmentLibrary();

  bool Init( LPDIRECT3DDEVICE9 device );

  bool AddCode( const char *source );
  bool AddFile( const char *fileName );

  int  CreateWeld( lrVertexShaderFragment frag );
  int  LookupWeld( lrVertexShaderFragment frag );

  LPDIRECT3DVERTEXSHADER9  GetShaderFromFragment( LPDIRECT3DDEVICE9 device, lrVertexShaderFragment frag, const char *target = "vs_1_1" );
  LPDIRECT3DVERTEXSHADER9  GetShader( LPDIRECT3DDEVICE9 device, int fragWeld, const char *target = "vs_1_1" );

private:
  lArray<fragmentWeld>   m_weldList;

  LPDIRECT3DDEVICE9       m_pD3DDevice;

  LPD3DXFRAGMENTLINKER    m_fragLinker;
  lArray<const char *>   m_sourceData;
  lArray<const char *>   m_filenameData;

  int                     m_lastSource;
  int                     m_lastFilename;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrStateArray - Array based state block
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// An array based state block
// This is good for having a preset array of states that you wish to apply quickly.

class LRENDER_API lrStateArray
{
  lArray<RenderState>  m_renderStateBlock;
  lArray<SamplerState> m_samplerStateBlock[lrRenderState::NUM_SAMPLER_STAGES];
  lArray<TextureState> m_textureStateBlock[lrRenderState::NUM_TEXTURE_STAGES];
  bool                  m_bDefault;

public:

  enum StateSetType
  {
    APPEND_STATES,
    DEFAULT_STATES
  };

  lrStateArray();
  lrStateArray( StateSetType type );
  void Default();
  void SetRenderState( D3DRENDERSTATETYPE State, uint32 Value );
  void SetSamplerState( uint32 Sampler, D3DSAMPLERSTATETYPE Type, uint32 Value );
  void SetTextureStageState( uint32 Stage, D3DTEXTURESTAGESTATETYPE Type, uint32 Value );
  void Apply();

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrStateBlock - Linked list based state block
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Linked list state block
// This is good for setting states dynamically, and applying changes
// incrementally.

class LRENDER_API lrStateBlock
{
  StateSystemPC<RenderState>::State  * m_renderStateHead;
  StateSystemPC<SamplerState>::State * m_samplerStateHead[lrRenderState::NUM_SAMPLER_STAGES];
  StateSystemPC<TextureState>::State * m_textureStateHead[lrRenderState::NUM_TEXTURE_STAGES];
  bool                                 m_bDefault;

public:

  enum StateSetType
  {
    APPEND_STATES,
    DEFAULT_STATES
  };

  lrStateBlock( StateSetType type = DEFAULT_STATES );
  ~lrStateBlock();

  void Default();
  void SetRenderState( D3DRENDERSTATETYPE State, uint32 Value );
  void SetSamplerState( uint32 Sampler, D3DSAMPLERSTATETYPE Type, uint32 Value );
  void SetTextureStageState( uint32 Stage, D3DTEXTURESTAGESTATETYPE Type, uint32 Value );
  void Apply();

};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRenderPC - PC static render interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lRenderPC : public lRenderDXCommon
{
public:

  static const char *            GetDX9DriverName();
  static D3DADAPTER_IDENTIFIER9 &GetDX9DriverIdent();
  static LPDIRECT3DDEVICE9       GetDX9Device();

  static int                     GetVertexShaderVersion();
  static int                     GetPixelShaderVersion();

  //##
  // Set whether pixel scissoring is enabled
  // NOTE: x, y, w, h are specified in normalized coordinates
  //       if the resulting scissor rectangle is outside of (0 - 1) then the box will be clamped to prevent driver issues
  //       related to illegal settings of the scissor rect
  static void                    SetScissor( bool bScissorEnable, float x = 0.0f, float y = 0.0f, float w = 1.0f, float h = 1.0f );
  static bool                    IsScissorEnabled();
  static void                    GetScissorRect( float &x, float &y, float &w, float &h );

  static bool                    SurfaceCopyToSurfaceTexture( lrSurfaceTexture *surfTex, bool bLinearFilter = false );

  // lRenderDX9 implementation

  static HWND GetHWnd();
};

END_L_NAMESPACE

#endif
