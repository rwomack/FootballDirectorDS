//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdfrontend.h
// Description : FDFrontEnd interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDFRONTEND_H_INCLUDED
#define FDFRONTEND_H_INCLUDED

#include <fdgameworld.h>
#include <lemon/containers/lstring.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lUILayer;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDFrontEnd
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDFrontEnd : public FDSubGame, public lHasSlots<>
{
public:
  FDFrontEnd();
  virtual ~FDFrontEnd();

  // FDSubGame interface

  virtual void          Init();
  virtual void          Deinit();
  virtual void          Activate();
  virtual void          Deactivate();
  virtual void          Tick(float);

  // FDFrontEnd interface

private:
  lUILayer * m_layer;
  bool       m_bFirstTime;
};


END_L_NAMESPACE
#endif

