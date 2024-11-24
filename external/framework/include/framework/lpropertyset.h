//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lpropertyset.h
// Description : lPropertySet
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LPROPERTYSET_H_INCLUDED
#define LPROPERTYSET_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

class lProperty;
class lClass;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// hePropertySet
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertySet
{
public:
  void                       AddProperty( lProperty * prop );
  int                        GetNumProperties() const;
  const lProperty          * GetProperty( int index ) const;
  lProperty                * GetProperty( int index );

  static lPropertySet      & GetGlobalPropertySet();
  static lPropertySet      & GetClassPropertySet( const lClass * );

private:
  lArray<lProperty *> m_propList;
};


END_L_NAMESPACE
#endif
