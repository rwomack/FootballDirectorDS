//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchDaySubsscreen.cpp
// Description : FDMatchDaySubsScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fddragwidget.h>

#include <screens/fdmatchdayscreen.h>
#include <screens/fdstrategyformationscreen.h>
#include <screens/fdstrategyrolesscreen.h>
#include <screens/fdtacticsscreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchDaySubsScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMatchDaySubsScreen::FDMatchDaySubsScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDMatchDaySubsScreen::~FDMatchDaySubsScreen()
{
}

void FDMatchDaySubsScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  int colA = L_ARGB( 255, 29, 9, 208 );
  int colB = L_ARGB( 255, 211, 108, 5 );

  //  - Subs sheet screen
  m_matchSubsScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "match_team_screen" );
  m_matchSubsScreen->Hide();

  m_teamtable = FD_GAME_NEW FDTableWidget( m_matchSubsScreen, 0, "teamtable",
            FDTableWidget::TABLE_FLAG_ROW_SELECT |
			FDTableWidget::TABLE_FLAG_BACKGROUND | FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 20, 0 ), lUIPoint( 118, 192 ) );

  m_substable = FD_GAME_NEW FDTableWidget( m_matchSubsScreen, 0, "substable",
            FDTableWidget::TABLE_FLAG_ROW_SELECT |
			FDTableWidget::TABLE_FLAG_BACKGROUND | FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 138, 0 ), lUIPoint( 118, 192 ) );

  m_teamtable->SignalRowSelect.Connect( this, &FDMatchDaySubsScreen::OnRowSelect );
  m_substable->SignalRowSelect.Connect( this, &FDMatchDaySubsScreen::OnRowSelect );


  // buttons
  m_tacticsButton = FD_GAME_NEW FDButtonWidget( m_matchSubsScreen, 0, "tactics_button",
    FDResourceManager::GetSprite("uiicon", "tacticsbutton"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 130, 175 ));
  m_tacticsButton->SignalControlCommand.Connect(this, &FDMatchDaySubsScreen::OnTacticsButton);

  m_formationsButton = FD_GAME_NEW FDButtonWidget( m_matchSubsScreen, 0, "formation_button",
	  FDResourceManager::GetSprite("uiicon", "formationbutton"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 165, 175 ));
  m_formationsButton->SignalControlCommand.Connect(this, &FDMatchDaySubsScreen::OnFormationsButton);

  m_rolesButton = FD_GAME_NEW FDButtonWidget( m_matchSubsScreen, 0, "roles_button",
	  FDResourceManager::GetSprite("uiicon", "rolesbutton"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 200, 175 ));
  m_rolesButton->SignalControlCommand.Connect(this, &FDMatchDaySubsScreen::OnRolesButton);

  m_swapButton = FD_GAME_NEW FDButtonWidget( m_matchSubsScreen, 0, "swap_button",
	  FDResourceManager::GetSprite("uiicon", "swap_player"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 185, 120 ));
  m_swapButton->SignalControlCommand.Connect(this, &FDMatchDaySubsScreen::OnSwapButton);

  m_subsRemainingText = FD_GAME_NEW lUITextWidget( m_matchSubsScreen, 0, "remaingSubs", lUIText(""), LUI_LEFT_CENTER, lUIPoint( 150, 90 ) );

  m_selectedPlayerIdx = -1;
	m_bSwap = false;
}

void FDMatchDaySubsScreen::OnFormationsButton(lUIControlWidget *widget)
{
	CClub& CurrentClub = m_matchPlay->GetCurrentUserControlledClub();
	GetMainUI()->GetStrategyFormationScreen()->SetMatchPlay(m_matchPlay);
	GetMainUI()->GetStrategyFormationScreen()->SetClub(&CurrentClub);
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_STRATEGY_FORMATION, false);
}

void FDMatchDaySubsScreen::OnRolesButton(lUIControlWidget *widget)
{
	CClub& CurrentClub = m_matchPlay->GetCurrentUserControlledClub();
	GetMainUI()->GetStrategyRolesScreen()->SetMatchPlay(m_matchPlay);
	GetMainUI()->GetStrategyRolesScreen()->SetClub(&CurrentClub);
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_STRATEGY_ROLES, false);
}

void FDMatchDaySubsScreen::OnTacticsButton(lUIControlWidget *widget)
{
	CClub& CurrentClub = m_matchPlay->GetCurrentUserControlledClub();
	GetMainUI()->GetTacticsScreen()->SetMatchDetails( m_matchPlay );
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_TACTICS_IN_MATCH, false);
}

void FDMatchDaySubsScreen::OnSwapButton(lUIControlWidget *widget)
{
	// Subs disallowed before the match starts.
	if (m_matchPlay->GetTimePlayed() <= 0)
	{
		FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
		return;
	}

	if( m_bSwap )
	{
    m_bSwap = false;
	  m_swapButton->SetSprite(FDResourceManager::GetSprite("uiicon","swap_player"));
	  m_swapButton->GetSprite()->SetColour( L_ARGB( 255, 80, 80, 80 ) );
	}
	else
	{
	  m_bSwap = true;
		m_swapButton->SetSprite(FDResourceManager::GetSprite("uiicon","swap_player_activated"));
	}

	// Reset selection each time
	//m_teamtable->SelectRow( -1 );
	//m_substable->SelectRow( -1 );
}

void FDMatchDaySubsScreen::OnRowSelect(FDTableWidget * pTable, int row)
{
  if( pTable == m_substable )
  {
    lLogFmt(" Subs table row selected %d \n", row);
    m_selectedSubPlayerIdx = row;
    if( !m_bSwap )
    {
      m_selectedPlayerIdx = -1;
      m_teamtable->SelectRow( -1, true );
      m_swapButton->GetSprite()->SetColour( L_ARGB( 255, 255, 255, 255 ) );
    }
  }

  if( pTable == m_teamtable )
  {
    lLogFmt(" Player table row selected %d\n", row);
    m_selectedPlayerIdx = row;
    if( !m_bSwap )
    {
      m_selectedSubPlayerIdx = -1;
      m_substable->SelectRow( -1, true );
      m_swapButton->GetSprite()->SetColour( L_ARGB( 255, 255, 255, 255 ) );
    }
  }

	if( m_bSwap && ( m_selectedPlayerIdx != -1 && m_selectedSubPlayerIdx != -1 ) )
	{
	  int iRow = m_selectedPlayerIdx;
		int iOtherRow = m_selectedSubPlayerIdx;

		if ( iOtherRow >= 1)
		{
			// Swap players.
			lLogFmt("  Swapping %d with %d\n", iOtherRow - 1,
											iRow - 1 );

			const int iMainSquadPlayer = m_teamtable->GetTagDataForRow(iRow);

			eSubstitutionAction eAction = m_matchPlay->DoSwapPlayers( m_matchPlay->GetCurrentUserVenue(), iMainSquadPlayer );
			bool bNeedToCancel = false;
			switch (eAction)
			{
			case eNOSUBSLEFT:
				FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
				break;
			case eSWAP1SET:
			case eSWAPPEDONPITCH:
			case eSWAPPEDONBENCH:
			case eSUBSTITUTIONMADE:
				bNeedToCancel = true;
				break;
			case eSUBSTITUTIONCANCELLED:
				bNeedToCancel = true;
				break;
			default:
				EXT_ASSERT(false);
			};

			if (bNeedToCancel == false)
			{
				m_matchPlay->DoSwapPlayers(m_matchPlay->GetCurrentUserVenue(), NOPLAYER);
				return;
			}

			bNeedToCancel = false;
			const int iSubPlayer = m_substable->GetTagDataForRow(iOtherRow);
			eSubstitutionAction eAction2 = m_matchPlay->DoSwapPlayers( m_matchPlay->GetCurrentUserVenue(), iSubPlayer );
			switch (eAction2)
			{
			case eNOSUBSLEFT:
				FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
				break;
			case eSWAP1SET:
			case eSWAPPEDONPITCH:
			case eSWAPPEDONBENCH:
			case eSUBSTITUTIONMADE:
				bNeedToCancel = true;
				break;
			case eSUBSTITUTIONCANCELLED:
				bNeedToCancel = true;
				break;
			default:
				EXT_ASSERT(false);
			};

			if (bNeedToCancel == false)
			{
				m_matchPlay->DoSwapPlayers(m_matchPlay->GetCurrentUserVenue(), NOPLAYER);
				return;
			}

		//    SubsInfo().MakeSubstitution( m_matchPlay->GetTimePlayed(), m_matchPlay->GetCurrentUserVenue(),
		//                                 m_teamtable->GetSelectedRow(), m_substable->GetSelectedRow() + PLAYERSINTEAM );
			Update(m_matchPlay);

      m_bSwap = false;
      m_selectedPlayerIdx = -1;
      m_selectedSubPlayerIdx = -1;
      m_swapButton->SetSprite(FDResourceManager::GetSprite("uiicon","swap_player"));

		}
	}
}

void FDMatchDaySubsScreen::DoShow()
{
  m_matchSubsScreen->Show();
}

void FDMatchDaySubsScreen::DoHide()
{
  m_matchSubsScreen->Hide();
}

void FDMatchDaySubsScreen::Update( CMatchPlay* _MatchPlay )
{
  m_matchPlay = _MatchPlay;

  char strnum[12];

  int teamIdx = m_teamtable->GetSelectedRow();
  int subIdx = m_substable->GetSelectedRow();

  m_teamtable->ClearAll();
  m_substable->ClearAll();

  m_teamtable->AddColumn( lUICoord( 6.0f ) , LUI_LEFT_CENTER );
  m_teamtable->AddColumn( lUICoord( 88.0f ) );
  m_teamtable->AddColumn( lUICoord( 18.0f ) );
  m_teamtable->AddColumn( lUICoord( 6.0f ) );

  m_substable->AddColumn( lUICoord( 6.0f ), LUI_LEFT_CENTER );
  m_substable->AddColumn( lUICoord( 88.0f ) );
  m_substable->AddColumn( lUICoord( 18.0f ) );
  m_substable->AddColumn( lUICoord( 6.0f ) );

  m_teamtable->AddRows( PLAYERSINTEAM + 1);
  m_substable->AddRows( _MatchPlay->GetFixture().GetSubsSelect() + 1);

  // header
  m_teamtable->SetText( 0, 1, lUIText( IDS_NAME ) );
  m_teamtable->SetText( 0, 2, lUIText( IDS_SKILL_SINGLE ) );

  m_substable->SetText( 0, 1, lUIText( IDS_SUBS32 ) );
  m_substable->SetText( 0, 2, lUIText( IDS_SKILL_SINGLE ) );

  m_teamtable->SetRowBkgColour( 0, BLACK );
  m_substable->SetRowBkgColour( 0, BLACK );

  // reselect
  m_teamtable->SelectRow( teamIdx, true );
  m_substable->SelectRow( subIdx, true );

  CClub& CurrentClub = _MatchPlay->GetCurrentUserControlledClub();

  for( uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++ )
	{
    CPlayer* pPlayer = &CurrentClub.GetPlayer(LoopCount);

    // team 1
    m_teamtable->SetTextColour( LoopCount+1, 0, WHITE );
    m_teamtable->SetTextColour( LoopCount+1, 1, WHITE );
    m_teamtable->SetTextColour( LoopCount+1, 2, WHITE );

    sprintf( strnum, "%d", pPlayer->GetSkills().GetOverallSkill());
    m_teamtable->SetText( LoopCount+1, 2, lUIText( strnum ) );

    //m_TeamGrid.SetRowPointer(LoopCount, pPlayer);
 		if (pPlayer->GetMatchStatus() != MAXPLAYERSTATUSES)
 		{
      //m_TeamGrid.SetItemBitmap(LoopCount, 0, &m_pngArrowLR, pPlayer->GetMatchStatus() + eINJURED);
      m_teamtable->SetRowBkgColour( LoopCount+1, PALERED );
      m_teamtable->SetRowFlash( LoopCount+1, true );

      CString str("INJ");
      m_teamtable->SetText( LoopCount+1, 0,  lUIText( str ) );
 		}
 		else
    if( SubsInfo().HasPlayerBeenSubstitutedOn(CurrentClub.GetFirstTeamSquad().GetSquad(LoopCount)) == true )
		{
      m_teamtable->SetRowBkgColour( LoopCount+1, PosnColour[pPlayer->GetSkills().GetPosition()] );

      m_teamtable->SetTextColour( LoopCount+1, 0, GREY );
      m_teamtable->SetTextColour( LoopCount+1, 1, GREY );
      m_teamtable->SetTextColour( LoopCount+1, 2, GREY );
      //m_TeamGrid.DisableRow(LoopCount, true);
		}
		else
		{
      m_teamtable->SetRowBkgColour( LoopCount+1, PosnColour[pPlayer->GetSkills().GetPosition()] );
		}

//    sprintf( strnum, "%d", LoopCount+1);
//    m_teamtable->SetText( LoopCount, 0, lUIText( strnum ) );

		m_teamtable->SetText( LoopCount+1, 1, lUIText( CNewsItem::PreprocessPlayerName(pPlayer).CStr() ) );

    m_teamtable->SetTagDataForRow(LoopCount + 1, CurrentClub.GetFirstTeamSquad().DoFindSquadNumber(pPlayer));
  }

  for( uint LoopCount = PLAYERSINTEAM; LoopCount < PLAYERSINTEAM + _MatchPlay->GetFixture().GetSubsSelect(); LoopCount++ )
	{
    CPlayer* pPlayer = &CurrentClub.GetPlayer(LoopCount);

    if( SubsInfo().HasPlayerBeenSubstitutedOff(CurrentClub.GetFirstTeamSquad().GetSquad(LoopCount)) == true )
		{
      m_substable->SetRowBkgColour( LoopCount - PLAYERSINTEAM+1, L_ARGB(80, 0x50, 0x50, 0x50) );
      //m_SubGrid.SetItemBitmap(LoopCount - PLAYERSINTEAM, 0, &m_pngArrowLR, eTAKENOFF);
      //m_teamtable->SetRowBkgColour( LoopCount - PLAYERSINTEAM+1, RED );
      m_substable->SetTextColour( LoopCount - PLAYERSINTEAM+1, 1, GREY );
      m_substable->SetTextColour( LoopCount - PLAYERSINTEAM+1, 2, GREY );
      //m_substable->SetRowFlash( LoopCount - PLAYERSINTEAM+1, true );
		}
		else
		{
      m_substable->SetTextColour( LoopCount - PLAYERSINTEAM+1, 1, WHITE );
      m_substable->SetTextColour( LoopCount - PLAYERSINTEAM+1, 2, WHITE );
      m_substable->SetRowBkgColour( LoopCount - PLAYERSINTEAM+1, PosnColour[pPlayer->GetSkills().GetPosition()] );
		}

		m_substable->SetText( LoopCount - PLAYERSINTEAM+1, 1, lUIText( CNewsItem::PreprocessPlayerName(pPlayer).CStr() ) );

	::sprintf(strnum, "%d", pPlayer->GetSkills().GetOverallSkill());
	m_substable->SetText( LoopCount - PLAYERSINTEAM+1, 2, lUIText(strnum));
	m_substable->SetTagDataForRow(LoopCount - PLAYERSINTEAM+1, CurrentClub.GetFirstTeamSquad().DoFindSquadNumber(pPlayer));
	}

	bool bClubHome = m_matchPlay->IsUserControlledClub( HOME );

  int subsLeft = SubsInfo().DoCountSubstitutionsLeft( bClubHome ? HOME : AWAY );
  int subsTotal = SubsInfo().GetNumSubstitutionsAllowed();
  CString str;
  str.Format("SUBS REMAIN: %d/%d", subsLeft, subsTotal );
	m_subsRemainingText->SetText( lUIText( str.CStr() ) );
}

