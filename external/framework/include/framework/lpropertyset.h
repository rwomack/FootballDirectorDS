//����������������������������������������������������������������������������
// File        : lpropertyset.h
// Description : lPropertySet
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LPROPERTYSET_H_INCLUDED
#define LPROPERTYSET_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

class lProperty;
class lClass;

//����������������������������������������������������������������������������
// hePropertySet
//����������������������������������������������������������������������������

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
