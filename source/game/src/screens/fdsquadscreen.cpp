//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.cpp
// Description : FDSquadScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fdtext.h>

#include <screens/fdsquadscreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdstrategyformationscreen.h>
#include <screens/fdstrategyrolesscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>
#include <ui/luimouse.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>
#include <fdfullscreenwidget.h>
#include <fdscrollbarwidget.h>
#include <fdsquadtablewidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDSquadScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDSquadScreen::FDSquadScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
  m_pCurrentClub = NULL;
  m_bSwapMode = false;
  m_playerSelectedIdx = -1;
  m_playerSwapIdx = -1;
  m_bClearPlayerInfo = true;
}

FDSquadScreen::~FDSquadScreen()
{
}

void FDSquadScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_squadScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "squad_screen" );

  // * Table for player names

  m_squadTable = FD_GAME_NEW FDSquadTableWidget( m_squadScreen, 0, "playertable",
            LUI_TOP_LEFT, lUIPoint( 20, 0 ), lUIPoint( 256 - 20, 145 ) );

  m_squadTable->SignalRowSelect.Connect(this, &FDSquadScreen::OnSquadRowSelect);

  // select information shown by table
  FDComboWidget *m_squadTableInfoType = FD_GAME_NEW FDComboWidget( m_squadScreen, 0, "tableinfo_combo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 137, 155 ) );
  m_squadTableInfoType->SetAllToUpper( true );
  m_squadTableInfoType->AddItem( lUIText( IDS_STRING3601I ) );
  m_squadTableInfoType->AddItem( lUIText( IDS_SCREEN03 ) );
  m_squadTableInfoType->AddItem( lUIText( IDS_CONTRACT ) );
  m_squadTableInfoType->AddItem( lUIText( IDS_TAB0504 ) );

  // TODO LOCALE
  m_squadTableInfoType->AddItem( lUIText( "WELL BEING" ) );

  m_squadTableInfoType->SignalSelection.Connect( this, &FDSquadScreen::OnTableInfoCombo );

  m_otherClubName = FD_GAME_NEW lUITextWidget( m_squadScreen, 0, "otherclublavel", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 175), lUIPoint(160, 16) );
  m_otherClubName->SetFont( FDResourceManager::GetFont( "font16", "font16" ) );
  m_otherClubName->SetShadow( true, L_ARGB( 255, 0, 0,0 ) );

  // buttons to perform actions on selected player
  m_transferStatusButton = FD_GAME_NEW FDButtonWidget( m_squadScreen, 0, "squadtransfer_sqbutton", FDResourceManager::GetSprite("uiicon","transfer_status"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 28, 147 ) );
  m_sackPlayerButton = FD_GAME_NEW FDButtonWidget( m_squadScreen, 0, "sack_sqbutton", FDResourceManager::GetSprite("uiicon","fire"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 52, 167 ) );
  m_renewContractButton = FD_GAME_NEW FDButtonWidget( m_squadScreen, 0, "renew_sqbutton", FDResourceManager::GetSprite("uiicon","offercontract"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 198, 167 ) );
  m_swapButton = FD_GAME_NEW FDButtonWidget( m_squadScreen, 0, "swap_sqbutton", FDResourceManager::GetSprite("uiicon","swap_player"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 221, 147 ) );
  m_coachButton = FD_GAME_NEW FDButtonWidget( m_squadScreen, 0, "coach_sqbutton", FDResourceManager::GetSprite("uiicon","coach"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 89, 167 ) );

  m_transferStatusButton->SignalControlCommand.Connect( this, &FDSquadScreen::OnTransferStatusButton );
  m_sackPlayerButton->SignalControlCommand.Connect( this, &FDSquadScreen::OnSackPlayerButton );
  m_renewContractButton->SignalControlCommand.Connect( this, &FDSquadScreen::OnRenewContractButton );
  m_swapButton->SignalControlCommand.Connect( this, &FDSquadScreen::OnSwapButton );

  m_coachButton->SignalControlCommand.Connect( this, &FDSquadScreen::OnCoachButton );

  m_squadButtonFormation = FD_GAME_NEW FDButtonWidget(m_squadScreen, 0, "squadform", FDResourceManager::GetSprite("uiicon", "formationbutton"),
	  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(125, 167));
  m_squadButtonRoles = FD_GAME_NEW FDButtonWidget(m_squadScreen, 0, "squadroles", FDResourceManager::GetSprite("uiicon", "rolesbutton"),
	  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(158, 167));

  m_squadButtonFormation->SignalControlCommand.Connect(this, &FDSquadScreen::OnSquadFormationButton);
  m_squadButtonRoles->SignalControlCommand.Connect(this, &FDSquadScreen::OnSquadRolesButton);
}

void FDSquadScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    // save club ID
    ushort clubID = NOCLUB;
    if( m_pCurrentClub != NULL )
    {
      clubID = m_pCurrentClub->GetID().id;
    }
    lLogFmt("Saving club ID %d\n", clubID);
    state << clubID;

    // save selected row in table
    int iSelectedRow  = m_squadTable->GetUnderlyingTable()->GetSelectedRow();
    int iSelectedCell = m_squadTable->GetUnderlyingTable()->GetSelectedCol();
    state << iSelectedRow;
    state << iSelectedCell;
    lLogFmt("Saving cell %d,%d\n", iSelectedRow, iSelectedCell);

    bool bScrollDirection = false;
    int iCurrentTopRow;
    m_squadTable->GetUnderlyingTable()->GetScrollPosition(iCurrentTopRow, bScrollDirection);
    state << iCurrentTopRow;
    state << bScrollDirection;
    lLogFmt("Saved row %d (scrollpos %d)\n", iSelectedRow, iCurrentTopRow);
  } else
  {
    SetForcedUpdateRequested(true);
    WorldData().SetCurrentPlayer( NULL );

    ushort clubID;
    state << clubID;

    if( clubID == NOCLUB )
    {
      lLogFmt("Loading NO club\n");
      SetSquadInfo( NULL );
    } else
    {
      lLogFmt("Loading club %d\n", clubID);
      SetSquadInfo( &WorldData().GetClubByID( ClubID( clubID ) ) );
    }

    // set selected row
    int iSelectedRow;
    int iSelectedCell;
    state << iSelectedRow;
    state << iSelectedCell;
    lLogFmt("Selecting cell %d,%d\n", iSelectedRow, iSelectedCell);
    m_squadTable->GetUnderlyingTable()->SelectCell(iSelectedRow, iSelectedCell);

    // set scroll position
    bool bScrollDirection = false;
    int iCurrentTopRow;
    state << iCurrentTopRow;
    state << bScrollDirection;
    lLogFmt("Loaded row %d (scrollpos %d)\n", iSelectedRow, iCurrentTopRow);
    m_squadTable->GetUnderlyingTable()->SetScrollPosition(iCurrentTopRow, bScrollDirection);
    m_bClearPlayerInfo = false;
  }
}

void FDSquadScreen::Update()
{
  if (IsForcedUpdateRequested() == false)
	  return;

  SetForcedUpdateRequested(false);

  m_squadTable->RefreshTable();

  if( !m_bUserClub )
  {
    m_transferStatusButton->Hide();
    m_sackPlayerButton->Hide();
    m_renewContractButton->Hide();
    m_swapButton->Hide();
    m_coachButton->Hide();
    m_squadButtonFormation->Hide();
    m_squadButtonRoles->Hide();
  }
  else
  {
    m_transferStatusButton->Show();
    m_sackPlayerButton->Show();
    m_renewContractButton->Show();
    m_swapButton->Show();
    m_coachButton->Show();
    m_squadButtonFormation->Show();
    m_squadButtonRoles->Show();
  }
}

void FDSquadScreen::DoShow()
{
  m_squadScreen->Show();
  SetSwapButtonState(false);

  if (( m_bClearPlayerInfo ) || (m_squadTable->GetLastSelectedPlayer() == NULL))
  {
    WorldData().SetCurrentPlayer( NULL );
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( NULL );
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
  }
}

void FDSquadScreen::DoHide()
{
  m_squadScreen->Hide();
  m_bClearPlayerInfo = true;
}

void FDSquadScreen::OnTableInfoCombo( FDComboWidget *, int idx )
{
  lLogFmt("Info to show %d\n", idx);

  CPlayer *lastPlayer = m_squadTable->GetLastSelectedPlayer();
  m_squadTable->SetInfoToShow((FDSquadTableWidget::SquadTableInfoType)idx);
  m_squadTable->SelectPlayer( lastPlayer );

  lLogFmt("Swap mode disabled\n");
  m_playerSelectedIdx = m_playerSwapIdx;
  m_playerSwapIdx = -1;
  m_bSwapMode = false;
	SetSwapButtonState(false);
}

void FDSquadScreen::SetSquadInfo( CClub * club )
{
  if (m_pCurrentClub != club)
  {
	  SetForcedUpdateRequested(true);
  }

  m_pCurrentClub = club;

  if( WorldData().GetCurrentUserClub() != club )
  {
    m_bUserClub = false;
  } else
  {
    m_bUserClub = true;
  }

  // Prevent squad controls during match
  if (WorldData().GetMasterState() == CWorld::IN_PROCESSING)
  {
    m_bUserClub = false;
  }

  if( !m_bUserClub )
    m_otherClubName->SetText( lUIText( m_pCurrentClub->GetName() ), true );
  else
    m_otherClubName->SetText( lUIText( "" ));

  m_squadTable->SetClub(m_pCurrentClub);
  Update();
}

void FDSquadScreen::OnSquadRowSelect( FDTableWidget *, CPlayer *pPlayer, int iPlayerIndex )
{
  lLogFmt("Player %d\n", iPlayerIndex);
  if (pPlayer != NULL)
  {
    // must be set to allow sacking and renewing contracts
    WorldData().SetCurrentPlayer( pPlayer );
    WorldData().SetCurrentClub( m_pCurrentClub );
  }

  GetMainUI()->HideSubMessage();

  if( pPlayer != NULL )
  {
    lLogFmt("Player: %d - %s\n", iPlayerIndex, pPlayer->GetName());

    if( m_bUserClub )
    {
      // hide / show transfer status button depending on whether is player on loan
      if(pPlayer->IsOnLoan() == true)
      {
        m_transferStatusButton->Hide();
        m_renewContractButton->Hide();
      } else
      {
        m_transferStatusButton->Show();
        m_renewContractButton->Show();
      }
    }

    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( pPlayer );
    m_playerSelectedIdx = m_squadTable->GetPlayerSquadIndex(iPlayerIndex);

    if( m_bSwapMode && m_playerSwapIdx != -1 )
    {
      // swapping this and the last player
      if( m_pCurrentClub == WorldData().GetCurrentUserClub() )
      {
        CPlayer *lastPlayer = m_squadTable->GetLastSelectedPlayer();

        g_DebugStr.Format(_T("Swapping %d & %d\n"),m_playerSwapIdx , m_playerSelectedIdx );
        g_DebugStr.OutputDebug();
        m_pCurrentClub->GetFirstTeamSquad().DoSwapPlayers( m_playerSwapIdx, m_playerSelectedIdx );

        int iCurrentTablePosition;
        bool bCurrentDirection;
        m_squadTable->GetUnderlyingTable()->GetScrollPosition(iCurrentTablePosition, bCurrentDirection);
        int iCurrentScrollPosition = m_squadTable->GetScrollBar()->GetCurrentPosition();

        SetForcedUpdateRequested(true);
        Update();

        // This will also take care of resetting variables associated with the swap functionality.
        SetSwapButtonState(false);
        m_playerSwapIdx = -1;

        // reselect player
        m_squadTable->SelectPlayer( lastPlayer, false );

        m_squadTable->GetUnderlyingTable()->SetScrollPosition(iCurrentScrollPosition, false);
        m_squadTable->GetScrollBar()->SetCurrentPosition(iCurrentTablePosition, bCurrentDirection);

      } else
      {
        // kill swap mode if clubs dont match
        m_bSwapMode = false;
      }
    }
  } else
  {
    lLogFmt("Not a valid playeridx!\n");
    if( IsVisible() )
    {
      GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
      GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( NULL );
    }
  }
}

void FDSquadScreen::OnTransferStatusButton( lUIControlWidget * )
{
  if( WorldData().GetCurrentPlayer() != NULL )
  {
    if(WorldData().GetCurrentPlayer()->IsOnLoan() == true)
      return; // TODO MESSAGE - cant transfer a loaned player

    CPlayer *lastPlayer = m_squadTable->GetLastSelectedPlayer();

    theApp.OnChangeTransferStatus();
    WorldData().SetCurrentPlayer( NULL );
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE

    // ***TODO: determine if the transfer status actually changed, and take action
    // accordingly.
    GetMainUI()->ForceUpdateForAllScreensOnNextShow();
    Update();

    m_squadTable->SelectPlayer( lastPlayer );
  }
}

void FDSquadScreen::OnSackPlayerButton( lUIControlWidget * )
{
  if( WorldData().GetCurrentPlayer() != NULL )
  {
    if(WorldData().GetCurrentPlayer()->IsOnLoan() == true)
      return; // TODO MESSAGE - can't sack a loaned player?

    CPlayer *lastPlayer = m_squadTable->GetLastSelectedPlayer();

    theApp.OnSackPlayer();

    if(  (WorldData().GetCurrentPlayer()->GetClubPtr() != NULL) && WorldData().GetCurrentPlayer()->GetClub().DoFindID() == m_pCurrentClub->DoFindID() )
    {
      // still with club
      m_squadTable->SelectPlayer( lastPlayer );
    } else
    {
      // sacked
      WorldData().SetCurrentPlayer( NULL );
      GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( NULL );
      GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
      GetMainUI()->ForceUpdateForAllScreensOnNextShow();
      // ***TODO: remove the player directly from this screen and clear the forced update flag.
    }
    Update();
  }
}

void FDSquadScreen::OnRenewContractButton( lUIControlWidget * )
{
  if( WorldData().GetCurrentPlayer() != NULL )
  {
    CPlayer *lastPlayer = m_squadTable->GetLastSelectedPlayer();

    theApp.OnNegotiatePlayerContract();
    WorldData().SetCurrentPlayer( NULL );
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE

    // ***TODO: determine if the transfer status actually changed, and take action
    // accordingly.
    GetMainUI()->ForceUpdateForAllScreensOnNextShow();
    Update();

    m_squadTable->SelectPlayer( lastPlayer );
  }
}

void FDSquadScreen::OnSwapButton( lUIControlWidget * )
{
  lLogFmt("SwapButton pressed - %d / %d\n", m_bSwapMode, m_playerSelectedIdx);
  lLogFmt("Table selection %d\n", m_squadTable->GetUnderlyingTable()->GetSelectedRow() );
  if( !m_bSwapMode && m_playerSelectedIdx != -1 && m_squadTable->GetUnderlyingTable()->GetSelectedRow() != 0)
  {
    lLogFmt("Swap mode enabled\n");
    m_playerSwapIdx = m_playerSelectedIdx;
    m_bSwapMode = true;
    SetSwapButtonState(true);
  } else
  {
    lLogFmt("Swap mode disabled\n");
    m_playerSelectedIdx = m_playerSwapIdx;
    m_playerSwapIdx = -1;
    m_bSwapMode = false;
    SetSwapButtonState(false);
  }
}

void FDSquadScreen::OnCoachButton( lUIControlWidget * )
{
  lLogFmt("Coach buton pressed\n" );

  CClub * club = WorldData().GetCurrentUserClub();

  if (club->GetCoachStaffNumber() == wNOTFOUND)

  {

    UserMessageBox( "You do not currently employ a coach!", MB_ICONEXCLAMATION + MB_OK);
    return;

  }


  int avail = club->GetFirstTeamSquad().DoCountAvailablePlayers();
  lLogFmt("%d avail\n");

  if( avail < PLAYERSINTEAM + 5 )
  {
    UserMessageBox( "There are not enough available players for the coach to select a full team.", MB_ICONEXCLAMATION + MB_OK);
  } else
  {
    if(UserMessageBox( IDS_COACH_DIALOG, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES )
    {
      club->GetFirstTeamSquad().DoSelectTeam( 5, *club );
      SetForcedUpdateRequested(true);
      Update();
    }
  }
}

void FDSquadScreen::OnSquadFormationButton( lUIControlWidget * )
{
	GetMainUI()->GetStrategyFormationScreen()->SetClub(m_pCurrentClub);
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_STRATEGY_FORMATION, false);
}

void FDSquadScreen::OnSquadRolesButton( lUIControlWidget * )
{
	GetMainUI()->GetStrategyRolesScreen()->SetClub(m_pCurrentClub);
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_STRATEGY_ROLES, false);
}

void FDSquadScreen::AutoplaySkip()
{
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_NEWSSTATUS, false);
}

void FDSquadScreen::SetSwapButtonState(const bool bActivated)
{
	if (bActivated)
	{
		m_swapButton->SetSprite(FDResourceManager::GetSprite("uiicon","swap_player_activated"));
	}
	else
	{
		m_swapButton->SetSprite(FDResourceManager::GetSprite("uiicon","swap_player"));
		//m_playerSelectedIdx = -1;
		m_playerSwapIdx = -1;
		m_bSwapMode = false;
	}
}

int FDSquadScreen::GetInfoToShow()
{
  return m_squadTable->GetInfoToShow();
}

