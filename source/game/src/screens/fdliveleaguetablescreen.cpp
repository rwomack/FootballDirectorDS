//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDLiveLeagueTableScreen.cpp
// Description : FDLiveLeagueTableScreen implementation
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
#include <fdscrollbarwidget.h>


#include <screens/fdliveleaguetablescreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDLiveLeagueTableScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDLiveLeagueTableScreen::FDLiveLeagueTableScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  // init our selves last
  Init();
}

FDLiveLeagueTableScreen::~FDLiveLeagueTableScreen()
{
}

void FDLiveLeagueTableScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  //  - Sub screen

  m_screen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "liveresult_screen_sub" );
  m_screen->Hide();

  m_table = FD_GAME_NEW FDTableWidget( m_screen, 0, "leaguetable",
              /*FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|*/
              FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELLBACKGROUND |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
              LUI_TOP_LEFT, lUIPoint( 20, 0 ), lUIPoint( 256 - 21 - GetDefaultScrollBarWidth(), 192 ) );


  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget( m_screen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-8, 0), lUIPoint(GetDefaultScrollBarWidth(), 192) );
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar(pScrollBar);
  m_table->SetCellRenderCallback(TableArrowRenderCallback, this);

  m_upArrow = FDResourceManager::GetSprite("uiicon", "uparrow");
  m_downArrow = FDResourceManager::GetSprite("uiicon", "downarrow");
}

void FDLiveLeagueTableScreen::DoShow()
{
  m_screen->Show();
}

void FDLiveLeagueTableScreen::DoHide()
{
  m_screen->Hide();
}


struct TableInfo
{
  CClub     *club;
  int        oldpos;
  int        gd;
  int        pts;
  int        goalsscored;
};

static int SortTableInfoFunc(const void *v1, const void *v2)
{
	TableInfo *t1 = (TableInfo *)v1;
	TableInfo *t2 = (TableInfo *)v2;

	if (t1->pts > t2->pts)
		return -1;
	if (t1->pts < t2->pts)
		return 1;

	if (t1->gd > t2->gd)
		return -1;
	if (t1->gd < t2->gd)
		return 1;

	if (t1->goalsscored > t2->goalsscored)
		return -1;
	if (t1->goalsscored < t2->goalsscored)
		return 1;

	return ::stricmp(t1->club->GetName().CStr(), t2->club->GetName().CStr());
}

static void CalculateTable( CMatchPlay* _MatchPlay, CFixtureList & fixtureList, lArray<TableInfo> &tableArray )
{
	tableArray.Empty( _MatchPlay->GetDivision().GetNumberOfClubsInDivision()  );
	tableArray.Add( _MatchPlay->GetDivision().GetNumberOfClubsInDivision()  );

	CDivision& pDivision = _MatchPlay->GetDivision();

	// get current position + info
	pDivision.SortLeagueTable();
	for( int LoopCount = 0; LoopCount < pDivision.GetNumberOfClubsInDivision(); LoopCount++ )
	{
		CClub* pClub = &pDivision.DoFindClubInTablePosition(LoopCount);
		TableInfo &tinfo = tableArray( LoopCount );

		tinfo.club   = pClub;
		tinfo.oldpos = LoopCount+1;
		tinfo.gd     = pClub->GetTableData().GetNumberGoalsScored() - pClub->GetTableData().GetNumberGoalsConceded();
		tinfo.pts    = pClub->GetTableData().GetPoints();
		tinfo.goalsscored = pClub->GetTableData().GetNumberGoalsScored();
	}

	// add on current results
	uint HomeGoals, AwayGoals;
	for( int LoopCount = 0; LoopCount < fixtureList.GetSize(); LoopCount++ )
	{
		CFixture* pFixture = fixtureList.GetAt(LoopCount);

		for( int TableCount = 0; TableCount < pDivision.GetNumberOfClubsInDivision(); TableCount++ )
		{
			if( tableArray( TableCount ).club == &pFixture->GetClub(HOME) )
			{
				HomeGoals = pFixture->GetNumberGoals(HOME, _MatchPlay->GetTimePlayed());
				AwayGoals = pFixture->GetNumberGoals(AWAY, _MatchPlay->GetTimePlayed());

				tableArray( TableCount ).gd += (HomeGoals - AwayGoals);

				if(HomeGoals > AwayGoals)
				{
					tableArray( TableCount ).pts += 3;
				}
				else if (HomeGoals == AwayGoals)
				{
					tableArray( TableCount ).pts += 1;
				}
			}
			else if( tableArray( TableCount ).club == &pFixture->GetClub(AWAY) )
			{
				HomeGoals = pFixture->GetNumberGoals(HOME, _MatchPlay->GetTimePlayed());
				AwayGoals = pFixture->GetNumberGoals(AWAY, _MatchPlay->GetTimePlayed());

				tableArray( TableCount ).gd += (AwayGoals - HomeGoals);

				if(HomeGoals < AwayGoals)
				{
					tableArray( TableCount ).pts += 3;
				}
				else if (HomeGoals == AwayGoals)
				{
					tableArray( TableCount ).pts += 1;
				}
			}
		}
 	}

	// sort list with new pts / gd
	::qsort(tableArray.Begin(), tableArray.Size(), sizeof(TableInfo), SortTableInfoFunc);
}

void FDLiveLeagueTableScreen::Update( CMatchPlay* _MatchPlay )
{
  if( _MatchPlay == NULL )
  {
    return;
  }

  if( !_MatchPlay->AreLeagueTablesAvailable() )
	{
    return;
  }

  int currentTime = _MatchPlay->GetTimePlayed();
  CFixtureList m_Fixtures;

  if( _MatchPlay->IsLeagueMatch() )
  {
    _MatchPlay->GetDivision().DoBuildCurrentFixtureList( m_Fixtures );
  }
  else
  {
    _MatchPlay->GetCupRound().BuildCurrentNonUserFixtureList( m_Fixtures );
  }

  // calculate table
  lArray<TableInfo> tableArray;
  CalculateTable( _MatchPlay, m_Fixtures, tableArray );

  int prevScrollPosition = 0;
  bool bDirection = false;

  m_table->GetScrollPosition(prevScrollPosition, bDirection);
  int prevScrollBarPos = m_table->GetScrollBar()->GetCurrentPosition();

  // clear
  m_table->ClearAll();

  m_table->AddColumn( lUICoord( 12.0f ), LUI_CENTER_CENTER );  // Arrow (up, down, same)
  m_table->AddColumn( lUICoord( 25.0f ), LUI_CENTER_CENTER );  // Position
  m_table->AddColumn( lUICoord( 25.0f ), LUI_CENTER_CENTER );  // Previous
  m_table->AddColumn( lUICoord( 101.0f ), LUI_CENTER_CENTER );  // club name
  m_table->AddColumn( lUICoord( 32.0f ), LUI_CENTER_CENTER );  // GD
  m_table->AddColumn( lUICoord( 32.0f ), LUI_CENTER_CENTER );  // Pts

  m_table->AddRows( _MatchPlay->GetDivision().GetNumberOfClubsInDivision() + 1 );

  m_table->SetText( 0, 0, lUIText( "" ) );
  m_table->SetText( 0, 1, lUIText( IDS_ABBR_POS ) );
  m_table->SetText( 0, 2, lUIText( IDS_ABBR_PREV ) );
  m_table->SetText( 0, 3, lUIText( IDS_CLUBS ) );
  m_table->SetText( 0, 4, lUIText( IDS_ABBR_GD ) );
  m_table->SetText( 0, 5, lUIText( IDS_ABBR_PTS ) );

  m_table->SetRowBkgColour( 0, L_ARGB(255, 35, 35, 35));

	const uint32 BkgColour[] =
	{
    L_ARGB(80, 0x50, 0x50, 0x50),      // Normal
		L_ARGB(128,255,0,0),      // Relegation spot
		L_ARGB(128,0,255,0),      // Playoff spot
    L_ARGB(128,200,200,200),  // Promotion spot
	};

  CDivision& pDivision = _MatchPlay->GetDivision();

  for( int i = 0; i < tableArray.Size(); i++ )
	{
    CClub* pClub = tableArray(i).club;

    if( pClub == WorldData().GetCurrentUserClub() )
    {
      m_table->SetRowBkgColour( i+1, L_ARGB(0xFF, 72, 134, 203) );
    } else
    {
      m_table->SetRowBkgColour( i+1, BkgColour[ pDivision.LeaguePositionStatus(i) ] );
    }

    m_table->SetTextNumber( i+1, 1, i+1 );
    m_table->SetTextNumber( i+1, 2, tableArray(i).oldpos );
    m_table->SetText( i+1, 3, lUIText( FDMakeClubNameLink(pClub) ) );
    m_table->SetTextNumber( i+1, 4, tableArray(i).gd );
    m_table->SetTextNumber( i+1, 5, tableArray(i).pts );

    luint32_t uiTagData = 0;
    if (tableArray(i).oldpos > (i + 1))
      uiTagData = 1; // rose in standings
    else if (tableArray(i).oldpos < (i + 1))
      uiTagData = 2; // fell in standings
    m_table->SetTagDataForRow(i + 1, uiTagData);
  }

  m_table->SetScrollPosition(prevScrollPosition, bDirection);
  m_table->GetScrollBar()->SetCurrentPosition( prevScrollBarPos, false );
}

//////////////////////////////////////////////////////////////////////////
// Table rendering callback. Used to render up or down arrows for each
// cell, as appropriate.
//////////////////////////////////////////////////////////////////////////
void FDLiveLeagueTableScreen::TableArrowRenderCallback(FDTableWidget *pTable, void *pScreen, luint32_t uiRow, luint32_t uiCol, lUIRect cCellRect)
{
	FDLiveLeagueTableScreen *pLeagueScreen = (FDLiveLeagueTableScreen *)pScreen;

	if ((uiCol > 0) || (uiRow == 0))
		return;

	// Use the stored tag data to determine what to render.
	luint32_t uiTagData = pTable->GetTagDataForRow(uiRow);
	if (uiTagData == 1)
	{
		pLeagueScreen->m_upArrow->SetPosition( cCellRect.x, cCellRect.y );
		pLeagueScreen->m_upArrow->SetWidth( cCellRect.w );
		pLeagueScreen->m_upArrow->SetHeight( cCellRect.h );
		pLeagueScreen->m_upArrow->Render();
	}
	else if (uiTagData == 2)
	{
    pLeagueScreen->m_downArrow->SetPosition( cCellRect.x, cCellRect.y );
    pLeagueScreen->m_downArrow->SetWidth( cCellRect.w );
    pLeagueScreen->m_downArrow->SetHeight( cCellRect.h );
    pLeagueScreen->m_downArrow->Render();
	}
}
