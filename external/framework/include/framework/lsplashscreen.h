//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsplashscreen.h
// Description : Splash Screen
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#ifndef LSPLASHSCREEN_H_INCLUDED
#define LSPLASHSCREEN_H_INCLUDED


#include <lemon/platform/lcore.h>
#include <lemon/object/lsharedptr.h>
#include <lemon/movie/lmovie.h>

BEGIN_L_NAMESPACE

class lSpriteBank;
class InputObject;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSplashScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lSplashScreen
{
public:

  #if defined(L_PLATFORM_NDS) || defined(L_PLATFORM_WINDOWS)
  static void AddImage( lSharedPtr<lSpriteBank> spriteBank, const char* topImageId, const char* bottomImageId, float time, float fadeIn, float fadeOut, bool bAllowKeySkip );
  #endif

  #if !defined(L_PLATFORM_NDS)
  static void AddImage( lSharedPtr<lSpriteBank> spriteBank, const char* imageId, float time, float fadeIn, float fadeOut, bool bAllowKeySkip );
  #endif
  static void AddMovie( const char* filename, float time, bool bAllowKeySkip);
  static void SetSkipInput( InputObject* );
  static void Start();
  static bool IsFinished();
  static void AdvanceState(bool bSkip = false);
  static bool IsAllowKeySkip();

  static void PauseMovie();
  static void ResumeMovie();

  static void Tick(float delta);
  static void Render();

  static void Clear();
  
  static bool IsMovieAborted();
};

END_L_NAMESPACE

#endif
