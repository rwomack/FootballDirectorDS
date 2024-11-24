//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmcapsulegeom.h
// Description : CapsuleGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMCAPSULEGEOM_H_INCLUDED
#define LMCAPSULEGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lmCapsuleGeom - capsule geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Capsule geometry class
class LMATH_API lmCapsuleGeom : public lRTTI<lmCapsuleGeom,lmGeom, lCLSID<0xB7C96643,0x66A0,0x4C98,0xB9F1BFC53EFD272F> >
{
  lmCapsuleGeom(const lmCapsuleGeom &);
  void operator= (const lmCapsuleGeom &);
  static void Register();
  friend class lmGeom;

  float       m_radius;
  float       m_length;

public:

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct capsule geom
  inline lmCapsuleGeom( float radius = 1.0f, float length = 1.0f ) : m_radius( radius ), m_length( length )
  {
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
    return LMGEOM_TYPE_CAPSULE;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    if( !bTightFit )
    {
      float fulllen = m_length + m_radius + m_radius;
      lmVector3 size( fulllen, fulllen, fulllen );
      box.SetSize( size );
    } else
    {
      float halflen = m_length * 0.5f;

      box.Empty();
      box.Extend( lmVector3( -m_radius, -m_radius, -( halflen + m_radius ) ) );
      box.Extend( lmVector3(  m_radius,  m_radius,  ( halflen + m_radius ) ) );
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns the radius + length of the capsule
  inline void GetParams( float & rad, float & len ) const
  {
    rad = m_radius;
    len = m_length;
  }

  //##
  // Returns the radius of the capsule
  inline float GetRadius() const
  {
    return m_radius;
  }

  //##
  // Returns the length of the capsule
  inline float GetLength() const
  {
    return m_length;
  }

  //##
  // Set the radius + length of the capsule
  inline void SetParams( float rad, float len )
  {
    m_radius = rad;
    m_length = len;
  }
};

END_L_NAMESPACE
#endif
