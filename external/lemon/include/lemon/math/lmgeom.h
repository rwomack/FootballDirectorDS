//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmgeom.h
// Description : Geometry class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMGEOM_H_INCLUDED
#define LMGEOM_H_INCLUDED

#include "../containers/larray.h"
#include "lmath.h"
#include "lmmatrix.h"
#include "../object/lobject.h"
#include "../object/loldarchive.h"

BEGIN_L_NAMESPACE

class lmGeom;
class lmGeomContactSet;
template <unsigned int N, typename T> class LMATH_API lmAABox;
typedef lmAABox<3,float> lmAABox3;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Typedef of collision functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

typedef int     fplmCollideFnc ( lmGeomContactSet &contactSet, lmGeom *o1, lmMatrix4 &m1, lmGeom *o2, lmMatrix4 &m2 );
typedef lmGeom *fplmConstructFnc ();

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmGeom  -  Geometry class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Geometry base type
enum
{
  LMGEOM_TYPE_UNKNOWN = -1,
  LMGEOM_TYPE_SPHERE = 0,
  LMGEOM_TYPE_BOX,
  LMGEOM_TYPE_CAPSULE,
  LMGEOM_TYPE_COMPOSITE,
  LMGEOM_TYPE_OFFSET,
  LMGEOM_TYPE_RAY,
  LMGEOM_TYPE_PLANE,
  LMGEOM_TYPE_CYLINDER,
  LMGEOM_TYPE_CONE,
  LMGEOM_TYPE_GJK,
  LMGEOM_TYPE_CONVEXHULL,
  LMGEOM_TYPE_SDM,
  LMGEOM_TYPE_TRIMESH,
  LMGEOM_TYPE_MAX,

  // user defined type space
  LMGEOM_TYPE_USER = 100,
};

//##
//  Base geometry class
class LMATH_API lmGeom : public lAbstractRTTI<lmGeom, lObject, lCLSID<0xEF7976E7,0x07A4,0x4E63,0xB633860B9A3AEEEE> >
{
 public:
  lmGeom() : m_flags(0)
  {
  }

  virtual ~lmGeom()
  {
  }

  // lmGeom functions

  friend lStream& operator << ( lStream& ar, lmGeom *& obj );
  virtual void Serialize( lOldArchive & ar );

  //##
  // Construct an aabox around the lmGeom
  // In:
  //   box - Box to return in
  //   bTightFit - if true a tight fit of the geom is made, if false a box that would fit the geom in any orientation is returned
  virtual void GetAABox  ( lmAABox3 &box, bool bTightFit = true ) = 0;

  //##
  // Return type of lmGeom
  virtual int  GetType()
  {
    return LMGEOM_TYPE_UNKNOWN;
  }

  // Flags

  enum
  {
    // Geom flags
    GEOM_WORLDSPACE       = 0x00001,           // geom is in world space

    // Ray specific
    RAY_BACKFACECULL      = 0x20000,
    RAY_CLOSESTHIT        = 0x40000,
  };

  inline  unsigned int              GetFlags() const                     { return m_flags; }
  inline  void                      SetFlags( unsigned int flags )       { m_flags = flags; }
  inline  bool                      GetFlag( unsigned int flag ) const   { return ( m_flags & flag )  !=  0; }
  inline  void                      SetFlag( unsigned int flag )         { m_flags = m_flags | flag; }
  inline  void                      ClearFlag( unsigned int flag )       { m_flags = m_flags & ~flag; }
  inline  void                      InvertFlag( unsigned int flag )      { m_flags = m_flags ^ flag; }

  // static functions

  static void          InitClasses();
  static void          RegisterGeom( int type, const char *name, fplmConstructFnc *constructFunction );
  static void          RegisterCollider( int type1, int type2, fplmCollideFnc *collideFunction );
  static void          RegisterAllCollider( int type1, fplmCollideFnc *collideFunction );
  static const char  * TypeName( int );
  static int           TypeId( const char * );

  static int           Collide( lmGeomContactSet &set, lmGeom *g1, lmMatrix4 &m1, lmGeom *g2, lmMatrix4 &m2, void *id1 = NULL, void *id2 = NULL );

 protected:
  unsigned int m_flags;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Contact set utility functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//int LMATH_API ClusterContacts( lArray<lPhysicsContact> &set, int startContact );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmGeomContactPoint -  Geometry contact point
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lmGeomContactPoint
{
  lmGeomContactPoint() : pos(0,0,0), normal(0,0,0), depth(0), g1(NULL), g2(NULL), id1(NULL), id2(NULL)
  {}

  lmGeomContactPoint( const lmVector3 &p, const lmVector3 &n, float d ) : pos(p), normal(n), depth(d), g1(NULL), g2(NULL), id1(NULL), id2(NULL)
  {}

  friend inline lStream & operator<<( lStream &ar, lmGeomContactPoint &point )
  {
    ar << point.pos;
    ar << point.normal;
    ar << point.depth;
    ar << point.g1;
    ar << point.g2;
//    ar << point.id1;
//    ar << point.id2;
    return ar;
  }

  inline bool operator==( const lmGeomContactPoint &other ) const
  {
    if( pos != other.pos ) return false;
    if( normal != other.normal ) return false;
    if( depth != other.depth ) return false;
    if( g1 != other.g1 ) return false;
    if( g2 != other.g2 ) return false;
    return true;
  }

  lmVector3       pos;
  lmVector3       normal;
  float           depth;
  lmGeom        * g1;
  lmGeom        * g2;

  void          * id1;
  void          * id2;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmGeomContactSet -  Geometry contact set
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<unsigned int MAX_SIZE>
class lmGeomContactStaticMemArrayTrait
{
  uint8 m_data[MAX_SIZE]
  #if defined(L_PLATFORM_PS2) || defined(L_PLATFORM_PSP)
  __attribute__ ((aligned(16)));
  #endif
  ;

public:
  lmGeomContactStaticMemArrayTrait()
  {
  }

  ~lmGeomContactStaticMemArrayTrait()
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

class lmGeomContactSet
{
 public:
  lmGeomContactSet()
  {
  }

  virtual ~lmGeomContactSet()
  {
  }

  int GetNumContacts() const
  {
    return m_contacts.Size();
  }

  lmGeomContactPoint &GetContact( int idx )
  {
    return m_contacts( idx );
  }

  void PushContact( lmGeomContactPoint contact )
  {
    m_contacts.Push( contact );
  }

  int AddContact( int num = 1 )
  {
    return m_contacts.Add( num );
  }

  lmGeomContactPoint &AddSingleContact()
  {
    m_contacts.Add(1);
    return m_contacts( m_contacts.Size()-1 );
  }

  void Remove( int startContact, int numContacts )
  {
    m_contacts.Remove( startContact, numContacts );
  }

  void Append( lArray<lmGeomContactPoint> &array )
  {
    //m_contacts.Append( array );
    for( int i=0;i<array.Size();i++ )
    {
      m_contacts.Push(array(i));
    }
  }

  void Empty()
  {
    m_contacts.Empty(m_contacts.Size());
  }

  bool Equal( lmGeomContactSet &set )
  {
    if( GetNumContacts() != set.GetNumContacts() )
    {
      return false;
    }

    for(int i = 0; i < GetNumContacts(); i++)
    {
      if( GetContact(i) == set.GetContact(i) )
      {
      } else
      {
        return false;
      }
    }

    return true;
  }

  friend inline lStream & operator<<( lStream &ar, lmGeomContactSet &cset )
  {
    ar << cset.m_contacts;
    return ar;
  }

 private:
  friend class lmGeom;

  //##
  // Used by lmGeom::Collide to reverse results
  void ReverseContact( int idx )
  {
    m_contacts(idx).normal = -m_contacts(idx).normal;
    lSwap( m_contacts(idx).id1, m_contacts(idx).id2 );
    lSwap( m_contacts(idx).g1, m_contacts(idx).g2 );
  }

  lArray<lmGeomContactPoint, lmGeomContactStaticMemArrayTrait<51200> > m_contacts;
};

END_L_NAMESPACE


#endif


