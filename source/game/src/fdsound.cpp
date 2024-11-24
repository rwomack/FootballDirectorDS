//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsound.cpp
// Description : FDSound implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>
#include <lemon/math/lmath.h>
#include <fdsound.h>
#include "../../vendor/dsaudio/include/directsound.h"

#include "../../vendor/dsaudio/data/sfx.h"
#include "../../vendor/dsaudio/data/music.h"

#include <fdmemory.h>

using namespace L_NAMESPACE_NAME;

#define ENABLE_MUSIC
#define ENABLE_SOUND

static bool s_init = false;
static lArray<int> s_sfxMap;            // map from our enums to the sound systems enums
static lArray<int> s_musicMap;            // map from our enums to the sound systems enums
static int s_musicPlayingIdx = -1;
static int s_soundVolume = 64;
static int s_crowdVolume = 64;
static bool s_musicFading = false;

void MapSound( int fdIndex, int sfxIndex )
{
  s_sfxMap( fdIndex ) = sfxIndex;
}

void MapMusic( int fdIndex, int musicIndex )
{
  s_musicMap( fdIndex ) = musicIndex;
}

static void *SNDAlloc( u32 size, u32 align )
{
  void *ptr = FD_LOADSAVEMUSIC_NEW u8[ size ];
  //void *ptr = lAlloc( size );
  //lLogFmt("SNDAlloc: %d (align %d) = %x\n", size, align, ptr);
  return ptr;
}

static void SNDFree( void *ptr )
{
  //lLogFmt("SNDFree %x\n", ptr);
  delete [] (u8*)ptr;
  //lFree( ptr );
}

void FDSound::Init()
{
  if( ! s_init )
  {
    lLogFmt("FDSound Initialising\n");

#ifdef ENABLE_SOUND
    snd_sys_init( 0, &SNDAlloc, &SNDFree );

    snd_sng_init_s("music.bin");
    snd_sfx_init_s(4, "sfx.bin");

    s_musicPlayingIdx = -1;

    // set up mapping
    s_musicMap.Empty();
    s_musicMap.Add(FD_NUM_MUSIC);

    for(int i = 0; i < s_musicMap.Size(); i++)
    {
      // set as invalid by default
      s_musicMap(i) = -1;
    }

    MapMusic( FD_MENU_MUSIC, SNG_XM_MENU );

    // set up mapping
    s_sfxMap.Empty();
    s_sfxMap.Add(FD_NUM_SFX);

    for(int i = 0; i < s_sfxMap.Size(); i++)
    {
      // set as invalid by default
      s_sfxMap(i) = -1;
    }

    // map from sound system enums
    MapSound( FD_SFX_CROWD_CARD_APPROVAL,         SFX_CROWD_CARD_APPROVAL );
    MapSound( FD_SFX_CROWD_CARD_DONTAGREE,        SFX_CROWD_CARD_DONTAGREE );
    MapSound( FD_SFX_CROWD_CLOSESHOT1,            SFX_CROWD_CLOSESHOT1 );
    MapSound( FD_SFX_CROWD_CLOSESHOT2,            SFX_CROWD_CLOSESHOT2 );
    MapSound( FD_SFX_CROWD_DISAPPOINTED_REACTION, SFX_CROWD_DISAPPOINTED_REACTION );
    MapSound( FD_SFX_CROWD_GOAL1,                 SFX_CROWD_GOAL1 );
    MapSound( FD_SFX_CROWD_GOAL2,                 SFX_CROWD_GOAL2 );

    MapSound( FD_SFX_MATCH_PAUSE,                 SFX_MATCH_PAUSE );
    MapSound( FD_SFX_MATCH_RESUME,                SFX_MATCH_RESUME );
    MapSound( FD_SFX_REF_WHISTLE,                 SFX_REF_WHISTLE );
    MapSound( FD_SFX_REF_WHISTLE_FULLTIME,        SFX_REF_WHISTLE_FULLTIME );
    MapSound( FD_SFX_REF_WHISTLE_HALFTIME,        SFX_REF_WHISTLE_HALFTIME );
    MapSound( FD_SFX_UI_ADVANCE_TIME,             SFX_UI_ADVANCE_TIME );
    MapSound( FD_SFX_UI_BACK_BUTTON,              SFX_UI_BACK_BUTTON );
    MapSound( FD_SFX_UI_CLICK,                    SFX_UI_CLICK );
    MapSound( FD_SFX_UI_DISABLE,                  SFX_UI_DISABLE );
    MapSound( FD_SFX_UI_KERCHING,                 SFX_UI_KERCHING );
    MapSound( FD_SFX_UI_KEYBOARD_BACKSPACE,       SFX_UI_KEYBOARD_BACKSPACE );
    MapSound( FD_SFX_UI_KEYBOARD_CLICK,           SFX_UI_KEYBOARD_CLICK );
    MapSound( FD_SFX_UI_SCROLL,                   SFX_UI_SCROLL );
    MapSound( FD_SFX_UI_SCROLL_NEGATIVE,          SFX_UI_SCROLL_NEGATIVE );
    MapSound( FD_SFX_UI_SIDE_BUTTON,              SFX_UI_SIDE_BUTTON );
    MapSound( FD_SFX_VIDIPRINTER,                 SFX_VIDIPRINTER );
#endif
  }
}

void FDSound::DeInit()
{
  if( s_init )
  {
#ifdef ENABLE_SOUND
    lLogFmt("FDSound DeInitialising\n");
    snd_sys_exit();
#endif
  }
}


void FDSound::PlaySoundEffect( int index, int vol )
{
#ifdef ENABLE_SOUND
  lAssertAlways( index >= 0 && index < FD_NUM_SFX );  // check sound is within range

  if( vol == -1 )
  {
    vol = s_soundVolume;
  }

  if( s_sfxMap( index ) != -1 )
  {
    lLogFmt("FDSound::PlaySoundEffect no.%d (internal %d) - vol %d\n", index, s_sfxMap( index ), vol);
    snd_sfx_play( s_sfxMap( index ), vol, 0 );
  } else
  {
    lLogFmt("FDSound::PlaySoundEffect no.%d - no internal mapping\n", index);
  }
#endif
}

void FDSound::SetSoundEffectVolume( int volume )
{
#ifdef ENABLE_SOUND
  lLogFmt("SetSound: %d\n", volume);
  volume = lmClamp( volume, 0, 5 );

  if( volume > 0 )
  {
    lLogFmt(" vol %d - scale\n", volume);
    // scale up and add a little on to get rid of ultra low volumes
    volume = 20 + (volume * 50);
    volume = lmClamp( volume, 0, 255 );
    volume /= 4;
  }

  lLogFmt("Set vol %d\n", volume);
  s_soundVolume = volume;
  //snd_sfx_setMasterVol( volume );
#endif
}

void FDSound::PlayMusic( int index, bool bFadeIn )
{
  lAssertAlways( index >= 0 && index < FD_NUM_MUSIC );  // check music index is within range

#ifdef ENABLE_MUSIC
  // cap sfx to 4 channels as music needs 12
  snd_sfx_setChannels( 4 );

  if( s_musicMap( index ) != -1 )
  {
    // Note: Music/master volume seems to lower somewhere between starting a new game and returning to main menu
    SetMusicVolume( 5 );
    if( bFadeIn )
    {
      //snd_sng_fade( 20 ); // doesn't work on ds atm
    }
    lLogFmt("FDSound::PlayMusic no.%d (internal %d)\n", index, s_musicMap( index ));
    snd_sng_play( s_musicMap( index ) );
    s_musicPlayingIdx = index;
    s_musicFading = false;

  } else
  {
    lLogFmt("FDSound::PlayMusic no.%d - no internal mapping\n", index);
    s_musicPlayingIdx = -1;
  }
#endif
}

void FDSound::SetMusicVolume( int volume )
{
#ifdef ENABLE_SOUND
  volume = lmClamp( volume, 0, 5 );

  if( volume > 0 )
  {
    // scale up and add a little on to get rid of ultra low volumes
    volume = 20 + (volume * 50);
    volume = lmClamp( volume, 0, 255 );
  }

  snd_sng_setMasterVol( volume );
#endif
}

void FDSound::StopMusic()
{
  if( s_musicPlayingIdx != -1 )
  {
    snd_sng_stop();
    s_musicPlayingIdx = -1;
  }

  // reset sfx to have 16 channels
  snd_sfx_setChannels( 16 );
}

bool FDSound::IsMusicFading()
{
  return s_musicFading;
}

extern bool DoMainLoop();

void FDSound::FadeMusicWait()
{
  if( s_musicPlayingIdx != -1 )
  {
    // fade faster
    snd_sng_fade( -20 );
    s_musicFading = true;

    while( 1 )
    {
      DoMainLoop();
      DoMainLoop();

      if( s_musicPlayingIdx == -1 )
      {
        lLogFmt("Done fading music\n");
        return;
      }
    }
  }
}

void FDSound::FadeMusic()
{
  if( s_musicPlayingIdx != -1 )
  {
    snd_sng_fade( -10 );
    s_musicFading = true;
    //s_musicPlayingIdx = -1;
  }
}

int FDSound::GetMusicPlayingIdx()
{
  return s_musicPlayingIdx;
}

void FDSound::PauseAll()
{
#ifdef ENABLE_SOUND
  // pause all music / sounds
  snd_sng_stop();

  snd_sfx_pauseAll();
  //snd_sfx_stopAll();
#endif
}

void FDSound::ResumeAll()
{
  // resume all music / sounds
  if( s_musicPlayingIdx != -1 )
  {
    if( s_musicFading )
    {
      s_musicPlayingIdx = -1;
    } else
    {
      // only resume if not already fading
      PlayMusic( s_musicPlayingIdx );
    }
  }
  snd_sfx_resumeAll();
}

// Manage crowd mode here
// Resume crowd
//
// Start crowd at start of match

static int        s_crowdState       = CROWD_STATE_NONE;
static int        s_crowdTargetState = CROWD_STATE_NONE;

static SND_SFX_EX s_crowdSFX1;
static SND_SFX_EX s_crowdSFX2;

static float      s_crowdSfxActiveTimeRemain = 0.0f;

static float      s_crowdBlendTime   = 0.0f;
static float      s_crowdBlendLength = 0.0f;

static bool       s_crowdPlaying = false;
static bool       s_crowdPaused  = false;

static void CrowdPlay( int sfxID, int vol = 64, int pan = 0 )
{
  // setup structure
  s_crowdSFX1.fx   = sfxID;
  s_crowdSFX1.pri  = 1;
  s_crowdSFX1.vol  = vol;
  s_crowdSFX1.pan  = pan;
  s_crowdSFX1.freq = 0;

  // setup loop point
  switch( sfxID )
  {
    case SFX_CROWD_CHANT1_LOOP_837:     s_crowdSFX1.loopStart = 837;   break;
    case SFX_CROWD_CHANT2_LOOP_7015:    s_crowdSFX1.loopStart = 7015;  break;
    case SFX_CROWD_CHANT3_LOOP_1325:    s_crowdSFX1.loopStart = 1325;  break;
    case SFX_CROWD_EXCITED_LOOP_351:    s_crowdSFX1.loopStart = 351;   break;
    case SFX_CROWD_GENERAL1_LOOP_3029:  s_crowdSFX1.loopStart = 3029;  break;
    case SFX_CROWD_GENERAL1_LOOP_886:   s_crowdSFX1.loopStart = 886;   break;
    case SFX_CROWD_HAPPY_LOOP_15436:    s_crowdSFX1.loopStart = 15436; break;
    case SFX_CROWD_JUBILANT_LOOP_3554:  s_crowdSFX1.loopStart = 3554;  break;
    case SFX_CROWD_PREMATCH_LOOP_3003:  s_crowdSFX1.loopStart = 3003;  break;
    case SFX_CROWD_UPSET_LOOP_17490:    s_crowdSFX1.loopStart = 17490; break;
    default:
      s_crowdSFX1.loopStart = 0;
      break;
  }

  // play
  snd_sfx_playEx( &s_crowdSFX1 );
  s_crowdPlaying = true;
  s_crowdPaused  = false;
}

static void CrowdStop()
{
  snd_sfx_stopEx( &s_crowdSFX1 );
  ///snd_sfx_stopEx( &s_crowdSFX2 );
  s_crowdPlaying = false;
}

static void CrowdPause( bool b )
{
  if( !s_crowdPlaying ) return;

  //TODO: Check current status

  if( b )
  {
    s_crowdPaused = true;
    snd_sfx_pauseEx( &s_crowdSFX1 );
  } else
  {
    s_crowdPaused = false;
    snd_sfx_resumeEx( &s_crowdSFX1 );
  }
}


static void CrowdFadeTo( int sfxID, float time )
{
  if( s_crowdBlendLength > 0.0f )
  {
    lLogFmt("Already fading to %d (%.2f / %.2f)\n", s_crowdSFX2.fx, s_crowdBlendTime, s_crowdBlendLength);

    //if( s_crowdBlendTime >= (s_crowdBlendLength * 0.5f) )
    //{
    //
    // TODO: Switch around?
    //}

    // stop current fading to
    //snd_sfx_stopEx( &s_crowdSFX2 );
  }

  // check current sound
  snd_sfx_updateEx( &s_crowdSFX1 );

  if( s_crowdSFX1.status == 0 && s_crowdSFX1.fx == sfxID )
  {
    // sound already playing
    return;
  }

  // setup sound to fade to
//TODO
}


void FDSound::ChangeCrowd( int state )
{
#ifdef ENABLE_SOUND
  //TODO: Check if change already in progress
  if( s_crowdState == state && s_crowdTargetState == CROWD_STATE_NONE )
  {
    return;
  }

  bool  bFade   = true;                    // by default we fade to the new crowd
  float fadeLen = 0.5f;

  switch( s_crowdState )
  {
    case CROWD_STATE_NONE:
    {
      // just switch straight away
      bFade = false;
      break;
    }
  }

  // now choose the crowd sound to fade to
  int newSfxID = 0xff;

  switch( state )
  {
    case CROWD_STATE_PREMATCH:
      newSfxID = SFX_CROWD_PREMATCH_LOOP_3003;
      break;

    case CROWD_STATE_HAPPY:
      newSfxID = SFX_CROWD_HAPPY_LOOP_15436;
      break;

    case CROWD_STATE_VERY_HAPPY:
      newSfxID = SFX_CROWD_EXCITED_LOOP_351;
      //newSfxID = SFX_CROWD_JUBILANT_LOOP_3554
      break;

    case CROWD_STATE_NEUTRAL:
      newSfxID = SFX_CROWD_GENERAL1_LOOP_3029;
      //newSfxID = SFX_CROWD_GENERAL1_LOOP_886;
      break;

    case CROWD_STATE_UNHAPPY:
      newSfxID = SFX_CROWD_UPSET_LOOP_17490;
      break;

    default:
      break;
  }

  if( newSfxID == 0xff )
  {
    // no sound effect, stop crowd
    CrowdStop();
  } else
  {
    //TODOif( bFade )
    CrowdPlay( newSfxID, s_crowdVolume );
  }

  s_crowdState = state;
#endif
}

void FDSound::CrowdEvent( int event )
{
  // spawn sound over the top
  // possibly
  switch( event )
  {
    case CROWD_EVENT_CARD_GOOD:
      PlaySoundEffect( FD_SFX_CROWD_CARD_APPROVAL, s_crowdVolume );
      break;

    case CROWD_EVENT_CARD_BAD:
      PlaySoundEffect( FD_SFX_CROWD_CARD_DONTAGREE, s_crowdVolume );
      break;

    case CROWD_EVENT_CLOSESHOT:
      PlaySoundEffect( FD_SFX_CROWD_CLOSESHOT1, s_crowdVolume );
      //PlaySoundEffect( FD_SFX_CROWD_CLOSESHOT2 );
      break;


    case CROWD_EVENT_GOAL:
      PlaySoundEffect( FD_SFX_CROWD_GOAL1, s_crowdVolume );
      //PlaySoundEffect( FD_SFX_CROWD_GOAL2 );
      break;

    case CROWD_EVENT_DISAPPOINTED:
      PlaySoundEffect( FD_SFX_CROWD_DISAPPOINTED_REACTION, s_crowdVolume );
      break;
  }
}

bool FDSound::CrowdIsEventPlaying()
{
  return false;
}

void FDSound::SetCrowdVolume(const int iVolume)
{
  int volume = lmClamp( iVolume, 0, 5 );

  if( volume > 0 )
  {
    // scale up and add a little on to get rid of ultra low volumes
    volume = 20 + (volume * 50);
    volume = lmClamp( volume, 0, 255 );
    volume /= 4;
  }

  s_crowdVolume = volume;
}

int  FDSound::GetCurCrowdVolume()
{
  return s_crowdVolume;
}

void FDSound::Tick(float delta)
{
  if( snd_sng_isPaused() )
  {
    lLogFmt("Fading finished - stopping and increasing sfx channels\n");
    StopMusic();
    s_musicFading = false;
  }

  // when this expires switch to a new sample for this crowd
  s_crowdSfxActiveTimeRemain -= delta;


  // TODO spawn random chant over top
  // SFX_CROWD_CHANT1_LOOP_837,
  // SFX_CROWD_CHANT2_LOOP_7015,
  // SFX_CROWD_CHANT3_LOOP_1325,
}

