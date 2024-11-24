//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsoundmanager.h
// Description : lSoundManager class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2006 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LSOUNDMANAGER_H
#define __LSOUNDMANAGER_H

#include <lemon/platform/lcore.h>
#include <lemon/math/lmmatrix.h>
#include <lemon/containers/lstring.h>
#include <lemon/object/lsharedptr.h>
#include <lemon/audio/laudio.h>


BEGIN_L_NAMESPACE

#define L_SOUNDMANAGER_MAX_CHANNELS 24
#define L_SOUNDMANAGER_HIGH_PRIORITY_CHANNELS 5
#define L_SOUNDMANAGER_3D_CUT_OFF_DISTANCE 100
#define L_SOUNDMANAGER_3D_PAN_MAX_THRESHOLD 100
#define L_SOUNDMANAGER_3D_PAN_MIN_THRESHOLD 2

// Pre-declared classes

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSoundManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lSoundManager
{
public:

  static void                        Init();
  static void                        DeInit();
  
  static int                         Load( lIOStreamSeekable* stream, const char* filename );
  
  static bool                        Play( int soundId, bool bHighPriority=false );
  static bool                        PlayLooped( int soundId, bool bHighPriority=false );
  static void                        Stop( int soundId );
  
  static bool                        IsPlaying( int soundId );

  static void                        Set3D( int soundId, lmVector3 position, lmVector3 normal );
  static void                        SetPitch( int soundId, const int pitch );
  static void                        SetVolume( int soundId, float volume );
  static void                        SetMasterVolume( float volume );

  static void                        Free( int& soundId );

  static void                        SetReservedChannel( lAudioChannel chan );

private:

  static void                        StopChannel( int channelId );
  static bool                        AssignChannel( int soundId, int& outChannelId, bool bHighPriority=false );

};


END_L_NAMESPACE
#endif
