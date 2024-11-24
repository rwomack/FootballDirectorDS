//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDLanguageScreen.cpp
// Description : FDLanguageScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdlanguagescreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDLanguageScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDLanguageScreen::FDLanguageScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDLanguageScreen::~FDLanguageScreen()
{
}

void FDLanguageScreen::Init()
{
  // Bottom screen
  m_languageScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "languagescreen" );
  m_languageScreen->Hide();

  m_flag = FD_GAME_NEW lUISpriteWidget( m_languageScreen, 0, "flag", FDResourceManager::GetSprite("uiflags","uk"), LUI_CENTER_CENTER, lUIPointPercent( 50, 45 ));

  m_instructions = FD_GAME_NEW lUITextWidget( m_languageScreen, 0, "instr", lUIText(""), LUI_CENTER_CENTER, lUIPointPercent( 50, 20 ) );

  m_languageSelection = FD_GAME_NEW FDComboWidget( m_languageScreen,  0, "languageselection", lUIText(""), LUI_CENTER_CENTER, lUIPointPercent( 50, 70 ) );
  m_languageSelection->SignalSelection.Connect( this, &FDLanguageScreen::OnLanguageSelection );
  m_languageSelection->SetAllToUpper( true );

  // Set combo options / different languages
  lArray<int> supported = lText::GetSupportedLanguages( );
  for(int i = 0; i < supported.Size(); i++)
  {
    m_languageSelection->AddItem( lUIText( lText::GetLanguageName(supported(i)) ) );
  }

  m_acceptButton = FD_GAME_NEW FDButtonWidget( m_languageScreen, 0, "accept_button", FDResourceManager::GetSprite("ui","button45"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 50, 90 ) );
  m_acceptButton->SetText( lUIText( IDS_ACCEPT ) );
  m_acceptButton->SignalControlCommand.Connect( this, &FDLanguageScreen::OnAccept );

  // Top screen
  m_languageScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "languagescreen_sub" );
  m_languageScreenSub->Hide();

  OnLanguageSelection( NULL, m_languageSelection->GetSelectedItem() );
}

void FDLanguageScreen::DoShow()
{
  m_languageScreen->Show();
  m_languageScreenSub->Show();
}

void FDLanguageScreen::DoHide()
{
  m_languageScreen->Hide();
  m_languageScreenSub->Hide();
}

void FDLanguageScreen::Update()
{
  int currentLanguage = lText::GetLanguage();

  m_instructions->SetText( lUIText( IDS_SELECT_LANGUAGE ));

  // Set current flag
  switch( currentLanguage )
  {
    case LTEXT_LANGUAGE_UK_ENGLISH:
      m_flag->SetSprite( FDResourceManager::GetSprite("uiflags","uk") );
      break;

    case LTEXT_LANGUAGE_FRENCH:
      m_flag->SetSprite( FDResourceManager::GetSprite("uiflags","fr") );
      break;

    case LTEXT_LANGUAGE_ITALIAN:
      m_flag->SetSprite( FDResourceManager::GetSprite("uiflags","it") );
      break;

    case LTEXT_LANGUAGE_GERMAN:
      m_flag->SetSprite( FDResourceManager::GetSprite("uiflags","de") );
      break;

    case LTEXT_LANGUAGE_SPANISH:
      m_flag->SetSprite( FDResourceManager::GetSprite("uiflags","es") );
      break;
  }

  m_flag->GetSprite()->SetTransparent( false ); //HACK: this shouldn't really be needed. should be a flag in the sprite bank.

  //Set current combo selection
  //m_languageSelection->SetSelectedItem( currentLanguage, true );
}

void FDLanguageScreen::OnAccept( lUIControlWidget * )
{

}

void FDLanguageScreen::OnLanguageSelection( FDComboWidget *, int index )
{
  lLogFmt("Language Selection : %d\n", index );
  lArray<int> supported = lText::GetSupportedLanguages( );
  lText::SetLanguage( supported( index ) );
  Update();
}
