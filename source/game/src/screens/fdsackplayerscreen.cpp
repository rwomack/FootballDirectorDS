//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsackPlayerscreen.cpp
// Description : FDSackPlayerScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdsackplayerscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDSackPlayerScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDSackPlayerScreen::FDSackPlayerScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  m_player = NULL;
  Init();
}

FDSackPlayerScreen::~FDSackPlayerScreen()
{
}

void FDSackPlayerScreen::Init()
{
  m_sackPlayerScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "sackPlayer_screen" );
  m_sackPlayerScreen->Hide();

  // buttons
  m_confirmButton = FD_GAME_NEW FDButtonWidget( m_sackPlayerScreen, 0, "confirm_button", FDResourceManager::GetSprite("ui","button75"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 30, 80 ) );
  m_confirmButton->SetText( lUIText( IDS_CONFIRM ) );
  m_confirmButton->SetTextUpper();
  m_confirmButton->SignalControlCommand.Connect( this, &FDSackPlayerScreen::OnConfirm );

  m_cancelButton = FD_GAME_NEW FDButtonWidget( m_sackPlayerScreen, 0, "cancel_button", FDResourceManager::GetSprite("ui","button75"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 70, 80 ) );
  m_cancelButton->SetText( lUIText( IDS_CANCEL  ) );
  m_cancelButton->SetTextUpper();
  m_cancelButton->SignalControlCommand.Connect( this, &FDSackPlayerScreen::OnCancel );

  m_messageBar = FD_GAME_NEW lUISpriteWidget( m_sackPlayerScreen, 0, "messagebar", FDResourceManager::GetSprite("message_box_small","message_box_small"), LUI_CENTER_CENTER, lUIPointPercent( 50, 50 ) );
  m_text = FD_GAME_NEW lUITextWidget( m_sackPlayerScreen, 0, "text", lUIText(""), LUI_CENTER_CENTER, lUIPointPercent( 50, 50 ), lUIPoint( 236, 12 ) );

  // Accounts on top screen
  m_sackPlayerScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "sackPlayer_screen_sub" );
  m_sackPlayerScreenSub->Hide();

  m_fadeSub = FD_GAME_NEW FDFullScreenWidget( m_sackPlayerScreenSub, 0, "fade", L_ARGB( 127, 0, 0, 0 ) );

}

void FDSackPlayerScreen::DoShow()
{
  m_sackPlayerScreen->Show();
  m_sackPlayerScreenSub->Show();
}

void FDSackPlayerScreen::DoHide()
{
  m_sackPlayerScreen->Hide();
  m_sackPlayerScreenSub->Hide();
}

void FDSackPlayerScreen::SetPlayer( CPlayer *player )
{
  lAssert( player != NULL );
  m_player = player;

  uint ContractValue = m_player->ContractValue();

	CString str;
  str.Format(IDS_SACKPLAYER, m_player->GetName(), m_player->ContractValueStr(m_player->GetClub().GetCountry()));
  m_text->SetText( lUIText( str ) );

}

void FDSackPlayerScreen::Update()
{
  // update balance info
//  strPrint.Format( "Club Balance: %s", WorldData().GetCurrentUserClub()->GetCashStr() );
//  m_bankClubBalance->SetText( lUIText( strPrint ) );
}


extern bool g_hackLastMsgResponse;

void FDSackPlayerScreen::OnConfirm( lUIControlWidget * )
{
  lAssert( m_player != NULL );
	// Sack/Release the player
  lLogFmt("Confirm pressed\n");
  m_player->GetClub().SackPlayer(m_player);
  g_hackLastMsgResponse = true;

  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

void FDSackPlayerScreen::OnCancel( lUIControlWidget * )
{
  lLogFmt("Cancel pressed\n");
  g_hackLastMsgResponse = false;
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_CANCEL );
}
