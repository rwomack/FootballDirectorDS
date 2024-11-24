//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lscene.h
// Description : Scene include file
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 TODO
 ----

 Enum types - exporting the names to UI or whatever

 Check other model code

*/

#include "./lsceneapi.h"

#ifndef LSCENE_H_INCLUDED
#define LSCENE_H_INCLUDED

#include "../object/lobject.h"
#include "../containers/lstring.h"
#include "../math/lmaabox.h"
#include "../render/lrenderhelp.h"   // -- TODO: just for lrColour.
                                     // should change it?

BEGIN_L_NAMESPACE

class lOldArchive;


//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lSceneTraceHit result of a single trace operation
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

class LSCENE_API lSceneNode;

struct lSceneTraceHit
{
  lmVector3    hit;
  lmVector3    normal;
  lSceneNode * node;
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneNode - Base abstract node for our scene class
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LSCENE_API lSceneNode : public lAbstractRTTI<lSceneNode,lObject, lCLSID<0x8A965EF8,0xE486,0x4BCE,0x8433C6C55DA0D330>, 3 >
{
 public:
  lSceneNode();
  lSceneNode(const lSceneNode &);
  virtual ~lSceneNode();
  lSceneNode & operator= ( const lSceneNode &other );

  LSCENE_API friend lStream& operator << ( lStream& ar, lSceneNode *& obj );

  virtual void Serialize( lOldArchive & ar );

  enum NotifyCode
  {
    NotifyBranchAttach    = 0x01,   // internal, when a branch is attached
    NotifyBranchDetach    = 0x02,   // internal, when a branch is detached
    NotifyMatrixChange    = 0x03,   // internal, when a node matrix changes
  };

  // lSceneNode interface

  virtual void               PostNotifyEvent( lSceneNode::NotifyCode, lSceneNode * );

  // lSceneNode implementation

  // Property access

  const char               * GetPropertyString(const char * key)  const;
  void                       SetPropertyString(const char * key, const char * value);
  void                       DeleteProperty(const char * key);
  bool                       PropertyExists(const char * key) const;

  // Hierarchy access

  const char               * GetName() const                      { return (const char *)m_name; }
  void                       SetName(const char *s)               { m_name = s; }

  bool                       IsHidden() const                     { return m_bHidden; }
  void                       Hide()                               { m_bHidden = true; }
  void                       UnHide()                             { m_bHidden = false; }
  void                       SetHidden( bool b )                  { m_bHidden = b; }

  unsigned int               GetFlags() const                     { return m_flags; }
  void                       SetFlags(unsigned int  f)            { m_flags = f; }

  lSceneNode               * GetParent() const                    { return m_parent; }
  unsigned int               NumChildren() const                  { return m_childList.Size(); }
  lSceneNode               * GetChild(unsigned int i) const       { return m_childList(i); }

  const lmMatrix4          & GetLocalMatrix() const               { return m_localMatrix; }
  //lmMatrix4                  GetLocalMatrix() const;
  void                       SetLocalMatrix( const lmMatrix4 &m);
  lmMatrix4                  GetWorldMatrix( lSceneNode * = NULL );
  void                       SetWorldMatrix( const lmMatrix4 &m, lSceneNode * = NULL );

  const lmMatrix4          & GetAnimMatrix() const { return m_animMatrix; }

  lSceneNode               * FindName( lString );
  lSceneNode               * FindClass(lClass *);
  lSceneNode               * FindNameClass(lString, lClass *);

  lSceneNode               * FindParentName(lString);
  lSceneNode               * FindParentClass(lClass *);
  lSceneNode               * FindParentNameClass(lString,lClass *);

  bool                       BranchAttach( lSceneNode * );
  bool                       BranchDetach( lSceneNode * );

  void                       FlattenBranches();
  void                       CreateUniqueNodeName( char * dest, int maxSize, const char * baseName );

  // lSceneNode interface

  virtual void               GetLocalBoundingBox(lmAABox3 &box) = 0;
  virtual bool               TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out ) = 0;
  virtual void               Render() = 0;
  virtual bool               IsRenderable() { return true; }

  virtual void               SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld);
  void                       SpaceConvert(const lmMatrix4 &oldToNew);

  // lSceneNode iterator

  class LSCENE_API Iterator
  {
  public:
    Iterator( lSceneNode * root ) : m_current(root), m_skip(false), m_delete(false) {}
    void operator++();
    void operator++(int)                  { ++(*this); }
    bool operator==(lSceneNode *n) const { return (m_current == n); }
    bool operator!=(lSceneNode *n) const { return (m_current != n); }
    operator lSceneNode*() const         { return m_current; }
    lSceneNode& operator*() const        { return *m_current; }
    lSceneNode* operator->() const       { return m_current; }
    lSceneNode* GetNodePtr() const       { return m_current; }
    void         SkipChildren()           { m_skip = true; }
    unsigned int Depth() const            { return m_stack.Size(); }
    void         DeleteThis()             { m_delete = true; }
  private:
    struct Tree { lSceneNode * node; int childPos; };
    lArray<Tree,TSceneArrayTrait>  m_stack;
    lSceneNode  * m_current;
    bool          m_skip;
    bool          m_delete;
  };

  // variables

  lString                         m_name;              // node name
  unsigned int                    m_flags;             // node flags
  bool                            m_bHidden;           // true if node is hidden
  lSceneNode                    * m_parent;            // node graph parent
  lArray<lSceneNode *,TSceneArrayTrait>            m_childList;         // node graph children

  lmMatrix4                       m_localMatrix;         // local transform relative to parent
  lmMatrix4                       m_worldMatrixTemp;     // world transform for node

  lmMatrix4                       m_animMatrix;          // world transform from animation

  struct Property
  {
    friend lStream& operator << ( lStream& ar, Property &p )
    {
      return ar << p.key << p.value;
    }

    lString key;
    lString value;
  };

  lArray<Property,TSceneArrayTrait>                m_propertyList;

  enum { FlagRecalcWorldMatrix = 0x01 };
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneNull - Null scene object
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LSCENE_API lSceneNull : public lRTTI<lSceneNull, lSceneNode, lCLSID<0xC0777EDB,0x75E4,0x4B6E,0xA75A212E478FE262> >
{
 public:

  // lSceneNode interface

  virtual void                   GetLocalBoundingBox(lmAABox3 &box)
  {
    box.Empty();
  }

  virtual bool                   TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out )
  {
    L_UNUSED_PARAM(origin);
    L_UNUSED_PARAM(normal);
    L_UNUSED_PARAM(out);
    return false;
  }

  virtual void                   Render()
  {}

  virtual bool                   IsRenderable()
  { return false; }
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneGroup - Group scene object
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LSCENE_API lSceneGroup : public lRTTI<lSceneGroup,lSceneNull, lCLSID<0xA04162EC,0x89D4,0x44AD,0xACB41F38DB068F99> >
{
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneDummy - Dummy scene object
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LSCENE_API lSceneDummy : public lRTTI<lSceneDummy,lSceneNull, lCLSID<0xE8B18735,0x231F,0x4F2A,0xB3A6AE981981A46A> >
{
 public:
  lSceneDummy();
  virtual ~lSceneDummy();

  void Serialize( lOldArchive & ar );

  // lSceneNode interface

  virtual void                   GetLocalBoundingBox(lmAABox3 &box);
  virtual bool                   TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out );
  virtual void                   Render();
  virtual bool                   IsRenderable()                { return false; }

  // variables
  lmAABox3  m_aabox;
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneBone - Bone scene object
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LSCENE_API lSceneBone : public lRTTI<lSceneBone,lSceneNull, lCLSID<0x611527BD,0xF114,0x42C3,0xA371E9377679C951> >
{
 public:
  lSceneBone();
  virtual ~lSceneBone();

  void Serialize( lOldArchive & ar );
  LSCENE_API friend lStream& operator << ( lStream& ar, lSceneBone *& obj );

  // lSceneNode interface

  virtual void                   GetLocalBoundingBox(lmAABox3 &box);
  virtual bool                   TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out );
  virtual void                   Render();
  virtual bool                   IsRenderable()                { return false; }

  // variables
  lmVector3      m_envelopeStart;
  lmVector3      m_envelopeEnd;
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneShape - Shape object
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LSCENE_API lSceneShape : public lRTTI<lSceneShape,lSceneNode, lCLSID<0x4BCA16BA,0xA5E8,0x4ED1,0xB6DD1CE0A886FE29> >
{
public:
  lSceneShape();
  virtual ~lSceneShape();

  void Serialize( lOldArchive & ar );

  // lSceneNode interface

  virtual void                   GetLocalBoundingBox(lmAABox3 &box);
  virtual bool                   TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out );
  virtual void                   Render();
  virtual bool                   IsRenderable()  { return false; }

  virtual void                   SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld);

  // lSceneShape implementation

  lArray <lmVector3,TSceneArrayTrait> vertList;
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneCamera - Camera object
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
struct lrCamera;

class LSCENE_API lSceneCamera : public lRTTI<lSceneCamera,lSceneNode, lCLSID<0xF06DCDF2,0x6755,0x4379,0xAAF0F1859DCD1E1E> >
{
public:
  lSceneCamera();
  virtual ~lSceneCamera();

  void Serialize( lOldArchive & ar );

  // lSceneNode interface

  virtual void                   GetLocalBoundingBox(lmAABox3 &box);
  virtual bool                   TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out );
  virtual void                   Render();
  virtual bool                   IsRenderable()  { return false; }
  virtual void                   SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld);


  // lSceneCamera implementation

  lmMatrix4 cameraTransform;

  bool  orthographicProjection;

  float fovDegrees;

  float frontPlaneEnv;
  float backPlaneEnv;

  float frontPlaneClip;
  float backPlaneClip;

  float targetDistance;

  // TODO: not currently set in MAX

  bool  depthOfField;
  bool  dofUseTargetDistance;
  float focalDistance;

  bool  motionBlur;

  lSceneNode          * targetNode;

  const lmMatrix4 GetCameraMatrix( const lmMatrix4 &mCam, float * fNear = NULL, float *fFar = NULL );
  void SetCamera( lrCamera &cam, lmMatrix4 &worldMatrix, float aspect );
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lSceneLight - Light object
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LSCENE_API lSceneLight : public lRTTI<lSceneLight,lSceneNode, lCLSID<0x6E2AE06D,0xB155,0x4C63,0xA6E2025135A704E9> >
{
public:
  lSceneLight();
  virtual ~lSceneLight();

  void Serialize( lOldArchive & ar );

  // lSceneNode interface

  virtual void                   GetLocalBoundingBox(lmAABox3 &box);
  virtual bool                   TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out );
  virtual void                   Render();
  virtual bool                   IsRenderable()  { return false; }
  virtual void                   SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld);

  // lSceneLight implementation
  const lmVector3                GetLightDirection( const lmMatrix4 &mLight ) const;

  enum Type
  {
    TypeSpot         = 1,
    TypeDirectional  = 2,
    TypeOmni         = 3,
    TypeAmbient      = 4
  };

  enum Shape
  {
    ShapeNone   = 0,
    ShapeRect   = 1,
    ShapeCircle = 2
  };

  enum Atten
  {
    AttenNone = 0,
    AttenNear = 1,
    AttenFar  = 2,
  };

  enum
  {
    DecayNone = 0,
    DecayInverse,
    DecayInverseSquare,
  };

  lmMatrix4                 lightTransform;

  bool                      on;
  int                       type;
  bool                      volumeLight;

  lrColour                  colour;
  float                     colourMul;

  //lTexture *              projMap;

  int                       decayType;
  float                     decayStart;

  float                     hotsize;
  float                     fallsize;
  int                       shape;
  float                     aspect;

  int                       attenFlags;
  float                     nearAttenStart;
  float                     nearAttenEnd;
  float                     farAttenStart;
  float                     farAttenEnd;

  enum ShadowType
  {
    ShadowOff = 0,
    ShadowMap,
    ShadowArea,
    ShadowRaytraced,
    ShadowRaytracedAdv,
  };

  int                       shadowType;
  lrColour                  shadowColour;  // not set in max currently
  float                     shadowDensity; // not set in max currently
  //lTexture *              shadowMap;
  bool                      affectDiffuse;
  bool                      affectSpecular;

  int                       shadowMapSize;
  bool                      shadow2Sided;

  //lArray<lSceneNode *,TSceneArrayTrait>    shadowInclude
  //lArray<lSceneNode *,TSceneArrayTrait>    shadowExclude
  lArray<lSceneNode *,TSceneArrayTrait>    excludeList;
};

END_L_NAMESPACE

#endif
