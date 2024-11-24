//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmcompositegeom.h
// Description : CompositeGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMCOMPOSITEGEOM_H_INCLUDED
#define LMCOMPOSITEGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lmCompositeGeom - composite geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

class LMATH_API lmCompositeGeom : public lRTTI<lmCompositeGeom,lmGeom, lCLSID<0x2D08476A,0xC6EF,0x411C,0x9E9AB5E413125CF0> >
{
  lmCompositeGeom(const lmCompositeGeom &);
  void operator= (const lmCompositeGeom &);
  friend class lmGeom;
  static void Register();

public:
	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct geom
  inline lmCompositeGeom()
  {
    m_curMatrix.Identity();
  }

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
    return LMGEOM_TYPE_COMPOSITE;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    box.Empty();

    // if only one geom and we are doing non tight fit just pass straight through
    if( !bTightFit && m_geomList.Size() == 1 )
    {
      m_geomList(0)._geom->GetAABox( box, false );
      return;
    }

    // otherwise go through all geoms, work out a box for all the sub-boxes
    for(int i = 0; i < m_geomList.Size(); i++)
    {
      lmAABox3 temp;
      m_geomList(i)._geom->GetAABox( temp, true );
      temp.Transform( m_geomList(i)._initialMatrix );
      box.Extend( temp );
    }

    // now work out the radius from the centre to the lower / uppers
    if(!bTightFit)
    {
      float sphereRadius = box.GetCentre().SqrDistance( box.GetLower() );
      sphereRadius = 2.0f * lmSqrt( sphereRadius );
      lmVector3 size( sphereRadius, sphereRadius, sphereRadius );
      box.SetSize( size );
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Add a geom to the composite
  void AddGeom( lmGeom *geom, const lmMatrix4 &m )
  {
    lAssert( geom != NULL );

    m_geomList.Add(1);
    m_geomList.Last()._geom = geom;
    m_geomList.Last()._initialMatrix = m;
    m_geomList.Last()._curMatrix = m * m_curMatrix;
  }

  //##
  // Remove a specific geom from the composite
  void RemoveGeom( lmGeom *geom )
  {
    lAssert( geom != NULL );

    for(int i = 0; i < m_geomList.Size(); i++)
    {
      if(m_geomList(i)._geom == geom)
      {
        m_geomList.Remove(i);
        return;
      }
    }
  }

  //##
  // Remove all geoms from the composite
  void RemoveAllGeoms()
  {
    m_geomList.Empty();
  }

  //##
  // Return the number of geoms in the composite
  int  GetNumGeoms()
  {
    return m_geomList.Size();
  }

  //##
  // Return a specific geom from the composite
  lmGeom *GetGeom( int i )
  {
    return m_geomList(i)._geom;
  }

  //##
  // Return the initial matrix of a specific geom in the composite
  lmMatrix4    &GetInitialGeomMatrix( int i )
  {
    return m_geomList(i)._initialMatrix;
  }

  //##
  // Return the current matrix of a specific geom in the composite
  lmMatrix4    &GetCurrentGeomMatrix( int i )
  {
    return m_geomList(i)._curMatrix;
  }

  void UpdateMatrices( lmMatrix4 &m )
  {
    if( m.EqualTo( m_curMatrix ) )
    {
      return;
    }

    m_curMatrix = m;

    for(int i = 0; i < m_geomList.Size(); i++)
    {
      m_geomList(i)._curMatrix = m_geomList(i)._initialMatrix * m;
    }
  }

protected:
  struct subGeom
  {
    //lPhysicsGeomResource _geom;
    lmGeom             * _geom;
    lmMatrix4            _initialMatrix;
    lmMatrix4            _curMatrix;

    friend lStream& operator<< ( lStream& ar, subGeom &sgeom )
    {
      //TODOar << sgeom._geom;
      ar << sgeom._initialMatrix;
      ar << sgeom._curMatrix;

      return ar;
    }
  };

  lArray<subGeom> m_geomList;
  lmMatrix4       m_curMatrix;
};

END_L_NAMESPACE
#endif
