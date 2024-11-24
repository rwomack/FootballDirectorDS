//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : fdnegotiatescreen.cpp
// Description : FDNegotiateScreen implementation
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fdtext.h>

#include <screens/fdnegotiatescreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

static CString GetForeignStr(const int _PoundsAmount)
{
	CString Temp;
	CString Newstr;
	Temp.Format("%d", _PoundsAmount);
	Temp.MakeReverse();
	int Deci = Temp.Find(".");
	int Total = 0;
	int Lenth = Temp.GetLength();
	for (int Count = 0; Count < Lenth; Count ++)
	{
		if (Count >= Deci)
		{
			if (Total == 3)
			{
				Total = 1;
				Newstr += ",";
			}
			else
			{
				Total++;
			}
		}
		Newstr += Temp.GetAt(Count);
	}
	Newstr.MakeReverse();

	// If a ridiculously large amount truncate and show as millions
	/*if (Newstr.GetLength() > 7 )
	{
		/*if (Newstr.Right(7) != (CString)"000,000")
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 4) + "t";
		}
		else
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 8) + "m";
		}
	}*/
	CString szNumBuff;

	szNumBuff.Format("%s%s", "£", Newstr);

	if (_PoundsAmount < 0)
	{
    szNumBuff = CString("-") + szNumBuff;
	}
	return szNumBuff;
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// FDNegotiateScreen
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

FDNegotiateScreen::FDNegotiateScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDNegotiateScreen::~FDNegotiateScreen()
{
}

void FDNegotiateScreen::Init()
{
  m_transferOffer = NULL;
  m_negotiations = NULL;
  m_employee = NULL;
  m_youth = NULL;
  m_club = NULL;
  m_player = NULL;
  m_eLastContractOfferResponse = MAXCONTRACTSTATUS;

  m_negotiateScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "negotiate_screen" );
  m_negotiateScreen->Hide();

  // backing stripes
  for( int i = 0; i < 3; i++ )
  {
    m_backingStripes[i] = FD_GAME_NEW lUISpriteWidget(m_negotiateScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
     lUIPoint( 0, 9 + i * 14 ), lUIPoint(256, 12));
	  m_backingStripes[i]->GetSprite()->SetWidth( 256 );

    m_backingBars[i] = FD_GAME_NEW FDBarWidget(m_negotiateScreen, 0, "bar", LUI_TOP_LEFT,
     lUIPoint( 129,9 + i * 14 ), lUIPoint(79, 12));
  }

  m_backingBars[0]->Move( lUIWidgetPosition(lUIPoint( 10, 9 ) ), lUIWidgetSize( lUIPoint( 236, 12) ) );

  for( int i = 0; i < 3; i++ )
  {
    m_backingStripes[3 + i] = FD_GAME_NEW lUISpriteWidget(m_negotiateScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
     lUIPoint( 0, 69 + i * 32 ), lUIPoint(256, 12));
	  m_backingStripes[i]->GetSprite()->SetWidth( 256 );

    m_backingBars[3 + i] = FD_GAME_NEW FDBarWidget(m_negotiateScreen, 0, "bar", LUI_TOP_LEFT,
     lUIPoint( 129,69 + i * 32 ), lUIPoint(79, 12));
  }

  for( int i = 0; i < 6; i++ )
  {
    m_backingBars[i]->SetPctA( 0.0f );
    m_backingBars[i]->SetColourB( L_ARGB( 127, 80, 80, 80 ) );
  }

  // slider bars / buttons / text on bottom screen

  m_slider1 = FD_GAME_NEW FDSliderWidget( m_negotiateScreen, 0, "slider1", 0, lUIText(""), LUI_TOP_CENTER,  lUIPoint( 127, 83 ));
  m_slider2 = FD_GAME_NEW FDSliderWidget( m_negotiateScreen, 0, "slider2", 0, lUIText(""), LUI_TOP_CENTER,  lUIPoint( 127, 83 + 32 ));
  m_slider3 = FD_GAME_NEW FDSliderWidget( m_negotiateScreen, 0, "slider3", 0, lUIText(""), LUI_TOP_CENTER,   lUIPoint( 127,83 + 64 ));
  m_slider1->SignalSliderChange.Connect( this, &FDNegotiateScreen::OnSliderChange );
  m_slider2->SignalSliderChange.Connect( this, &FDNegotiateScreen::OnSliderChange );
  m_slider3->SignalSliderChange.Connect( this, &FDNegotiateScreen::OnSliderChange );

  m_slider1->SetSliderSprite( FDResourceManager::GetSprite("uiicon","slider_bar2") );
  m_slider2->SetSliderSprite( FDResourceManager::GetSprite("uiicon","slider_bar2") );
  m_slider3->SetSliderSprite( FDResourceManager::GetSprite("uiicon","slider_bar2") );

  m_acceptButton = FD_GAME_NEW FDButtonWidget( m_negotiateScreen, 0, "accept_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 40, 90 ) );
  m_acceptButton->SetText( lUIText("ACCEPT") );
  m_acceptButton->SignalControlCommand.Connect( this, &FDNegotiateScreen::OnAccept );

  m_rejectButton = FD_GAME_NEW FDButtonWidget( m_negotiateScreen, 0, "reject_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 60, 90 ) );
  m_rejectButton->SetText( lUIText("REJECT") );
  m_rejectButton->SignalControlCommand.Connect( this, &FDNegotiateScreen::OnReject );

  m_counterButton = FD_GAME_NEW FDButtonWidget( m_negotiateScreen, 0, "counter_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 40, 90 ) );
  m_counterButton->SetText( lUIText("COUNTER") );
  m_counterButton->SignalControlCommand.Connect( this, &FDNegotiateScreen::OnCounter );

  m_continueButton = FD_GAME_NEW FDButtonWidget( m_negotiateScreen, 0, "continue_button", FDResourceManager::GetSprite("ui","button75"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 50, 90 ) );
  m_continueButton->SetText( lUIText("CONTINUE") );
  m_continueButton->SignalControlCommand.Connect( this, &FDNegotiateScreen::OnContinue );

  m_offerButton = FD_GAME_NEW FDButtonWidget( m_negotiateScreen, 0, "offer_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 40, 90 ) );
  m_offerButton->SetText( lUIText("OFFER") );
  m_offerButton->SignalControlCommand.Connect( this, &FDNegotiateScreen::OnOffer );

  m_cancelButton = FD_GAME_NEW FDButtonWidget( m_negotiateScreen, 0, "cancel_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 60, 90 ) );
  m_cancelButton->SetText( lUIText("CANCEL") );
  m_cancelButton->SignalControlCommand.Connect( this, &FDNegotiateScreen::OnCancel );

  // Accounts on top screen
  m_negotiateScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "negotiate_screen_sub" );
  m_negotiateScreenSub->Hide();

  m_messageBar = FD_GAME_NEW lUISpriteWidget( m_negotiateScreen, 0, "mbBgbar", FDResourceManager::GetSprite("message_box_small","message_box_small"), LUI_CENTER_CENTER, lUIPoint( 127, 95 ));

  m_typeText = FD_GAME_NEW lUITextWidget( m_negotiateScreen, 0, "typetext", lUIText(""), LUI_CENTER_CENTER, lUIPointPercent( 50, 35 ) );
  m_text = FD_GAME_NEW lUITextWidget( m_negotiateScreen, 0, "text", lUIText(""), LUI_CENTER_CENTER, lUIPointPercent( 50, 50 ), lUIPointPercent( 95, 40 ) );

  m_mainTitleText = FD_GAME_NEW lUITextWidget( m_negotiateScreen, 0, "titletext", lUIText(""), LUI_TOP_CENTER,  lUIPoint( 127, 9 ) );
  m_mainBidAmount = FD_GAME_NEW lUITextWidget( m_negotiateScreen, 0, "bidamount", lUIText(""), LUI_TOP_RIGHT,  lUIPoint( 118, 9 + 14 ));
  m_mainBiddingClub = FD_GAME_NEW lUITextWidget( m_negotiateScreen, 0, "biddingclub", lUIText(""), LUI_TOP_RIGHT,  lUIPoint( 118, 9 + 28 ) );

  m_mainBidAmountValue = FD_GAME_NEW lUITextWidget( m_negotiateScreen, 0, "bidamountv", lUIText(""), LUI_TOP_RIGHT,  lUIPoint( 207, 9 + 14 ), lUIPoint( 80, 12 ));
  m_mainBiddingClubValue = FD_GAME_NEW lUITextWidget( m_negotiateScreen, 0, "biddingclubv", lUIText(""), LUI_TOP_RIGHT,  lUIPoint( 207, 9 + 28 ), lUIPoint( 80, 12 ) );

  m_mainSellOnFeeAmountStripe = FD_GAME_NEW lUISpriteWidget(m_negotiateScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
     lUIPoint( 0, 101 ), lUIPoint(256, 12));
  m_mainSellOnFeeAmountStripe->GetSprite()->SetWidth(256);
  m_mainSellOnFeeAmountBar = FD_GAME_NEW lUISpriteWidget(m_negotiateScreen, 0, "bar", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT,
	 lUIPoint(127, 101), lUIPoint(80, 12));
  m_mainSellOnFeeAmount = FD_GAME_NEW lUITextWidget(m_negotiateScreen, 0, "sellonfeeamount", lUIText(""), LUI_TOP_RIGHT, lUIPoint(118, 102));
  m_mainSellOnFeeAmountValue = FD_GAME_NEW lUITextWidget(m_negotiateScreen, 0, "sellonfeeamountvalue", lUIText(""), LUI_TOP_LEFT, lUIPoint(127, 101), lUIPoint(80, 12));
  m_mainSellOnFeeAmountValue->SetAlign(LUI_CENTER_CENTER);

  //TODO LOCALE
  m_mainBidAmount->SetText( lUIText("Bidding Amount") );
  m_mainBiddingClub->SetText( lUIText("Bidding Club") );
  m_mainSellOnFeeAmount->SetText(lUIText(IDS_SELLONFEE));

  m_mainBidAmount->SetTextToUpper();
  m_mainBiddingClub->SetTextToUpper();
  m_mainSellOnFeeAmount->SetTextToUpper();

  //m_mainTitleText->SetFont( FDResourceManager::GetFont( "fontim", "fontim") );

  m_topFade = FD_GAME_NEW FDFullScreenWidget( m_negotiateScreenSub, 0, "topfade", L_ARGB( 127, 0, 0, 0 ), LUI_TOP_LEFT, lUIPoint(0, 0), lUIPoint(255, 191) );
  m_topFade->Hide();

  m_state = NEG_MAIN_SCREEN;
  m_numSliders = 0; // keeps the number of sliders when recieveing a bid/loan offer
  m_bShowSellOnFeeStuff = false;
}

void FDNegotiateScreen::DoShow()
{
  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  GetMainUI()->GetEmployeeInfoScreen()->Hide();
  GetMainUI()->GetYouthInfoScreen()->Hide();
  m_negotiateScreenSub->Show();

  if( m_state == NEG_MAIN_SCREEN )
  {
    m_messageBar->Hide();
    m_text->Hide();
    m_typeText->Hide();

    m_mainTitleText->Show();

    switch( m_numSliders )
    {
      case 3: m_slider3->Show();
      case 2: m_slider2->Show();
      case 1: m_slider1->Show();
    };

	if (m_bShowSellOnFeeStuff)
	{
		m_mainSellOnFeeAmount->Show();
		m_mainSellOnFeeAmountBar->Show();
		m_mainSellOnFeeAmountStripe->Show();
		m_mainSellOnFeeAmountValue->Show();
	}

    //m_counterButton->Move( lUIWidgetPosition( lUIPointPercent( 40, 90 ) ) );
    //m_acceptButton->Move( lUIWidgetPosition( lUIPointPercent( 40, 90 ) ) );

    // Positions sliders and show stripes/backing
    if( m_slider3->IsVisible() )
    {
      //m_slider1->Move( lUIWidgetPosition ( lUIPoint( 127, 83 ) ) );
      //m_slider2->Move( lUIWidgetPosition ( lUIPoint( 127, 83 + 32 ) ) );
      //m_slider3->Move( lUIWidgetPosition ( lUIPoint( 127, 83 + 64 ) ) );
      m_backingStripes[3]->Show();
      m_backingStripes[4]->Show();
      m_backingStripes[5]->Show();
      m_backingBars[3]->Show();
      m_backingBars[4]->Show();
      m_backingBars[5]->Show();
    }
    else if( m_slider2->IsVisible() )
    {
      //m_slider1->Move( lUIWidgetPosition ( lUIPoint( 127, 83 ) ) );
      //m_slider2->Move( lUIWidgetPosition ( lUIPoint( 127, 83 + 32 ) ) );
      m_backingStripes[3]->Show();
      m_backingStripes[4]->Show();
      m_backingBars[3]->Show();
      m_backingBars[4]->Show();
    }
    else if( m_slider1->IsVisible() )
    {
      //m_slider1->Move( lUIWidgetPosition ( lUIPoint( 127, 83 ) ) );
      m_backingStripes[3]->Show();
      m_backingBars[3]->Show();
    }

    if( m_type ==  NEGOTIATE_HIRE_EMPLOYEE || m_type == NEGOTIATE_RENEW_EMPLOYEE )
    {
      m_backingStripes[0]->Show();
      m_backingBars[0]->Show();

      GetMainUI()->GetEmployeeInfoScreen()->Show();
      GetMainUI()->GetEmployeeInfoScreen()->SetEmployeeInfo(m_employee);
      m_negotiateScreenSub->Hide();
    }
    else if( m_type == NEGOTIATE_YOUTH )
    {
      m_backingStripes[0]->Show();
      m_backingBars[0]->Show();

      m_continueButton->Hide();
      m_offerButton->Show();
      m_cancelButton->Show();

      GetMainUI()->GetYouthInfoScreen()->Show();
      GetMainUI()->GetYouthInfoScreen()->SetYouthInfo(m_youth);
      m_negotiateScreenSub->Hide();
    }

    if( m_type == NEGOTIATE_RECEIVEOFFER )
    {
      for( int i = 0; i < 3; i++ )
      {
        m_backingStripes[i]->Show();
        m_backingBars[i]->Show();
      }

      m_continueButton->Hide();
      m_acceptButton->Show();
      m_rejectButton->Show();

      m_mainBidAmount->Show();
      m_mainBiddingClub->Show();
      m_mainBidAmountValue->Show();
      m_mainBiddingClubValue->Show();
    }

    if( m_type == NEGOTIATE_PLAYER_REQUEST_TRANSFER )
    {
      m_messageBar->Show();
      m_text->Show();
    }

    if( m_type == NEGOTIATE_PLAYER_CONTRACT ||
        m_type == NEGOTIATE_RECEIVEOFFER ||
        m_type == NEGOTIATE_OUTOFCONTRACT_PLAYER_CONTRACT ||
        m_type == NEGOTIATE_PLAYER_CONTRACT_NEGOTIATIONS ||
        m_type == NEGOTIATE_MAKE_OFFER_PLAYER ||
        m_type == NEGOTIATE_APPROACH_CLUB_LOAN_IN_PLAYER ||
        m_type == NEGOTIATE_PLAYER_REQUEST_TRANSFER )
    {
      /*for( int i = 0; i < 3; i++ )
      {
        m_backingStripes[i]->Show();
        m_backingBars[i]->Show();
      }*/

      GetMainUI()->GetPlayerInfoSubScreen()->Show();
      GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(m_player);
      m_negotiateScreenSub->Hide();
    }
  }

  lLogFmt( "\n\nNegotiaton messages\n", m_typeText->GetText().str.CStr() );
  lLogFmt( "typeText = %s\n", m_typeText->GetText().str.CStr() );
  lLogFmt( "text = %s\n", m_text->GetText().str.CStr() );

  m_negotiateScreen->Show();
}

void FDNegotiateScreen::DoHide()
{
  m_negotiateScreen->Hide();
  m_negotiateScreenSub->Hide();

  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  GetMainUI()->GetEmployeeInfoScreen()->Hide();
  GetMainUI()->GetYouthInfoScreen()->Hide();
}

void FDNegotiateScreen::HideAll()
{
  m_topFade->Hide();

  m_messageBar->Hide();

  for( int i = 0; i < 6; i++ )
  {
    m_backingStripes[i]->Hide();
    m_backingBars[i]->Hide();
  }

  m_typeText->Hide();
  m_text->Hide();

  m_mainTitleText->Hide();
  m_mainBidAmount->Hide();
  m_mainBiddingClub->Hide();
  m_mainSellOnFeeAmount->Hide();
  m_mainSellOnFeeAmountValue->Hide();
  m_mainSellOnFeeAmountStripe->Hide();
  m_mainSellOnFeeAmountBar->Hide();

  m_mainBidAmountValue->Hide();
  m_mainBiddingClubValue->Hide();

  m_slider1->Hide();
  m_slider2->Hide();
  m_slider3->Hide();

  m_acceptButton->Hide();
  m_offerButton->Hide();
  m_rejectButton->Hide();
  m_counterButton->Hide();
  m_continueButton->Hide();
  m_cancelButton->Hide();

  //GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  //GetMainUI()->GetEmployeeInfoScreen()->Hide();
  //GetMainUI()->GetYouthInfoScreen()->Hide();
}

void FDNegotiateScreen::Update()
{
}

void FDNegotiateScreen::ShowMessageScreen()
{
  // Show the message, background bar and continue button, cover top screen with fullscreenwidget
  HideAll();

  // Top Screen
  m_topFade->Show();

  // Bottom
  m_messageBar->Show();
  m_text->Show();
  m_continueButton->Show();

  // Set state
  m_state = NEG_START_SCREEN;
}

eContractOfferResponse FDNegotiateScreen::GetLastContractOfferResponse() const
{
	return m_eLastContractOfferResponse;
}

void FDNegotiateScreen::ResetLastContractOfferResponse()
{
	m_eLastContractOfferResponse = MAXCONTRACTSTATUS;
}

void FDNegotiateScreen::OnSliderChange( lUIWidget *, float )
{
  // replace accept button with counter
  // hide and move
  if( m_type == NEGOTIATE_RECEIVEOFFER )
  {
    if( m_transferOffer->GetAmountOffered() != (int) m_slider1->GetValue() ||  m_transferOffer->GetSellOnPercentOffered() != (int)m_slider2->GetValue() )
    {
      if( m_acceptButton->IsVisible() )
      {
        lLogFmt("Hiding and moving buttons\n");
        m_counterButton->Move( lUIWidgetPosition( lUIPointPercent( 23, 90 ) ) );
        m_acceptButton->Hide();
        m_counterButton->Show();
      }
    }
    else
    {
      if( m_counterButton->IsVisible() )
      {
        lLogFmt("Hiding and moving buttons\n");
        m_counterButton->Hide();
        m_acceptButton->Show();
      }
    }
  }
}

void FDNegotiateScreen::OnEndNegotiations(const CString &strMessage)
{
  HideAll();

  //m_state = NEG_MAIN_SCREEN;
  m_transferOffer = NULL;

  // Display back message
  m_messageBar->Show();
  m_text->Show();
  m_text->SetText( lUIText( StripTags(strMessage.CStr()) ) );

  m_continueButton->Show();

  m_topFade->Show();

  m_negotiateScreenSub->Show();
}

void FDNegotiateScreen::OnAccept( lUIControlWidget * )
{
	CString Str;
  if( m_type == NEGOTIATE_PLAYER_REQUEST_TRANSFER )
  {
  	bool retValue = m_player->OnTransferRequestGranted(WorldData().GetCurrentUserClub()->GetCountry());
    if( retValue )
      lLogFmt("Transfer Request Granted = true\n");
    else
      lLogFmt("Transfer Request Granted = false\n");

  	// transfer request granted
  	CString strMessage;
  	if (RandomNumber.IntLessThan(10) < 6)
  	{
      strMessage.Format(IDS_REQUESTTRANSFER, m_player->GetName().CStr(), WorldData().GetCurrentUserClub()->GetName().CStr());
  	}
  	else
  	{
      strMessage.Format(IDS_UNHAPPYLISTED, m_player->GetName().CStr());
  	}
  	OnEndNegotiations(strMessage);
  } else
  {
    if( m_transferOffer->GetPlayer().IsLoanListed() == true && m_transferOffer->GetLoanPeriod() < 9)
    {
      // Note: Not sure of the right message here. using PLAYER_SIGNEDONLOAN
      Str.Format( IDS_QUESTION_ACCEPT_LOANOFFER, FDMakePlayerNameLink(&m_transferOffer->GetPlayer()).CStr(), FDMakeClubNameLink(&m_transferOffer->GetBidClub()).CStr() );
      if( UserMessageBox(Str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES )
    	{
        lLogFmt("Deal complete!!\n");
        m_transferOffer->SetDealCompleted(true);

        CString strMessage;
        strMessage.Format(IDS_PLAYER_SIGNEDONLOAN, m_transferOffer->GetBidClub().GetName().CStr(), m_transferOffer->GetPlayer().GetName().CStr(), m_transferOffer->GetLoanPeriod() );
        WorldData().OnLoanPlayerOut(m_transferOffer->GetPlayerID(), m_transferOffer->GetBidClubID(), m_transferOffer->GetLoanPeriod());
        OnEndNegotiations( strMessage );

        m_eLastContractOfferResponse = ACCEPTED;
      } else
      {
        lLogFmt("Deal not accepted\n");
		CString strMessage;
    strMessage.Format(IDS_REFUSED_BID);
        OnEndNegotiations( strMessage );

		m_eLastContractOfferResponse = REFUSED;
	  }
    }
    else
    {
      Str.Format(IDS_AGREETOSELLPLAYER, FDMakePlayerNameLink(&m_transferOffer->GetPlayer()).CStr(), FDMakeClubNameLink(&m_transferOffer->GetBidClub()).CStr());
      if( UserMessageBox(Str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES )
      {
        lLogFmt("Deal complete!!\n");
        m_transferOffer->SetDealCompleted(true);

        CString strMessage;
        strMessage.Format(IDS_PLAYER_SIGNED, m_transferOffer->GetPlayer().GetName().CStr(), m_transferOffer->GetBidClub().GetName().CStr());
        OnEndNegotiations( strMessage );

		m_eLastContractOfferResponse = ACCEPTED;
	  } else
      {
        lLogFmt("Deal not accepted\n");
		CString strMessage;
    strMessage.Format(IDS_REFUSED_BID);
        OnEndNegotiations( strMessage );

		m_eLastContractOfferResponse = REFUSED;
	  }
    }
  }
}

extern bool g_hackLastMsgResponse;

void FDNegotiateScreen::OnOffer( lUIControlWidget * )
{
  CString strMessage;

  m_eLastContractOfferResponse = MAXCONTRACTSTATUS;
  eContractOfferResponse eResponse = MAXCONTRACTSTATUS;

  if( m_type == NEGOTIATE_HIRE_EMPLOYEE )
  {
  	CContract ContractOffered;
  	ContractOffered.SetWage( (int) m_slider1->GetValue() );
  	ContractOffered.SetContractLength( (int) m_slider2->GetValue() );

    eResponse = m_employee->TryAcceptContract(m_ContractRequired, m_employee->GetClub(), ContractOffered);

  	if (eResponse == ACCEPTED)
  	{
      strMessage.Format( m_employee->szContractResponse(eResponse, ContractOffered, m_employee->GetClub()));
      WorldData().GetCurrentUserClub()->DoEmployStaff(m_employee);
      OnEndNegotiations( strMessage );
      FDSound::PlaySoundEffect( FD_SFX_UI_KERCHING );
  	}
  	else
  	{
      lLogFmt("Deal not accepted\n");
  		strMessage.Format( m_employee->szContractResponse(eResponse, ContractOffered, m_employee->GetClub()));
      OnEndNegotiations( strMessage );
  	}
  }
  else if ( m_type == NEGOTIATE_RENEW_EMPLOYEE )
  {
  	CContract ContractOffered;
  	ContractOffered.SetWage( (int) m_slider1->GetValue() );
  	ContractOffered.SetContractLength( (int) m_slider2->GetValue() );

    eResponse = m_employee->TryAcceptContract(m_ContractRequired, m_employee->GetClub(), ContractOffered);

  	if (eResponse == ACCEPTED)
  	{
      strMessage.Format( m_employee->szContractResponse(eResponse, ContractOffered, m_employee->GetClub()));
      OnEndNegotiations( strMessage );
  	}
  	else
  	{
      lLogFmt("Deal not accepted\n");
  		strMessage.Format( m_employee->szContractResponse(eResponse, ContractOffered, m_employee->GetClub()));
      OnEndNegotiations( strMessage );
  	}
  }
  else if ( m_type == NEGOTIATE_YOUTH )
  {
  	CContract ContractOffered;
    ContractOffered.SetSignOnFee( (int) m_slider1->GetValue() );
  	ContractOffered.SetWage( (int) m_slider2->GetValue() );
  	ContractOffered.SetContractLength( (int) m_slider3->GetValue() );

  	if (m_youth->GetClub().GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD)
  	{
  		eResponse = m_youth->TryAcceptContract(&m_ContractRequired,  m_youth->GetClub().GetCountry(), &ContractOffered);
  		CString szStr = m_youth->GetContractResponseStr(eResponse, &ContractOffered);
  		if (eResponse == ACCEPTED)
  		{
        g_hackLastMsgResponse = true;
  			OnEndNegotiations( szStr );
  			m_youth->GetClub().PromoteYouth(m_youth->GetClub().GetYouthList().Find(m_youth), &ContractOffered);
  			m_youth = null;		// Current Youth no longer exists
  			FDSound::PlaySoundEffect( FD_SFX_UI_KERCHING );
  		}
      else
      {
        g_hackLastMsgResponse = false;
        OnEndNegotiations( szStr );
      }
  	}
  	else
  	{
      CString szStr;
      szStr.Format( IDS_SQUADFULL );
  		OnEndNegotiations( szStr );
  	}
  }
  else if ( m_type == NEGOTIATE_PLAYER_CONTRACT )
  {
  	CContract ContractOffered;
    ContractOffered.SetSignOnFee( (int) m_slider1->GetValue() );
  	ContractOffered.SetWage( (int) m_slider2->GetValue() );
  	ContractOffered.SetContractLength( (int) m_slider3->GetValue() );

	eResponse = m_player->TryAcceptContract(&m_ContractRequired,  &ContractOffered, *m_club );
    CString szStr = m_player->GetContractResponseStr(eResponse, &ContractOffered);
    if (eResponse == ACCEPTED)
    {
      m_player->DoUpdateContractOffered(ContractOffered);
    }
    OnEndNegotiations( szStr );
  }
  else if ( m_type == NEGOTIATE_OUTOFCONTRACT_PLAYER_CONTRACT )
  {
  	CContract ContractOffered;
    ContractOffered.SetSignOnFee( (int) m_slider1->GetValue() );
  	ContractOffered.SetWage( (int) m_slider2->GetValue() );
  	ContractOffered.SetContractLength( (int) m_slider3->GetValue() );

    if (m_club->GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD)
    {
  	  eResponse = m_player->TryAcceptContract(&m_ContractRequired,  &ContractOffered, *m_club );
      CString szStr = m_player->GetContractResponseStr(eResponse, &ContractOffered);
      if (eResponse == ACCEPTED)
      {
        WorldData().GetOutOfContractPlayerList().DoSignedForClub(WorldData().GetOutOfContractPlayerList().DoFindListPosition(WorldData().GetCurrentPlayer()->DoFindID()),
			m_ContractRequired, m_club );
			FDSound::PlaySoundEffect( FD_SFX_UI_KERCHING );
      }
      OnEndNegotiations( szStr );
    }
    else
  	{
      CString szStr;
      szStr.Format(IDS_SQUADFULL);
  		OnEndNegotiations( szStr );
  	}
  }
  else if ( m_type == NEGOTIATE_PLAYER_CONTRACT_NEGOTIATIONS )
  {
  	CContract ContractOffered;
    ContractOffered.SetSignOnFee( (int) m_slider1->GetValue() );
  	ContractOffered.SetWage( (int) m_slider2->GetValue() );
  	ContractOffered.SetContractLength( (int) m_slider3->GetValue() );

 	eResponse = m_player->TryAcceptContract(&m_ContractRequired,  &ContractOffered, *m_club );
    CString szStr = m_player->GetContractResponseStr(eResponse, &ContractOffered);
    if (eResponse == ACCEPTED)
    {
      m_negotiations->SetPlayerAgreed( &ContractOffered );
      FDSound::PlaySoundEffect( FD_SFX_UI_KERCHING );
    }
    OnEndNegotiations( szStr );
  }
  else if ( m_type == NEGOTIATE_MAKE_OFFER_PLAYER )
  {
  	m_bidDetails.SetAmountOffered( (int) m_slider1->GetValue() );
    CString str;
  	if (m_player->GetClub().DoTryAcceptTransferBid(m_bidDetails) == true)
  	{
      str.Format( "%s %s", m_bidDetails.m_szReasonString, WorldData().GetCurrentUser()->AddTransferNegotiation(m_bidDetails));
	  eResponse = ACCEPTED;
    } else
    {
      str.Format( "%s", m_bidDetails.m_szReasonString );
	  eResponse = REFUSED;
    }
    OnEndNegotiations( str );
  }
  else if ( m_type == NEGOTIATE_APPROACH_CLUB_LOAN_IN_PLAYER )
  {
  	CString szResponse;
    if (WorldData().GetCurrentUserClub()->GetFirstTeamSquad().TotalSquadSize() == PLAYERSINSQUAD)
  	{
      szResponse.Format( IDS_NO_ROOM_SQUAD, m_player->GetName().CStr(), WorldData().GetCurrentUserClub()->GetName().CStr());
  	}
  	else if (WorldData().GetCurrentUserClub()->GetFirstTeamSquad().GetLoansThisSeason () == MAXPLAYERLOANSPERSEASON)
  	{
      szResponse.Format( IDS_NOPLAYERLOANSLEFT );
  	}
    else if (m_player->GetClub().TryAcceptLoanTransferBid(m_player, WorldData().GetCurrentUserClub(), m_slider1->GetValue(), &szResponse) == true)
  	{
  		m_player->DoGoingOnLoan(&m_player->GetClub(), m_slider1->GetValue());
  		ushort iPlayer = m_player->DoFindID();
  		m_player->GetClub().DoPlayerGoingOutOnLoan(iPlayer);
  		WorldData().GetCurrentUserClub()->GotPlayerOnLoan(iPlayer, m_slider1->GetValue());

		eResponse = ACCEPTED;
		FDSound::PlaySoundEffect( FD_SFX_UI_KERCHING );
    }
    OnEndNegotiations( szResponse );
  }

  m_eLastContractOfferResponse = eResponse;
}

void FDNegotiateScreen::OnReject( lUIControlWidget * )
{
  lAssert( m_player != NULL );

  m_eLastContractOfferResponse = NONEINPROGRESS;

  if( m_type == NEGOTIATE_PLAYER_REQUEST_TRANSFER )
  {
    CString strMessage = m_player->DoRespondTransferRequestDenied();
    OnEndNegotiations(strMessage);
  } else
  {
  	CString strMessage;
    strMessage.Format(IDS_BIDREJECTED, m_transferOffer->GetBidClub().GetName().CStr(), m_player->GetName().CStr());
    OnEndNegotiations( strMessage );
  }
}

void FDNegotiateScreen::OnCancel( lUIControlWidget * )
{
  m_eLastContractOfferResponse = NONEINPROGRESS;

  CString strMessage;
  strMessage.Format( IDS_NEGOTIATIONS_CANCELLED );
  if ( m_type == NEGOTIATE_YOUTH )
  {
    g_hackLastMsgResponse = false;
    m_youth->SetContractRenewalStatus(CLUBREFUSED);
    strMessage.Format( IDS_REFUSED_CONTRACT);
  }
  else if( m_type == NEGOTIATE_RENEW_EMPLOYEE )
  {
    m_employee->SetContractRenewalStatus(CLUBREFUSED);
    strMessage.Format(IDS_REFUSED_CONTRACT);
  }
  OnEndNegotiations(strMessage);
}

void FDNegotiateScreen::OnCounter( lUIControlWidget * )
{
	CString strMessage;
  if( m_transferOffer->GetPlayer().IsLoanListed() && m_transferOffer->GetLoanPeriod() < 9)
  {
  	if (m_transferOffer->GetBidClub().DoSubmitCounterLoanPeriod(m_transferOffer, m_slider1->GetValue()) == true)
  	{
      // NOTE: Until DoSubmitCounterLoanPeriod function is complete, loan period doesnt change to counter value
  		// Bidding club agree to user demands
      strMessage.Format(IDS_AGREETOCOUNTERBIDDEMANDS, m_transferOffer->GetBidClub().GetName().CStr(), m_transferOffer->GetPlayer().GetName().CStr());
  		m_text->SetText( lUIText( strMessage ) );
  		OnAccept( NULL);
  	}
    else
    {
      // TODO: Display refused message
    }
  }
  else
  {
    CClubTransferOffer CounterBidDetails(m_transferOffer);
    CounterBidDetails.SetAmountOffered( (int)m_slider1->GetValue() );
    CounterBidDetails.SetSellOnPercentOffered( (int)m_slider2->GetValue() );

    if (m_transferOffer->GetBidClub().DoSubmitCounterAskingPrice(m_transferOffer, CounterBidDetails) == true)
  	{
  		// Bidding club agree to user demands
      strMessage.Format(IDS_AGREETOCOUNTERBIDDEMANDS, m_transferOffer->GetBidClub().GetName().CStr(), m_transferOffer->GetPlayer().GetName().CStr());
      m_text->SetText( lUIText( strMessage ) );
      OnAccept( NULL );
  	}
  	else
  	{
      strMessage.Format(IDS_REFUSETOCOUNTERBIDDEMANDS, m_transferOffer->GetBidClub().GetName().CStr(), m_transferOffer->GetPlayer().GetName().CStr());
      m_text->SetText( lUIText( strMessage ) );
  	}
  }
}

void FDNegotiateScreen::OnContinue( lUIControlWidget * )
{
  lLogFmt("Continue pressed\n");
  if( m_transferOffer != NULL )
    lLogFmt("Continue: %x: bidclub %d  player %d\n", m_transferOffer, m_transferOffer->GetBidClubID().id,  m_transferOffer->GetPlayerID());
  else
    lLogFmt("Continue:\n");

  if( m_state == NEG_START_SCREEN )
  {
    m_state = NEG_MAIN_SCREEN;
    DoShow();
  }
  else
  {
    GetMainUI()->GetPlayerInfoSubScreen()->Hide();
    GetMainUI()->GetEmployeeInfoScreen()->Hide();
    GetMainUI()->GetYouthInfoScreen()->Hide();

    GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
  }
}

void FDNegotiateScreen::SetOnReceiveOffer( CClubTransferOffer* _TheOffer )
{
  lLogFmt("SetOnRecv: %x: bidclub %d  player %d\n", _TheOffer, _TheOffer->GetBidClubID().id,  _TheOffer->GetPlayerID());

  m_type          = NEGOTIATE_RECEIVEOFFER;
  m_transferOffer = _TheOffer;

  if( m_transferOffer == NULL )
  {
    lLogFmt("ERROR: Null transfer passed in\n");
    return;
  }

  lLogFmt("1\n");

  CString str;
  str.Format("%s\n%s", lText::Text(IDS_SCREEN12), m_transferOffer->GetPlayer().GetName().CStr());
  //m_typeText->SetText( lUIText( str ) );

  lLogFmt("1\n");

  m_player = &m_transferOffer->GetPlayer();
  //m_mainName->SetText( lUIText( FDMakePlayerNameLink(&m_transferOffer->GetPlayer()) ) );
  //m_mainClub->SetText( lUIText( FDMakeClubNameLink(&m_transferOffer->GetPlayer().GetClub())  ) );

  lLogFmt("1\n");

  HideAll();


  // NOTE: A club transfer offer can be a loan transfer if the player is loan listed
  // amountOffered becomes how many weeks to loan player for

  m_slider1->Show();
  m_slider1->Enable();

  if( m_transferOffer->GetPlayer().IsLoanListed() == true && m_transferOffer->GetLoanPeriod() < 9 )
  {
    m_mainBidAmount->SetText( lUIText( IDS_LENGTH_WEEKS ) );
    m_mainBidAmount->SetTextToUpper();

	CString sWeeks;
	sWeeks.Format("%d", m_transferOffer->GetLoanPeriod());
	m_mainBidAmountValue->SetText(lUIText(sWeeks.CStr()));
    m_numSliders = 0;
	m_bShowSellOnFeeStuff = false;
  }
  else
  {
    // TODO LOCALE
    m_mainBidAmount->SetText( lUIText("Bidding Amount") );
    m_mainBidAmount->SetTextToUpper();

    m_mainBidAmountValue->SetText( lUIText( GetForeignStr(m_transferOffer->GetAmountOffered() ) ) );

    CString sSellOnFee;
    sSellOnFee.Format("%d%%", (int)(m_transferOffer->GetSellOnPercentOffered()*100.0f));
    m_mainSellOnFeeAmountValue->SetText(lUIText(sSellOnFee.CStr()));
    m_numSliders = 0;

    m_bShowSellOnFeeStuff = true;
  }

  lLogFmt("1\n");

  CString strMessage;
  CString titleMessage;
  CString strValue;

  if( m_transferOffer->GetPlayer().IsLoanListed() == true && m_transferOffer->GetLoanPeriod() < 9) // Loan Offer
  {
    strMessage.Format(IDS_RECEIVELOANBID, WorldData().GetCurrentUserManager()->GetName().CStr(), m_transferOffer->GetBidClub().GetName().CStr(), m_transferOffer->GetPlayer().GetName().CStr(), m_transferOffer->GetLoanPeriod() );
    titleMessage.Format( IDS_SCREEN24 );
  }
  else
  {
    titleMessage.Format(IDS_SCREEN12);
	strValue = m_player->GetClub().GetForeignStr(m_transferOffer->GetAmountOffered());
    if (m_transferOffer->GetSellOnPercentOffered() == 0)
  	{
      strMessage.Format(IDS_ACCEPTBID, strValue, m_transferOffer->GetBidClub().GetName().CStr(), m_transferOffer->GetPlayer().GetName().CStr() );
  	}
  	else
  	{
//    CString strSellOn;
//    strSellOn.Format("%d", m_transferOffer->GetSellOnPercentOffered());
    //IDS_ACCEPTBIDSELLON "Do you want to accept the bid of %s with %d% of the fee if they sell him at a later date by %s?"
      strMessage.Format(IDS_ACCEPTBIDSELLON, strValue, m_transferOffer->GetSellOnPercentOffered(), m_transferOffer->GetBidClub().GetName().CStr());
  	}
  }

  m_mainBiddingClubValue->SetText( lUIText( m_transferOffer->GetBidClub().GetName() ) , true);

  m_text->SetText( lUIText( strMessage) );
  m_mainTitleText->SetText( lUIText( titleMessage) );
  m_mainTitleText->SetTextToUpper();

  lLogFmt("1\n");

  m_acceptButton->Show();
  m_rejectButton->Show();

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

  ShowMessageScreen();

  lLogFmt("Done\n");
}

void FDNegotiateScreen::SetOnHireEmployee( CEmployee * _employee )
{
  lLogFmt("SetOnHireEmployee\n");
  m_numSliders = 2;
  m_bShowSellOnFeeStuff = false;
  m_type          = NEGOTIATE_HIRE_EMPLOYEE;
  m_employee = _employee;

  //m_mainName->SetText( lUIText( m_employee->GetName() ) );
  //m_mainClub->SetText( lUIText( "" ) );

  m_employee->DoGenerateRandomContract( m_ContractRequired, m_employee->GetClub());

  CString str;
  str.Format("%s\n%s", lText::Text(IDS_SCREEN14), m_employee->GetName());
  //m_typeText->SetText( lUIText( str ) );

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText(  IDS_TAB0801 ) ); // Wage
  m_slider1->SetValueRange( 0.0f, m_ContractRequired.GetWage() * 1.5f );
  m_slider1->SetValue( (float)m_ContractRequired.GetWage() );
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  int maxLength = 2 + m_ContractRequired.GetContractLength();
  if( maxLength >= 6 ) maxLength = 5;

  m_slider2->Show();
  m_slider2->SetLabel( lUIText( IDS_LENGTH_SEASONS ) ); // End of date
  m_slider2->SetValueRange( 1.0f, maxLength );
  m_slider2->SetValue( m_ContractRequired.GetContractLength() );
  m_slider2->SetSliderFlags( 0 );

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

	CString strMessage;
  strMessage.Format( IDS_HIRE_EMPLOYEE );
  m_mainTitleText->SetText( lUIText( strMessage) );
  m_mainTitleText->SetTextToUpper();
  m_text->SetText("");

}

void FDNegotiateScreen::SetOnNegotiateEmployeeContract( CEmployee * _employee, const CString &_Prompt )
{
  lLogFmt("SetOnNegotiateEmployeeContract\n");
  m_numSliders = 2;
  m_bShowSellOnFeeStuff = false;
  m_type          = NEGOTIATE_RENEW_EMPLOYEE;
  m_employee = _employee;

  //m_mainName->SetText( lUIText( m_employee->GetName() ) );
  //m_mainClub->SetText( lUIText( m_employee->GetClub().GetName() ) );

  m_employee->DoGenerateRandomContract( m_ContractRequired, m_employee->GetClub());

  //CString str;
  //str.Format("%s\n%s", lText::Text(IDS_SCREEN14), m_employee->GetName());
  //m_typeText->SetText( lUIText( _Prompt ) );

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText(  IDS_TAB0801 ) ); // Wage
  m_slider1->SetValueRange( 0.0f, m_ContractRequired.GetWage() * 1.5f );
  m_slider1->SetValue( (float)m_ContractRequired.GetWage() );
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  int maxLength = 2 + m_ContractRequired.GetContractLength();
  if( maxLength >= 6 ) maxLength = 5;

  m_slider2->Show();
  m_slider2->SetLabel( lUIText( IDS_LENGTH_SEASONS ) ); // End of date
  m_slider2->SetValueRange( 1.0f, maxLength );
  m_slider2->SetValue( m_ContractRequired.GetContractLength());
  m_slider2->SetSliderFlags( 0);

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

	CString strMessage;
  strMessage.Format( IDS_NEGOTIATE_EMPLOYEE_CONTACT );
  m_mainTitleText->SetText( lUIText( strMessage) );
  m_mainTitleText->SetTextToUpper();
  m_text->SetText("");
}

void FDNegotiateScreen::SetOnPromoteYouth( CYouth * youth, const CString &prompt )
{
  lLogFmt("SetOnPromoteYouth\n");

  m_numSliders = 3;
  m_bShowSellOnFeeStuff = false;
  m_type          = NEGOTIATE_YOUTH;
  m_youth = youth;
  m_youth->DoGenerateRandomContract(m_ContractRequired, m_youth->GetClub().GetCountry());

  CString str;
  str.Format("%s\n%s", lText::Text(IDS_SCREEN59), m_youth->GetName());
  //m_typeText->SetText( lUIText( str ) );

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText( IDS_SIGN_ON_FEE ) ); //  Sign on Fee
  m_slider1->SetValueRange( 0.0f, m_ContractRequired.GetSignOnFee() * 1.5f );
  m_slider1->SetValue(m_ContractRequired.GetSignOnFee());
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  m_slider2->Show();
  m_slider2->SetLabel( lUIText(  IDS_WEEKLYWAGE ) ); // Weekly Wage
  m_slider2->SetValueRange( 0.0f, m_ContractRequired.GetWage() * 1.5f );
	m_slider2->SetValue(m_ContractRequired.GetWage());
	m_slider2->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  int maxLength = 2 + m_ContractRequired.GetContractLength();
  if( maxLength >= 6 ) maxLength = 5;

  m_slider3->Show();
  m_slider3->SetLabel( lUIText( IDS_LENGTH_SEASONS ) ); // End of date
  m_slider3->SetValueRange( 1.0f, maxLength );
	m_slider3->SetValue(m_ContractRequired.GetContractLength());

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

	CString strMessage;
  strMessage.Format(IDS_YOUTH_NEGOTIATIONS );
  m_mainTitleText->SetText( lUIText( strMessage) );
  m_mainTitleText->SetTextToUpper();
  m_text->SetText( lUIText(prompt) );

  if( prompt.Size() > 0 )
  {
    ShowMessageScreen();
    m_state = NEG_START_SCREEN;
  }
}

void FDNegotiateScreen::SetOnNegotiatePlayerContract( CPlayer * _Player, CClub * _Club, const CString &_Prompt )
{
  lLogFmt("SetOnNegotiatePlayerContract\n");
  m_numSliders = 3;
  m_bShowSellOnFeeStuff = false;
  m_type = NEGOTIATE_PLAYER_CONTRACT;
  m_player = _Player;
  m_club = _Club;
  m_player->DoGenerateRandomContract(m_ContractRequired, m_club->GetCountry());

  //CString str;
  //str.Format("%s\n%s", lText::Text(IDS_SCREEN14), m_youth->GetName());
  //m_typeText->SetText( lUIText( str ) );
  //m_typeText->SetText( lUIText( _Prompt ) );

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText( IDS_SIGN_ON_FEE ) ); //  Sign on Fee
  m_slider1->SetValueRange( 0.0f, m_ContractRequired.GetSignOnFee() * 1.5f );
  m_slider1->SetValue(m_ContractRequired.GetSignOnFee());
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  m_slider2->Show();
  m_slider2->SetLabel( lUIText(  IDS_WEEKLYWAGE ) ); // Weekly Wage
  m_slider2->SetValueRange( 0.0f, m_ContractRequired.GetWage() * 1.5f );
	m_slider2->SetValue(m_ContractRequired.GetWage());
	m_slider2->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  int maxLength = 2 + m_ContractRequired.GetContractLength();
  if( maxLength >= 6 ) maxLength = 5;

  m_slider3->Show();
  m_slider3->SetLabel( lUIText( IDS_LENGTH_SEASONS ) ); // End of date
  m_slider3->SetValueRange( 1.0f, maxLength );
	m_slider3->SetValue(m_ContractRequired.GetContractLength());

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

	CString strMessage;
  strMessage.Format( IDS_NEGOTIATE_PLAYER_CONTACT );
  m_mainTitleText->SetText( lUIText( strMessage) );
  m_mainTitleText->SetTextToUpper();
  m_text->SetText("");
}

void FDNegotiateScreen::SetOnNegotiateOutOfContractPlayerContract( CPlayer * _Player, CClub * _Club, bool _Accepted, CContract _PlayerContract )
{
  lLogFmt("SetOnNegotiateOutOfContractPlayerContract\n");
  m_numSliders = 3;
  m_bShowSellOnFeeStuff = false;
  m_type = NEGOTIATE_OUTOFCONTRACT_PLAYER_CONTRACT;
  m_player = _Player;
  m_club = _Club;
  m_ContractRequired = _PlayerContract;
  // Note: Not sure if this is the right place for it
  m_player->DoGenerateRandomContract(m_ContractRequired, m_club->GetCountry());
//  m_ContractRequired.SetContractLength(m_player->GetSkills().DoGenerateRandomContractLength(m_player->GetAge()));
//  m_ContractRequired.SetWage(m_player->GetWage());

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText( IDS_SIGN_ON_FEE ) ); //  Sign on Fee
  m_slider1->SetValueRange( 0.0f, m_ContractRequired.GetSignOnFee() * 1.5f );
  m_slider1->SetValue(m_ContractRequired.GetSignOnFee());
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  m_slider2->Show();
  m_slider2->SetLabel( lUIText(  IDS_WEEKLYWAGE ) ); // Weekly Wage
  m_slider2->SetValueRange( 0.0f, m_ContractRequired.GetWage() );
	m_slider2->SetValue(m_ContractRequired.GetWage());
	m_slider2->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  int maxLength = 2 + m_ContractRequired.GetContractLength();
  if( maxLength >= 6 ) maxLength = 5;

  m_slider3->Show();
  m_slider3->SetLabel( lUIText( IDS_LENGTH_SEASONS ) ); // End of date
  m_slider3->SetValueRange(1.0f, maxLength );
	m_slider3->SetValue(m_ContractRequired.GetContractLength());

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

	CString strMessage;
  strMessage.Format( IDS_NEGOTIATE_PLAYER_OUTOFCONTRACT );
  m_mainTitleText->SetText( lUIText( strMessage) );
  m_mainTitleText->SetTextToUpper();
  m_text->SetText("");
  m_typeText->SetText("");

  // Note: There isn't any point in displaying the initial screen
  //OnOffer(NULL);
}

void FDNegotiateScreen::SetNotOwnedPlayerContractNegotiations( CTransferNegotiation* _Negotiations )
{
  m_type = NEGOTIATE_PLAYER_CONTRACT_NEGOTIATIONS;
  m_numSliders = 3;
  m_bShowSellOnFeeStuff = false;
  m_negotiations = _Negotiations;
  m_club = &WorldData().GetClubByID(m_negotiations->GetSellClubID());
  m_player = WorldData().GetPlayer(m_negotiations->GetPlayerID());

  m_player->DoGenerateRandomContract(m_ContractRequired, m_club->GetCountry());

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText( IDS_SIGN_ON_FEE ) ); //  Sign on Fee
  m_slider1->SetValueRange( 0.0f, m_ContractRequired.GetSignOnFee() * 1.5f );
  m_slider1->SetValue(m_ContractRequired.GetSignOnFee());
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  m_slider2->Show();
  m_slider2->SetLabel( lUIText(  IDS_WEEKLYWAGE ) ); // Weekly Wage
  m_slider2->SetValueRange( 0.0f, m_ContractRequired.GetWage() * 1.5f );
	m_slider2->SetValue(m_ContractRequired.GetWage());
	m_slider2->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  int maxLength = 2 + m_ContractRequired.GetContractLength();
  if( maxLength >= 6 ) maxLength = 5;
  m_slider3->Show();

  m_slider3->SetLabel( lUIText( IDS_LENGTH_SEASONS ) ); // End of date
  m_slider3->SetValueRange( 1.0f, maxLength );
	m_slider3->SetValue(m_ContractRequired.GetContractLength());

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

	CString strMessage;
  strMessage.Format( IDS_NEGOTIATE_PLAYER_NOTOWNED );
  m_mainTitleText->SetText( lUIText( strMessage) );
  m_mainTitleText->SetTextToUpper();
  m_text->SetText("");
  m_typeText->SetText("");
}

// Club negotiaton screen 30
void FDNegotiateScreen::SetOnMakeOfferForPlayer( CPlayer * _Player )
{
  m_type = NEGOTIATE_MAKE_OFFER_PLAYER;
  m_numSliders = 2;
  m_bShowSellOnFeeStuff = false;
  m_player = _Player;
  m_club = &m_player->GetClub();

  /*m_mainName->SetText( lUIText( FDMakePlayerNameLink(m_player) ) );
  if( m_club != NULL )
    m_mainClub->SetText( lUIText( FDMakeClubNameLink(m_club) ));
  else
    m_mainClub->SetText( lUIText("") );*/

  CClubTransferOffer   bidDetails;

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText( IDS_FEE ) );
  m_slider1->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_POUND);

  const ushort usPlayerID = m_player->DoFindID();
  bool bBosman = false;
  if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == true)
  {
    lLogFmt("Player is transfer listed\n");
    const uint uiAskingPrice = WorldData().GetTransferListManager().GetAskingPrice(usPlayerID);
    m_slider1->SetValueRange( 0.0f, uiAskingPrice * 1.5f );
    m_slider1->SetValue(uiAskingPrice);
  }
  else
  {
    int valuation = m_player->GetValuation(WorldData().GetCurrentUserClub()->GetCountry());
    lLogFmt("Valuation = %d\n", valuation);
    if( valuation == 0 )
    {
      bBosman = true;
      m_slider1->SetValueRange( 0.0f, 0.0f );
      m_slider1->SetValue(0.0f);
      m_slider1->Disable();
    } else
    {
      if( valuation < 1000 ) valuation = 1000;
      m_slider1->SetValueRange( 0.0f, valuation * 1.5f );
      m_slider1->SetValue(valuation);
    }
  }

  m_slider2->Show();
  m_slider2->SetLabel( lUIText(  IDS_SELLONFEE ) );
  m_slider2->SetValueRange( 0.0f, 100.0f );
	m_slider2->SetValue(bidDetails.GetSellOnPercentOffered());
	m_slider2->SetSliderFlags(FDSliderWidget::SLIDER_FLAG_PERCENTAGE);

  bidDetails.SetPlayer(m_player);
	bidDetails.SetBidClub(WorldData().GetCurrentUserClub());
	bidDetails.SetSellOnPercentOffered(0);
  m_bidDetails = bidDetails;

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

  if( bBosman )
  {
    m_mainTitleText->SetText( lUIText( "MAKE OFFER (FREE UNDER BOSMAN)" ) );
    m_mainTitleText->SetTextToUpper();
  } else
  {
    m_mainTitleText->SetText( lUIText( IDS_MAKEOFFER ) );
    m_mainTitleText->SetTextToUpper();
  }
  m_text->SetText("");
  m_typeText->SetText( "" );
}

void FDNegotiateScreen::SetOnApproachClubLoanInPlayer( CPlayer * _Player )
{
  lLogFmt("OnApproachClubLoanInPlayer\n");
  m_type = NEGOTIATE_APPROACH_CLUB_LOAN_IN_PLAYER;

  m_numSliders = 1;
  m_bShowSellOnFeeStuff = false;
  m_player = _Player;
  m_club = &m_player->GetClub();

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_offerButton->Show();
  m_cancelButton->Show();

  m_slider1->Show();
  m_slider1->Enable();
  m_slider1->SetLabel( lUIText( IDS_LOANEDIN ) );
  m_slider1->SetValueRange( 5.0f, 11.0f ); //
	m_slider1->SetValue( 5.0f );
	m_slider1->SetSliderFlags( 0 );


  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

  m_mainTitleText->SetText( lUIText( IDS_LOANPLAYER ) );
  m_mainTitleText->SetTextToUpper();
  m_text->SetText("");
  m_typeText->SetText("");
}

void FDNegotiateScreen::SetOnPlayerRequestTransfer( CPlayer * _Player )
{
  lLogFmt("SetOnPlayerRequestTransfer\n");
  m_type = NEGOTIATE_PLAYER_REQUEST_TRANSFER;
  m_numSliders = 0;
  m_bShowSellOnFeeStuff = false;

  m_player = _Player;
  m_club = &m_player->GetClub();

  HideAll();

  for( int i = 0; i < 1; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_acceptButton->Show();
  m_rejectButton->Show();

  m_mainTitleText->Show();
  m_text->Show();
  m_typeText->Show();

  m_mainTitleText->SetText( lUIText( IDS_PLAYERTRANSFERREQUEST ) );
  m_mainTitleText->SetTextToUpper();

  lString strMessage;
  strMessage.Format("%s has requested to be placed on the transfer list, do you accept his request?", _Player->GetName().CStr() );

  m_text->SetText(lUIText(strMessage));
  m_typeText->SetText("");
}

void FDNegotiateScreen::AutoplaySkip()
{
  // Kill off negotiation process
  switch( m_type )
  {
    case NEGOTIATE_RECEIVEOFFER:
    case NEGOTIATE_PLAYER_REQUEST_TRANSFER:
      if( m_transferOffer != NULL )
      {
        OnReject(NULL);
      }
      break;

    case NEGOTIATE_MAKE_OFFER_PLAYER:
    case NEGOTIATE_YOUTH:
    case NEGOTIATE_HIRE_EMPLOYEE:
    case NEGOTIATE_RENEW_EMPLOYEE:
    case NEGOTIATE_PLAYER_CONTRACT:
    case NEGOTIATE_OUTOFCONTRACT_PLAYER_CONTRACT:
    case NEGOTIATE_PLAYER_CONTRACT_NEGOTIATIONS:
    case NEGOTIATE_APPROACH_CLUB_LOAN_IN_PLAYER:
      OnCancel(NULL);
      break;

    default:
      lLogFmt("Don't know how to skip type %d\n", m_type);
      break;
  }

  // skip continue also
  OnContinue( NULL );
}

