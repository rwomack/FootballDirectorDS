//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmplanegeom.h
// Description : PlaneGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMPLANEGEOM_H_INCLUDED
#define LMPLANEGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lmPlaneGeom - plane geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Plane geometry class
class LMATH_API lmPlaneGeom : public lRTTI<lmPlaneGeom,lmGeom, lCLSID<0xC62BEDFA,0x5D0A,0x462E,0x95E9D46735788746> >
{
  lmPlaneGeom(const lmPlaneGeom &);
  void operator= (const lmPlaneGeom &);
  static void Register();
  friend class lmGeom;

  lmVector3   m_n;
  float       m_d;

public:

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct plane geom
  inline lmPlaneGeom( float a = 0, float b = 1, float c = 0, float d = 0 )
  {
    m_n.Init( a, b, c );
    m_n.Normalize();
    m_d = d;
  }

  //##
  // Construct plane geom
  inline lmPlaneGeom( lmVector4 &vp )
  {
    m_n.Init( vp(0), vp(1), vp(2) );
    m_n.Normalize();
    m_d = vp(3);
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
    return LMGEOM_TYPE_PLANE;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    L_UNUSED_PARAM( bTightFit );
    box.Empty();
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns the params of the plane
  inline void GetParams( lmVector3 & n, float & d ) const
  {
    n = m_n;
    d = m_d;
  }

  //##
  // Returns the normal of the plane
  inline const lmVector3 &GetNormal() const
  {
    return m_n;
  }

  //##
  // Returns the offset of the plane
  inline float GetOffset() const
  {
    return m_d;
  }

  //##
  // Set the plane params
  inline void SetParams( const lmVector3 &n, const float &d )
  {
    m_n = n;
    m_d = d;
  }
};

END_L_NAMESPACE
#endif
