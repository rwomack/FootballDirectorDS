//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : laudiopc.h
// Description : Win32 PC audio interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#if !defined(L_PLATFORM_WINDOWS)
#error laudiopc.h should only be included on pc win32 platforms
#endif

#ifndef LAUDIOPC_H_INCLUDED
#define LAUDIOPC_H_INCLUDED

//#include <DSOUND.H>

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Audio PC defines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define L_AUDIO_DSOUND_DRIVER       -1

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioDriverDesc - Audio driver description interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LAUDIO_API lAudioDriverDesc : public lAudioDriverDescCommon
{
public:

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioOutputDesc - Audio output description interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LAUDIO_API lAudioOutputDesc : public lAudioOutputDescCommon
{
public:

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioDevice - Audio device interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LAUDIO_API lAudioDevice : public lAudioDeviceCommon
{
public:

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudio - PC static audio interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LAUDIO_API lAudioPC : public lAudio
{
public:

  static void SetSoftwareChannelsOnly();

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioStream - PC static audio stream interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lAudioStream
{
public:
  static bool  Init();
  static bool  Deinit();
  static bool  Load( const char * fileName, bool bLoop = false );
  static bool  Play();
  static bool  Pause();
  static bool  Stop();
  static bool  Reset();

  static bool  IsPlaying();
  static bool  IsPaused();

  static void  SetLoop(bool);
  static bool  GetLoop();

  static int   GetLoopCount();
  static void  SetLoopCount(int);

  static bool  SetVolume(float);
  static float GetVolume();
  static void  SetLoopStartEnd( float start, float end, bool bSeekToLoopStart = false );

  static int   Tick();

  static lAudioChannel GetChannel();
  static void  SetShouldSteal( bool b );
  static void  SetChannel( lAudioChannel chan );
};


END_L_NAMESPACE

#endif
