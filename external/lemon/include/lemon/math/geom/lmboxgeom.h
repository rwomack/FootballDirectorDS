//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmboxgeom.h
// Description : BoxGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMBOXGEOM_H_INCLUDED
#define LMBOXGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lmBoxGeom - box geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Box geometry class
class LMATH_API lmBoxGeom : public lRTTI<lmBoxGeom,lmGeom, lCLSID<0xD7FF96F5,0x9087,0x4A9F,0x84090FB204D45B9A> >
{
  lmBoxGeom(const lmBoxGeom &);
  void operator= (const lmBoxGeom &);

  static void Register();
  friend class lmGeom;

  lmVector3 m_side;

public:

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct Box geom
  inline lmBoxGeom() : m_side( 1.0f,1.0f,1.0f )
  {
  }

  //##
  // Construct Box geom
  inline lmBoxGeom( const lmVector3 &v ) : m_side( v )
  {
  }

  //##
  // Construct Box geom
  inline lmBoxGeom( float lx , float ly, float lz ) : m_side( lx, ly, lz )
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
    return LMGEOM_TYPE_BOX;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    if( !bTightFit )
    {
      // use longest side as our box size
      lmVector3 upper( m_side(0) * 0.5f, m_side(1) * 0.5f, m_side(2) * 0.5f );
      float largestAxis = upper.Length() * 2.0f;

      lmVector3 size( largestAxis, largestAxis, largestAxis );
      box.SetSize( size );
    } else
    {
      box.SetSize( m_side );
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Returns the size of the side of the box
  inline const lmVector3 &GetSize() const
  {
    return m_side;
  }

  //##
  // Set the size of the side of the box
  inline void SetSize( const lmVector3 &v )
  {
    m_side = v;
  }
};

END_L_NAMESPACE
#endif
