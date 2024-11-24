//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDLoadscreen.cpp
// Description : FDLoadScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/FDLoadscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDLoadScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

static FDLoadScreen *s_loadScreenInstance = NULL;

FDLoadScreen::FDLoadScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  lAssert( s_loadScreenInstance == NULL );  // only one should be created
  s_loadScreenInstance = this;
  Init();
}

FDLoadScreen::~FDLoadScreen()
{
}

void FDLoadScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_loadScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "load_screen" );
  m_loadScreen->Hide();

  m_progressText = FD_GAME_NEW lUISpriteWidget( m_loadScreen, 0, "progresstext", FDResourceManager::GetSprite("processing","loading"),
                                                  LUI_TOP_LEFT, lUIPoint( 0, 0 ) );

  m_progressSprite = FD_GAME_NEW lUISpriteWidget( m_loadScreen, 0, "loadSpr", FDResourceManager::GetSprite("processing","processing"),
                                                  LUI_TOP_LEFT, lUIPoint( 78, 56 ) );
}

void FDLoadScreen::DoShow()
{
  lLogFmt("Loadscreen show\n");
  m_loadScreen->Show();
  m_bActive = true;
}

void FDLoadScreen::DoHide()
{
  m_loadScreen->Hide();
  m_bActive = false;
}

void FDLoadScreen::SetType( int processType )
{
  m_processType = processType;
  if( processType == PROCESS_LOAD )
  {
  } else
  {
  }
}

static void SetProgressFrame( lSharedPtr<lSprite> sprite )
{
  static float s_lastFrameTime = 0.0f;
  float newTime   = lPlatform_GetSeconds();
  float sinceLastFrameTime = (newTime - s_lastFrameTime);

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


void FDLoadScreen::LoadProgress( float pct )
{
  if( !m_bActive ) return;

  SetProgressFrame( m_progressSprite->GetSprite() );

  if( m_processType != PROCESS_LOAD )
  {
    //// NOTE: Need to work out what we're doing here
    //char str[32];
    //
    //if( pct >= 1.0f )
    //{
    //  //TODO LOCALE
    //  m_progressText->SetText( "Saved" );
    //} else
    //{
    //  m_progressText->SetText( lUIText(IDS_SAVINGGAME) );
    //}
    //
    //sprintf(str, "%d%%", int(pct * 100.0f) );
    //m_progressNumberText->SetText( lUIText( str ) );
  }
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

void LoadScreenUpdate()
{
  s_loadScreenInstance->LoadProgress( 0.0f );
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif







