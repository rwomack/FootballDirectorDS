//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmraygeom.h
// Description : RayGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMRAYGEOM_H_INCLUDED
#define LMRAYGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lmRayGeom - ray geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Ray geometry class
class LMATH_API lmRayGeom : public lRTTI<lmRayGeom,lmGeom, lCLSID<0x6609AA5B,0x1AF4,0x4662,0xACE43721AD21AC72> >
{
  lmRayGeom(const lmRayGeom &);
  void operator= (const lmRayGeom &);
  static void Register();
  friend class lmGeom;

  float m_length;

public:

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct ray geom
  inline lmRayGeom( float length = 1.0f ) : m_length ( length )
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
    return LMGEOM_TYPE_RAY;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    if(!bTightFit)
    {
      // ray could point in any direction by its length
      box.Empty();

      lmVector3 size( 2.0f*m_length, 2.0f*m_length, 2.0f*m_length );
      box.SetSize( size );
    } else
    {
      box.Empty();
      box.Extend( lmVector3( -0.005f, -0.005f, 0 ) );
      box.Extend( lmVector3(  0.005f,  0.005f, m_length ) );
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns the length of the ray
  inline float GetLength() const
  {
    return m_length;
  }

  //##
  // Set the length of the ray
  inline void SetLength( float len )
  {
    m_length = len;
  }

  //##
  // Set parameters on ray geom
  void SetParams( bool backfaceCull )
  {
    if( backfaceCull )
    {
      m_flags |= RAY_BACKFACECULL;
    }
    else
    {
      m_flags &= ~RAY_BACKFACECULL;
    }
  }

  //##
  // Get parameters of ray geom
  void GetParams( bool &backfaceCull )
  {
    backfaceCull = ((m_flags & RAY_BACKFACECULL) != 0);
  }
};

END_L_NAMESPACE
#endif
