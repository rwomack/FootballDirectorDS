//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luimouse.h
// Description : Mouse Interface
// Notes       : MOVE TO FRAMEWORK INPUT SYSTEM
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUIMOUSE_H_INCLUDED
#define LUIMOUSE_H_INCLUDED

#include <lemon/platform/lcore.h>

BEGIN_L_NAMESPACE

class lUIMouse
{
public:
  static int   GetPixelX();
  static int   GetPixelY();
  static float GetX();
  static float GetY();
  static int   GetIntX();
  static int   GetIntY();
  static void  Tick(float);
};

END_L_NAMESPACE
#endif

