//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmoffsetgeom.h
// Description : OffsetGeom class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMOFFSETGEOM_H_INCLUDED
#define LMOFFSETGEOM_H_INCLUDED

#include "../lmath.h"
#include "../lmgeom.h"
#include "../lmaabox.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//  lmOffsetGeom - offset geom class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//##
//  Offset utility geometry class
class LMATH_API lmOffsetGeom : public lRTTI<lmOffsetGeom,lmGeom, lCLSID<0xB121BE51,0x3F3C,0x43E8,0x85501BC1FB0D355B> >
{
  friend class lmGeom;
  static void Register();
public:

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct offsetgeom from a geom pointer
  lmOffsetGeom( lmGeom *geom = NULL ) : m_geom( geom )
  {
    m_offsetMatrix.Identity();
  }

  //##
  // Construct offsetgeom from a geom pointer and matrix offset
  lmOffsetGeom( lmGeom *geom, const lmMatrix4 &offset ) : m_geom( geom )
  {
    m_offsetMatrix = offset;
  }

  //##
  // Deconstruct
  virtual ~lmOffsetGeom()
  {
  }

  //##
  // Initialise offsetgeom
  inline void Init( lmGeom *geom, const lmMatrix4 &offset )
  {
    m_geom = geom;
    m_offsetMatrix = offset;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // lmGeom functions
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return type of this geom
  virtual int GetType()
  {
    return LMGEOM_TYPE_OFFSET;
  }

  //##
  // Return aabox around geom
  virtual void GetAABox ( lmAABox3 &box, bool bTightFit )
  {
    if ( GetGeom() != NULL )
    {
      GetGeom()->GetAABox( box, bTightFit );
      box.Transform( GetOffsetMatrix() );
    }
    else
    {
      box.Empty();
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Get offset matrix
  inline const lmMatrix4 & GetOffsetMatrix() const
  {
    return m_offsetMatrix;
  }

  //##
  // Set offset matrix
  inline void SetOffsetMatrix(const lmMatrix4 &offset)
  {
    m_offsetMatrix = offset;
  }

  //##
  // Get geom
  inline lmGeom * GetGeom()
  {
    return m_geom;
  }

  //##
  // Set geom
  inline void SetGeom( lmGeom *geom )
  {
    m_geom = geom;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  virtual void Serialize( lOldArchive & ar );

protected:
  //lPhysicsGeomResource  m_geom;
  lmGeom              * m_geom;
  lmMatrix4             m_offsetMatrix;
};

END_L_NAMESPACE
#endif
