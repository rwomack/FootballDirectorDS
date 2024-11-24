//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdplayerinfoscreen.cpp
// Description : FDPlayerInfoScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdplayerinfoscreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDPlayerInfoScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDPlayerInfoScreen::FDPlayerInfoScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDPlayerInfoScreen::~FDPlayerInfoScreen()
{
}

void FDPlayerInfoScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_playerScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "player_screen_sub" );
  m_playerScreen->Hide();

  m_playerInfoTable = FD_GAME_NEW FDTableWidget( m_playerScreen, 0, "playertable",
                                  FDTableWidget::TABLE_FLAG_CELLBACKGROUND,
                                         LUI_TOP_LEFT, lUIPoint( 20, 40 ), lUIPoint( 238, 115 ) );
  m_playerInfoTable->SetRowGap( 2 );

  // type
  m_playerInfoType = FD_GAME_NEW FDComboWidget( m_playerScreen, 0, "typeselect", lUIText(""), LUI_TOP_CENTER, lUIPoint( 190, 165 ) );
  m_playerInfoType->SignalSelection.Connect( this, &FDPlayerInfoScreen::OnTypeSelect );
  m_playerInfoType->SetAllToUpper( true );

  m_playerInfoType->AddItem( lUIText( IDS_TAB0504 ) );
  m_playerInfoType->AddItem( lUIText( IDS_TAB6102) );
  m_playerInfoType->AddItem( lUIText( IDS_TAB6103 ) );
  m_playerInfoType->AddItem( lUIText( IDS_AVAILABILITY ) ); //IDS_TAB6104 - short version

  // buttons

  m_transferStatusButton = FD_GAME_NEW FDButtonWidget( m_playerScreen, 0, "transferstatus", FDResourceManager::GetSprite("uiicon","transfer_status"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 35, 175 ) );
  m_sackPlayerButton = FD_GAME_NEW FDButtonWidget( m_playerScreen, 0, "sack", FDResourceManager::GetSprite("uiicon","fire"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 65, 175 ) );
  m_renewContractButton = FD_GAME_NEW FDButtonWidget( m_playerScreen, 0, "renew", FDResourceManager::GetSprite("uiicon","offercontract"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 95, 175 ) );
  m_loanButton = FD_GAME_NEW FDButtonWidget( m_playerScreen, 0, "loan", FDResourceManager::GetSprite("uiicon","loanplayer"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 35, 175 ) );
  m_buyButton  = FD_GAME_NEW FDButtonWidget( m_playerScreen, 0, "buy", FDResourceManager::GetSprite("uiicon","buyplayer"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 65, 175 ) );

  m_transferStatusButton->SignalControlCommand.Connect( this, &FDPlayerInfoScreen::OnButton );
  m_sackPlayerButton->SignalControlCommand.Connect( this, &FDPlayerInfoScreen::OnButton );
  m_renewContractButton->SignalControlCommand.Connect( this, &FDPlayerInfoScreen::OnButton );
  m_loanButton->SignalControlCommand.Connect( this, &FDPlayerInfoScreen::OnButton );
  m_buyButton->SignalControlCommand.Connect( this, &FDPlayerInfoScreen::OnButton );

  m_messageBar = FD_GAME_NEW lUISpriteWidget(m_playerScreen, 0, "messagerBar", FDResourceManager::GetSprite("message_box_small", "message_box_small"), LUI_TOP_LEFT,
   lUIPoint( 20, 75 ));
  m_messageBar->Hide();

  m_injuryText = FD_GAME_NEW lUITextWidget( m_playerScreen, 0, "injruytext", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 105 ), lUIPoint( 228, 36 ) );
  m_injuryText->Hide();
}

void FDPlayerInfoScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    bool bPlayerValid = false;

    if( m_player != NULL )
    {
      bPlayerValid = true;
    }

    state << bPlayerValid;

    lLogFmt("Saving Player: %d\n", bPlayerValid);

    if( bPlayerValid )
    {
      int playerID = m_player->DoFindID();
      int playerInfo = m_playerInfoType->GetSelectedItem();
      lLogFmt("  saving player ID: %d, info type %d\n", playerID, playerInfo);
      state << playerID;
      state << playerInfo;
    }
  } else
  {
    bool bPlayerValid;
    state << bPlayerValid;

    lLogFmt("Loading Player: %d\n", bPlayerValid);

    if( bPlayerValid )
    {
      int playerID;
      int playerInfo;
      state << playerID;
      state << playerInfo;

      lLogFmt("  loading player ID: %d, info type %d\n", playerID, playerInfo);

      m_player = WorldData().GetPlayer( playerID );
      SetPlayerInfo( m_player );
      GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( m_player );
      m_playerInfoType->SetSelectedItem(playerInfo, false);
    } else
    {
      m_player = NULL;
      SetPlayerInfo( m_player );
      GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( m_player );
    }
  }
}


void FDPlayerInfoScreen::OnButton( lUIControlWidget *button )
{
  if( ((FDButtonWidget*) button)->IsDisabled() )
    return;

  if( button == m_transferStatusButton )
  {
    lLogFmt("Player: Transfer status\n");

    if( WorldData().GetCurrentPlayer() != NULL )
    {
      theApp.OnChangeTransferStatus();
      // ***TODO: determine if the transfer status actually changed, and take action
      // accordingly.
      GetMainUI()->ForceUpdateForAllScreensOnNextShow();
    }
  }

  if( button == m_sackPlayerButton )
  {
    lLogFmt("Player: Sack player\n");

    if( WorldData().GetCurrentPlayer() != NULL )
    {
      theApp.OnSackPlayer();

	  if (WorldData().GetCurrentPlayer()->GetClubPtr() == NULL)
	  {
		GetMainUI()->ForceUpdateForAllScreensOnNextShow();
	  }
    }
  }

  if( button == m_renewContractButton )
  {
    lLogFmt("Player: Renew contract\n");

    if( WorldData().GetCurrentPlayer() != NULL )
    {
      theApp.OnNegotiatePlayerContract();
	  // ***TODO: determine if the transfer status actually changed, and take action
	  // accordingly.
	  GetMainUI()->ForceUpdateForAllScreensOnNextShow();
    }
  }

  if( button == m_loanButton )
  {
    lLogFmt("Player: Loan player\n");
    if( WorldData().GetCurrentPlayer() != NULL )
    {
      theApp.OnApproachClubLoanInPlayer();

	  // ***TODO: determine if the transfer status actually changed, and take action
	  // accordingly.
	  GetMainUI()->ForceUpdateForAllScreensOnNextShow();
	}
  }

  if( button == m_buyButton )
  {
    lLogFmt("Player: Buy player\n");

    if( WorldData().GetCurrentPlayer() != NULL )
    {
      if( !WorldData().GetTransferListManager().IsOutOfContract(WorldData().GetCurrentPlayer()->DoFindID()) )
      {
        theApp.OnMakeOfferForPlayer();
      }
      else
      {
        theApp.OnNegotiateOutOfContractPlayerContract();
      }

      // ***TODO: determine if the transfer status actually changed, and take action
      // accordingly.
      GetMainUI()->ForceUpdateForAllScreensOnNextShow();
    }
  }

  SetPlayerInfo( m_player );
  GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( m_player );
}


void FDPlayerInfoScreen::DoShow()
{
  m_playerScreen->Show();
}

void FDPlayerInfoScreen::DoHide()
{
  m_playerScreen->Hide();
}

void FDPlayerInfoScreen::OnTypeSelect( FDComboWidget *, int index )
{
  m_playerInfoTable->ClearAll();

  m_injuryText->Hide();
  m_messageBar->Hide();

  switch( index )
  {
    // Stats
    case 0:
    {
      m_playerInfoTable->AddColumn( lUICoordPercent( 0.5f ), LUI_CENTER_CENTER );
      m_playerInfoTable->AddColumn( lUICoordPercent( 0.5f ), LUI_CENTER_CENTER );

      m_playerInfoTable->AddRows( 12 );

      m_playerInfoTable->SetTextID( 0, 0, IDS_SKILL );
      m_playerInfoTable->SetTextToUpper( 0, 0 );
      m_playerInfoTable->SetTextNumber( 0, 1, m_player->GetSkills().GetOverallSkill());
      m_playerInfoTable->SetTextID( 1, 0, IDS_TEMPER);
      m_playerInfoTable->SetTextToUpper( 1, 0 );
      m_playerInfoTable->SetText(1, 1, lUIText( m_player->TemperamentStr()) );

      m_playerInfoTable->SetTextID(2, 0, IDS_MORALE);
      m_playerInfoTable->SetTextToUpper( 2, 0 );
      m_playerInfoTable->SetTextNumber(2, 1, m_player->GetMorale());

      m_playerInfoTable->SetTextID(3, 0, IDS_DISCIPLINARYPOINTS);
      m_playerInfoTable->SetTextToUpper( 3, 0 );
      m_playerInfoTable->SetTextNumber(3, 1, m_player->GetDiscipline().GetDisciplinaryPoints());

      for(int i = 0; i < 4; i++)
      {
        m_playerInfoTable->SetTextColour(i, 1, L_ARGB(255,0,255,0) );
        m_playerInfoTable->SetBkgColour(i, 0, L_ARGB(127,130,130,130) );
        m_playerInfoTable->SetBkgColour(i, 1, L_ARGB(127,80,80,80) );
      }

      break;
    }

    // Personal
    case 1:
    {
      m_playerInfoTable->AddColumn( lUICoordPercent( 0.5f ), LUI_CENTER_CENTER );
      m_playerInfoTable->AddColumn( lUICoordPercent( 0.5f ), LUI_CENTER_CENTER );

      m_playerInfoTable->AddRows( 5 );

      m_playerInfoTable->SetTextID(0, 0, IDS_AGE);
      m_playerInfoTable->SetTextToUpper( 0, 0 );
      m_playerInfoTable->SetTextNumber(0, 1, m_player->GetAge());

      m_playerInfoTable->SetTextID(1, 0, IDS_WEEKLYWAGE);
      m_playerInfoTable->SetTextToUpper( 1, 0 );

      if( m_player->GetClubPtr() != NULL )
      {
        CCountry& rCountry = m_player->GetClub().GetCountry();
        if (WorldData().GetCurrentUserClub() != NULL)
        {
          rCountry = WorldData().GetCurrentUserClub()->GetCountry();
        }

        if( m_player->GetClubPtr()->IsUserControlled() == false )
        {
          m_playerInfoTable->SetText(1, 1, lUIText("n/a"));
        } else
        {
          m_playerInfoTable->SetText(1, 1, lUIText( rCountry.GetCurrency().GetForeignStr(m_player->GetWage())) );
        }

        m_playerInfoTable->SetTextID(2, 0, IDS_CLUBVALUATION);
        m_playerInfoTable->SetTextToUpper( 2, 0 );
        m_playerInfoTable->SetText(2, 1, lUIText( rCountry.GetCurrency().GetForeignStr(m_player->GetSellingPrice()) ) );
      } else
      {
        m_playerInfoTable->SetText( 1, 1, lUIText( "") );
        m_playerInfoTable->SetTextID(2, 0, IDS_CLUBVALUATION);
        m_playerInfoTable->SetTextToUpper( 2, 0 );
        m_playerInfoTable->SetText( 2, 1, lUIText( "") );
      }

      //m_playerInfoTable->SetTextID(3, 0, IDS_LASTCLUB);
      //m_playerInfoTable->SetText(3, 1, lUIText( m_player->LastClubName()) );

      m_playerInfoTable->SetTextID(3, 0, IDS_NATIONALITY);
      m_playerInfoTable->SetTextToUpper( 3, 0 );
      m_playerInfoTable->SetText(3, 1, lUIText( m_player->NationalityStr()) );

      m_playerInfoTable->SetText(4, 0, lUIText( "CONTRACT END" ));
      //m_playerInfoTable->SetTextToUpper( 4, 0 );

      if( m_player->GetClubPtr() != NULL )
      {
        if(m_player->GetClubPtr()->IsUserControlled() == false )
          m_playerInfoTable->SetText(4, 1, lUIText("n/a"));
        else
          m_playerInfoTable->SetText(4, 1, lUIText( m_player->ContractStr()) );
      }
      else
      {
        m_playerInfoTable->SetText(4, 1, lUIText("n/a"));
      }

      for(int i = 0; i < 5; i++)
      {
        m_playerInfoTable->SetTextColour(i, 1, L_ARGB(255,0,255,0) );
        m_playerInfoTable->SetBkgColour(i, 0, L_ARGB(127,130,130,130) );
        m_playerInfoTable->SetBkgColour(i, 1, L_ARGB(127,80,80,80) );
      }

      break;
    }

    // Career
    case 2:
    {
      m_playerInfoTable->AddColumn( lUICoordPercent( 0.25f ), LUI_CENTER_CENTER );
      m_playerInfoTable->AddColumn( lUICoordPercent( 0.5f ), LUI_CENTER_CENTER );
      m_playerInfoTable->AddColumn( lUICoordPercent( 0.25f ), LUI_CENTER_CENTER );

      m_playerInfoTable->AddRows( 6 );

      m_playerInfoTable->SetText(0, 0, lUIText( IDS_ABBR_APPEARANCES ));
      m_playerInfoTable->SetTextToUpper( 0, 0 );
      m_playerInfoTable->SetText(0, 2, lUIText( IDS_GOALS ));
      m_playerInfoTable->SetTextToUpper( 0, 2 );

      m_playerInfoTable->SetTextNumber(1, 0, m_player->GetNumberAppearancesThisSeason());
      m_playerInfoTable->SetTextID(1, 1, IDS_SEASON);
      m_playerInfoTable->SetTextToUpper( 1, 1 );
      m_playerInfoTable->SetTextNumber(1, 2, m_player->GetClubSeasonGoal());

      m_playerInfoTable->SetTextNumber(2, 0, m_player->GetLastSeasonAppear());
      m_playerInfoTable->SetTextID(2, 1, IDS_LASTSEASON);
      m_playerInfoTable->SetTextToUpper( 2, 1 );
      m_playerInfoTable->SetTextNumber(2, 2, m_player->GetLastSeasonGoal());

      m_playerInfoTable->SetTextNumber(3, 0, m_player->GetCupAppearancesThisSeason());
      m_playerInfoTable->SetTextID(3, 1, IDS_CUP);
      m_playerInfoTable->SetTextToUpper( 3, 1 );
      m_playerInfoTable->SetTextNumber(3, 2, m_player->GetCupGoalsScoredThisSeason());

      m_playerInfoTable->SetTextNumber(4, 0, m_player->GetClubAppearances());
      m_playerInfoTable->SetTextID(4, 1, IDS_CLUBS);
      m_playerInfoTable->SetTextToUpper( 4, 1 );
      m_playerInfoTable->SetTextNumber(4, 2, m_player->GetClubGoal());

      m_playerInfoTable->SetTextNumber(5, 0, m_player->GetCareerAppearances());
      m_playerInfoTable->SetTextID(5, 1, IDS_CAREER);
      m_playerInfoTable->SetTextToUpper( 5, 1 );
      m_playerInfoTable->SetTextNumber(5, 2, m_player->GetCareerGoals());

      m_playerInfoTable->SetRowBkgColour( 0, L_ARGB(127,130,130,130) );
      for(int i = 1; i < 6; i++)
      {
        m_playerInfoTable->SetTextColour(i, 0, L_ARGB(255,0,255,0) );
        m_playerInfoTable->SetTextColour(i, 2, L_ARGB(255,0,255,0) );
        m_playerInfoTable->SetBkgColour(i, 0, L_ARGB(127,80,80,80) );
        m_playerInfoTable->SetBkgColour(i, 1, L_ARGB(127,130,130,130) );
        m_playerInfoTable->SetBkgColour(i, 2, L_ARGB(127,80,80,80) );
      }

      break;
    }

    // Availability
    case 3:
    {
      int rowIdx = 0;
      m_playerInfoTable->AddColumn( lUICoordPercent( 1.0f ), LUI_TOP_CENTER );
      bool bAvailable = true;
      CString str;

      if( m_player->GetDiscipline().SuspensionStr().Size() )
      {
        bAvailable = false;
        //m_playerInfoTable->AddRows( 1 );
        //m_playerInfoTable->SetText(rowIdx++, 0, lUIText( m_player->GetDiscipline().SuspensionStr() ));
        str += m_player->GetDiscipline().SuspensionStr();
      }

      if( m_player->InjuryDetailsStr().Size() )
      {
        bAvailable = false;
        //m_playerInfoTable->AddRows( 1 );
        //m_playerInfoTable->SetText(rowIdx++, 0, lUIText( m_player->InjuryDetailsStr() ));
        str += m_player->InjuryDetailsStr();
      }

      if( m_player->HowLongUnableToPlayStr().Size() )
      {
        bAvailable = false;
        m_playerInfoTable->AddRows( 1 );
        m_playerInfoTable->SetText(rowIdx++, 0, lUIText( m_player->HowLongUnableToPlayStr() ));
      }

      if (m_player->GetWeeksOnLoan() > 0)
      {
        CString loanstr;

        switch (m_player->GetWeeksOnLoan())
        {
          case 1:
            loanstr.Format(IDS_LOANED_IN_1WEEK);
            break;

          case MAXLOANPLAYERWEEKS:
            loanstr.Format(IDS_LOANED_IN_SEASON);
            break;

          default :
            loanstr.Format( IDS_LOANED_IN, m_player->GetWeeksOnLoan());
            break;
        }

        m_playerInfoTable->AddRows( 1 );
        m_playerInfoTable->SetText(rowIdx++, 0, lUIText( loanstr.CStr() ));
      }

      if( bAvailable )
      {
        m_playerInfoTable->AddRows( 1 );
        m_playerInfoTable->SetText(rowIdx++, 0, lUIText( IDS_FULLY_AVAILABLE ));
      } else
      {
        m_messageBar->Show();
        m_injuryText->Show();
        m_injuryText->SetText( lUIText( str.CStr() ) );
      }

      for(int i = 0; i < m_playerInfoTable->GetRowCount(); i++)
      {
        m_playerInfoTable->SetRowBkgColour(i, L_ARGB(127,80,80,80) );
      }

      break;
    }
  }
}

void FDPlayerInfoScreen::SetPlayerInfo( CPlayer *player )
{
  lLogFmt("SetPlayerInfo\n");
  m_player = player;

  if( player == NULL )
  {
    WorldData().SetCurrentPlayer( NULL );
    WorldData().SetCurrentClub( NULL );
    return;
  }

  GetMainUI()->HideSubMessage();

  WorldData().SetCurrentPlayer( m_player );
  WorldData().SetCurrentClub( m_player->GetClubPtr() );

  // disable buttons based on transfers allowed
  CClub *pUserClub = WorldData().GetCurrentUserClub();
  if (pUserClub != NULL)
  {
	  if (pUserClub->AreBuyTransfersAllowed())
		  m_buyButton->Enable();
	  else
		  m_buyButton->Disable();
	  if (pUserClub->AreLoanTransfersAllowed())
		  m_loanButton->Enable();
	  else
		  m_loanButton->Disable();
  }
  else
  {
	  m_buyButton->Disable();
	  m_loanButton->Disable();
  }

  // select stats
  m_playerInfoType->SetSelectedItem( 0 );

  // setup buttons
  m_transferStatusButton->Hide();
  m_sackPlayerButton->Hide();
  m_renewContractButton->Hide();

  m_loanButton->Hide();
  m_buyButton->Hide();

	if (WorldData().GetMasterState() != CWorld::IN_PROCESSING)
	{
    if( m_player->GetContractRenewalStatus() == RETIRED )
    {
      lLogFmt("Player retired\n");
    } else
		if (WorldData().GetCurrentUser()->IsManagingAClub() == false)
		{
      // not managing, no options available
      lLogFmt("You are not managing a club\n");
		}
    else if( WorldData().GetTransferListManager().IsOutOfContract(m_player->DoFindID()) == true )
		{
		  lLogFmt("Player is out of contract\n");
		  m_buyButton->Show();
		}
    else if( m_player->IsAtCurrentUserControlledClub() == false )
		{
		  lLogFmt("Player is owned by another club\n");
      // not owned by us
			/*if (WorldData().GetCurrentUserClub()->AreBuyTransfersAllowed() == false)
			{
			  lLogFmt("Player is not transfer listed\n");
				if (WorldData().GetCurrentUserClub()->AreLoanTransfersAllowed() == false)
				{
          // no purchase options
				}
				else
				{
          // not owned by us - loan available
          lLogFmt("Player is loan listed\n");
          m_loanButton->Show();
				}
			}
			else*/
			{
        // not owned by us - can be bought / loaned

        lLogFmt("Player is transfer listed\n");
        lLogFmt("Player is loan listed\n");
        if( !WorldData().GetCurrentPlayer()->IsOnLoan() ) // prevent buttons showing for a loaned player
        {
          m_buyButton->Show();
          m_loanButton->Show();
        }
			}
    } else
    {
      // otherwise we own them
      lLogFmt("Player is owned/on our team\n");

      if( !WorldData().GetCurrentPlayer()->IsOnLoan() ) // prevent buttons showing for a loaned player
      {
        lLogFmt("Player is not on loan from another club\n");
        m_transferStatusButton->Show();
        m_sackPlayerButton->Show();
        m_renewContractButton->Show();
      } else
      {
        lLogFmt("Player is on loan from another club\n");
        //m_renewContractButton->Show();
      }
    }
	}
}
