//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmcylindergeom.h
// Description : CylinderGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMCYLINDERGEOM_H_INCLUDED
#define LMCYLINDERGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lmCylinderGeom - cylinder geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Cylinder geometry class
class LMATH_API lmCylinderGeom : public lRTTI<lmCylinderGeom,lmGeom, lCLSID<0xEC20847E,0xD0C4,0x4AB7,0xA6C4EFD450DB6600> >
{
  lmCylinderGeom(const lmCylinderGeom &);
  void operator= (const lmCylinderGeom &);
  static void Register();
  friend class lmGeom;

  float m_radius;
  float m_length;

public:

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct cylinder geom
  inline lmCylinderGeom( float radius = 1.0f, float length = 1.0f ) : m_radius ( radius ), m_length( length )
  {
    lAssert( radius > 0.0f );
    lAssert( length > 0.0f );
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
    return LMGEOM_TYPE_CYLINDER;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    if(!bTightFit)
    {
      float fulllen = m_length;
      lmVector3 size( fulllen, fulllen, fulllen );
      box.SetSize( size );
    } else
    {
      float halflen = m_length * 0.5f;

      box.Empty();
      box.Extend( lmVector3( -m_radius, -m_radius, -halflen ) );
      box.Extend( lmVector3(  m_radius,  m_radius,  halflen ) );
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns the radius of the cylinder
  inline const float & GetRadius() const
  {
    return m_radius;
  }

  //##
  // Set the radius of the cylinder
  inline void SetRadius( float rad )
  {
    // Dirty
    m_radius = rad;
  }

  //##
  // Returns the length of the cylinder
  inline const float & GetLength() const
  {
    return m_length;
  }

  //##
  // Set the length of the cylinder
  inline void SetLength( float len )
  {
    // Dirty
    m_length = len;
  }
};

END_L_NAMESPACE
#endif
