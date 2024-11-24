//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lgameworld.h
// Description : lGameWorld interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LGAMEWORLD_H_INCLUDED
#define LGAMEWORLD_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/object/lobject.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lGameClientView;
class lrCamera;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lGameWorld
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lGameWorld : public lAbstractRTTI<lGameWorld, lObject, lCLSID<0x886B5F1B,0x2D3C,0x4C6D,0xB6847767788F3326> >
{
public:

  lGameWorld();
  virtual ~lGameWorld();

  // lGameWorld interface

  virtual void Init() = 0;
  virtual void Deinit() = 0;

  virtual void BeginPlay() = 0;
  virtual void EndPlay() = 0;
  virtual void Tick(float) = 0;

  virtual void RenderClientView( lGameClientView * view, unsigned int flags, lrCamera * cullCamera ) = 0;

  // lGameWorld implementation

private:
};

END_L_NAMESPACE

#endif

