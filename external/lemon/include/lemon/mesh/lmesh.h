//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±s±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmesh.h
// Description :
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Implementor : Barog
 Library     :
 Version     : 1.0

 Modification History
 --------------------

 14/03/2006 - First version                                       - Barog

 Notes
 -----

 Write out full design

 ---

Mesh clases -
  Redundant vert/edge/face removal
  Degenerate face removal
  Incorrect orientation of single-sided faces
  Removing of holes / cracks / tjunctions
  Nonplanar face

 mesh processing -

 generation of vertex shader extrudable for shadow volume
 Creation of shadow volume

 Clipper versus planes

 remove unused verts
 snap degenerate verts
 fix tjoints
 close holes
 simplify with PM
 generate texture atlas
 generate normal map
 generate optimal b-sphere
 generate convex hull
 generate optimal OBB

 loose octree would work well for doing epsilon based dedupe

 Surface tracking
  - Idx, verts, indices, desc, flags

 Render - take params for all kinds of render type
   Will work on all platforms

 MeshGen
 Surface change / merge

 LOD

 Bone understanding / morph target
  - Reduce ones
  - Apply bones (Fast realtime)

 Import / export collada

 --

 Save to C++ file -
  represent data as hex or as floats etc
 Generate Geom types

 lMeshGen gen;

 gen.Begin();

 gen.BeginElement( MeshGen::RAW | MeshGen::TRI_LIST );
   gen.VertexInput( void **, int vertSize, vertexDesc );
 //  gen.IndexInput( void *, int indexSize, int indexCount );
 gen.EndElement();


 L-System parser? (k3d implements one)

 Mesh system will store two representations? The actual commands used to generate the mesh as it is and the current
 representation

 Mesh system will be used by all things working with meshes, including the lSceneMesh classes
 it will store references to streams

 The original representation will be blankable to free up the ram when it isn't needed.

 Unwrapping texturing
 Selection via CSG / Bone style / Intersection with other primitives

 Collision / Intersection will use whichever optimised collision representation was available

 Merging of vertices

 Adjacency information

 Unwrap functions
 Normal mapping functions
 Convex decomposition
 Hull generation
 Volume / Centre of mass generation / Intersection / Containment / Generate shadow volume
 Silhoutte
 Store compressed
 Quantize
 Generate AABBTree / OBBTree / KDtree

 Generator functions / classes will be seperate and will just pass output back into the mesh

 Mesh class will have slow accessors for individual vertices / indices and fast accessors with pointer + stride returns

 Will work on unoptimised vertex data at first and will have the option to produce an optimised set with any vertex declaration

 Collision data should be external to mesh class ?

 Collision commands will also just work ON mesh classes not from within them, it will simply look at what collision representations
 are available from the mesh class to speed the process up.

 Optimise vertex desc based on current data (and platform limitations)

 AoS to SoA

 --

 Seperate lib -
 Texture packing functions for atlas

 ---

 Vertex pointers to do quicker scans through all the data.. i.e. get a pointer for each part of desc

 Templates for the conversions

 Render desc creation

*/

#ifndef LMMESH_H_INCLUDED
#define LMMESH_H_INCLUDED

#include "../containers/larray.h"
#include "../math/lmmatrix.h"
#include "../render/lrender.h"

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LMESH_BUILD
#  define LMESH_API L_EXPORT
#else
#  define LMESH_API L_IMPORT
#endif

BEGIN_L_NAMESPACE

// mesh array trait
typedef TArrayTrait<3,8,16,L_ARENA_MESH> TMeshArrayTrait;  // 3/8 + 16

template<typename T>
class LMESH_API lMeshVertexDataAccessor
{
 public:
  lMeshVertexDataAccessor( uint8 *ptr, uint32 stride ) : m_ptr(ptr), m_stride(stride)
  {
    m_cur = m_ptr;
  }

  T *Cur()
  {
    return (T*)m_cur;
  }

  void Next()
  {
    m_cur += m_stride;
  }

  T *Get( int index )
  {
    return (T *)(m_ptr + (index * m_stride));
  }

  const T& Get(int index) const
  {
    return *((const T *)(m_ptr + (index * m_stride)));
  }

 private:
  uint8 *m_ptr;
  uint8 *m_cur;
  uint32 m_stride;
};

/*
 Vertex desc system
 Need flexible + fast Read <-> Write system from vertex desc based data of all types

 Vertex descs testable on render platform for compatibility.
 Mesh can try to find the tightest representation of a mesh from the data

 */

class LMESH_API lVertexDesc
{
 public:
  enum
  {
    TYPE_FLOAT1   = 0, // 32 bit float
    TYPE_FLOAT2      , // 2 x 32 bit float
    TYPE_FLOAT3      , // 3 x 32 bit float
    TYPE_FLOAT4      , // 4 x 32 bit float
    TYPE_FLOAT16_2   , // 2 x 16 bit float
    TYPE_FLOAT16_4   , // 4 x 16 bit float
    TYPE_COLOUR      , // Packed ARGB colour
    TYPE_UBYTE4      , // 4 x 8 bit unsigned integer
    TYPE_UBYTE4N     , // 4 x 8 bit unsigned integer normalized
    TYPE_SHORT2      , // 2 x 16-bit signed integer
    TYPE_SHORT2N     , // 2 x 16-bit signed integer normalized
    TYPE_SHORT4      , // 4 x 16-bit signed integer
    TYPE_SHORT4N     , // 4 x 16-bit signed integer normalized
    TYPE_USHORT2N    , // 2 x 16-bit unsigned integer normalized
    TYPE_USHORT4N    , // 4 x 16-bit unsigned integer normalized
    TYPE_UDEC3       , // 3 x 10-bit unsigned format
    TYPE_DEC3N       , // 3 x 10-bit signed format normalized
    TYPE_MAX,
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

  static int         TypeSize( int type );
  static const char *TypeName( int type );
  static const char *UsageName( int usage );

  static int ToRenderType( int type )
  {
    switch( type )
    {
      case TYPE_FLOAT1     : return lrVertexDescCommon::TYPE_FLOAT1;
      case TYPE_FLOAT2     : return lrVertexDescCommon::TYPE_FLOAT2;
      case TYPE_FLOAT3     : return lrVertexDescCommon::TYPE_FLOAT3;
      case TYPE_FLOAT4     : return lrVertexDescCommon::TYPE_FLOAT4;
      case TYPE_FLOAT16_2  : return -1;
      case TYPE_FLOAT16_4  : return -1;
      case TYPE_COLOUR     : return lrVertexDescCommon::TYPE_COLOUR;
      case TYPE_UBYTE4     : return lrVertexDescCommon::TYPE_UBYTE4;
      case TYPE_UBYTE4N    : return -1;
      case TYPE_SHORT2     : return lrVertexDescCommon::TYPE_SHORT2;
      case TYPE_SHORT2N    : return -1;
      case TYPE_SHORT4     : return lrVertexDescCommon::TYPE_SHORT4;
      case TYPE_SHORT4N    : return -1;
      case TYPE_USHORT2N   : return -1;
      case TYPE_USHORT4N   : return -1;
      case TYPE_UDEC3      : return -1;
      case TYPE_DEC3N      : return -1; //lrVertexDescCommon::TYPE_NORMPACKED3;
      default : break;
    }
    return -1;
  }

  static int ToRenderUsage( int usage )
  {
    return usage;
  }

  ///
  ///

  struct Value
  {
    uint16 offset;
    uint8  stream;
    uint8  type;
    uint8  usage;
    uint8  usageIndex;
  };

  lArray<Value>  desc;    // value
  unsigned int   descID;  // unique identifier for this vertex description

  static unsigned int curdescID;

  bool IsEmpty()
  {
    return desc.Size() == 0;
  }

  void Empty()
  {
    desc.Empty();
    descID = curdescID++;  // gets a new ID for it if we blank it
  }

  int Find( unsigned int usage, unsigned int usageIdx, unsigned int type, int skip = 0 )
  {
    for(int i = 0; i < desc.Size(); i++)
    {
      if( (usage == 0xffffffff    || desc(i).usage == usage) &&
          (usageIdx == 0xffffffff || desc(i).usageIndex == usageIdx) &&
          (type == 0xffffffff     || desc(i).type == type ) )
      {
        if( skip == 0 )
        {
          return i;
        }

        skip--;
      }
    }

    return -1;
  }

  int RegNext( unsigned int stream, unsigned int type, unsigned int usage, unsigned int usageIndex )
  {
    return Reg( stream, Stride( stream ), type, usage, usageIndex );
  }

  int Reg( unsigned int stream, unsigned int offset, unsigned int type, unsigned int usage, unsigned int usageIndex )
  {
    Value val;
    val.offset     = (uint16)offset;
    val.stream     = (uint8)stream;
    val.type       = (uint8)type;
    val.usage      = (uint8)usage;
    val.usageIndex = (uint8)usageIndex;

    desc.Push( val );

    return desc.Size()-1;
  }

  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str )
      lLogFmt("%s (%d): %dbytes\n", str, descID, Size() );
    else
      lLogFmt("%d: %dbytes\n", descID, Size() );

    // TODO: sort as we print
    for(int i=0;i<desc.Size();i++)
    {
      lLogFmt(" %d:%d: %s%d - %s\n", desc(i).stream, desc(i).offset, UsageName( desc(i).usage ), desc(i).usageIndex, TypeName(desc(i).type) );
    }
  }
  #endif

  int Size() const
  {
    int total = 0;
    for(int i = 0; i < desc.Size(); i++)
    {
      total += TypeSize( desc(i).type );
    }
    return total;
  }

  int Stride( int stream = 0 ) const
  {
    int lastoffset = 0;
    for(int i = 0; i < desc.Size(); i++)
    {
      if( desc(i).stream == stream )
      {
        int typesize = TypeSize( desc(i).type );
        if( desc(i).offset + typesize >= lastoffset )
        {
          lastoffset = desc(i).offset + typesize;
        }
      }
    }
    return lastoffset;
  }

  int NumStreams() const
  {
    int numstreams = -1;
    for(int i = 0; i < desc.Size(); i++)
    {
      if( desc(i).stream > numstreams )
      {
        numstreams = desc(i).stream;
      }
    }
    return numstreams+1;
  }

  lVertexDesc PackStreams()
  {
    lVertexDesc output;

    for(int i = 0; i < desc.Size(); i++)
    {
      output.RegNext( 0, desc(i).type, desc(i).usage, desc(i).usageIndex );
    }

    return output;
  }

  void *GetPtr( void **streams, int descIdx, int &stride ) const
  {
    lAssert( descIdx != -1 );
    uint8 *stream = (uint8 *)streams[ desc(descIdx).stream ];
    stride = Stride( desc(descIdx).stream );
    return (void *)(stream + desc(descIdx).offset);
  }

  template<typename T>
  lMeshVertexDataAccessor<T> GetAccessor( void **streams, int descIdx ) const
  {
    lAssert( descIdx != -1 );
    uint8 *stream = (uint8*)streams[ desc(descIdx).stream ];
    return lMeshVertexDataAccessor<T>( (uint8 *)(stream + desc(descIdx).offset), Stride( desc(descIdx).stream ) );
  }

  lrVertexDesc CreateRenderDesc()
  {
    lrVertexDesc renderDesc;
    CreateRenderDesc( renderDesc );
    return renderDesc;
  }

  void CreateRenderDesc( lrVertexDesc  &renderDesc )
  {
    renderDesc.Empty();
    for(int i = 0; i < desc.Size(); i++)
    {
      renderDesc.Reg( desc(i).stream, desc(i).offset, ToRenderType( desc(i).type ), ToRenderUsage( desc(i).usage ), desc(i).usageIndex );
    }
  }
};

bool LMESH_API ConvertDesc( lVertexDesc &srcDesc, lVertexDesc &targetDesc, void **srcStreams, void **dstStreams, int numVerts, int flags );
//  create the other functions + Accessors from these

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lMeshSurface class
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LMESH_API lMeshSurface
{
 public:
  lMeshSurface() : m_flags(0), m_indexData(NULL), m_numVerts(0), m_numIndices(0), m_materialIdx(0xffffffff), m_cullMode(LR_CULL_NONE)
  {}

 //private:

  enum
  {
    MESHSURFACE_FLAG_TRILIST             = 0x0001,
    MESHSURFACE_FLAG_TRISTRIP            = 0x0002,
    MESHSURFACE_FLAG_TRIFAN              = 0x0004,
    MESHSURFACE_FLAG_POLYLIST            = 0x0008,

    MESHSURFACE_FLAG_16BIT_INDICES       = 0x0010,
    MESHSURFACE_FLAG_32BIT_INDICES       = 0x0020,

    MESHSURFACE_FLAG_USEROWNSVERTEXMEM   = 0x0040,
    MESHSURFACE_FLAG_USEROWNSINDEXMEM    = 0x0080,
  };

  //##
  // Split this surface into multiple surfaces with the same properties but with
  //  16 bit indices
  int SplitTriList( lArray<lMeshSurface> &array );

  inline uint32 GetNumTris() const
  {
    if( m_flags & MESHSURFACE_FLAG_TRILIST )
      return (m_numIndices / 3);
    if( m_flags & MESHSURFACE_FLAG_TRISTRIP )
      return (m_numIndices - 2);
    if( m_flags & MESHSURFACE_FLAG_TRIFAN )
      return (m_numIndices - 2);

    return 0;
  }

  uint32 GetIndexDataSize() const
  {
    return m_numIndices * ((m_flags & MESHSURFACE_FLAG_32BIT_INDICES) ? 4 : 2);
  }

  void Free()
  {
    // if user doesnt own the data stored in this surface, free it
    if( !(m_flags & lMeshSurface::MESHSURFACE_FLAG_USEROWNSVERTEXMEM) )
    {
      for(int i = 0; i < m_vertexData.Size(); i++)
      {
        lFree( m_vertexData(i) );
      }
      m_vertexData.Empty();
    }

    // if user doesnt own the data stored in this surface, free it
    if( !(m_flags & lMeshSurface::MESHSURFACE_FLAG_USEROWNSINDEXMEM) )
    {
      lFree( m_indexData );
      m_indexData = NULL;
    }

    m_numVerts = 0;
    m_numIndices = 0;
    m_flags = 0;
    m_materialIdx = 0;
    m_desc.Empty();
    m_renderDesc.Empty();
  }


  unsigned int    m_flags;
  lArray<void *>  m_vertexData;           // TODO: strides? ( could pack after or need more pointers
  void          * m_indexData;

  int             m_numVerts;
  int             m_numIndices;

  //lArray< lrVertexStream<VT,sizeof(VT)> >  vertList;
  //lrIndexStream<unsigned short, 2>       triList;

  lrVertexDesc    m_renderDesc;
  lVertexDesc     m_desc;                 // desc could be better off in surface, might provide for optimisation in some cases (especially
                                        // if we don't store the full desc here?)

  unsigned int  m_materialIdx;

  lrCullMode m_cullMode;
};

struct lMeshIndexAccessor
{
  lMeshIndexAccessor( lMeshSurface &surf )
  {
    m_ptr = (uint8*)surf.m_indexData;
    m_numIndices = surf.m_numIndices;
    m_b32Bit = (surf.m_flags & lMeshSurface::MESHSURFACE_FLAG_32BIT_INDICES) ? true : false;
    m_type   = surf.m_flags & ( lMeshSurface::MESHSURFACE_FLAG_TRILIST   |
                                lMeshSurface::MESHSURFACE_FLAG_TRISTRIP  |
                                lMeshSurface::MESHSURFACE_FLAG_TRIFAN );

    lAssert( m_type != 0 );             // should have a valid type
  }

  lMeshIndexAccessor( void *ptr, uint32 numIndices, uint32 surfFlags ) : m_ptr((uint8*)ptr), m_numIndices(numIndices)
  {
    m_b32Bit = (surfFlags & lMeshSurface::MESHSURFACE_FLAG_32BIT_INDICES) ? true : false;
    m_type   = surfFlags & ( lMeshSurface::MESHSURFACE_FLAG_TRILIST   |
                             lMeshSurface::MESHSURFACE_FLAG_TRISTRIP  |
                             lMeshSurface::MESHSURFACE_FLAG_TRIFAN );

    lAssert( m_type != 0 );             // should have a valid type
  }

  inline uint32 GetNumTris()
  {
    switch( m_type )
    {
      case lMeshSurface::MESHSURFACE_FLAG_TRILIST:  return (m_numIndices / 3);
      case lMeshSurface::MESHSURFACE_FLAG_TRISTRIP: return (m_numIndices - 2);
      case lMeshSurface::MESHSURFACE_FLAG_TRIFAN:   return (m_numIndices - 2);
      default:                               return 0;
    }

    return 0;
  }

  inline void GetTri( uint32 triIdx, uint32 &idx1, uint32 &idx2, uint32 &idx3 )
  {
    switch( m_type )
    {
      case lMeshSurface::MESHSURFACE_FLAG_TRILIST:
      {
        idx1 = GetIndex( triIdx * 3 + 0 );
        idx2 = GetIndex( triIdx * 3 + 1 );
        idx3 = GetIndex( triIdx * 3 + 2 );
        return;
      }

      case lMeshSurface::MESHSURFACE_FLAG_TRISTRIP:
      {
        //TODO: orientation ?
        idx1 = GetIndex( triIdx );
        idx2 = GetIndex( triIdx+1 );
        idx3 = GetIndex( triIdx+2 );
        return;
      }

      case lMeshSurface::MESHSURFACE_FLAG_TRIFAN:
      {
        idx1 = GetIndex( triIdx+1 );
        idx2 = GetIndex( triIdx+2 );
        idx3 = GetIndex( 0 );
        return;
      }
      default:                               return;
    }
  }

  inline uint32 GetIndex( uint32 idx )
  {
    if( m_b32Bit )
    {
      uint32 *ptr = ((uint32*)m_ptr) + idx;
      return *(ptr);
    }

    uint16 *ptr = ((uint16*)m_ptr) + idx;
    uint16 val = *(ptr);
    return (uint32)val;
  }

  inline void SetIndex( uint32 idx, uint32 val )
  {
    if( m_b32Bit )
    {
      uint32 *ptr = (uint32*)m_ptr;
      *(ptr + idx) = val;
      return;
    }

    uint16 *ptr = (uint16*)m_ptr;
    *(ptr + idx) = (uint16)val;
  }

  uint8  *m_ptr;
  bool    m_b32Bit;
  uint32  m_type;
  uint32  m_numIndices;
};

struct lMeshIndexArray
{
  lMeshIndexArray( bool b32Bit ) : m_b32Bit(b32Bit)
  {
  }

  inline uint32 GetIndex( uint32 idx )
  {
    if( m_b32Bit )
    {
      uint32 *ptr = (uint32*)m_data.Data();
      return *(ptr + idx);
    }

    uint16 *ptr = (uint16*)m_data.Data();
    return (uint32)*(ptr + idx);
  }


  inline void PushIndex( uint32 val )
  {
    if( m_b32Bit )
    {
      m_data.Add( 4 );
      uint32 *ptr = (uint32*)(m_data.Data() + (m_data.Size()-4));
      *ptr = val;
      return;
    }

    m_data.Add( 2 );
    uint16 *ptr = (uint16*)(m_data.Data() + (m_data.Size()-2));
    *ptr = (uint16)val;
  }

  inline void RemoveLastIndex()
  {
    if( m_b32Bit )
    {
      lLogFmt("Remove 4 - %d\n", m_data.Size());
      m_data.Remove( m_data.Size() - 4, 4 );
      return;
    }

    lLogFmt("Remove 2 - %d\n", m_data.Size());
    m_data.Remove( m_data.Size() - 2, 2 );
  }

//  void SetIndex( uint32 idx, uint32 val )
//  {
//    if( m_b32Bit )
//    {
//      uint32 *ptr = (uint32*)m_ptr;
//      *(ptr + idx) = val;
//      return;
//    }
//
//    uint16 *ptr = (uint16*)m_ptr;
//    *(ptr + idx) = (uint16)val;
//  }

  bool          m_b32Bit;
  lArray<uint8> m_data;
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lMesh
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//##
//  Mesh storage and manipulation class
class LMESH_API lMesh
{
 public:
  // bool ConvertDesc( lVertexDesc &targetDesc );

  enum
  {
    MESH_GEOM_FLAG_COPYMESHDATA,
  };

  // lMeshGeom * GenerateGeom( int method, int flags );

  // file load / save / conversion

  enum
  {
    MESH_FORMAT_3DS,
  };

  enum
  {
    MESH_LOAD_FLAGS_BONES = 0x000001,
  };

  //##
  // Check all the data is valid
  // bool IsValid() - good idea for classes with constructors?

  //##
  // Wipe all data in the structure so we have a blank lMesh to work with
  void Reset();

  ////##
  ////
  bool LoadSurfaceFromMemory( uint32 surfIdx, void **vertexData, uint32 numVerts, lVertexDesc &vertDesc,
                              void *indexData, uint32 numIndices,
                              uint32 surfFlags, uint32 materialIdx );

  //##
  // Load from all sorts of formats (COLLADA, IFF, X, 3DS, OBJ, PLY)
  bool LoadMesh( const char *fileName, int format, int flags, const lmMatrix4 applyMat = lmMatrix4::GetIdentity() );
  bool LoadPLY( const char *fileName );

  //##
  // Save to (C++ file, COLLADA, IFF, X etc)
  bool SaveMesh( const char *fileName, int format, int flags );

  lMesh *Copy();
  void Normalize( bool bCreateNormals = false );
  void Scale( lmVector3 &scale );

  void CalculateTangents( bool bBinormal = false );

  // tri stripping methods
  enum
  {
    MESH_STRIP_NVTRISTRIP,
    MESH_STRIP_XBSTRIP,
    MESH_STRIP_DXSTRIP,
    MESH_STRIP_STRIPS,
  };

  bool BuildTriStrips( int mode, int cacheSize );
  bool RemoveDegen();
  bool ConvertToTriList();
  float TestVertexCache( int cacheSize, int vertSize = 0, bool printMe = false, bool silent = false, float *pdegenpct = NULL );

  // render

  enum
  {
    MESH_RENDER_FLAG_WIREFRAME = 0x0000001,
  };

  struct RenderParams
  {
    RenderParams( int _flags ) : flags(_flags)
    {
    }

    int flags;
    // bones
    // material
  };

  static RenderParams &DefaultRenderParams()
  {
    static RenderParams params( 0 );
    return params;
  }

  void Render( RenderParams &params, int lights );

  //
  unsigned int          m_flags;

  lArray<lMeshSurface>   m_surfaces;
};


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lMeshGen class - allows many varied ways to create a mesh
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//##
//  Mesh generation helper class
class LMESH_API lMeshGen
{
 public:
  lMeshGen( int mode = MODE_NONE );
  ~lMeshGen();

  // begin / end mesh generation + elements

  void Begin( int mode );
  void End( lMesh *mesh, int outputMode = MODE_TRIANGLE_LIST, int flags = 0 );
      // flags -
      //    merge with existing surfaces?
      //    possibly an option to not change the vertex desc?

  void BeginElement();                  // element able to take a geom type and we convert on the fly to base mode?
  void EndElement();

  // data input
  // Vertex calls increment the vertex count, all data between this and next Vertex call specifies a vertex

  void Vertex();

  void Vertex1( float x );

  void Vertex2( const lmVector2 & v );
  void Vertex2( float x, float y );

  void Vertex3( const lmVector3 & v );
  void Vertex3( float x, float y, float z );

  void Vertex4( const lmVector4 & v );
  void Vertex4( float x, float y, float z, float w );

  // colours

  void Colour( int idx, float r, float g, float b, float a = 1.0f );
  void Colour( int idx, const lmVector4 & );
  void Colour( int idx, unsigned int );

  // TODO: ColourF to specify it staying as floats? - should this be a flag?

  // normals

  void Normal2( int idx, const lmVector2 & v );
  void Normal2( int idx, float x, float y );

  void Normal3( int idx, const lmVector3 & v );
  void Normal3( int idx, float x, float y, float z );

  void Normal4( int idx, const lmVector4 & v );
  void Normal4( int idx, float x, float y, float z, float w );

  // tex coords

  void TexCoord1( int idx, float );

  void TexCoord2( int idx, const lmVector2 & );
  void TexCoord2( int idx, float, float );

  void TexCoord3( int idx, const lmVector3 & );
  void TexCoord3( int idx, float, float, float );

  void TexCoord4( int idx, const lmVector4 & );
  void TexCoord4( int idx, float, float, float, float );

  // general specifier

  void Data( int usageIndex, int usage, int type, void *data, int size );

  // set defaults

  bool SetDefault( unsigned int usageIndex, unsigned int usage, unsigned int type, void *data, int size );

  // specify indices

  //##
  // Specify indices of a single poly
  void PolyIndices( const lArray<unsigned int> &indexArray );

  // modes

  enum
  {
    MODE_NONE       = 0x000,
    MODE_POINT,
    MODE_LINE_STRIP,
    MODE_LINE_LOOP,
    MODE_TRIANGLE_FAN,
    MODE_TRIANGLE_STRIP,
    MODE_TRIANGLE_LIST,
    MODE_QUAD,
    MODE_QUAD_STRIP,
    MODE_POLYGON,                       // element based (only special case here?)

    MODE_INDICES    = 0x000100,         // will specify the indices seperately from the order of the verts
  };

 private:
  void WriteData( unsigned int usage, unsigned int usageIdx, unsigned int type, void * data );
  void VertStart();
  void VertEnd();

  int                     m_mode;
  lVertexDesc             m_desc;
  lArray< lArray<char> >  m_streams;
  lArray< bool >          m_writtenStream;  //  TODO: bit array would work well
  int                     m_numVerts;

  struct streamDefault
  {
    streamDefault() : size(0) {}

    int   size;
    char  data[16];
  };

  lArray<streamDefault>   m_defaults;

  bool                    m_elementOpen;
  lArray< unsigned int >  m_indices;
};


////°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//// lMeshGenGeom class - create a mesh from geometry types
////
////°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//class LMESH_API lMeshGenGeom : public lMeshGen
//{
// public:
//  lMeshGenGeom( int mode = MODE_NONE );
//  ~lMeshGenGeom();
//
//  // geom functions
//
//  void Sphere( float radius, int nParallels, int nMeridians );
//  void HalfSphere( float radius, int nParallels, int nMeridians );
//  void Cylinder( float radius, int nParallels, int nMeridians );
//
// private:
//  lMeshGen                m_gen;
//};


/*
MeshGenerator meshGen1;

meshGen.Begin( MESH_POLYGON | MESH_INDICES );

 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );
 meshGen.Vertex( 1, 1, 1 );

 meshGen.Select();

 meshGen.PolyIndices( 0, 1, 2, 3 );     // select verts 0, 1, 2, 3 as a polygon
 meshGen.TriIndices( 5, 6, 7 );     // select verts 5, 6, 7 as a triangle

 meshGen.Colour( 0, 0xff00ff00 );       // change colours of all selected as specified

 meshGen.Deselect();                    // deselect and continue as normal

mesh.End();

//

mesh.Material( 2 );

mesh.BeginBoolean( MESH_BOOLEAN_UNION );

  mesh.BooleanABegin();
    mesh.Sphere( 20, 10, 20, 100.0f );
  mesh.BooleanABegin();

  mesh.BooleanBBegin();
    mesh.Box( 2, 2, 2, 20.0f );
    mesh.Cylinder( 0, 0, 0, 10, 20 );
  mesh.BooleanBBegin();

mesh.EndBoolean();

mesh.Cone( 5, 10 );
mesh.Torus( 9, 10 );

//

mesh.Material( 3 );
mesh.BeginGroup( "Two Spheres" );
  mesh.TexGenBegin( MESH_TEXGEN_SPHERE );
    mesh.Sphere( 20, 10, 20, 100.0f );
  mesh.TexGenEnd();

  mesh.Sphere( 20, 10, 20, 100.0f );
mesh.EndGroup();

//

mesh.Material( 4 );
mesh.VertListInput( vertices, sizeof( vertices[0] ), sizeof( vertices ) / sizeof( vertices[0] ), verticesDescription, flags (KEEP_VERT_DESC | USE_NEW) );

mesh.VertIndicesInput( vertices, sizeof( vertices[0] ), sizeof( vertices ) / sizeof( vertices[0] ), verticesDescription,
                       indices, sizeof( indices[0] ), sizeof( indices ) / sizeof( indices[0] ), MESH_INDICES_TRISTRIP );

//

mesh.SelectMaterial( 0 );
mesh.SelectMaterial( 3 );
mesh.SelectGroup( "Two Spheres" );

mesh.GenerateTangents();

mesh.Deselect();

mesh.SelectMaterial( 0 );
mesh.Normalize();
mesh.Deselect();

//

mesh.RemoveDuplicates( 0.005f );

//

mesh.ReduceDetail( 0.5f );
mesh.Tesselate();

//

mesh.BeginGroup( "Submesh" );
mesh.LoadMesh( "test.3ds", scale, rotate, translate, MESH_LOAD_BONES );
mesh.EndGroup();

//

mesh.MarchingCube( scalarfield, 30, 30, 30, 2.0f );

//

mesh.Knot();
mesh.Teapot();

mesh.GeneratePRTData();
mesh.Optimise( MESH_TRISTRIP );

mesh.ConvertToProgressiveMesh();

meshMaterial mat = mesh.Material( 1 );

  mat.Texture( "main.tga" );
  mat.Diffuse( 0.5, 0.5, 0.5, 1 );

//

mesh.Bone( skeleton, MESH_BONE_FLAGS );

//

mesh.GenerateFromHeightmap( hmapData, 1.0f );


  */


END_L_NAMESPACE

#endif
