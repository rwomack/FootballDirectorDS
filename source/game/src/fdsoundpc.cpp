//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsoundpc.cpp
// Description : FDSound implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>
#include <lemon/math/lmath.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

void FDSound::Init()
{
}

void FDSound::DeInit()
{
}


void FDSound::PlaySoundEffect( int index, int vol )
{
}

void FDSound::SetSoundEffectVolume( int volume )
{
}

void FDSound::PlayMusic( int index, bool bFadeIn )
{
}

void FDSound::SetMusicVolume( int volume )
{
}

int  FDSound::GetCurCrowdVolume()
{
  return 0;
}

void FDSound::FadeMusicWait()
{
}

bool FDSound::IsMusicFading()
{
  return false;
}

void FDSound::StopMusic()
{
}

int FDSound::GetMusicPlayingIdx()
{
  return -1;
}

void FDSound::PauseAll()
{
}

void FDSound::ResumeAll()
{
}

void FDSound::Tick(float)
{
}

void FDSound::FadeMusic()
{
}

void FDSound::ChangeCrowd( int stage )
{
}


void FDSound::CrowdEvent( int event )
{
}


bool FDSound::CrowdIsEventPlaying()
{
  return false;
}

void FDSound::SetCrowdVolume(const int iVolume)
{
}
