//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lloadingscreen.h
// Description : Loading Screen
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#ifndef LLOADINGSCREEN_H_INCLUDED
#define LLOADINGSCREEN_H_INCLUDED

#include <lemon/platform/lcore.h>
#include <lemon/platform/lcorefile.h>
#include <lemon/io/lfileiostream.h>
#include <lemon/object/lsharedptr.h>

BEGIN_L_NAMESPACE

class lSpriteBank;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lLoadingScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lLoadingScreen
{
public:

  static void SetBackground( lSharedPtr<lSpriteBank> spriteBank, const char* backgroundId );

  static void SetBar( lSharedPtr<lSpriteBank> spriteBank, const char* barId, float x, float y, float w, float h );
  static void SetBar( float x, float y, float w, float h, luint32_t colour );

  static void LinkAnimationToPercent();

  static void SetPercent( int percent );
  static void Render();

};

END_L_NAMESPACE

#endif

