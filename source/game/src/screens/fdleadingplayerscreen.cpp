//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDLeadingPlayersscreen.cpp
// Description : FDLeadingPlayersScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdscrollbarwidget.h>
#include <fdgameworld.h>
#include <fdtext.h>

#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdleadingplayerscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDLeadingPlayersScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
#define MAXLISTSIZE 20


FDLeadingPlayersScreen::FDLeadingPlayersScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDLeadingPlayersScreen::~FDLeadingPlayersScreen()
{
}

void FDLeadingPlayersScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_leadingScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "leading_screen" );
  m_leadingScreen->Hide();

  // bottom screen table
  m_table = FD_GAME_NEW FDTableWidget( m_leadingScreen, 0, "leading",
                               FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_BACKGROUND|
							   FDTableWidget::TABLE_FLAG_ROW_SELECT | FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
                               LUI_TOP_LEFT, lUIPoint( 20, 9 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 120 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_leadingScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 9), lUIPoint(GetDefaultScrollBarWidth(), 120));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar( pScrollBar );
  m_table->SignalRowSelect.Connect( this, &FDLeadingPlayersScreen::OnTableSelect );

  // combo to select division + date
  m_divisionSelector = FD_GAME_NEW FDComboWidget( m_leadingScreen, 0, "lpdivisionselect", lUIText(""), LUI_TOP_LEFT, lUIPoint( 63, 139 ), lUIPoint( 150, 19 ) );
  m_divisionSelector->SignalSelection.Connect( this, &FDLeadingPlayersScreen::OnDivisionSelect );
  m_divisionSelector->SetAllToUpper( true );

  // view mode
  m_viewSelector = FD_GAME_NEW FDComboWidget( m_leadingScreen, 0, "lpviewselect", lUIText(""), LUI_TOP_LEFT, lUIPoint( 63, 163 ), lUIPoint( 150, 19 ) );
  m_viewSelector->SignalSelection.Connect( this, &FDLeadingPlayersScreen::OnViewSelect );
  m_viewSelector->SetAllToUpper( true );

  m_viewSelector->AddItem( lUIText( IDS_SCREEN3302 ) );
  m_viewSelector->AddItem( lUIText( IDS_GOALS ) );
}

void FDLeadingPlayersScreen::OnDivisionSelect(  FDComboWidget *, int index )
{
  lLogFmt("OnDivisionSelect %d\n", index);

  UpdateTable();
}

void FDLeadingPlayersScreen::OnViewSelect(  FDComboWidget *, int index )
{
  lLogFmt("OnViewSelect %d\n", index);

  if(index != -1 )
  {
    m_viewMode = (LeadingPlayerViewType)index;
    UpdateTable();
  }
}

//////////////////////////////////////////////////////////////////////////
// Returns whether or not there will be any data displayed on this screen.
// Used by the menu system to determine whether or not to disable the
// menu option for this screen.
//////////////////////////////////////////////////////////////////////////
bool FDLeadingPlayersScreen::HasDataToDisplay() const
{
  // build result list
  CDivisionList &divisionList = WorldData().GetDivisionList();
  CDivision &division = divisionList.GetAt( m_divisionSelector->GetSelectedItem() );

  if( WorldData().GetClubByID(division.GetClubs()[0]).GetTableData().GetNumberGamesPlayed() )
  {
	  return true;
  }
  return false;
}

void FDLeadingPlayersScreen::UpdateTable()
{
  // build result list
  CDivisionList &divisionList = WorldData().GetDivisionList();
  CDivision &division = divisionList.GetAt( m_divisionSelector->GetSelectedItem() );

  m_pointScorers.RemoveAll();
  m_goalScorers.RemoveAll();

  m_playerList.Empty();
  m_table->ClearAll();

  if( HasDataToDisplay() == false )
  {
    return;
  }

  m_pointScorers.CreateList(division,  MAXLISTSIZE);
  m_goalScorers.DoCreateList(division, MAXLISTSIZE);

  // add columns
  CString str;

  m_table->AddColumn( lUICoord( 80 ), LUI_CENTER_LEFT );  // name
  m_table->AddColumn( lUICoord( 3 ), LUI_CENTER_CENTER );  // gap
  m_table->AddColumn( lUICoord( 80 ), LUI_CENTER_LEFT );  // club name
  m_table->AddColumn( lUICoord( 3 ), LUI_CENTER_CENTER );  // gap
  m_table->AddColumn( lUICoord( 18 ), LUI_CENTER_CENTER );  // Yellow / League Goals
  m_table->AddColumn( lUICoord( 3 ), LUI_CENTER_CENTER );  // gap
  m_table->AddColumn( lUICoord( 18 ), LUI_CENTER_CENTER );  // Red / Cup Goals
  m_table->AddColumn( lUICoord( 3 ), LUI_CENTER_CENTER );  // gap
  m_table->AddColumn( lUICoord( 18 ), LUI_CENTER_CENTER );  // Overall
  m_table->AddColumn( lUICoord( 3 ), LUI_CENTER_CENTER );  // gap

  m_table->SetSortWhenColumnHeaderClicked(true);
  m_table->SetSortFunctionForColumn(0, FDTableWidget::GenericFirstInitialLastNameSort);
  m_table->SetSortFunctionForColumn(2, FDTableWidget::GenericStringSort);
  m_table->SetSortFunctionForColumn(4, FDTableWidget::GenericNumericSort);
  m_table->SetSortFunctionForColumn(6, FDTableWidget::GenericNumericSort);
  m_table->SetSortFunctionForColumn(8, FDTableWidget::GenericNumericSort);
  m_table->ClearTiebreakerColumns();
  m_table->AddTiebreakerColumn(0);

  m_table->AddRows( 1 );

  m_table->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));
  m_table->SetText( 0, 0, lUIText( IDS_NAME ) );  // name
  m_table->SetText( 0, 2, lUIText( IDS_TAB1101 ) );  // club name

  if( m_viewMode == VIEW_DISCIPLINARY )
  {
    // disc
    m_table->SetText( 0, 4, lUIText( IDS_YELLOW_SINGLE_CHAR ) );  // Yellow
    m_table->SetText( 0, 6, lUIText( IDS_RED_SINGLE_CHAR ) );  // Red
    m_table->SetText( 0, 8, lUIText( "PTS" ) );  // Pts

    m_table->SetTextColour( 0, 4, L_ARGB( 255, 255, 255, 0));
    m_table->SetTextColour( 0, 6, L_ARGB( 255, 255, 0, 0));
  } else if (m_viewMode == VIEW_GOALS)
  {
    // goalscorers
    m_table->SetText( 0, 4, lUIText( IDS_LEAGUE_SINGLE_CHAR ) );  // League Goals
    m_table->SetText( 0, 6, lUIText( IDS_CUP_SINGLE_CHAR ) );  // Cup Goals
    m_table->SetText( 0, 8, lUIText( IDS_OVERALL_SINGLE_CHAR ) );  // Overall

    m_table->SetTextColour( 0, 4, L_ARGB( 255, 255, 255, 255));
    m_table->SetTextColour( 0, 6, L_ARGB( 255, 255, 255, 255));
  }
  else
  {
	  EXT_ASSERT(false && "Unknown view mode");
  }



  switch(m_viewMode)
	{
		case VIEW_DISCIPLINARY:
    {
      // Disciplinary
      m_table->AddRows( m_pointScorers.GetSize() );

      for (int LoopCount = 0; LoopCount < m_pointScorers.GetSize(); LoopCount++)
      {
        CLeadingDiscipinaryPoints *pLeadingDiscPlayer = static_cast<CLeadingDiscipinaryPoints*>(m_pointScorers.GetAt(LoopCount));
        CPlayer *pPlayer = WorldData().GetPlayer(pLeadingDiscPlayer->GetPlayerNum());
        CClub *pClub = &WorldData().GetClubByID(pLeadingDiscPlayer->GetClubNum());

        m_playerList.Push( pPlayer );

		m_table->SetTagDataForRow(LoopCount + 1, m_playerList.Size() - 1);
        //m_table->SetRowBkgColour( LoopCount + 1, L_ARGB( 128, 100, 100, 100 ) );

        m_table->SetBkgColour(LoopCount + 1, 0, PosnColour[ pPlayer->GetSkills().GetPosition() ]);
        m_table->SetBkgColour(LoopCount + 1, 1, L_ARGB( 127, 130, 130, 130 ) );
        m_table->SetBkgColour(LoopCount + 1, 2, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 3, L_ARGB( 127, 130, 130, 130 ));
        m_table->SetBkgColour(LoopCount + 1, 4, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 5, L_ARGB( 127, 130, 130, 130 ));
        m_table->SetBkgColour(LoopCount + 1, 6, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 7, L_ARGB( 127, 130, 130, 130 ));
        m_table->SetBkgColour(LoopCount + 1, 8, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 9, L_ARGB( 127, 130, 130, 130 ));

        m_table->SetText( LoopCount + 1, 0, lUIText( FDMakePlayerNameLink(pPlayer) ) );
        m_table->SetText( LoopCount + 1, 2, lUIText( FDMakeClubNameLink(pClub) ) );

        str.Format("%d", pLeadingDiscPlayer->GetYellow());
        m_table->SetText( LoopCount + 1, 4, lUIText( str ) );

        str.Format("%d", pLeadingDiscPlayer->GetRed());
        m_table->SetText( LoopCount + 1, 6, lUIText( str ) );

        str.Format("%d", pLeadingDiscPlayer->GetTotalScored());
        m_table->SetText( LoopCount + 1, 8, lUIText( str ) );
      }
    }
    break;

		case VIEW_GOALS:
    {
      m_table->AddRows( m_goalScorers.GetSize() );

      for (int LoopCount = 0; LoopCount < m_goalScorers.GetSize(); LoopCount++)
      {
        CLeadingGoalScorer *pLeadingGoalScorer = m_goalScorers.GetAt(LoopCount);
        CPlayer *pPlayer = WorldData().GetPlayer(pLeadingGoalScorer->GetPlayerNum());
        CClub *pClub = &WorldData().GetClubByID(pLeadingGoalScorer->GetClubNum());

        m_playerList.Push( pPlayer );

		m_table->SetTagDataForRow(LoopCount + 1, m_playerList.Size() - 1);
        //m_table->SetRowBkgColour( LoopCount + 1, L_ARGB( 128, 100, 100, 100 ) );

        m_table->SetBkgColour(LoopCount + 1, 0, PosnColour[ pPlayer->GetSkills().GetPosition() ]);
        m_table->SetBkgColour(LoopCount + 1, 1, L_ARGB( 127, 130, 130, 130 ) );
        m_table->SetBkgColour(LoopCount + 1, 2, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 3, L_ARGB( 127, 130, 130, 130 ));
        m_table->SetBkgColour(LoopCount + 1, 4, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 5, L_ARGB( 127, 130, 130, 130 ));
        m_table->SetBkgColour(LoopCount + 1, 6, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 7, L_ARGB( 127, 130, 130, 130 ));
        m_table->SetBkgColour(LoopCount + 1, 8, L_ARGB( 127, 80, 80, 80));
        m_table->SetBkgColour(LoopCount + 1, 9, L_ARGB( 127, 130, 130, 130 ));

        m_table->SetText( LoopCount + 1, 0, lUIText( FDMakePlayerNameLink(pPlayer) ) );
        m_table->SetText( LoopCount + 1, 2, lUIText( FDMakeClubNameLink(pClub) ) );

        str.Format("%d", pLeadingGoalScorer->GetLeagueScored());
        m_table->SetText( LoopCount + 1, 4, lUIText( str ) );

        str.Format("%d", pLeadingGoalScorer->GetCupScored());
        m_table->SetText( LoopCount + 1, 6, lUIText( str ) );

        str.Format("%d", pLeadingGoalScorer->GetTotalScored());
        m_table->SetText( LoopCount + 1, 8, lUIText( str ) );
      }
    }
	}
}

void FDLeadingPlayersScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int selectedDiv  = m_divisionSelector->GetSelectedItem();
    int selectedView = m_viewSelector->GetSelectedItem();
    int selectedPly  = m_table->GetSelectedRow();
    state << selectedDiv;
    state << selectedView;
    state << selectedPly;
    lLogFmt("Saving div %d view %d player %d\n", selectedDiv, selectedView, selectedPly);
  } else
  {
    int selectedDiv;
    int selectedView;
    int selectedPly;
    state << selectedDiv;
    state << selectedView;
    state << selectedPly;
    lLogFmt("Loading div %d view %d player %d\n", selectedDiv, selectedView, selectedPly);
    m_divisionSelector->SetSelectedItem( selectedDiv, false );
    m_viewSelector->SetSelectedItem( selectedView, false );
    m_table->SelectRow( selectedPly );
  }
}


void FDLeadingPlayersScreen::Update()
{
  if (IsForcedUpdateRequested() == false)
	  return;

  SetForcedUpdateRequested(false);
  m_viewMode = VIEW_DISCIPLINARY;
  m_viewSelector->SetSelectedItem((int)m_viewMode, true);

  // set up division table
  m_divisionSelector->ClearAll();
  CDivisionList &divisionList = WorldData().GetDivisionList();

  for(int i = 0; i < divisionList.GetSize(); i++)
  {
    CDivision &division = divisionList.GetAt(i);
    m_divisionSelector->AddItem( lUIText( division.GetName() ) );
  }

  OnDivisionSelect( m_divisionSelector, m_divisionSelector->GetSelectedItem() );

  // update table
  UpdateTable();
}

void FDLeadingPlayersScreen::Prepare()
{
  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
}

void FDLeadingPlayersScreen::DoShow()
{
  m_leadingScreen->Show();
}

void FDLeadingPlayersScreen::DoHide()
{
  m_leadingScreen->Hide();
}

void FDLeadingPlayersScreen::OnTableSelect( FDTableWidget *, int row )
{
  lLogFmt("LeadingPlayer Select %d\n", row);

  int idx = row - 1;
  if( idx < 0 || idx >= m_playerList.Size() )
  {
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
    GetMainUI()->GetPlayerInfoSubScreen()->Hide();
    return;
  }

  GetMainUI()->HideSubMessage();

  const int iPlayerListIdx = m_table->GetTagDataForRow(row);
  GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( m_playerList( iPlayerListIdx )  );
  GetMainUI()->GetPlayerInfoSubScreen()->Show();
}

