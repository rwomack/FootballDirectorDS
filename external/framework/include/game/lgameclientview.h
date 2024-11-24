//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lgameclientview.h
// Description : lGameClientView interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LGAMECLIENTVIEW_H_INCLUDED
#define LGAMECLIENTVIEW_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/object/lobject.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lGame;
class lGameClient;
class lGameClientView;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lGameClient
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lGameClientView : public lRTTI<lGameClientView, lObject, lCLSID<0xE6EB3E38,0xFA94,0x4A48,0xB4761A743B84548E> >
{
public:
  lGameClientView();
  virtual ~lGameClientView();

  // lGameClientView interface

  lGameClient * GetClient();

private:
  friend class lGame;
  lGameClient * m_client;
};


END_L_NAMESPACE

#endif

