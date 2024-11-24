//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lscenemesh.h
// Description : Scene Mesh include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 TODO
 ----

 Materials could be used on multiple surfaces, at the moment we'll set up
 animated values for each of these materials. It should be options for each
 specific material.

 We can do this by having a list of all the materials used in the mesh class
 and list them in the RegisterChannels() from there.

 Morph

*/

#ifndef LSCENEMESH_H_INCLUDED
#define LSCENEMESH_H_INCLUDED

#include "./lsceneapi.h"
#include "../render/lrender.h"
#include "../mesh/lmesh.h"
#include "../math/lmsphericalharmonics.h"
#include "./lresourcemanager.h"
#include "../io/lfileiostream.h"

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// lSceneMesh - Mesh object
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Predeclared classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRenderEnvironment - Render environment
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API lAnimNode;
class LSCENE_API lSceneMeshSurface;

struct LSCENE_API lRenderEnvironment
{
  enum
  {
    LIGHTING           = 0x0000001,
    SH                 = 0x0000002,
    SPECULAR           = 0x0000004,
    SHADOWS            = 0x0000008,
    TEXTURES           = 0x0000010,
    WIREFRAME          = 0x0000020,
    MATERIALS          = 0x0000040,
    MESHES             = 0x0000080,
    RENDERBONES        = 0x0000100,
    SHOWSURFACES       = 0x0000200,
    SHOWPOSE           = 0x0000400,
    SHADOWDRAW         = 0x0000800,       //## In progres
    MATERIAL_AMBIENT   = 0x0001000,
    VERTEXCOLOURS      = 0x0002000,
    RENDERPOSEBONES    = 0x0004000,
    RENDERBONED        = 0x0008000,
    CREATESH           = 0x0010000,       //## Create SH representation for m_lightArray lights instead of using traditional lighting
    FOG                = 0x0020000,       //## Enable fog, fog parameters are set in other fields
    NO_ZWRITE          = 0x0040000,       //## Don't write Z to Z buffer
    NO_ZCHECK          = 0x0080000,       //## Don't check against Z buffer)
    PER_PIXEL_LIGHTING = 0x0100000,
    BASESHCONSTANTS    = 0x0200000,
    VERTEXANIM         = 0x0400000,       //## Use vertex animation - currently breaks the base vertex data

    ONLY_ALPHA         = 0x0800000,       //## Only render alpha
    ONLY_NONALPHA      = 0x1000000,       //## Only render non-alpha

  };

  lRenderEnvironment() : m_objectId(0), m_flags( MATERIALS | LIGHTING | SH | SPECULAR | SHADOWS | TEXTURES )
  {
    SetFog( FOG_TYPE_LINEAR, 0, 10000.0f, 1.0f, lrColour(0,0,0,0) );
    m_specialPath = 0;
    m_baseSHConstants = 0;
    m_vertexAnimTime = 0.0f;
  }

  lmSphericalHarmonics    m_baseSH;      // base SH environment (may be an environment map or a light set)
  lmVector4             * m_baseSHConstants;

  lmMatrix4               m_shadowMatrix;
  int                     m_objectId;

  int                     m_specialPath;  // special render path (ps2 only)

  lArray<lSceneMeshSurface *> m_surfaceOverride;

  enum
  {
    FOG_TYPE_NONE = 0,
    FOG_TYPE_LINEAR,                    //## uses m_fogStart and m_fogEnd, saturate((fogEnd - d) / (fogEnd - fogStart))
    FOG_TYPE_EXP,                       //## uses m_fogDensity, 1.f/exp(d * fogDensity)
    FOG_TYPE_EXP2,                      //## uses m_fogDensity, 1.f/exp(pow(d * fogDensity, 2))
  };

  int                     m_fogType;
  uint32                  m_fogColourPacked;
  lmVector4               m_fogParams;

  float                   m_vertexAnimTime;

  void SetFog( int type, float start, float end, float density, const lrColour &col )
  {
    m_fogType = type;

    if( type != FOG_TYPE_NONE )
    {
      float oorange = end-start;
      if( oorange != 0.0f ) oorange = 1.0f / oorange;

      #ifdef L_PLATFORM_PS2
      m_fogParams.Init( start, oorange, end, density );
      #else
      m_fogParams.Init( start, end, oorange, density );
      #endif
      m_fogColourPacked = lrPackedColourFromColour( col );
    }
  }


  lArray<lAnimNode *>   m_lightArray;  // lighting array
  unsigned int            m_flags;       // flags on what is enabled
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshVertex
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef L_HAS_PRAGMA_PACK
#pragma pack(push,1)
#endif

struct L_GCC_PACKED(1) LSCENE_API lSceneMeshVertex
{
  friend inline lStream& operator<< ( lStream& ar, lSceneMeshVertex &v )
  {
#ifdef L_PLATFORM_PSP
#define COLOURSWIZZLE( col ) ((unsigned int)(((col & 0xff000000) | ((int)(col&0xff)<<16) | ((int)((col>>8)&0xff)<<8) | (int)((col>>16)&0xff) )))

    if( ar.IsLoading() )
    {
      lrColour col;
      ar << v.p << v.n << col << v.u << v.v;
      v.colour = COLOURSWIZZLE( lrPackedColourFromColour( col ) );
    } else
    {
      lrColour col = lrColourFromPackedColour( COLOURSWIZZLE( v.colour ) );
      ar << v.p << v.n << col << v.u << v.v;
    }

#undef COLOURSWIZZLE
#elif L_PLATFORM_WII

#define COLOURSWIZZLE( col ) ((unsigned int)((((col>>16)&0xff)<<24) | ((int)((col>>8)&0xff)<<16) | ((int)((col&0xff)<<8)) | ((int)((col>>24)&0xff)) ))

    if( ar.IsLoading() )
    {
      lrColour col;
      ar << v.p << v.n << col << v.u << v.v;
      v.colour = COLOURSWIZZLE( lrPackedColourFromColour( col ) );

//      v.p.Log("p");
//      v.n.Log("n");
//      lLogFmt("Col: %08x\n", v.colour);
//      lLogFmt("UV:  %.3f, %.3f\n", v.u, v.v);

    } else
    {
      lrColour col = lrColourFromPackedColour( COLOURSWIZZLE( v.colour ) );
      ar << v.p << v.n << col << v.u << v.v;
    }
#undef COLOURSWIZZLE
#else
    ar << v.p << v.n << v.colour << v.u << v.v;

/*    lmVector3Packed original = v.p;
    uint32 croppedfloat;

    croppedfloat = (*(uint32*)&v.p(0)) & 0xffffff00;
    v.p(0) = (*(float*)&croppedfloat);

    croppedfloat = (*(uint32*)&v.p(1)) & 0xffffff00;
    v.p(1) = (*(float*)&croppedfloat);

    croppedfloat = (*(uint32*)&v.p(2)) & 0xffffff00;
    v.p(2) = (*(float*)&croppedfloat);

//    lLogFmt("\n%.5f->%.5f\n", original(0), v.p(0));
//    lLogFmt("%.5f->%.5f\n", original(1), v.p(1));
//    lLogFmt("%.5f->%.5f\n", original(2), v.p(2));
*/
#endif
    return ar;
  }

  static lrVertexDesc & GetDesc();
  static lrVertexDesc & GetOptimisedDesc();
  static lrVertexDesc & GetBonedDesc();
  static lVertexDesc & GetMeshDesc();

  static void MakeDesc( lrVertexDesc & );
  static void MakeMeshDesc( lVertexDesc & );

  void Transform(const lmMatrix4 &m)
  {
    p = lmVector3(p)*m;
    n = n.SubMul3(m);
  }

#ifdef L_PLATFORM_PSP
  float           u,v;
  unsigned int    colour;
  lmVector3Packed n;
  lmVector3Packed p;
#elif L_PLATFORM_WII
  lmVector3Packed p;
  lmVector3Packed n;
  unsigned int    colour;
  float           u,v;
#else
  lmVector3Packed p;
  lmVector3Packed n;
  lrColour        colour;                // r, g, b, a
  float           u,v;
#endif
};

#ifdef L_HAS_PRAGMA_PACK
#pragma pack(pop)
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshTexture - Material Texture
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API lSceneMeshTexture
{
public:
  lSceneMeshTexture();

  friend inline lStream & operator<<( lStream &ar, lSceneMeshTexture &tex )
  {
    ar << tex.m_fileName;
    ar << tex.m_influenceAmount;
    ar << tex.m_uvOffset;
    ar << tex.m_uvTiling;
    ar << tex.m_uvMirror[0];
    ar << tex.m_uvMirror[1];
    ar << tex.m_uvTile[0];
    ar << tex.m_uvTile[1];
    ar << tex.m_mappingType;
    ar << tex.m_alphaSource;
    return ar;
  }

  // variables
  lString       m_fileName;

  float         m_influenceAmount;

  lmVector2     m_uvOffset;
  lmVector2     m_uvTiling;
  bool          m_uvMirror[2];
  bool          m_uvTile[2];

  // mapping type, from texture coords, planar from object xyz, planar from world xyz

  // xy, yz, zx mapping ^

  enum
  {
    MAP_EXPLICIT,
    MAP_SPHERE,
    MAP_CYL,
    MAP_SHRINK,
    MAP_SCREEN,
    MAP_XY,
    MAP_YZ,
    MAP_ZX,
  };

  int            m_mappingType;

  // cropping / placement

  // alpha source
  enum
  {
    ALPHA_SOURCE_NONE,
    ALPHA_SOURCE_RGB,
    ALPHA_SOURCE_FILE,
  };

  int            m_alphaSource;

  RcPtr<lrTextureResource> m_texOverride; // don't use the file name, if this is set.

  // animation
  enum PlayMode
  {
    PLAY_ONCE,
    PLAY_ONCE_REVERSE,
    PLAY_LOOP,
    PLAY_LOOP_REVERSE,
  };

  bool        m_bAnimated;
  float       m_fFrameRate;
  int         m_numFrames;
  int         m_currentFrame;
  int         m_playMode;
  bool        m_bPlaying;
  float       m_fCurrentDelta;
  lArray< RcPtr<lrTextureResource>, TSceneArrayTrait > m_frameList;

  void Tick(float delta)
  {
    if( m_bAnimated )
    {
      if( m_bPlaying )
      {
        m_fCurrentDelta += delta;
        if( m_fCurrentDelta >= ( 1.0f / m_fFrameRate ) )
        {
          m_fCurrentDelta -= ( 1.0f / m_fFrameRate );

          if( m_playMode == PLAY_ONCE || m_playMode == PLAY_LOOP )
          {
            m_currentFrame++;
            if( m_currentFrame >= m_numFrames )
            {
              if( m_playMode == PLAY_LOOP )
              {
                m_currentFrame = 0;
              }
              else
              {
                Stop();
              }
            }
          }
          else
          {
            m_currentFrame--;
            if( m_currentFrame < 0 )
            {
              if( m_playMode == PLAY_LOOP_REVERSE )
              {
                m_currentFrame = m_numFrames-1;
              }
              else
              {
                Stop();
              }
            }
          }

          if( m_currentFrame >= m_frameList.Size() )
          {
            //Attempt to load next frame
            m_currentFrame = m_frameList.Size();
            lString frameName = m_fileName;
            lString frameNumber;
            frameNumber.Format("_%d",m_currentFrame);
            frameName += frameNumber;
            RcPtr<lrTextureResource> tex = lResourceManager::GetTexture( frameName );
            m_frameList.Push(tex);
          }

          //Load existing frame
          m_texOverride = m_frameList(m_currentFrame);
        }
      }
    }
  }

  void Play()
  {
    m_bPlaying = true;
  }

  void Pause()
  {
    m_bPlaying = false;
  }

  void Stop()
  {
    m_bPlaying = false;
    m_currentFrame = 0;
    m_texOverride = NULL;
  }

  void SetPlayMode( lSceneMeshTexture::PlayMode mode )
  {
    m_playMode = mode;
  }

  void SetFrameRate( float rate )
  {
    m_fFrameRate = rate;
  }

  void SetNumFrames( int num )
  {
    m_numFrames = num;
  }

  void PreConvert()
  {
    lResourceManager::PreConvertTexture( m_fileName );
  }

  void PreLoad()
  {
    //Load base texture
    m_texOverride = lResourceManager::GetTexture( m_fileName );

    //Load frames
    lString animDataFile = m_fileName;
    animDataFile += lString(".dat");
    if( lPlatform_FileExists(animDataFile) )
    {
      lFileIOStream file;
      if( file.Open( animDataFile, L_IO_ACCESS_READ ) )
      {
        uint8 data[2];
        if( file.Read( data, 2 ) == 2 )
        {
          m_bAnimated = true;
          m_numFrames = (int)data[0];
          m_fFrameRate = (float)data[1];
          m_frameList.Empty();
          for( int i = 0; i < m_numFrames; i++ )
          {
            lString frameName = m_fileName;
            lString frameNumber;
            frameNumber.Format("_%d",i);
            frameName += frameNumber;
            RcPtr<lrTextureResource> tex = lResourceManager::GetTexture( frameName );
            m_frameList.Push(tex);
          }
        }
        file.Close();
      }
    }
  }

};

// TODO : Do we want to support more than just bitmaps for our Maps

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshMaterial - Mesh Material
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API lSceneMeshMaterial
{
public:
  lSceneMeshMaterial();

  // TODO : need a compare function (and an operator==) which calls it

  friend inline lStream & operator<<( lStream &ar, lSceneMeshMaterial &mat )
  {
    ar << mat.m_name;
    ar << mat.m_ambientTexture;
    ar << mat.m_diffuseTexture;
    ar << mat.m_specularTexture;
    ar << mat.m_specularLevelTexture;
    ar << mat.m_glossinessTexture;
    ar << mat.m_selfilluminationTexture;
    ar << mat.m_opacityTexture;
    ar << mat.m_bumpTexture;
    ar << mat.m_reflectionTexture;
    ar << mat.m_refractionTexture;
    ar << mat.m_displacementTexture;
    ar << mat.m_diffuse;
    ar << mat.m_ambient;
    ar << mat.m_specular;
    ar << mat.m_specularExp;
    ar << mat.m_emissive;
    ar << mat.m_bWireframe;
    ar << mat.m_bTwoSided;
    ar << mat.m_transparency;


    return ar;
  }

  // variables
  lString             m_name;

  // textures

  lSceneMeshTexture   m_ambientTexture;
  lSceneMeshTexture   m_diffuseTexture;
  lSceneMeshTexture   m_specularTexture;
  lSceneMeshTexture   m_specularLevelTexture;
  lSceneMeshTexture   m_glossinessTexture;
  lSceneMeshTexture   m_selfilluminationTexture;
  lSceneMeshTexture   m_opacityTexture;
  lSceneMeshTexture   m_bumpTexture;
  lSceneMeshTexture   m_reflectionTexture;
  lSceneMeshTexture   m_refractionTexture;
  lSceneMeshTexture   m_displacementTexture;

  // colours

  lrColour            m_diffuse;
  lrColour            m_ambient;
  lrColour            m_specular;
  float               m_specularExp;

  lrColour            m_emissive;         // self illumination

  // alpha source

  bool                m_bWireframe;
  bool                m_bTwoSided;

  // transparency
  enum
  {
    TRANSPARENCY_NONE,
    TRANSPARENCY_ALPHA,
    TRANSPARENCY_ADDITIVE,
    TRANSPARENCY_SUBTRACTIVE
  };

  int                 m_transparency;

  bool                m_bNoMipMaps; // NOT SERIALIZED!
  bool                m_bNoAlphaTest; // NOT SERIALIZED!

  // glossiness
  // soften
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshBoneWeight - Mesh Bone Weight
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北


#ifdef L_HAS_PRAGMA_PACK
#pragma pack(push,1)
#endif

struct L_GCC_PACKED(1)  LSCENE_API lSceneMeshBoneWeight
{
  float         w[4];
  unsigned int  packedIndex;

  friend inline lStream & operator<<( lStream &ar, lSceneMeshBoneWeight &weight )
  {
    ar.Serialize( &weight, sizeof(lSceneMeshBoneWeight) );
    return ar;
  }
};

#ifdef L_HAS_PRAGMA_PACK
#pragma pack(pop)
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshBone - Mesh Bone
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LSCENE_API lSceneMeshBone
{
  lSceneNode *  node;
  lmMatrix4     poseMatrix;
  lmMatrix4     inversePoseMatrix;

  friend inline lStream & operator<<( lStream &ar, lSceneMeshBone &bone )
  {
    ar << bone.node;
    ar << bone.poseMatrix;
    ar << bone.inversePoseMatrix;
    return ar;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshVertexAnim - Vertex Animation data
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LSCENE_API lSceneMeshVertexAnimFrame
{
  float                   time;

  // Raw storage, each of these values corresponds to a new value for the same vertex index in the original surface
  // If the array is empty the values don't change
  lArray<lmVector3Packed> posFrame;
  lArray<lmVector2>       uvFrame;

  //lArray<lmVector3Packed> nrmFrame;
  //lArray<lmVector4Packed> colFrame;

  friend inline lStream & operator<<( lStream &ar, lSceneMeshVertexAnimFrame &vertexanim )
  {
    ar << vertexanim.time;
    ar << vertexanim.posFrame;
    ar << vertexanim.uvFrame;
    return ar;
  }
};

struct LSCENE_API lSceneMeshVertexAnim
{
  int                     fps;

  lArray<lSceneMeshVertexAnimFrame> frameList;

  friend inline lStream & operator<<( lStream &ar, lSceneMeshVertexAnim &vertexanim )
  {
    ar << vertexanim.fps;
    ar << vertexanim.frameList;
    return ar;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshSurface - Mesh Surface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API lSceneMeshSurface
{
public:

  enum
  {
    LMESH_SURFACE_TRILIST       = 0x00004,
    LMESH_SURFACE_TRISTRIP      = 0x00008,
    LMESH_SURFACE_PS2_GEOMETRY  = 0x00010,  //## PS2 specific surface data is stored in m_indices. m_vertices + m_indices are invalid from the PC perspective
    LMESH_SURFACE_PSP_GEOMETRY  = 0x00020,  //## PSP specific surface data is stored in m_indices. m_vertices + m_indices are invalid from the PC perspective
  };

  lSceneMeshSurface() : m_flags(0)
  {
    m_material = NULL;
    m_vertexAnim = NULL;
  }

  ~lSceneMeshSurface()
  {
    if( m_material )
    {
      delete m_material;
      m_material = NULL;
    }

    if( m_vertexAnim )
    {
      delete m_vertexAnim;
      m_vertexAnim = NULL;
    }
  }

  lSceneMeshSurface(const lSceneMeshSurface &rhs)
  {
    m_flags    = 0;
    m_material = NULL;
    m_vertexAnim  = NULL;
    *this = rhs;
  }

  lSceneMeshSurface &operator= (const lSceneMeshSurface &rhs)
  {
    if (&rhs != this)
    {
      if( m_material )
      {
        if( rhs.m_material )
        {
          *m_material = *rhs.m_material;
        }
        else
        {
          delete m_material;
          m_material = NULL;
        }
      }
      else
      {
        if( rhs.m_material )
        {
          m_material = new lSceneMeshMaterial;
          *m_material = *rhs.m_material;
        }
      }

      if( m_vertexAnim )
      {
        if( rhs.m_vertexAnim )
        {
          *m_vertexAnim = *rhs.m_vertexAnim;
        }
        else
        {
          delete m_vertexAnim;
          m_vertexAnim = NULL;
        }
      }
      else
      {
        if( rhs.m_vertexAnim )
        {
          m_vertexAnim = new lSceneMeshVertexAnim;
          *m_vertexAnim = *rhs.m_vertexAnim;
        }
      }


      m_flags              = rhs.m_flags;
      m_cullMode           = rhs.m_cullMode;
      m_vertices           = rhs.m_vertices;
      m_vertices_cacheID   = rhs.m_vertices_cacheID;
      m_indices            = rhs.m_indices;
      m_indices_cacheID    = rhs.m_indices_cacheID;
      m_boneWeightList     = rhs.m_boneWeightList;
      m_boneWeight_cacheID = rhs.m_boneWeight_cacheID;
      m_boneList           = rhs.m_boneList;

      Dirty();
    }

    return *this;
  }

  friend inline lStream& operator<< ( lStream& ar, lSceneMeshSurface &s )
  {
    ar << s.m_flags;

    ar << s.m_cullMode;
    ar << s.m_vertices;
    ar << s.m_indices;

    ar << s.m_boneWeightList;
    ar << s.m_boneList;

    if( ar.IsLoading() )
    {
      bool bmat;
      ar << bmat;
      if( bmat )
      {
        s.m_material = new lSceneMeshMaterial;
        ar << *s.m_material;
      } else
      {
        s.m_material = NULL;
      }

      bool vanm;
      ar << vanm;
      if( vanm )
      {
        s.m_vertexAnim = new lSceneMeshVertexAnim;
        ar << *s.m_vertexAnim;
      } else
      {
        s.m_vertexAnim = NULL;
      }
    } else
    {
      bool bmat = s.m_material != NULL;
      ar << bmat;
      if(bmat)
      {
        ar << *s.m_material;
      }

      bool vanm = s.m_vertexAnim != NULL;
      ar << vanm;
      if(vanm)
      {
        ar << *s.m_vertexAnim;
      }
    }

    return ar;
  }

  void                                                SetSurfaceFlags( unsigned int );
  unsigned int                                        GetSurfaceFlags() const;

//  typename TTRAIT::VertexStreamType                 & GetVertices();
//  const typename TTRAIT::VertexStreamType           & GetVertices() const;
//
//  typename TTRAIT::IndexStreamType                  & GetIndices();
//  const typename TTRAIT::IndexStreamType            & GetIndices() const;


//protected:
  inline void                  Dirty()
  {
#ifndef L_PLATFORM_XBOX
    m_vertices_cacheID.Flush();
    m_indices_cacheID.Flush();
#endif
  }

  unsigned int                                      m_flags;

  lrCullMode                                        m_cullMode;

  lArray< lSceneMeshVertex,TSceneArrayTrait >       m_vertices;
  lrCacheId                                         m_vertices_cacheID;

  lArray< unsigned short,TSceneArrayTrait >         m_indices;  // int
  lrCacheId                                         m_indices_cacheID;

  lArray< lSceneMeshBoneWeight,TSceneArrayTrait >   m_boneWeightList;
  lrCacheId                                         m_boneWeight_cacheID;

  lArray< lSceneMeshBone,TSceneArrayTrait >         m_boneList;

  lSceneMeshVertexAnim                            * m_vertexAnim;
  lSceneMeshMaterial                              * m_material;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMeshMorph - Mesh Morph
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API lSceneMeshMorph
{
public:
  lSceneMeshMorph() : m_name(""), m_minLimit(0.0f), m_maxLimit(0.0f)
  {
  }

  // variables

  struct Vertex
  {
    int        surfaceIndex;
    int        vertexIndex;
    lmVector3Packed  delta;
    friend lStream& operator<< ( lStream& ar, Vertex &v ) { return ar << v.surfaceIndex << v.vertexIndex << v.delta; }
  };

  lString               m_name;          // Morph name
  float                 m_minLimit;      // minimum and maximum % effect this morph is allowed to have.
  float                 m_maxLimit;      // minimum and maximum % effect this morph is allowed to have.
  float                 m_curPct;        // current % effect this morph has on Model (should save per frame for anims ?)
  lArray<Vertex,TSceneArrayTrait>        m_vertDeltaList; // morph delta for vertices

  friend lStream& operator<< ( lStream& ar, lSceneMeshMorph &m ) { return ar << m.m_name << m.m_minLimit << m.m_maxLimit << m.m_curPct << m.m_vertDeltaList; }
//  void MapVertices(const ModelVertexMapping &mapping);
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSceneMesh - Mesh Object
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// This is the scene mesh class.
class LSCENE_API lSceneMesh : public lRTTI<lSceneMesh,lSceneNode, lCLSID<0x888CE99E,0x3A80,0x47D6,0xACFED71F83DEFD79> >
{
public:
  lSceneMesh();
  virtual ~lSceneMesh();

  void Serialize( lOldArchive & ar );

  // lSceneNode interface

  virtual void                   GetLocalBoundingBox(lmAABox3 &box);
  virtual bool                   TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out );
  virtual void                   Render();
  virtual bool                   IsRenderable()  { return false; }
  virtual void                   SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld);

  //##
  // Render the lSceneMesh with the rendering options given in env which is a lRenderEnvironment.
  //
  // In:
  //  env - lRenderEnvironment which specifies options for rendering
  void                           RenderOptions( lRenderEnvironment &env );

  //##
  // Converts this lSceneMesh into an lMesh
  //
  // Out:
  //   Returns a pointer to an lMesh if successful, otherwise returns NULL.
  lMesh                        * ToMesh( lmMatrix4 *mat = NULL );
  void                           FromMesh( lMesh *, lmMatrix4 *mat = NULL );
  void                           FromMeshIndicesOnly( lMesh * );

  //##
  // Create PS2 specific rendering data in surfaces
  void                           OptimiseForPS2( int maxChunkSize );

  //##
  // Create PSP specific rendering data in surfaces
  void                           OptimiseForPSP();

#ifdef L_PLATFORM_PS2
  static void                    Log( float multiplier );
#endif

#ifdef L_PLATFORM_WII
  static void                    Log( float multiplier );
  static void                    LogClear();
#endif

#ifdef L_PLATFORM_PSP
  static void                    Log( float multiplier );
  static void                    LogClear();
#endif

#ifdef L_PLATFORM_WINDOWS
  static void                    SetVertexShaderVersion( const char * );
  static void                    SetPixelShaderVersion( const char * );
  static void                    PrecompileShaders();
#endif


  // lSceneMesh implementation

//private:
  bool                          m_aaboxDirty;
  lmAABox3                      m_aabox;

  lArray<lSceneMeshSurface,TSceneArrayTrait>     m_surfaceList;
  lArray<lSceneMeshMorph,TSceneArrayTrait>       m_morphList;
};

END_L_NAMESPACE

#endif
