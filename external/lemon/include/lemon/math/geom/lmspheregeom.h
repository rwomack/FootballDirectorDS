//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmspheregeom.h
// Description : SphereGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMSPHEREGEOM_H_INCLUDED
#define LMSPHEREGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lmSphereGeom - sphere geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Sphere geometry class
class LMATH_API lmSphereGeom : public lRTTI<lmSphereGeom,lmGeom, lCLSID<0xD775F827,0x5CFC,0x43D3,0xB030D444CD27F81A> >
{
  lmSphereGeom(const lmSphereGeom &);
  void operator= (const lmSphereGeom &);
  static void Register();
  friend class lmGeom;

  float m_radius;

public:

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct sphere geom
  inline lmSphereGeom( float radius = 1.0f ) : m_radius ( radius )
  {
    lAssert( radius > 0.0f );
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
    return LMGEOM_TYPE_SPHERE;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    L_UNUSED_PARAM( bTightFit );
    // box is same for a sphere whether tight fit or not
    float diam = m_radius * 2.0f;

    lmVector3 size( diam, diam, diam );
    box.SetSize( size );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns the radius of the sphere
  inline float GetRadius() const
  {
    return m_radius;
  }

  //##
  // Returns the squared radius of the sphere
  inline float GetSqrRadius() const
  {
    return (m_radius * m_radius);
  }

  //##
  // Set the radius of the sphere
  inline void SetRadius( float rad )
  {
    // Dirty
    m_radius = rad;
  }
};

END_L_NAMESPACE
#endif
