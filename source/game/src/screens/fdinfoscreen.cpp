//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDInfoscreen.cpp
// Description : FDInfoScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdinfoscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDInfoScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDInfoScreen::FDInfoScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDInfoScreen::~FDInfoScreen()
{
}

void FDInfoScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  m_infoScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "info_screen" );
  m_infoScreen->Hide();

  // top screen
  m_infoScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "info_screen_sub" );
  m_infoScreenSub->Hide();

  //
  m_backSprite = FD_GAME_NEW lUISpriteWidget( m_infoScreen, 0, "text", FDResourceManager::GetSprite("winlose","congratulations") );
  //m_backSprite = FD_GAME_NEW lUISpriteWidget( m_infoScreen, 0, "text", FDResourceManager::GetSprite("winlose","commiserations") );

  // info
  m_text = FD_GAME_NEW lUITextWidget( m_infoScreen, 0, "text", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 128, 115 ), lUIPoint( 200, 50 ) );
  m_text->SetFont( FDResourceManager::GetFont("font16", "font16" ) );
  m_text->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  lUITextWidget *tapText = FD_GAME_NEW lUITextWidget( m_infoScreen, 0, "taptext", lUIText("Tap the screen to continue"), LUI_CENTER_BOTTOM, lUIPoint( 128, 190 ) );
  tapText->SetTextColour( L_ARGB(255, 0, 0, 0 ) );

  m_tapScreen = FD_GAME_NEW FDFullScreenWidget( m_infoScreen, 0, "infoTap", L_ARGB( 0, 0, 0, 0 ) );
  m_tapScreen->SignalTapped.Connect( this, &FDInfoScreen::OnScreenTap );
}

void FDInfoScreen::OnScreenTap( FDFullScreenWidget * )
{
  lLogFmt("Info screen tapped\n");
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

void FDInfoScreen::DoShow()
{
  m_tapScreen->ClearTapped();
  m_tapScreen->SetTapIgnoreDelay( 2.0f );
  m_infoScreen->Show();
  m_infoScreenSub->Show();

  if( m_bPlayCheer )
  {
    FDSound::PlaySoundEffect( FD_SFX_CROWD_GOAL1 );
  }
}

void FDInfoScreen::DoHide()
{
  m_infoScreen->Hide();
  m_infoScreenSub->Hide();
}

void FDInfoScreen::SetWonMOM( lString divName )
{
  m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","congratulations") );
  lString str;
  str.Format("You are the Manager of the Month!");
  m_text->SetText( lUIText( str ) );
  m_bPlayCheer = true;
}

void FDInfoScreen::SetCupKnockedOut( lString cupName )
{
  m_bPlayCheer = false;
  m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","commiserations") );
  lString str;
  str.Format("You were knocked out of the %s", cupName.CStr());
  m_text->SetText( lUIText( str ) );
}

void FDInfoScreen::SetCupFinalResult( lString cupName, bool bWin )
{
  if( bWin )
  {
    m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","congratulations") );
    lString str;
    str.Format("You won the %s", cupName.CStr());
    m_text->SetText( lUIText( str ) );
    m_bPlayCheer = true;
  } else
  {
    m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","commiserations") );
    lString str;
    str.Format("You lost in the final of the %s", cupName.CStr());
    m_text->SetText( lUIText( str ) );
    m_bPlayCheer = false;
  }
}

void FDInfoScreen::SetEndOfSeason( lString divName, int divResult )
{
  m_bPlayCheer = false;

  switch( divResult )
  {
    case ENDSEASON_CHAMPIONS:
    {
      m_bPlayCheer = true;
      m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","congratulations") );
      lString str;
      str.Format("You finished as Champions of the %s!", divName.CStr());
      m_text->SetText( lUIText( str ) );
      break;
    }

    case ENDSEASON_PROMOTION:
    {
      m_bPlayCheer = true;
      m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","congratulations") );
      lString str;
      str.Format("You were promoted from the %s!", divName.CStr());
      m_text->SetText( lUIText( str ) );
      break;
    }

    case ENDSEASON_TOP:
    case ENDSEASON_MIDTABLE:
    {
      m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","congratulations") );
      lString str;
      str.Format("A respectable finish in the %s!", divName.CStr());
      m_text->SetText( lUIText( str ) );
      break;
    }

    case ENDSEASON_BOTTOM:
    {
      m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","congratulations") );
      lString str;
      str.Format("You avoided relegation from the %s!", divName.CStr());
      m_text->SetText( lUIText( str ) );
      break;
    }

    case ENDSEASON_RELEGATION:
    {
      m_backSprite->SetSprite( FDResourceManager::GetSprite("winlose","commiserations") );
      lString str;
      str.Format("You were relegated from the %s!", divName.CStr());
      m_text->SetText( lUIText( str ) );
      break;
    }

    default:
      m_text->SetText( lUIText("The season is over!") );
      break;
  }
}

void FDInfoScreen::AutoplaySkip()
{
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

