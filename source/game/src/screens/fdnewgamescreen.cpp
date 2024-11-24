//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDNewGamescreen.cpp
// Description : FDNewGameScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdnewgamescreen.h>
#include <screens/fdloadscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>
#include <fdfullscreenwidget.h>
#include <fdkeyboardwidget.h>
#include <fdbackingwidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDNewGameScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDNewGameScreen::FDNewGameScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDNewGameScreen::~FDNewGameScreen()
{
}

void FDNewGameScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  // Sub Screen
  m_newgameScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "newgame_screensub" );
  m_newgameScreenSub->Hide();

  FDBackingWidget * nameBacking = FD_GAME_NEW FDBackingWidget( m_newgameScreenSub, 0, "newgame_keyboard_back", L_ARGB( 80, 45, 95, 85 ), LUI_LEFT_TOP, lUIPoint(0, 90 ), lUIPoint( 256, 35) );
  nameBacking->SetLines( 3, BLACK );
  m_newgameName = FD_GAME_NEW lUITextWidget( m_newgameScreenSub, 0, "newgamename", lUIText("_"), LUI_CENTER_TOP, lUIPoint( 128, 95 ) );
  m_newgameName->SetFont( FDResourceManager::GetFont("font26", "font26" ) );
  m_newgameName->SetShadow( true, L_ARGB( 127, 0, 0, 0 ) );


  // Main screen
  m_newgameScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "newgame_screen" );
  m_newgameScreen->Hide();

  FDBackingWidget * keyboardBacking = FD_GAME_NEW FDBackingWidget( m_newgameScreen, 0, "newgame_keyboard_back", L_ARGB( 80, 45, 95, 85 ), LUI_LEFT_TOP, lUIPoint(20, 50 ), lUIPoint( 236, 110) );
  keyboardBacking->SetLines( 3, BLACK );
  m_keyboard = FD_GAME_NEW FDKeyboardWidget(m_newgameScreen, 0, "newgame_keyboard", LUI_LEFT_TOP, lUIPoint(25, 60));
  m_keyboard->SignalKeyPressed.Connect(this, &FDNewGameScreen::OnNewGameKeyboard);

  m_newgameDone = FD_GAME_NEW FDButtonWidget( m_newgameScreen, 0, "newgame_donebutton", FDResourceManager::GetSprite("ui","button75"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint( 185, 166 ) );
  m_newgameDone->SetText( lUIText(  IDS_DONE_CAPS ) );
  m_newgameDone->Disable();
  m_newgameDone->SignalControlCommand.Connect( this, &FDNewGameScreen::OnNewGameDone );

  m_newgameBack = FD_GAME_NEW FDButtonWidget(m_newgameScreen, 0, "newgame_backbutton", FDResourceManager::GetSprite("ui", "button75"),
	  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint(68, 166));
  lUIText backText(IDS_BACK);
  backText.ToUpper();
  m_newgameBack->SetText(backText);
  m_newgameBack->SignalControlCommand.Connect(this, &FDNewGameScreen::OnNewGameBack);
}

void FDNewGameScreen::DoShow()
{
  m_newgameScreen->Show();
  m_newgameScreenSub->Show();
  m_newgameName->SetText( lUIText( "_" ) );
  m_newgameNameStr = "";
  m_keyboard->EnableCapsLock( true );
}

void FDNewGameScreen::DoHide()
{
  m_newgameScreen->Hide();
  m_newgameScreenSub->Hide();
}

void FDNewGameScreen::OnNewGameKeyboard( char cPressed )
{
    lLogFmt("Key '%c' pressed\n", cPressed);

    if( cPressed == 8 )
    {
		// backspace
		if( m_newgameNameStr.Size() )
		{
			FDSound::PlaySoundEffect( FD_SFX_UI_KEYBOARD_BACKSPACE );
			m_newgameNameStr = m_newgameNameStr.Left( m_newgameNameStr.Size() - 1 );
		}

		if( m_newgameNameStr.Size() )
		{
			// set to new text
			m_newgameName->SetText( lUIText( m_newgameNameStr.CStr() ) );
		}
		else
		{
			// set to _ to represent empty
			m_newgameName->SetText( lUIText( "_" ) );
		}
    }
	else
    {
		// set to current
		m_newgameNameStr += lString( cPressed );

		if( m_newgameNameStr.Size() > 8 )
		{
			m_newgameNameStr = m_newgameNameStr.Left( 8 );
		}

    // check that length of text on screen isnt too large
    lSharedPtr<lFont> font = m_newgameName->GetFont();
    int Width = 0, Height = 0;
    font->GetStringBound( Width, Height, m_newgameNameStr.CStr() );

    if( Width > 240 )
    {
      m_newgameNameStr = m_newgameNameStr.Left( m_newgameNameStr.Size()-1 );
    }

		m_newgameName->SetText( lUIText( m_newgameNameStr.CStr() ) );

		FDSound::PlaySoundEffect( FD_SFX_UI_KEYBOARD_CLICK );
    }

    // check that name is valid
    bool bValid = m_newgameNameStr.Size() > 0;

    if( bValid )
    {
      bool bAllSpaces = true;
      for(int i = 0; i < m_newgameNameStr.Size(); i++)
      {
        if( m_newgameNameStr(i) != ' ' )
        {
          bAllSpaces = false;
        }
      }
      if( bAllSpaces ) bValid = false;
    }

    // set text element
    if( bValid )
    {
      m_newgameDone->Enable();
    } else
    {
      m_newgameDone->Disable();
    }
}

void FDNewGameScreen::OnNewGameDone( lUIControlWidget *widget )
{
  FDButtonWidget * button = (FDButtonWidget *)widget;

  if( !button->IsDisabled() )
  {
    lLogFmt("New game done\n");

    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );

    // go to loading screen
    GetMainUI()->ShowProcessScreen( true, PROCESS_LOAD );

    // Reinit current world data (if needed)
    GetMainUI()->ReinitWorldData(true);

    // hide loading screen
    GetMainUI()->ShowProcessScreen( false, PROCESS_LOAD );

    //// Save manager name to game
    //CString strForeName = _T("@");
    //CString newGameName( m_newgameNameStr );
    //WorldData().GetCurrentUser()->SetFullName( strForeName, newGameName );

    // Go to team selection screen
    GetMainUI()->SwitchToScreenCombo( SCREENCOMBO_NEWGAME_SELECTTEAM, false );
  } else
  {
    lLogFmt("Not done - disabled\n");
  }
}

void FDNewGameScreen::OnNewGameBack( lUIControlWidget * )
{
	GetMainUI()->ClearScreenStack();
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
}
