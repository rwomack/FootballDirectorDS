
#ifndef _FD_SOUND_H_
#define _FD_SOUND_H_

#include <lemon/lapi.h>
#include <fdsoundenums.h>

BEGIN_L_NAMESPACE

enum
{
  CROWD_STATE_NONE,
  CROWD_STATE_PREMATCH,
  CROWD_STATE_HAPPY,
  CROWD_STATE_VERY_HAPPY,
  CROWD_STATE_NEUTRAL,
  CROWD_STATE_UNHAPPY,
};

enum
{
  CROWD_EVENT_CARD_GOOD,
  CROWD_EVENT_CARD_BAD,
  CROWD_EVENT_CLOSESHOT,
  CROWD_EVENT_GOAL,
  CROWD_EVENT_DISAPPOINTED,
};

class FDSound
{
public:

  static void Init();
  static void DeInit();

  static void DoLoad();

  // sound effect control
  static void PlaySoundEffect( int index, int vol = -1 );
  static void SetSoundEffectVolume( int volume );

  // music control
  static void PlayMusic( int index, bool bFadeIn = false );
  static void StopMusic();
  static void FadeMusic();
  static void FadeMusicWait();
  static bool IsMusicFading();
  static void SetMusicVolume( int volume );
  static int  GetMusicPlayingIdx();

  // crowd sound
  static void ChangeCrowd( int state );
  static void CrowdEvent( int event );
  static bool CrowdIsEventPlaying();
  static void SetCrowdVolume(const int iVolume);
  static int  GetCurCrowdVolume();

  // general
  static void PauseAll();
  static void ResumeAll();

  static void Tick(float delta);

  static void FadeOut(float fadeTime);
  static void FadeIn(float fadeTime);
  static float GetFadeTime();
};

END_L_NAMESPACE
#endif
