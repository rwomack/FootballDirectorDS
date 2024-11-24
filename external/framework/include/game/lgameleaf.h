//����������������������������������������������������������������������������
// File        : lgameleaf.h
// Description : lGameLeaf interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LGAMELEAF_H_INCLUDED
#define LGAMELEAF_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/object/lobject.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

//����������������������������������������������������������������������������
// lGameLeaf
//����������������������������������������������������������������������������

class lGameLeaf : public lRTTI<lGameLeaf, lObject, lCLSID<0xE529B555,0xCC6C,0x4FFE,0xA505F1F5ACDC055C> >
{
public:

  lGameLeaf();
  virtual ~lGameLeaf();

private:
};

END_L_NAMESPACE

#endif

