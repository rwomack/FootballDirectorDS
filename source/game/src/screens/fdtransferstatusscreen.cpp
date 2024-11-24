//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdnegotiatescreen.cpp
// Description : FDNegotiateScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <playersearch.h>
#include <screens/fdtransferstatusscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDNegotiateScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDTransferStatusScreen::FDTransferStatusScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDTransferStatusScreen::~FDTransferStatusScreen()
{
}

void FDTransferStatusScreen::Init()
{
  m_player = NULL;

  m_transferStatusScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "transferstartus_screen" );
  m_transferStatusScreen->Hide();

  // backing
  for(int i = 0; i < TSIFT_MAX; i++)
  {
    m_backingStripes[i] = FD_GAME_NEW lUISpriteWidget( m_transferStatusScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
                                                           lUIPoint( 20, 69 + i * 32 ), lUIPoint(256-20, 12));
    m_backingStripes[i]->GetSprite()->SetWidth( 256-20 );

    m_backingBars[i] = FD_GAME_NEW FDBarWidget( m_transferStatusScreen, 0, "bar", LUI_TOP_LEFT,
                                                  lUIPoint( 128-80,69 + i * 32 ), lUIPoint(160, 12));

    m_backingBars[i]->SetPctA( 0.0f );
    m_backingBars[i]->SetColourB( L_ARGB( 127, 80, 80, 80 ) );
  }

  m_backingStripes[TSIFT_TRANSFERLIST]->Move( lUIWidgetPosition( lUIPoint( 20, 30 ) ) );
  m_backingStripes[TSIFT_PRICE]->Move( lUIWidgetPosition( lUIPoint( 20, 70 ) ) );
  m_backingStripes[TSIFT_REJECTALLBIDS]->Move( lUIWidgetPosition( lUIPoint( 20, 110 ) ) );
  m_backingStripes[TSIFT_AVAILABLEFORLOAN]->Move( lUIWidgetPosition( lUIPoint( 20, 150 ) ) );

  m_backingBars[TSIFT_TRANSFERLIST]->Move( lUIWidgetPosition( lUIPoint( 128-80, 30 ) ) );
  m_backingBars[TSIFT_PRICE]->Move( lUIWidgetPosition( lUIPoint( 128-80, 70 ) ) );
  m_backingBars[TSIFT_REJECTALLBIDS]->Move( lUIWidgetPosition( lUIPoint( 128-80, 110 ) ) );
  m_backingBars[TSIFT_AVAILABLEFORLOAN]->Move( lUIWidgetPosition( lUIPoint( 128-80, 150 ) ) );

  // slider bars / buttons / text on bottom screen

  m_slider1 = FD_GAME_NEW FDSliderWidget( m_transferStatusScreen, 0, "slider1", FDSliderWidget::SLIDER_FLAG_POUND, lUIText(""), LUI_TOP_CENTER, lUIPoint( 128, 70+12 ) );
  m_slider1->SetSliderSprite( FDResourceManager::GetSprite("uiicon","slider_bar2") );
  m_slider1->SetValueRange( (float) MONEYSTEPS[0], (float) MONEYSTEPS[NUMMONEYSTEPS-1] );
  m_slider1->SignalSliderChange.Connect( this, &FDTransferStatusScreen::OnSlider );
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);
  m_slider1->SetLabel( lUIText( "PRICE" ) );
  m_slider1->Disable();

  m_transferList = FD_GAME_NEW FDCheckBoxWidget( m_transferStatusScreen, 0, "transferlist_check", FDResourceManager::GetSprite("uiicon","tick_0"), FDResourceManager::GetSprite("uiicon","tick_1"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 66, 30-3 ));
  m_transferList->SetText( lUIText(IDS_SCREEN48 ) );
  m_transferList->SetTextUpper();
  m_transferList->SetChecked( false );
  m_transferList->SignalControlCommand.Connect( this, &FDTransferStatusScreen::OnCheckTransferList );

  m_rejectAllBids= FD_GAME_NEW FDCheckBoxWidget( m_transferStatusScreen, 0, "rejectallbIDS_check", FDResourceManager::GetSprite("uiicon","tick_0"), FDResourceManager::GetSprite("uiicon","tick_1"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 66, 110-3 ));
  m_rejectAllBids->SetText( lUIText( IDS_REJECT_ALL_BIDS ) );
  m_rejectAllBids->SetTextUpper();
  m_rejectAllBids->SetChecked( false );
  m_rejectAllBids->SignalControlCommand.Connect( this, &FDTransferStatusScreen::OnCheckRejectAllBids );

  m_availableForLoan= FD_GAME_NEW FDCheckBoxWidget( m_transferStatusScreen, 0, "availableforloan_check", FDResourceManager::GetSprite("uiicon","tick_0"), FDResourceManager::GetSprite("uiicon","tick_1"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 66, 150-3 ));
  m_availableForLoan->SetText( lUIText( IDS_AVAIL_FOR_LOAN ) );
  m_availableForLoan->SetTextUpper();
  m_availableForLoan->SetChecked( false );
  m_availableForLoan->SignalControlCommand.Connect( this, &FDTransferStatusScreen::OnCheckLoanList );

  m_acceptButton = FD_GAME_NEW FDButtonWidget( m_transferStatusScreen, 0, "tsaccept_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPointPercent( 28, 93 ) );
  lUIText text1( IDS_ACCEPT );
  text1.ToUpper();
  m_acceptButton->SetText( text1 );
  m_acceptButton->SignalControlCommand.Connect( this, &FDTransferStatusScreen::OnAccept );

  m_cancelButton = FD_GAME_NEW FDButtonWidget( m_transferStatusScreen, 0, "tscancel_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPointPercent( 58, 93 ) );
  lUIText text2( IDS_CANCEL );
  text2.ToUpper();
  m_cancelButton->SetText( text2 );
  m_cancelButton->SignalControlCommand.Connect( this, &FDTransferStatusScreen::OnCancel );
}

void FDTransferStatusScreen::DoShow()
{
  m_transferStatusScreen->Show();
}

void FDTransferStatusScreen::DoHide()
{
  m_transferStatusScreen->Hide();
}


void FDTransferStatusScreen::Update()
{

  // update balance info
//  strPrint.Format( "Club Balance: %s", WorldData().GetCurrentUserClub()->GetCashStr() );
//  m_bankClubBalance->SetText( lUIText( strPrint ) );
}

void FDTransferStatusScreen::SetPlayer( CPlayer * player )
{
  m_player = player;

  // Initialise
  uint playerPrice = m_player->GetSellingPrice();
  //m_slider1->SetValueRange( (float) MONEYSTEPS[0], (float) MONEYSTEPS[NUMMONEYSTEPS-1] );
  float minPrice = playerPrice * 0.01f;
  if( minPrice <= 1000 )
    minPrice = 1000;

  float maxPrice = playerPrice * 5.0f;
  if( maxPrice > (float)MONEYSTEPS[NUMMONEYSTEPS-1] )
    maxPrice = (float)MONEYSTEPS[NUMMONEYSTEPS-1];

  m_slider1->SetValueRange( minPrice, maxPrice );
  m_slider1->SetValue(playerPrice);

  // reject all bids?
  bool bRejectAllBids = WorldData().GetTransferListManager().IsRejectAllOffers(m_player->DoFindID());
  m_rejectAllBids->SetChecked(bRejectAllBids);

  // transfer listed?
  bool bTransferList = WorldData().GetTransferListManager().IsTransferListed(m_player->DoFindID());
  if( bTransferList )
  {
    m_transferList->SetChecked(true);
    m_slider1->Enable();
  }
  else
  {
    m_transferList->SetChecked(false);
    m_slider1->Disable();
  }

  // loan listed?
  bool bAvailableForLoan = WorldData().GetTransferListManager().IsLoanListed(m_player->DoFindID());
  m_availableForLoan->SetChecked(bAvailableForLoan);
}

void FDTransferStatusScreen::OnAccept( lUIControlWidget * )
{
  lLogFmt("Accept pressed\n");

  // apply changes to player
  m_player->DoUpdateTransferStatus( m_transferList->IsChecked(), m_slider1->GetValue() );
  WorldData().GetTransferListManager().SetRejectAllOffers(m_player->DoFindID(), m_rejectAllBids->IsChecked());
  WorldData().GetTransferListManager().SetLoanListed(m_player->DoFindID(), m_availableForLoan->IsChecked());

  // quit dialog
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}


void FDTransferStatusScreen::OnCancel( lUIControlWidget * )
{
  lLogFmt("Cancel pressed\n");
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_CANCEL );
}

void FDTransferStatusScreen::OnCheckTransferList( lUIControlWidget * checkbox)
{
	if ( ((FDCheckBoxWidget*) checkbox)->IsChecked() )
	{
		m_slider1->Enable();
	}
	else
	{
		m_slider1->Disable();
	}
}

void FDTransferStatusScreen::OnCheckRejectAllBids( lUIControlWidget * checkbox)
{
}

void FDTransferStatusScreen::OnCheckLoanList( lUIControlWidget * checkbox )
{
}

void FDTransferStatusScreen::OnSlider( lUIWidget * slider, float amount )
{
}

