//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDProgressscreen.cpp
// Description : FDProgressScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdprogressscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDProgressScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

static FDProgressScreen *s_progressScreenInstance = NULL;

FDProgressScreen::FDProgressScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  lAssert( s_progressScreenInstance == NULL );
  s_progressScreenInstance = this;
  Init();
}

FDProgressScreen::~FDProgressScreen()
{
}

void FDProgressScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_progressScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "progress_screen" );
  m_progressScreen->Hide();

  m_progressScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "progress_screen" );
  m_progressScreenSub->Hide();

  m_progressText = FD_GAME_NEW lUISpriteWidget( m_progressScreen, 0, "progresstext", FDResourceManager::GetSprite("processing","processingtxt"),
                                                  LUI_TOP_LEFT, lUIPoint( 0, 0 ) );

  m_progressSprite = FD_GAME_NEW lUISpriteWidget( m_progressScreen, 0, "progressspr", FDResourceManager::GetSprite("processing","processing"),
                                                  LUI_TOP_LEFT, lUIPoint( 78, 56 ) );
}

void FDProgressScreen::DoShow()
{
  m_progressScreen->Show();
  m_progressScreenSub->Show();
}

void FDProgressScreen::DoHide()
{
  m_progressScreen->Hide();
  m_progressScreenSub->Hide();
}

extern bool DoMainLoop();
extern bool s_skipButtonPressed;

static void SetProgressFrame( lSharedPtr<lSprite> sprite )
{
  static float s_lastFrameTime = 0.0f;
  float newTime   = lPlatform_GetSeconds();
  float sinceLastFrameTime = (newTime - s_lastFrameTime);

  //lLogFmt("SetProgressFrame %.2f\n", sinceLastFrameTime);

  // cap frame diff
  if( sinceLastFrameTime > 0.8f )
  {
    sinceLastFrameTime = 0.5f;
  }

  if( sinceLastFrameTime >= 0.5f )
  {
    int nextFrame = (sprite->GetFrame() + 1);
    if( nextFrame >= sprite->GetNumFrames() )
    {
      nextFrame -= sprite->GetNumFrames();
    }

    sprite->SetFrame( nextFrame );

    s_lastFrameTime = newTime - (sinceLastFrameTime - 0.5f);
  }
}

void FDProgressScreen::OnIncrementProgress( float pct )
{
  // Pause for a certain amount of time each increment
  float perIncrement = 0.05f;      // 1 second

  float oldTime = lPlatform_GetSeconds();

  lLogFmt("ProgressScreen::OnIncrementProgress - %3.f%%\n", pct * 100.0f);

  if( s_skipButtonPressed )
  {
    // don't delay
    perIncrement = 0.0f;
  }

  SetProgressFrame( m_progressSprite->GetSprite() );

  // switch back to progress screen
  GetMainUI()->SwitchToScreenCombo( SCREENCOMBO_PROGRESS, false );

  while( 1 )
  {
    DoMainLoop();

    float passedTime = lPlatform_GetSeconds() - oldTime;
    if( passedTime >= perIncrement )
    {
      break;
    }
  }

  lLogFmt("ProgressScreen::OnIncrementProgress done\n");
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

void ProgressScreenUpdate()
{
  if( s_progressScreenInstance )
  {
    SetProgressFrame( s_progressScreenInstance->m_progressSprite->GetSprite() );
  }
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif



