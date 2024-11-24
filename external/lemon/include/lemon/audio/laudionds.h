//����������������������������������������������������������������������������
// File        : laudionds.h
// Description : Wii audio interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#if !defined(L_PLATFORM_NDS)
#error laudionds.h should only be included on nds platforms
#endif

#ifndef LAUDIONDS_H_INCLUDED
#define LAUDIONDS_H_INCLUDED

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// Audio NDS defines
//����������������������������������������������������������������������������

#define L_AUDIO_NDS_DRIVER       -1

//����������������������������������������������������������������������������
// lAudioDriverDesc - Audio driver description interface
//����������������������������������������������������������������������������

class LAUDIO_API lAudioDriverDesc : public lAudioDriverDescCommon
{
public:

};

//����������������������������������������������������������������������������
// lAudioOutputDesc - Audio output description interface
//����������������������������������������������������������������������������

class LAUDIO_API lAudioOutputDesc : public lAudioOutputDescCommon
{
public:

};

//����������������������������������������������������������������������������
// lAudioDevice - Audio device interface
//����������������������������������������������������������������������������

class LAUDIO_API lAudioDevice : public lAudioDeviceCommon
{
public:

};

//����������������������������������������������������������������������������
// lAudio - NDS static audio interface
//����������������������������������������������������������������������������

class LAUDIO_API lAudioNDS : public lAudio
{
public:
};


//����������������������������������������������������������������������������
// lAudioStream - NDS static audio stream interface
//����������������������������������������������������������������������������

class lAudioStream
{
public:
  static bool  Init();
  static bool  Deinit();

  static bool  Load( const char * fileName, bool bLoop = false );
  //static bool  LoadFromMemory( uint8 *data, uint32 datasize, bool bLoop = false );

  static bool  Play();
  static bool  Pause();
  static bool  Stop();
  static bool  Reset();

  static bool  IsLoaded();
  static bool  IsPlaying();
  static bool  IsPaused();

  static void  SetLoop(bool);
  static bool  GetLoop();

  static bool  SetVolume(float);
  static float GetVolume();

  static int   Tick();
};



END_L_NAMESPACE

#endif
