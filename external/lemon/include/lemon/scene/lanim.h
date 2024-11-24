//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lanim.h
// Description : Animation include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 TODO
 ----

 Targetable animation (could be extra node in the way of a source)

 Need inputs supported - parse them like the animated values

*/

#ifndef LANIM_H_INCLUDED
#define LANIM_H_INCLUDED

#include "./lsceneapi.h"
#include "../containers/larray.h"
#include "../containers/lstring.h"

#include "../object/lobject.h"
#include "../object/loldarchive.h"
#include "../math/lmaabox.h"

#include "../math/lmquaternion.h"
#include "../math/lmdecomp.h"

BEGIN_L_NAMESPACE

class lSceneNode;
struct lSceneTraceHit;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAnimXform - Animation transformation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lAnimXform
{
public:
  inline lAnimXform() : translation( lmVector3( 0.0f, 0.0f, 0.0f ) ),
                           rotation( lmQuaternion( 0.0f, 0.0f, 0.0f, 1.0f ) ),
                              scale( lmVector3( 1.0f, 1.0f, 1.0f ) )
  {
  }

  inline lAnimXform(const lmMatrix4 &matrix)
  {
    lmAffineParts affine;
    lmDecompAffine( matrix, affine );

    translation = affine.t;
    rotation = affine.q;
    scale    = affine.k * affine.f;
  }

  inline void Identity()
  {
    translation.Zero();
    rotation.Identity();
    scale.Init( 1.0f, 1.0f, 1.0f );
  }

  inline void FromMatrixOrtho(const lmMatrix4 &matrix)
  {
    rotation.FromMatrix( matrix );
    translation = matrix.GetTranslate();
    scale.Init( 1.0f, 1.0f, 1.0f );
  }

  inline void ToMatrix(lmMatrix4 &matrix)
  {
    rotation.SetMatrix3x3( matrix );

    // blank rest of matrix including translation
    matrix(0,3) = 0.0f;
    matrix(1,3) = 0.0f;
    matrix(2,3) = 0.0f;
    matrix(3,0) = 0.0f;
    matrix(3,1) = 0.0f;
    matrix(3,2) = 0.0f;
    matrix(3,3) = 1.0f;

    // apply scale
    matrix.ApplyScale( scale );

    // add translation
    matrix(3,0) = translation(0);
    matrix(3,1) = translation(1);
    matrix(3,2) = translation(2);
  }

  inline void ScaleTranslation(float scale)
  {
    translation *= scale;
  }

  inline void NormalizeTranslation()
  {
    translation.Normalize();
  }

  inline static void Lerp(const lAnimXform &x0, const lAnimXform &x1, float t, lAnimXform &x)
  {
    x.translation = x0.translation  +  ( x1.translation - x0.translation ) * t;
    x.rotation    = lmQuaternion::Slerp( t, x0.rotation, x1.rotation );
    x.scale       = x0.scale  +  ( x1.scale - x0.scale ) * t;
  }

  inline bool operator==(const lAnimXform &x) const
  {
    return translation == x.translation  &&  rotation == x.rotation  &&  scale == x.scale;
  }

  inline bool operator!=(const lAnimXform &x) const
  {
    return translation != x.translation  ||  rotation != x.rotation  ||  scale != x.scale;
  }

  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s:\n", str );
    lLogFmt(" translation : %6.3f, %6.3f, %6.3f\n", translation(0), translation(1), translation(2) );
    lLogFmt(" rotation    : %6.3f, %6.3f, %6.3f, %6.3f\n", rotation(0), rotation(1), rotation(2), rotation(3) );
    lLogFmt(" scale       : %6.3f, %6.3f, %6.3f\n", scale(0), scale(1), scale(2) );
  }
  #endif

  // variables for this xform
  lmVector3     translation;
  lmQuaternion  rotation;
  lmVector3     scale;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAnimation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  LANIMATION_TYPE_UNKNOWN = 0,
  LANIMATION_TYPE_SIMPLE,
  LANIMATION_TYPE_SPLINED,
};

class LSCENE_API lAnimation
{
public:
  virtual ~lAnimation() {}

  virtual void          Serialize( lOldArchive & ar ) = 0;

  virtual int           GetType() = 0;
  virtual float         GetLength() = 0;                             //## returns the length in seconds
  virtual int           GetByteSize() = 0;                           //## returns the data size in bytes
  virtual void          GetMatrix( float t, lmMatrix4 &m ) = 0;      //## return the matrix at time 't' in seconds
  virtual void          GetXForm( float t, lAnimXform &xform ) = 0;  //## return the xform at time 't' in seconds
  virtual void          SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld) = 0;
  virtual void          PreMultiply( const lmMatrix4 &m ) = 0;
  virtual void          PostMultiply( const lmMatrix4 &m ) = 0;
  virtual lAnimation  * Clone() = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSimpleAnimation - Basic implementation of lAnimation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API lSimpleAnimation : public lAnimation
{
  struct Key
  {
    Key() : t(0), m(true) {}
    float     t;
    lmMatrix4 m;

    friend lStream& operator<< ( lStream& ar, Key &k )
    {
      ar << k.t << k.m;
      return ar;
    }
  };

  lArray<Key,TSceneArrayTrait> m_keyList;

public:
  lSimpleAnimation() {}

  lSimpleAnimation( const lSimpleAnimation &other )
  {
    m_keyList = other.m_keyList;
  }

  void Prepare( int size )
  {
    m_keyList.Empty( size );
  }

  lAnimation * Clone()
  {
    lSimpleAnimation * newAnim = new lSimpleAnimation;
    *newAnim = *this;
    return newAnim;
  }

  int GetType() { return LANIMATION_TYPE_SIMPLE; }

  Key & FindKey( float t )
  {
    for( int i=0;i<m_keyList.Size();i++ )
    {
      if( m_keyList(i).t == t )
      {
        return m_keyList(i);
      }

      bool bInsertKey = false;

      if( m_keyList(i).t > t )
      {
        bInsertKey = true;
      }

      if( bInsertKey )
      {
        m_keyList.Insert( i, 1 );
        m_keyList(i).t = t;
        return m_keyList(i);
      }
    }

    m_keyList.Add();
    m_keyList.Last().t = t;
    return m_keyList.Last();
  }

  void Log()
  {
    lLogFmt("%d keys\n", m_keyList.Size() );

    for( int i=0;i<m_keyList.Size();i++ )
    {
      lLogFmt("  %f: %f,%f,%f\n", m_keyList(i).t, m_keyList(i).m.GetTranslate()(0), m_keyList(i).m.GetTranslate()(1), m_keyList(i).m.GetTranslate()(2) );
    }
  }

  void SetPositionKey( float t, float x, float y, float z )
  {
    Key & key = FindKey(t);
    key.m.SetTranslate( lmVector3(x,y,z) );
  }

  void SetScaleKey( float t, float x, float y, float z )
  {
    Key & key = FindKey(t);

    lmMatrix4 scale(true);
    scale.AxisScale3x3( lmVector3(x,y,z) );
    key.m *= scale;
  }

  void SetRotationKey( float t, float x, float y, float z, float w )
  {
    lmQuaternion q( x, y, z, w );

    lmMatrix4 m(1);
    q.SetMatrix3x3( m );

    Key & key = FindKey(t);
    key.m *= m;
    // TODO: should this be a set?
  }

  void SetMatrixKey( float t, const lmMatrix4 &m )
  {
    Key & key = FindKey(t);
    key.m = m;
  }

  int GetNumKeys()
  {
    return m_keyList.Size();
  }

  void GetMatrixKey( int key, lmMatrix4 &m, float &t )
  {
    m = m_keyList(key).m;
    t = m_keyList(key).t;
  }

  void Inverse()
  {
    for(int i = 0; i < m_keyList.Size(); i++)
    {
      m_keyList(i).m = m_keyList(i).m.Inverse();
    }
  }

  void PreMultiplyMerge( lSimpleAnimation * other )
  {
    // multiply other * this and add in any keyframes we don't have using interpolated values
    for(int i = 0; i < m_keyList.Size(); i++)
    {
      lmMatrix4 m;
      other->GetMatrix( m_keyList(i).t, m );
      m_keyList(i).m = m * m_keyList(i).m;
    }
  }

  void PostMultiplyMerge( lSimpleAnimation * other )
  {
    // multiply other * this and add in any keyframes we don't have using interpolated values
    for(int i = 0; i < m_keyList.Size(); i++)
    {
      lmMatrix4 m;
      other->GetMatrix( m_keyList(i).t, m );
      m_keyList(i).m = m_keyList(i).m * m;
    }
  }

  // lAnimation functions

  virtual void Serialize( lOldArchive & ar )
  {
    ar << m_keyList;
  }


  virtual int GetByteSize()
  {
    return m_keyList.NumBytes();
  }

  virtual void SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld)
  {
    for(int i = 0; i < m_keyList.Size(); i++)
    {
      m_keyList(i).m = newToOld * m_keyList(i).m * oldToNew;
    }
  }

  virtual void PreMultiply( const lmMatrix4 &m )
  {
    for(int i = 0; i < m_keyList.Size(); i++)
    {
      m_keyList(i).m = m * m_keyList(i).m;
    }
  }

  virtual void PostMultiply( const lmMatrix4 &m )
  {
    for(int i = 0; i < m_keyList.Size(); i++)
    {
      m_keyList(i).m = m_keyList(i).m * m;
    }
  }

  virtual float GetLength()
  {
    if( m_keyList.Size() == 0 ) return 0.0f;
    return m_keyList.Last().t;
  }

  virtual void  GetMatrix( float t, lmMatrix4 &m )
  {
    if( m_keyList.Size() == 0 )
    {
      m.Identity();
      return;
    }

    for( int i=0;i<m_keyList.Size();i++ )
    {
      if( m_keyList(i).t == t )
      {
        m = m_keyList(i).m;
        return;
      }

      if( m_keyList(i).t > t )
      {
        if( i==0 )
        {
          m = m_keyList(i).m;
          return;
        }

        // tween between the keys

        float dist = m_keyList(i).t - m_keyList(i-1).t;
        float relT = t - m_keyList(i-1).t;

        float tween = relT/dist;

        lmAffineParts p0,p1;

        lmDecompAffine( m_keyList(i-1).m, p0);
        lmDecompAffine( m_keyList(i).m, p1);

        lmAffineParts result = lmAffineParts::Interpolate(p0,p1,tween);

        lmRecompAffine( m, result );
        return;
      }
    }

    if( m_keyList.Size() > 0 )
    {
      m = m_keyList.Last().m;
      return;
    }

    m.Identity();
    return;
  }

  virtual void  GetXForm( float t, lAnimXform &xform )
  {
    if( m_keyList.Size() == 0 )
    {
      xform.Identity();
      return;
    }

    for( int i=0;i<m_keyList.Size();i++ )
    {
      if( m_keyList(i).t == t )
      {
        xform = lAnimXform( m_keyList(i).m );
        return;
      }

      if( m_keyList(i).t > t )
      {
        if( i==0 )
        {
          xform = lAnimXform( m_keyList(i).m );
          return;
        }

        // tween between the keys

        float dist = m_keyList(i).t - m_keyList(i-1).t;
        float relT = t - m_keyList(i-1).t;

        float tween = relT/dist;

        lmAffineParts p0,p1;

        lmDecompAffine( m_keyList(i-1).m, p0);
        lmDecompAffine( m_keyList(i).m, p1);

        lmAffineParts result = lmAffineParts::Interpolate(p0,p1,tween);

        // set up xform
        xform.translation = result.t;
        xform.rotation    = result.q;
        xform.scale       = result.k * result.f;

        return;
      }
    }

    if( m_keyList.Size() > 0 )
    {
      xform = lAnimXform( m_keyList.Last().m );
      return;
    }

    xform.Identity();
    return;
  }

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSplinedAnimation - Splined implementation of lAnimation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API  lVector3Spline
{
public:
  float                  m_startTime;
  float                  m_endTime;
  lmMatrix<4,1,float>    m_g[3];

  lVector3Spline();
  lVector3Spline(float startTime, float endTime, const lmVector3 &value);

  void      EvaluateTime(float time, lmVector3 &result) const;
  void      EvaluateT(float t, lmVector3 &result) const;

  friend lStream& operator<< ( lStream& ar, lVector3Spline &sp )
  {
    ar << sp.m_startTime << sp.m_endTime;
    ar << sp.m_g[0];
    ar << sp.m_g[1];
    ar << sp.m_g[2];
    return ar;
  }
};

class LSCENE_API  lQuatSpline
{
public:
  float                  m_startTime;
  float                  m_endTime;
  lmMatrix<4,1,float>    m_g[4];

  lQuatSpline();
  lQuatSpline(float startTime, float endTime, const lmQuaternion &value);

  friend lStream& operator<< ( lStream& ar, lQuatSpline &sp )
  {
    ar << sp.m_startTime << sp.m_endTime;
    ar << sp.m_g[0];
    ar << sp.m_g[1];
    ar << sp.m_g[2];
    ar << sp.m_g[3];
    return ar;
  }

  void      EvaluateTime(float time, lmQuaternion &result) const;
  void      EvaluateT(float t, lmQuaternion &result) const;
};


class LSCENE_API  lSplinedAnimation : public lAnimation
{
  lArray<lVector3Spline,TSceneArrayTrait>   m_translationSplines;
  lArray<lQuatSpline,TSceneArrayTrait>      m_rotationSplines;
  lArray<lVector3Spline,TSceneArrayTrait>   m_scaleSplines;

  // compression functions

  lArray<lVector3Spline,TSceneArrayTrait> PackVector3(const lArray<lmVector3, TSceneArrayTrait> &values, int startIndex, int lastIndex, float startTime, float endTime, float tolerence);
  lArray<lVector3Spline,TSceneArrayTrait> PackVector3( const lArray<lmVector3, TSceneArrayTrait> &values, float length, float tolerence )
  {
    return PackVector3( values, 0, values.Size() - 1, 0.0f, length, tolerence );
  }

  lArray<lQuatSpline,TSceneArrayTrait> PackQuaternion( const lArray<lmQuaternion, TSceneArrayTrait> &values, int startIndex, int lastIndex, float startTime, float endTime, float tolerence);
  lArray<lQuatSpline,TSceneArrayTrait> PackQuaternion( const lArray<lmQuaternion, TSceneArrayTrait> &values, float length, float tolerence )
  {
    return PackQuaternion( values, 0, values.Size() - 1, 0.0f, length, tolerence );
  }

public:

  lSplinedAnimation() {}

  void Compress( lAnimation *anim, float samplefps = 60, float tolerance = 0.001f );
  void Decompress( lSimpleAnimation *anim, float samplefps = 30 );

  virtual void Serialize( lOldArchive & ar )
  {
    ar << m_translationSplines;
    ar << m_rotationSplines;
    ar << m_scaleSplines;
  }

  virtual int GetByteSize()
  {
    return m_translationSplines.NumBytes() + m_rotationSplines.NumBytes() + m_scaleSplines.NumBytes();
  }

  int GetType() { return LANIMATION_TYPE_SPLINED; }

  // lAnimation functions

  virtual void SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld)
  {
    // Decompress animation
    lSimpleAnimation *tempAnim = new lSimpleAnimation;
    Decompress( tempAnim, 60 );

    // multiply it
    tempAnim->SpaceConvert( oldToNew, newToOld );

    // Compress animation
    Compress( tempAnim, 0.02f, 60 );

    delete tempAnim;
  }

  virtual void          PreMultiply( const lmMatrix4 &m )
  {
    // Decompress animation
    lSimpleAnimation *tempAnim = new lSimpleAnimation;
    Decompress( tempAnim, 60 );

    // multiply it
    tempAnim->PreMultiply( m );

    // Compress animation
    Compress( tempAnim, 0.02f, 60 );

    delete tempAnim;
  }

  virtual void          PostMultiply( const lmMatrix4 &m )
  {
    // Decompress animation
    lSimpleAnimation *tempAnim = new lSimpleAnimation;
    Decompress( tempAnim, 60 );

    // multiply it
    tempAnim->PostMultiply( m );

    // Compress animation
    Compress( tempAnim, 0.02f, 60 );

    delete tempAnim;
  }

  virtual lAnimation * Clone()
  {
    lSplinedAnimation * newAnim = new lSplinedAnimation(*this);
    return newAnim;
  }

  virtual float GetLength();
  virtual void  GetMatrix( float t, lmMatrix4 &m );
  virtual void  GetXForm( float t, lAnimXform &xform );
};


//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// lAnimNode - Anim object
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

class LSCENE_API lAnimNode : public lRTTI<lAnimNode, lObject, lCLSID<0x59B818DD,0x728C,0x49E2,0x98F0073E5EA10ED2> >
{
public:
  lAnimNode();
  lAnimNode(const lAnimNode &);
  virtual ~lAnimNode();
  lAnimNode & operator= ( const lAnimNode &other );

  LSCENE_API friend lStream& operator << ( lStream& ar, lAnimNode *& obj );

  virtual void Serialize( lOldArchive & ar );

  enum NotifyCode
  {
    NotifyBranchAttach    = 0x01,   // internal, when a branch is attached
    NotifyBranchDetach    = 0x02,   // internal, when a branch is detached
    NotifyMatrixChange    = 0x03,   // internal, when a node matrix changes
  };

  // animation functions

  virtual void              PostNotifyEvent( lAnimNode::NotifyCode, lAnimNode * );

  bool                      MatchHierarchy( lSceneNode * );

  virtual void              SetTime( float t );
  virtual float             GetLength();

  virtual void              GetLocalBoundingBox(lmAABox3 &box);
  virtual bool              TraceRay( const lmVector3& origin, const lmVector3& normal, lSceneTraceHit &out );
  virtual void              Render();
  virtual bool              IsRenderable() { return true; }

  // hierarchy functions

  const char              * GetName() const                      { return (const char *)m_name; }
  void                      SetName(const char *s)               { m_name = s; }

  lSceneNode              * GetNode() const                      { return m_sceneNode; }

//  const lGUID             & GetGUID() const                      { return m_type; }

  unsigned int              GetFlags() const                     { return m_flags; }
  void                      SetFlags(unsigned int  f)            { m_flags = f; }

  lAnimNode               * GetParent() const                    { return m_parent; }
  unsigned int              NumChildren() const                  { return m_childList.Size(); }
  lAnimNode               * GetChild(unsigned int i) const       { return m_childList(i); }

  const lmMatrix4         & GetLocalMatrix() const              { return m_localMatrix; }
  //lmMatrix4                 GetLocalMatrix() const;

  lmMatrix4                 GetWorldMatrix( lAnimNode * = NULL );

  bool                      BranchAttach( lAnimNode * );
  bool                      BranchDetach( lAnimNode * );

  virtual void              SpaceConvert(const lmMatrix4 &oldToNew, const lmMatrix4 &newToOld);
  void                      SpaceConvert(const lmMatrix4 &oldToNew);

  // lAnimNode iterator

  class LSCENE_API Iterator
  {
  public:
    Iterator( lAnimNode * root ) : m_current(root), m_skip(false), m_delete(false) {}
    void operator++();
    void operator++(int)                  { ++(*this); }
    bool operator==(lAnimNode *n) const { return (m_current == n); }
    bool operator!=(lAnimNode *n) const { return (m_current != n); }
    operator lAnimNode*() const         { return m_current; }
    lAnimNode& operator*() const        { return *m_current; }
    lAnimNode* operator->() const       { return m_current; }
    lAnimNode* GetNodePtr() const       { return m_current; }
    void         SkipChildren()         { m_skip = true; }
    unsigned int Depth() const          { return m_stack.Size(); }
    void         DeleteThis()           { m_delete = true; }
  private:
    struct Tree { lAnimNode * node; int childPos; };
    lArray<Tree,TSceneArrayTrait>  m_stack;
    lAnimNode   * m_current;
    bool          m_skip;
    bool          m_delete;
  };

//protected:
  lString                   m_name;              // node name
//  lGUID                     m_type;              // node type this data was for
  unsigned int              m_flags;             // node flags
  lAnimNode               * m_parent;            // node graph parent
  lArray<lAnimNode *,TSceneArrayTrait>       m_childList;         // node graph children

  lAnimation              * m_anim;
  lSceneNode              * m_sceneNode;

  lmMatrix4                 m_localMatrix;         // local transform relative to parent
  lmMatrix4                 m_worldMatrixTemp;     // world transform for node

  enum { FlagRecalcWorldMatrix = 0x01 };

};

END_L_NAMESPACE

#endif
