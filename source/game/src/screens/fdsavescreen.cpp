//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDSavescreen.cpp
// Description : FDSaveScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdsavescreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDSaveScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDSaveScreen::FDSaveScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDSaveScreen::~FDSaveScreen()
{
}

void FDSaveScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_saveScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "save_screen" );
  m_saveScreen->Hide();

  /*m_saveButton = FD_GAME_NEW FDButtonWidget( m_saveScreen, 0, "donebutton", FDResourceManager::GetSprite("ui","button75"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 50, 50 ) );
  m_saveButton->SetText( lUIText( IDS_SAVE_CAPS ) );

  m_saveButton->SignalControlCommand.Connect( this, &FDSaveScreen::OnSaveButton );*/

  m_warnText = FD_GAME_NEW lUISpriteWidget( m_saveScreen, 0, "warntext", FDResourceManager::GetSprite("processing","warn"),
                                                  LUI_TOP_LEFT, lUIPoint( 0, 0 ) );


  m_progressText = FD_GAME_NEW lUISpriteWidget( m_saveScreen, 0, "progresstext", FDResourceManager::GetSprite("processing","saving_game"),
                                                  LUI_TOP_LEFT, lUIPoint( 0, 0 ) );

  m_progressTextSaved = FD_GAME_NEW lUISpriteWidget( m_saveScreen, 0, "progresstext", FDResourceManager::GetSprite("processing","saved"),
                                                  LUI_TOP_LEFT, lUIPoint( 0, 0 ) );

  m_progressText->Hide();
  m_progressTextSaved->Hide();
  m_warnText->Hide();

  m_progressSprite = FD_GAME_NEW lUISpriteWidget( m_saveScreen, 0, "loadSpr", FDResourceManager::GetSprite("processing","processing"),
                                                  LUI_TOP_LEFT, lUIPoint( 78, 56 ) );
  m_progressSprite->Hide();

  m_progressSpriteComplete = FD_GAME_NEW lUISpriteWidget( m_saveScreen, 0, "loadSpr", FDResourceManager::GetSprite("processing","processcomplete"),
                                                  LUI_TOP_LEFT, lUIPoint( 78, 56 ) );
  m_progressSpriteComplete->Hide();
}

void FDSaveScreen::DoShow()
{
  m_saveScreen->Show();

  m_warnText->Show();
  m_progressText->Show();
  m_progressTextSaved->Hide();
  m_progressSpriteComplete->Hide();
}

void FDSaveScreen::DoHide()
{
  m_saveScreen->Hide();
}

void FDSaveScreen::OnSaveButton( lUIControlWidget *widget )
{
  //GetMainUI()->GamedataSave( this );
}

extern bool DoMainLoop();
extern bool s_skipButtonPressed;

void FDSaveScreen::SaveProgress( float pct )
{
  // do main loop
  SetProgress( pct, true );
}

static bool s_bEditMode = false;

void FDSaveScreen::SetEditMode( bool bEditMode )
{
  s_bEditMode = bEditMode;
}

void FDSaveScreen::SetProgress( float pct, bool bDoLoop )
{
  float oldTime = lPlatform_GetSeconds();

  lLogFmt("SaveScreen progress %3.f\n", pct*100.0f);

  // update frame
  int nextFrame = (m_progressSprite->GetSprite()->GetFrame() + 1);
  if( nextFrame >= m_progressSprite->GetSprite()->GetNumFrames() )
  {
    nextFrame -= m_progressSprite->GetSprite()->GetNumFrames();
  }

  m_progressSprite->GetSprite()->SetFrame( nextFrame );

  // NOTE: Need to work out what we're doing here
  char str[32];

  if( pct >= 1.0f )
  {
    m_warnText->Hide();
    m_progressSprite->Hide();
    m_progressText->Hide();
    m_progressTextSaved->Show();
    m_progressSpriteComplete->Show();
  } else
  {
    m_warnText->Show();
    m_progressSprite->Show();
    m_progressText->Show();
    m_progressTextSaved->Hide();
    m_progressSpriteComplete->Hide();
  }

  if( bDoLoop )
  {
    // render
    DoMainLoop();
    DoMainLoop();
  }

  if( pct >= 1.0f )
  {
    if( s_bEditMode )
    {
      // pause for a second on done message
      for(int i = 0; i < 60; i++)
      {
        DoMainLoop();
      }
    }
  }
}






