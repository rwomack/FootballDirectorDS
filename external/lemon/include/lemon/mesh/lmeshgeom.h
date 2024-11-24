//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : meshgeom.h
// Description :
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

/*
 Implementor : Barog
 Library     :
 Version     : 1.0

 Modification History
 --------------------

 26/03/2006 - First version                                       - Barog

 Notes
 -----

*/

#include "./lmesh.h" // for api

#include "../math/lmmatrix.h"

#include "../math/lmaabox.h"
#include "../math/lmcapsule.h"
#include "../math/lmplane.h"
#include "../math/lmray.h"
#include "../math/lmsphere.h"

#include "../math/lmgeom.h"
#include "../math/geom/lmspheregeom.h"
#include "../math/geom/lmboxgeom.h"
#include "../math/geom/lmcapsulegeom.h"
#include "../math/geom/lmplanegeom.h"
#include "../math/geom/lmraygeom.h"

#ifndef LMESHGEOM_H_INCLUDED
#define LMESHGEOM_H_INCLUDED

BEGIN_L_NAMESPACE

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Mesh information callback
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

//##
// Structure to return vertex information from callback
struct triangleData
{
  const lmVector3Packed *vertex[3];
};

typedef void  (*TriangleCallback)( unsigned int triangle_index, triangleData& triangle, void* user_data );


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Partitioning structure (AABBTree, OBBTree, Octree, BD-Tree etc)
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LMESH_API lMesh;
class LMESH_API MeshTree;

//##
// Tree building rules for generation
enum
{
  // Primitive split
  BUILDRULE_SPLIT_LARGEST_AXIS     = 0x0001,   //Split along the largest axis
  BUILDRULE_SPLIT_SPLATTER_POINTS  = 0x0002,   //Splatter primitive centers (QuickCD-style)
  BUILDRULE_SPLIT_BEST_AXIS        = 0x0004,   //Try largest axis, then second, then last
  BUILDRULE_SPLIT_BALANCED         = 0x0008,   //Try to keep a well-balanced tree
  BUILDRULE_SPLIT_FIFTY            = 0x0010,   //Arbitrary 50-50 split

  BUILDRULE_SPLIT_GEOM_CENTER      = 0x0020,   //Split at geometric center (else split in the middle)

  //
  BUILDRULE_SPLIT_FORCE_DWORD      = 0x7fffffff
};

//##
// Tree collide struct
enum
{
  MESHTREE_FLAG_TEMPORAL_COHERENCE          = 0x00001,                              // use temporal coherence
  MESHTREE_FLAG_CONTACT                     = 0x00002,                              // final contact status after a collision query
  MESHTREE_FLAG_TEMPORAL_HIT                = 0x00004,                              // there has been an early exit due to temporal coherence
  MESHTREE_FLAG_CONTACT_FOUND               = MESHTREE_FLAG_CONTACT,                        // contact found
  MESHTREE_FLAG_TEMPORAL_CONTACT            = MESHTREE_FLAG_TEMPORAL_HIT | MESHTREE_FLAG_CONTACT,   // temporal contact found
};

struct LMESH_API MeshTreeCollideResult
{
  MeshTreeCollideResult() : m_lastTree(NULL)
  {
    m_numBVChecks = 0;
    m_numPrimChecks = 0;
    m_numDumps = 0;
    m_flags = 0;
  }

  // Cache checks
  inline bool IsTemporalCoherenceEnabled() { return ((m_flags & MESHTREE_FLAG_TEMPORAL_COHERENCE) != 0); }
  inline bool GetContactStatus()     const { return ((m_flags & MESHTREE_FLAG_CONTACT           ) != 0); }
  inline bool IsValid( MeshTree *cur )
  {
    if(m_lastTree != cur)
    {
      m_lastTree = cur;
      return false;
    }

    return true;
  }

  //

  int            m_flags;

  // set by collide
  lArray<int>    m_triList;

  // not user defined
  unsigned int   m_numBVChecks;
  unsigned int   m_numPrimChecks;
  unsigned int   m_numDumps;

  // cache related
  MeshTree      *m_lastTree;
};

#define MESHCOLLLOG //lLogFmt

//TODO:
// At some point maybe all classes must implement all collision types
// and fallback based on the fact that the set returned doesn't have to be exact? (or maybe it returns a flag stating that it isnt exact)

//##
// Tree base class
class LMESH_API MeshTree
{
 public:
  MeshTree()
  {}

  virtual ~MeshTree()
  {}

  virtual void Serialize( lOldArchive & ar ) = 0;
  virtual void Render() = 0;

  // tree generation
  virtual bool Build( TriangleCallback triCallback, void *triCallbackUserData, unsigned int numTris, unsigned int buildRules ) = 0;
  virtual void SetCallback( TriangleCallback triCallback, void *triCallbackUserData ) = 0;

  // tree collision
  virtual bool Collide( const lmSphere3 &sphere,            MeshTreeCollideResult &result, lmMatrix4 *sphereMatrix = NULL,    lmMatrix4 *treeMatrix = NULL )
  { L_UNUSED_PARAM( sphere ); L_UNUSED_PARAM( result ); L_UNUSED_PARAM( sphereMatrix ); L_UNUSED_PARAM( treeMatrix ); return false; }

  virtual bool Collide( const lmAABox3 &box,                MeshTreeCollideResult &result, lmMatrix4 *boxMatrix = NULL,       lmMatrix4 *treeMatrix = NULL )
  { L_UNUSED_PARAM( box ); L_UNUSED_PARAM( result ); L_UNUSED_PARAM( boxMatrix ); L_UNUSED_PARAM( treeMatrix ); return false; }

  virtual bool Collide( const lmPlane3 &plane,              MeshTreeCollideResult &result, lmMatrix4 *planeMatrix = NULL,     lmMatrix4 *treeMatrix = NULL )
  { L_UNUSED_PARAM( plane ); L_UNUSED_PARAM( result ); L_UNUSED_PARAM( planeMatrix ); L_UNUSED_PARAM( treeMatrix ); return false; }

  virtual bool Collide( const lArray<lmPlane3> &planeList,  MeshTreeCollideResult &result, lmMatrix4 *planeMatrix = NULL,     lmMatrix4 *treeMatrix = NULL )  // Might not be supported by all?
  { L_UNUSED_PARAM( planeList ); L_UNUSED_PARAM( result ); L_UNUSED_PARAM( planeMatrix ); L_UNUSED_PARAM( treeMatrix ); return false; }

  virtual bool Collide( const lmRay3   &ray,                MeshTreeCollideResult &result, lmMatrix4 *rayMatrix = NULL,       lmMatrix4 *treeMatrix = NULL )
  { L_UNUSED_PARAM( ray ); L_UNUSED_PARAM( result ); L_UNUSED_PARAM( rayMatrix ); L_UNUSED_PARAM( treeMatrix ); return false; }

  #ifdef L_PLATFORM_WINDOWS
  virtual bool Collide( const lmCapsule  &capsule,          MeshTreeCollideResult &result, lmMatrix4 *capsuleMatrix = NULL,   lmMatrix4 *treeMatrix = NULL )
  { L_UNUSED_PARAM( capsule ); L_UNUSED_PARAM( result ); L_UNUSED_PARAM( capsuleMatrix ); L_UNUSED_PARAM( treeMatrix ); return false; }
  #endif

  // this may not be implemented for anything other than same tree versus same tree
  virtual bool Collide( const MeshTree *tree,               MeshTreeCollideResult &result, lmMatrix4 *paramtreeMatrix = NULL, lmMatrix4 *treeMatrix = NULL )
  { L_UNUSED_PARAM( tree ); L_UNUSED_PARAM( result ); L_UNUSED_PARAM( paramtreeMatrix ); L_UNUSED_PARAM( treeMatrix ); return false; }
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// MeshAABBTree
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

// Quantised aabbtree
class LMESH_API MeshAABBTree : public MeshTree
{
 public:
  MeshAABBTree();
  ~MeshAABBTree();

  virtual void Serialize( lOldArchive & ar );
  virtual void Render();

  // building function
  bool Build( TriangleCallback triCallback, void *triCallbackUserData, unsigned int numTris, unsigned int buildRules );
  void SetCallback( TriangleCallback triCallback, void *triCallbackUserData );

  bool Collide( const lmAABox3 &box,                MeshTreeCollideResult &result, lmMatrix4 *boxMatrix = NULL,       lmMatrix4 *treeMatrix = NULL );
  bool Collide( const lmSphere3 &sphere,            MeshTreeCollideResult &result, lmMatrix4 *sphereMatrix = NULL,    lmMatrix4 *treeMatrix = NULL );
  bool Collide( const lmPlane3 &plane,              MeshTreeCollideResult &result, lmMatrix4 *planeMatrix = NULL,     lmMatrix4 *treeMatrix = NULL );
  bool Collide( const lArray<lmPlane3> &planeList,  MeshTreeCollideResult &result, lmMatrix4 *planeMatrix = NULL,     lmMatrix4 *treeMatrix = NULL );
  bool Collide( const lmRay3   &ray,                MeshTreeCollideResult &result, lmMatrix4 *rayMatrix = NULL,       lmMatrix4 *treeMatrix = NULL );

#ifdef L_PLATFORM_WINDOWS
  bool Collide( const lmCapsule  &capsule,          MeshTreeCollideResult &result, lmMatrix4 *capsuleMatrix = NULL,   lmMatrix4 *treeMatrix = NULL );
#endif

  struct Node
  {
    int16    m_centre[3];  // Quantized center
    uint16   m_extents[3]; // Quantized extents

    uint32   m_posData;    // positive leaf
    uint32   m_negData;    // negative leaf

    inline bool   HasPosLeaf()                      const { return m_posData&1; }
    inline bool   HasNegLeaf()                      const { return m_negData&1; }
//    inline Node * GetPos( Node *relativeTo = NULL ) const { return (relativeTo + (Node *)(m_posData*sizeof(Node))); }
//    inline Node * GetNeg( Node *relativeTo = NULL ) const { return (relativeTo + (Node *)(m_negData*sizeof(Node))); }
    inline Node * GetPos()                          const { return (Node *)(m_posData); }
    inline Node * GetNeg()                          const { return (Node *)(m_negData); }
    inline uint32 GetPosPrimitive()                 const { return (m_posData>>1);   }
    inline uint32 GetNegPrimitive()                 const { return (m_negData>>1);   }
  };

 protected:
  TriangleCallback  m_callback;
  void            * m_callbackUserData;

  unsigned int      m_numNodes;
  Node            * m_nodes;

  lmVector3         m_centreCoeff;
  lmVector3         m_extentsCoeff;
};

struct LMESH_API AABBTreeTest
{
  AABBTreeTest( MeshTreeCollideResult *result, TriangleCallback cb, void *cbUserData ) :
              m_result(result), m_callback(cb), m_callbackUserData(cbUserData)
              {}

  MeshTreeCollideResult * m_result;

  TriangleCallback        m_callback;
  void                  * m_callbackUserData;

  lmVector3               m_centreCoeff;
  lmVector3               m_extentsCoeff;

  // Dump rest of nodes to collide test
  void _Dump( MeshAABBTree::Node *node );
};


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Mesh Geom
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

struct LMESH_API lMeshGeomCallbackStructSurface
{
  lMeshGeomCallbackStructSurface() : vertexData(NULL), vertexStride(0), indexData(NULL), indexStride(0), vertCount(0), triIndex(0), triCount(0)
  {}

  ~lMeshGeomCallbackStructSurface()
  {
    if( vertexData )
    {
      lFree(vertexData);
      vertexData = NULL;
    }
    if( indexData )
    {
      lFree(indexData);
      indexData = NULL;
    }
  }

  char *vertexData;
  int   vertexStride;
  char *indexData;
  int   indexStride;

  bool  reverseTri;

  int   vertCount;
  int   triIndex;
  int   triCount;

  friend lStream& operator<< ( lStream& ar, lMeshGeomCallbackStructSurface &css )
  {
    ar << css.vertexStride;
    ar << css.indexStride;
    ar << css.reverseTri;
    ar << css.vertCount;
    ar << css.triIndex;
    ar << css.triCount;

    if( ar.IsLoading() )
    {
      css.vertexData = (char *)lAlloc( css.vertexStride * css.vertCount );
      css.indexData  = (char *)lAlloc( sizeof(unsigned short) * (css.triCount*3) );
    }

    ar.Serialize( css.vertexData, css.vertexStride * css.vertCount );
    ar.Serialize( css.indexData,  sizeof(unsigned short) * (css.triCount*3) );

    return ar;
  }
};

#define LMESHGEOMCALLBACK_NUM 2000

struct LMESH_API lMeshGeomCallbackStruct
{
  int                            numsurfs;
  lMeshGeomCallbackStructSurface surfs[ LMESHGEOMCALLBACK_NUM ];

  bool                           ownMemory;

  friend lStream& operator<< ( lStream& ar, lMeshGeomCallbackStruct &cs )
  {
    ar << cs.numsurfs;

    for(int i = 0; i < cs.numsurfs; i++)
    {
      ar << cs.surfs[i];
    }

    ar << cs.ownMemory;

    if( ar.IsLoading() )
    {
      // we just allocated this ram so set ownMemory
      cs.ownMemory = true;
    }

    return ar;
  }
};

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lMeshGeom - sphere geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

template<unsigned int MAX_SIZE>
class CacheStaticMemArrayTrait
{
public:
  #if defined(L_PLATFORM_PS2) || defined(L_PLATFORM_PSP)
  uint8 m_data[MAX_SIZE] __attribute__ ((aligned(16)));
  #else
  uint8 m_data[MAX_SIZE];
  #endif

public:
  CacheStaticMemArrayTrait()
  {
  }

  ~CacheStaticMemArrayTrait()
  {
  }

  template<typename T> T * Reallocate(T * existingPtr, unsigned int newSize, unsigned int )
  {
    L_UNUSED_PARAM( existingPtr );
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


//##
//  Mesh geometry class
class LMESH_API lMeshGeom : public lRTTI<lMeshGeom,lmGeom, lCLSID<0x829AF573,0x6123,0x4C4F,0x851A0DB013EA3C63> >
{
  lMeshGeom(const lMeshGeom &);
  void operator= (const lMeshGeom &);
  friend class lmGeom;

  static void       meshCallback( unsigned int triangle_index, triangleData& triangle, void* user_data );
  static uint16  *  getTriIndex( unsigned int triangle_index, void* user_data );

  lMesh                   * m_mesh;
  lmAABox3                  m_aabox;
  lMeshGeomCallbackStruct * m_cbs;
  MeshTree                * m_tree;
  lmVector3                 m_offset;

  unsigned char           * m_useFlags;

public:
  static void Register();

  static void LogRay();
  static void LogBox();

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct mesh geom
  lMeshGeom( lMesh *msh = NULL );
  ~lMeshGeom();

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  virtual void Serialize( lOldArchive & ar );

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // lmGeom functions
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return type of this geom
  virtual int GetType()
  {
    return LMGEOM_TYPE_TRIMESH;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    if( bTightFit )
    {
      box = m_aabox;
    } else
    {
      //TODO
      box = m_aabox;
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  void Render( unsigned int colour = 0xffffffff );

  lMesh                  * GetMesh() { return m_mesh; }
  MeshTree               * GetTree();
  lMeshGeomCallbackStruct* GetCallback();
  void                     SetTreeCallback( MeshTree *tree, lMeshGeomCallbackStruct *cbs );
  void                     SetOffset( const lmVector3 &off );

  unsigned int             GetNumTris();
  void                     GetTri( unsigned int triIdx, lmVector3 &v0, lmVector3 &v1, lmVector3 &v2 );

  void                     Generate();

  // this builds the callback structure + a tree using the info passed in
  void BuildTreeCopy( const float * vertexList, int vertexStride, int vertCount,
                      const unsigned short * indexList, int triStride, int triCount,     // stride from 1 triangle to next and number of tri's
                      bool reverseTri, bool isLastSurf, lmMatrix4 *m = NULL );

  // this builds the callback structure + a tree using the info passed in
  void BuildTree( const float * vertexList, int vertexStride, int vertCount,
                  const unsigned short * indexList, int triStride, int triCount,     // stride from 1 triangle to next and number of tri's
                  bool reverseTri, bool isLastSurf );

  // this rebuilds the callback structure without building a new tree
  void BuildCallback( const float * vertexList, int vertexStride, int vertCount,
                      const unsigned short * indexList, int triStride, int triCount, // stride from 1 triangle to next and number of tri's
                      bool reverseTri, bool isLastSurf );

  // Fetches a triangle
  inline void FetchTriangle( int Index, const lmMatrix4 &matrix, lmVector3 Out[3] )
  {
    lAssert( m_cbs != NULL );

    triangleData td;
    meshCallback( Index, td, (void *)m_cbs );

    Out[0] = lmVector3(*td.vertex[0]) * matrix;
    Out[1] = lmVector3(*td.vertex[1]) * matrix;
    Out[2] = lmVector3(*td.vertex[2]) * matrix;
  }

  enum UseFlags
  {
    kEdge0 = 0x1,
    kEdge1 = 0x2,
    kEdge2 = 0x4,
    kVert0 = 0x8,
    kVert1 = 0x10,
    kVert2 = 0x20,

    kUseAll = 0xFF,
  };

  void Preprocess();

  struct CachedTri
  {
    lmVector4 edge1;
    lmVector4 edge2;
    lmVector4 origin;
  };

  struct Cache
  {
    lmMatrix4              treeMatrix;
    lmAABox3               cacheBox;

    //note: support a maximum of 256 cached tris
    lArray< lmTri<float>, CacheStaticMemArrayTrait<12288> > cacheTris;
    lArray< CachedTri, CacheStaticMemArrayTrait<12288> >    cacheTris4;

    lArray< int, CacheStaticMemArrayTrait<1024> >          cacheTrisIndex;
  };

  // generate list of tris from an aabox
  int  GenerateCache( Cache *cacheData, const lmMatrix4 &mTree, const lmAABox3 &cacheBox );

  // set list of tris to use for next collide call
  void SetCacheData( Cache *cacheData );

  Cache *m_curCacheData;

  // mesh colliders

  static int collideMeshSphere ( lmGeomContactSet &contactSet, lmGeom *o1, lmMatrix4 &m1, lmGeom *o2, lmMatrix4 &m2 );
  static int collideMeshBox   ( lmGeomContactSet &contactSet, lmGeom *o1, lmMatrix4 &m1, lmGeom *o2, lmMatrix4 &m2 );
  static int collideMeshPlane ( lmGeomContactSet &contactSet, lmGeom *o1, lmMatrix4 &m1, lmGeom *o2, lmMatrix4 &m2 );
  static int collideMeshRay ( lmGeomContactSet &contactSet, lmGeom *o1, lmMatrix4 &m1, lmGeom *o2, lmMatrix4 &m2 );

#ifdef L_PLATFORM_WINDOWS
  static int collideMeshCapsule ( lmGeomContactSet &contactSet, lmGeom *o1, lmMatrix4 &m1, lmGeom *o2, lmMatrix4 &m2 );
#endif
};

/*

// experimental version of aabbtreeoptimal
class LPHYSICS_API AABBTreeOptimal2
{
  struct Tri                            // 9 bytes
  {
    uint8         mVert1[3];
    uint8         mVert2[3];
    uint8         mVert3[3];
    //unsigned char mBitField;            // 3 bits for each edge
                                          // 3 bits for each vertex
                                          // 1 bit for double sided?
                                          // 1 bit for
  };

// bitflags

// we want to reduce the amount of tri's we use for the aabbtree, at the same time
// we might as well speed it up by eliminating the callback / cache thrash

  struct NodeGPG2
  {
    uint8         flags;
    uint8         extents[6];

    uint16         leftNodeTri;
    uint16         rightNodeTri;
  };

  // gpg storage - 10,000 tris = 10,000 * 11 bytes (node) = 110k
  //               10,000 tris * 9 bytes = 90k

  // storage for 10,000 tris - 10,000 * 20bytes (node) + 10,000 * 9byte (tri) = 290k

  // aabbtreeoptimal - triangle callback system.. 3 shorts as indices        = 6 bytes
  //                                              3 vector3s for positions   = 12 * 3 = 36bytes
  //                                                                         = 42 bytes per tri (though we do have vertex reuse)
  // stepping back.. lets just use indices we had to have..                        10,000 * 6 bytes (tri indices) = 60k
  //                 if we have to store vertex data for 1/10 (not a lot) of them  1,000 * 36 bytes (vert data)   = 36k
  //
  //

 // building utils

 public:
  AABBTreeOptimal2();
  ~AABBTreeOptimal2();

 protected:
  unsigned int  m_numNodes;
  Node         *m_nodes;

  unsigned int  m_numTris;
  Tri          *m_tris;

  lmVector3    m_centerCoeff;
  lmVector3    m_extentsCoeff;

  friend AABBTreeOptimal;
};
   */

END_L_NAMESPACE

#endif
