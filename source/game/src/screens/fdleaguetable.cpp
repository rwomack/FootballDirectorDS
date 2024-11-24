//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDLeagueTablescreen.cpp
// Description : FDLeagueTableScreen implementation
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

#include <screens/fdleaguetablescreen.h>
#include <screens/fdclubinfosubscreen.h>
#include <IndexedIntArray.h>
#include <CurrentForm.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDLeagueTableScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDLeagueTableScreen::FDLeagueTableScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDLeagueTableScreen::~FDLeagueTableScreen()
{
}

void FDLeagueTableScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  ClearRowSelectHistory();

  m_leaguetableScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "leaguetable_screen" );
  m_leaguetableScreen->Hide();

  m_leaguetableScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "leaguetable_screen_sub" );
  m_leaguetableScreenSub->Hide();

  // Graph - Positions
  m_graph = FD_GAME_NEW FDGraphWidget( m_leaguetableScreenSub, 0, "graph", LUI_CENTER_BOTTOM, lUIPointPercent( 50, 100 ), lUIPoint( 256 - 10, 140 ));
  m_graph->FlipYAxis( true );
  m_graph->SetTitle( "Overall League Position" );
  m_graph->SetXAxisTitle( "League Match Number" );
  m_graph->SetYAxisTitle( lText::Text(IDS_ABBR_POS) );
  m_graph->Hide();

  m_graphButton = FD_GAME_NEW FDButtonWidget( m_leaguetableScreen, 0, "graphButton", FDResourceManager::GetSprite("uiicon","graph_off"),
                                              lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint( 73, 162 ) );
  m_graphButton->SignalControlCommand.Connect( this, &FDLeagueTableScreen::OnGraphButton );

  // combo to select division + date
  m_divisionSelector = FD_GAME_NEW FDComboWidget( m_leaguetableScreen, 0, "ltsdivisionselect", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 183, 151 ), lUIPoint( 135, 19 ) );
  m_divisionSelector->SignalSelection.Connect( this, &FDLeagueTableScreen::OnDivisionSelect );
  m_divisionSelector->SetAllToUpper( true );

  // bottom screen table
  m_table = FD_GAME_NEW FDTableWidget( m_leaguetableScreen, 0, "ltsleaguetable",
                               /*FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|*/
                               FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELLBACKGROUND|
                               FDTableWidget::TABLE_FLAG_ROW_SELECT|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
                               FDTableWidget::TABLE_FLAG_CAP_FIRSTROW|FDTableWidget::TABLE_FLAG_BACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF,
                               LUI_TOP_LEFT, lUIPoint( 20, 5 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 134 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_leaguetableScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(248, 5), lUIPoint(GetDefaultScrollBarWidth(), 134));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar( pScrollBar );
  m_table->SignalRowSelect.Connect( this, &FDLeagueTableScreen::OnTableSelect );

  // view mode
  m_viewMode = VIEW_OVERALL;
  m_viewSelector = FD_GAME_NEW FDComboWidget( m_leaguetableScreen, 0, "ltsviewselect", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 183, 176 ), lUIPoint( 135, 19 ) );
  m_viewSelector->SignalSelection.Connect( this, &FDLeagueTableScreen::OnViewSelect );
  m_viewSelector->SetAllToUpper( true );

  m_viewSelector->AddItem( lUIText( IDS_OVERALL ) );
  m_viewSelector->AddItem( lUIText( IDS_SCREEN5001 ) );
  m_viewSelector->AddItem( lUIText( IDS_SCREEN5002 ) );
  m_viewSelector->AddItem( lUIText( IDS_FORM ) );
  //m_viewSelector->AddItem( lUIText( IDS_DISCIPLINE ) );
}

void FDLeagueTableScreen::OnDivisionSelect(  FDComboWidget *, int index )
{
  lLogFmt("OnDivisionSelect %d\n", index);

  ClearRowSelectHistory();
  UpdateTable();
  m_table->SelectRow(-1);
}

void FDLeagueTableScreen::Prepare()
{
  if( m_graph->IsVisible() )
  {
    GetMainUI()->HideSubMessage();
  } else
  {
    GetMainUI()->DisplaySubMessage("Select Team"); //TODO LOCALE
  }

  // set up division table
  m_divisionSelector->ClearAll();
  CDivisionList &divisionList = WorldData().GetDivisionList();

  for(int i = 0; i < divisionList.GetSize(); i++)
  {
    CDivision &division = divisionList.GetAt(i);
    m_divisionSelector->AddItem( lUIText( division.GetName() ) );
  }

  // select users league by default
  m_divisionSelector->SetSelectedItem( WorldData().GetCurrentUserClub()->GetDivisionID(), false );

  ClearRowSelectHistory();
}


void FDLeagueTableScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int viewMode = (int)m_viewMode;
    int division = (int)m_divisionSelector->GetSelectedItem();
    int selectedClub = m_table->GetSelectedRow();
    bool bGraph = m_graph->IsVisible();

    state << viewMode;
    state << division;
    state << selectedClub;
    state << bGraph;

    if( bGraph )
    {
      for (int i = 0; i < 4; i++)
      {
        ushort hist = m_iRowSelectHistory[i].id;
        state << hist;
      }
    }

    lLogFmt("Saving view %d division %d team %d graph %d\n", viewMode, division, selectedClub, bGraph);
    lLogFmt("Saved rowhist %d,%d,%d,%d\n", m_iRowSelectHistory[0],m_iRowSelectHistory[1],m_iRowSelectHistory[2],m_iRowSelectHistory[3]);
  } else
  {
    SetForcedUpdateRequested(true);

    int viewMode;
    int division;
    int selectedClub;
    bool bGraph;

    state << viewMode;
    state << division;
    state << selectedClub;
    state << bGraph;

    lLogFmt("Loading view %d division %d team %d bGraph %d\n", viewMode, division, selectedClub, bGraph);
    m_divisionSelector->SetSelectedItem( division, false );
    m_viewSelector->SetSelectedItem( viewMode, false );

    // Hide graph to avoid select adding a club to it (we un-hide below where required)
    m_graph->Hide();

    if( !bGraph )
    {
      //m_graphButton->Highlight(false);
      m_graphButton->SetSprite( FDResourceManager::GetSprite("uiicon","graph_off") );

      GetMainUI()->GetClubInfoSubScreen()->Show();
    }
    else
    {
      GetMainUI()->GetClubInfoSubScreen()->Hide();
    }

    if( selectedClub >= 0 && selectedClub < m_table->GetRowCount() )
    {
      lLogFmt("Selecting club %d\n", selectedClub);
      m_table->SelectRow( selectedClub );
    } else
    {
      GetMainUI()->DisplaySubMessage("Select Team"); //TODO LOCALE
    }

    if( bGraph )
    {
      m_graph->Show();
      m_graphButton->SetSprite( FDResourceManager::GetSprite("uiicon","graph_on") );

      GetMainUI()->HideSubMessage();

      for (int i = 0; i < 4; i++)
      {
        ushort hist;
        state << hist;
        if( hist != -1 )
        {
          AddClubToGraph( ClubID(hist) );
          lLogFmt("Adding club %d\n", hist);
        }
      }
    }

    UpdateClubInfo();
  }
}


void FDLeagueTableScreen::OnViewSelect(  FDComboWidget *, int index )
{
  lLogFmt("OnViewSelect %d\n", index);
  if(index != -1 )
  {
    m_viewMode = (LeagueTableViewType)index;

	// Store off the currently-selected club ID, and restore the selection after the table is rebuilt.
	ClubID cSelectedClub = ClubID(NOCLUB);
	if (m_table->GetSelectedRow() > 0)
	{
		cSelectedClub.id = m_table->GetTagDataForRow(m_table->GetSelectedRow());
	}

    UpdateTable();

	if (cSelectedClub.id != NOCLUB)
	{
		const int iRowToSelect = m_table->FindTagData(cSelectedClub.id, true);
		m_table->SelectRow(iRowToSelect, true);

		// If not visible, then scroll so that it is visible.
		FDScrollBarWidget *pScroll = m_table->GetScrollBar();
		const bool bIsVisible = (pScroll->GetCurrentPosition() <= iRowToSelect) && ((pScroll->GetCurrentPosition() + pScroll->GetItemsViewedAtOnce()) > iRowToSelect);
		if (!bIsVisible)
			pScroll->SetCurrentPosition(lmMax(0, lmMin(iRowToSelect - (pScroll->GetItemsViewedAtOnce() / 2), pScroll->GetNumberOfItems() - pScroll->GetItemsViewedAtOnce())), true);
	}
  }
}


void FDLeagueTableScreen::UpdateTable()
{
	// build result list
	CDivisionList &divisionList = WorldData().GetDivisionList();
	CDivision &division = divisionList.GetAt( m_divisionSelector->GetSelectedItem() );

	// sort form lis
	division.SortCurrentForm();

	// create fair play list
	CIndexedIntArray<ClubID>    FairPlayList;
	for (int LoopCount = 0; LoopCount < division.GetNumberOfClubsInDivision(); LoopCount++)
	{
		FairPlayList.AddNew( division.GetClubs()[LoopCount],
							WorldData().GetClubByID(division.GetClubs()[LoopCount]).GetSeasonStats().GetSeasonDisciplinaryPoints() );
	}
	FairPlayList.SortHigh();

	// add to table
	m_table->ClearAll();

	m_table->SetSortWhenColumnHeaderClicked(true);
  m_table->AddColumn( lUICoord( 20 ), LUI_CENTER_CENTER );    // pos
  m_table->AddColumn( lUICoord( 83 ), LUI_LEFT_CENTER );   // name

	if( m_viewMode != VIEW_DISCIPLINE )
	{
		// Overall, home, away and current form
    m_table->AddColumn( lUICoord( 17 ) ); // Played
    m_table->AddColumn( lUICoord( 17 ) ); // Won
    m_table->AddColumn( lUICoord( 17 ) ); // Drawn
    m_table->AddColumn( lUICoord( 17 ) ); // Lost
    m_table->AddColumn( lUICoord( 17 ) ); // For
    m_table->AddColumn( lUICoord( 17 ) ); // Against
    m_table->AddColumn( lUICoord( 18 ) ); // Points
	}
	else
	{
		// Disciplinary
		m_table->AddColumn( lUICoord( 40 ) ); // Red
		m_table->AddColumn( lUICoord( 40 ) ); // Yellow
		m_table->AddColumn( lUICoord( 40 ) ); // Overall
	}

	const uint32 BkgColour[] =
	{
    L_ARGB(80, 0x50, 0x50, 0x50),      // Normal
		L_ARGB(128,255,0,0),      // Relegation spot
		L_ARGB(128,0,255,0),      // Playoff spot
    L_ARGB(128,200,200,200),  // Promotion spot
	};

	m_table->AddRows( division.GetNumberOfClubsInDivision() + 1 );  // top row for header

	m_table->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));

	// set up header text
  m_table->SetText( 0, 0, lUIText( "POS" ) );   // pos
  m_table->SetText( 0, 1, lUIText( IDS_NAME ) );   // name

  // IDS_SCREEN50HEADER "P  W  D   L  F   A  Pts"

	if( m_viewMode < 4 )
	{
		// Overall, home, away and current form
    m_table->SetText( 0, 2, lUIText( IDS_ABBR_PLAYED ) ); // Played
    m_table->SetText( 0, 3, lUIText( IDS_ABBR_WON ) ); // Won
    m_table->SetText( 0, 4, lUIText( IDS_ABBR_DRAWN ) ); // Drawn
    m_table->SetText( 0, 5, lUIText( IDS_ABBR_LOST ) ); // Lost
    m_table->SetText( 0, 6, lUIText( IDS_ABBR_FOR ) ); // For
    m_table->SetText( 0, 7, lUIText( IDS_ABBR_AGAINST ) ); // Against
    m_table->SetText( 0, 8, lUIText( IDS_ABBR_PTS ) ); // Points
    m_table->SetTextToUpper( 0, 8 );
	}
	else
	{
		// Disciplinary
    m_table->SetText( 0, 2, lUIText( IDS_RED ) ); // Red
    m_table->SetText( 0, 3, lUIText( IDS_ABBR_YELLOW ) ); // Yellow
    m_table->SetText( 0, 4, lUIText( IDS_OVERALL ) ); // Overall
	}

	// write actual values
	if (( m_viewMode == VIEW_OVERALL ) || (m_viewMode == VIEW_HOME) || (m_viewMode == VIEW_AWAY))
	{
		// Overall, home and away
		CString str;

		division.SortLeagueTable( (int)m_viewMode );
		for (int LoopCount = 0; LoopCount < division.GetNumberOfClubsInDivision(); LoopCount++)
		{
			CClub *pClub = &division.DoFindClubInTablePosition(LoopCount);

			str.Format( "%d", LoopCount+1 );
			m_table->SetText( LoopCount+1, 0, lUIText( str ) );

			m_table->SetText( LoopCount+1, 1, lUIText( FDMakeClubNameLink(pClub) ) );

      if( m_viewMode == VIEW_OVERALL )
        m_table->SetRowBkgColour( LoopCount+1, BkgColour[ division.LeaguePositionStatus(LoopCount) ] );
      else
        m_table->SetRowBkgColour( LoopCount+1, L_ARGB(80, 0x50, 0x50, 0x50) );

			m_table->SetTagDataForRow(LoopCount + 1, pClub->DoFindID().id);
			//TODOm_Grid.SetRowPointerDetails(LoopCount, pClub);

			switch(m_viewMode)
			{
			case VIEW_OVERALL:
				{
					str.Format( "%d", pClub->GetTableData().GetNumberGamesPlayed());
          m_table->SetText( LoopCount+1, 2, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetNumberGamesWon());
          m_table->SetText( LoopCount+1, 3, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetNumberGamesDrawn());
          m_table->SetText( LoopCount+1, 4, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetNumberGamesLost());
          m_table->SetText( LoopCount+1, 5, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetNumberGoalsScored());
          m_table->SetText( LoopCount+1, 6, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetNumberGoalsConceded());
          m_table->SetText( LoopCount+1, 7, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetPoints());
          m_table->SetText( LoopCount+1, 8, lUIText( str ) );
				}
				break;

			case VIEW_HOME:
				{
					str.Format( "%d", pClub->GetTableData().GetHomeGames()->GetNumberGamesPlayed());
          m_table->SetText( LoopCount+1, 2, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetHomeGames()->GetNumberGamesWon());
          m_table->SetText( LoopCount+1, 3, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetHomeGames()->GetNumberGamesDrawn());
          m_table->SetText( LoopCount+1, 4, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetHomeGames()->GetNumberGamesLost());
          m_table->SetText( LoopCount+1, 5, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetHomeGames()->GetNumberGoalsScored());
          m_table->SetText( LoopCount+1, 6, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetHomeGames()->GetNumberGoalsConceded());
          m_table->SetText( LoopCount+1, 7, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetHomeGames()->GetPoints());
          m_table->SetText( LoopCount+1, 8, lUIText( str ) );
				}
				break;


			case VIEW_AWAY:
				{
					str.Format( "%d", pClub->GetTableData().GetAwayGames()->GetNumberGamesPlayed());
          m_table->SetText( LoopCount+1, 2, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetAwayGames()->GetNumberGamesWon());
          m_table->SetText( LoopCount+1, 3, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetAwayGames()->GetNumberGamesDrawn());
          m_table->SetText( LoopCount+1, 4, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetAwayGames()->GetNumberGamesLost());
          m_table->SetText( LoopCount+1, 5, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetAwayGames()->GetNumberGoalsScored());
          m_table->SetText( LoopCount+1, 6, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetAwayGames()->GetNumberGoalsConceded());
          m_table->SetText( LoopCount+1, 7, lUIText( str ) );
					str.Format( "%d", pClub->GetTableData().GetAwayGames()->GetPoints());
          m_table->SetText( LoopCount+1, 8, lUIText( str ) );
				}
				break;
			}
		}
	}
	else
	{
		CString str;
		int iGamesPlayed = CCurrentForm::NUMFORMMATCHES;
		switch(m_viewMode)
		{
			case VIEW_FORM:
				{
					// Current form
					if( division.FindClubNumberInCurrentFormPosition(0).GetTableData().GetNumberGamesPlayed() < CCurrentForm::NUMFORMMATCHES )
					{
						iGamesPlayed = division.FindClubNumberInCurrentFormPosition(0).GetTableData().GetNumberGamesPlayed();
					}

					for( int LoopCount = 0; LoopCount < division.GetNumberOfClubsInDivision(); LoopCount++ )
					{
						CClub *pClub = &division.FindClubNumberInCurrentFormPosition(LoopCount);

            str.Format( "%d", LoopCount+1 );
            m_table->SetText( LoopCount+1, 0, lUIText( str ) );

            m_table->SetText( LoopCount+1, 1, lUIText( FDMakeClubNameLink(pClub) ) );
            m_table->SetTagDataForRow(LoopCount + 1, pClub->DoFindID().id);

						str.Format( "%d", iGamesPlayed);
            m_table->SetText( LoopCount+1, 2, lUIText( str ) );
						str.Format( "%d", pClub->GetCurrentForm()->GetCurrentFormWon());
            m_table->SetText( LoopCount+1, 3, lUIText( str ) );
						str.Format( "%d", pClub->GetCurrentForm()->GetCurrentFormDrawn());
            m_table->SetText( LoopCount+1, 4, lUIText( str ) );
						str.Format( "%d", pClub->GetCurrentForm()->GetCurrentFormLost());
            m_table->SetText( LoopCount+1, 5, lUIText( str ) );
						str.Format( "%d", pClub->GetCurrentForm()->GetCurrentFormScored());
            m_table->SetText( LoopCount+1, 6, lUIText( str ) );
						str.Format( "%d", pClub->GetCurrentForm()->GetCurrentFormConceded());
            m_table->SetText( LoopCount+1, 7, lUIText( str ) );
						str.Format( "%d", pClub->GetCurrentForm()->GetCurrentFormPts());
            m_table->SetText( LoopCount+1, 8, lUIText( str ) );

            m_table->SetRowBkgColour( LoopCount+1, L_ARGB(80, 0x50, 0x50, 0x50) );

					}
				}
				break;

			case VIEW_DISCIPLINE:
				{
					for (int LoopCount = 0; LoopCount < division.GetNumberOfClubsInDivision(); LoopCount++)
					{
						CClub *pClub = &WorldData().GetClub( FairPlayList.GetAt(LoopCount)->GetValue() );

            str.Format( "%d", LoopCount+1 );
            m_table->SetText( LoopCount+1, 0, lUIText( str ) );
			m_table->SetTagDataForRow(LoopCount + 1, pClub->DoFindID().id);

            m_table->SetText( LoopCount+1, 1, lUIText( FDMakeClubNameLink(pClub) ) );
						str.Format( "%d", pClub->GetSeasonStats().GetSeasonBookings());
            m_table->SetText( LoopCount+1, 2, lUIText( str ) );
						str.Format( "%d", pClub->GetSeasonStats().GetSeasonSentOff());
            m_table->SetText( LoopCount+1, 3, lUIText( str ) );
						str.Format( "%d", pClub->GetSeasonStats().GetSeasonDisciplinaryPoints());
            m_table->SetText( LoopCount+1, 4, lUIText( str ) );
					}
				}
				break;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_iRowSelectHistory[i] != NOCLUB)
		{
			const int iRowOfGraphedClub = m_table->FindTagData(m_iRowSelectHistory[i].id, true);
			if (iRowOfGraphedClub > 0)
			{
				m_iRowColorHistory[i] = m_table->GetBkgColour(iRowOfGraphedClub, 0);
				m_table->SetRowBkgColour(iRowOfGraphedClub, FDGraphWidget::GetColorForDataSet(i));
			}
		}
	}
}

void FDLeagueTableScreen::Update()
{
  if (IsForcedUpdateRequested() == false)
	return;

  SetForcedUpdateRequested(false);

  UpdateGraph();
  UpdateClubInfo();
}

void FDLeagueTableScreen::UpdateGraph()
{
	m_graph->ClearAll();
	m_graph->SetMinValue( 1 );
	m_graph->SetMaxValue( 24 );

	// Get division
	if( m_divisionSelector->GetSelectedItem() == -1 )
		return;

	CDivisionList &divisionList = WorldData().GetDivisionList();
	CDivision &division = divisionList.GetAt( m_divisionSelector->GetSelectedItem() );

	for (int i = 0; i < 4; i++)
	{
		if (m_iRowSelectHistory[i].id == NOCLUB)
			continue;

		// get club
		const int iClubRow = m_table->FindTagData(m_iRowSelectHistory[i].id, true);
		lAssert(iClubRow > 0);

		int clubNo = iClubRow - 1;
		if( clubNo >= 0 )
		{
			CClub *pClub = &division.DoFindClubInTablePosition( clubNo );

			for( int j = 0; j < pClub->GetTableData().GetNumberGamesPlayed(); j++ )
			{
				const int position = pClub->GetLeaguePositionAfterMatch()->GetAt(j);

				// NOTE: I believe positions start from 0 onwards
				m_graph->AddData(i, position+1);
			}
		}
	}
}

void FDLeagueTableScreen::UpdateClubInfo()
{
	CDivisionList &divisionList = WorldData().GetDivisionList();
	CDivision &division = divisionList.GetAt( m_divisionSelector->GetSelectedItem() );

	// get club
	int clubNo = m_table->GetSelectedRow() - 1;

	if( clubNo >= 0 )
	{
	  GetMainUI()->HideSubMessage();

		CClub *pClub = &division.DoFindClubInTablePosition( clubNo );
		GetMainUI()->GetClubInfoSubScreen()->SetClubInfo( pClub );
    if( !m_graph->IsVisible() )
    {
      lLogFmt("Showing club info\n");
      GetMainUI()->GetClubInfoSubScreen()->Show();
    }
	}
}

void FDLeagueTableScreen::DoShow()
{
  m_leaguetableScreen->Show();
  m_leaguetableScreenSub->Show();
}

void FDLeagueTableScreen::DoHide()
{
  lLogFmt("DoHide\n");
  m_leaguetableScreen->Hide();
  m_leaguetableScreenSub->Hide();
  //GetMainUI()->GetClubInfoSubScreen()->Hide();
}

void FDLeagueTableScreen::OnGraphButton( lUIControlWidget * )
{
  GetMainUI()->HideSubMessage();

  if( m_graph->IsVisible() )
  {
    m_graph->Hide();
    //m_graphButton->Highlight(false);
    m_graphButton->SetSprite( FDResourceManager::GetSprite("uiicon","graph_off") );

	// If there were any clubs shown on the graph, restore their row colouring.
	for (int i = 0; i < 4; i++)
	{
		if (m_iRowSelectHistory[i].id != NOCLUB)
		{
			const int iClubRow = m_table->FindTagData(m_iRowSelectHistory[i].id, true);
			lAssert(iClubRow > 0);

			m_table->SetRowBkgColour(iClubRow, m_iRowColorHistory[i]);
		}
	}

	ClearRowSelectHistory();
    GetMainUI()->GetClubInfoSubScreen()->Show();
  }
  else
  {
    m_graph->Show();

	ClearRowSelectHistory();

	// Add the currently-selected club to the graph.
	if (m_table->GetSelectedRow() >= 0)
	{
		AddClubToGraph(m_table->GetSelectedRow());
		UpdateGraph();
	}

    //m_graphButton->Highlight(true);
    m_graphButton->SetSprite( FDResourceManager::GetSprite("uiicon","graph_on") );

    GetMainUI()->GetClubInfoSubScreen()->Hide();
  }
}

void FDLeagueTableScreen::OnTableSelect( FDTableWidget *, int row )
{
	lLogFmt("LeagueTable Select %d\n", row);

	if ((row >= 0) && (m_graph->IsVisible()))
	{
		AddClubToGraph(row);
	}

	UpdateGraph();
	UpdateClubInfo();
}

//////////////////////////////////////////////////////////////////////////
// Clears the row select history.
//////////////////////////////////////////////////////////////////////////
void FDLeagueTableScreen::ClearRowSelectHistory()
{
	for (int i = 0; i < 4; i++)
		m_iRowSelectHistory[i].id = NOCLUB;

	m_iLastRowSelectIndex = -1;
}

//////////////////////////////////////////////////////////////////////////
// Adds a single club (by its row index) to the graph. Intended to be called
// from the row select handler and the graph button handler.
//////////////////////////////////////////////////////////////////////////
void FDLeagueTableScreen::AddClubToGraph(const int iRow)
{
	const ClubID cClubIDToFind = ClubID(m_table->GetTagDataForRow(iRow));
	AddClubToGraph(cClubIDToFind);
}

void FDLeagueTableScreen::AddClubToGraph(const ClubID cClubID)
{
	// If it's not already in the iRow select history, then add it.
	bool bNeedToAdd = true;
	const int iRow = m_table->FindTagData(cClubID.id, true);
	lAssert(iRow > 0);

	for (int i = 0; i < 4; i++)
	{
		if (m_iRowSelectHistory[i] == cClubID)
		{
			bNeedToAdd = false;
			break;
		}
	}

	if (bNeedToAdd == true)
	{
		bool bAddedToHistory = false;
		for (int i = 0; i < 4; i++)
		{
			if (m_iRowSelectHistory[i].id == NOCLUB)
			{
				m_iRowSelectHistory[i] = cClubID;
				m_iRowColorHistory[i] = m_table->GetBkgColour(iRow, 0);
				m_table->SetRowBkgColour(iRow, FDGraphWidget::GetColorForDataSet(i));
				bAddedToHistory = true;
				m_iLastRowSelectIndex = i;
				break;
			}
		}

		if (bAddedToHistory == false)
		{
			// Evict oldest entry.
			const int iRowIndexToEvict = (m_iLastRowSelectIndex + 1) % 4;

			// Reset color of that row.
			const int iRowToReset = m_table->FindTagData(m_iRowSelectHistory[iRowIndexToEvict].id, true);
			lAssert(iRowToReset > 0);
			m_table->SetRowBkgColour(iRowToReset, m_iRowColorHistory[iRowIndexToEvict]);

			// Now, set up the new row.
			m_iRowSelectHistory[iRowIndexToEvict] = cClubID;
			m_iRowColorHistory[iRowIndexToEvict] = m_table->GetBkgColour(iRow, 0);
			m_table->SetRowBkgColour(iRow, FDGraphWidget::GetColorForDataSet(iRowIndexToEvict));
			m_iLastRowSelectIndex = iRowIndexToEvict;
		}
	}
}
