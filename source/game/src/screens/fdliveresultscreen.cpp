//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDLiveResultScreen.cpp
// Description : FDLiveResultScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fdscrollbarwidget.h>


#include <screens/fdliveresultscreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDLiveResultsScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDLiveResultScreen::FDLiveResultScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDLiveResultScreen::~FDLiveResultScreen()
{
}

void FDLiveResultScreen::Init()
{
  m_screen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "liveleaguetable_screen_sub" );
  m_screen->Hide();

  // score table
  // NOTE: May need scroll bar for cup matches, or we could cycle between leagues / pages like Sky Sports news?
  m_table = FD_GAME_NEW FDTableWidget( m_screen, 0, "resulttable",
        /*FDTableWidget::TABLE_FLAG_ROW_LINES | */FDTableWidget::TABLE_FLAG_BORDER | FDTableWidget::TABLE_FLAG_CUT_OFF | FDTableWidget::TABLE_FLAG_CELLBACKGROUND,
              LUI_TOP_LEFT, lUIPoint( 20, 5 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 185 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget( m_screen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 0), lUIPoint(GetDefaultScrollBarWidth(), 190) );
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar(pScrollBar);
}

void FDLiveResultScreen::DoShow()
{
  m_screen->Show();
  m_table->Show();
}

void FDLiveResultScreen::DoHide()
{
  m_screen->Hide();
  m_table->Hide();
}

struct StLiveResultStruct
{
	int iFixtureIndex;
	CFixtureList *pFixtures;
};

static int LiveResultSortFunc(const void *v1, const void *v2)
{
	const StLiveResultStruct *s1 = (StLiveResultStruct *)v1;
	const StLiveResultStruct *s2 = (StLiveResultStruct *)v2;

	CFixtureList *pFixtures = s1->pFixtures;
	const CString &cName1 = (*pFixtures)[s1->iFixtureIndex].GetClubName(HOME);
	const CString &cName2 = (*pFixtures)[s2->iFixtureIndex].GetClubName(HOME);

	return ::stricmp(cName1.CStr(), cName2.CStr());
}

void FDLiveResultScreen::Update( CMatchPlay* _MatchPlay )
{
	int prevScrollPosition = 0;
	bool bDirection = false;

	m_table->GetScrollPosition(prevScrollPosition, bDirection);
	int prevScrollBarPos = m_table->GetScrollBar()->GetCurrentPosition();

	// clear
	m_table->ClearAll();

  m_table->AddColumn( lUICoord( 92.0f ), LUI_RIGHT_CENTER );   // home name
  m_table->AddColumn( lUICoord( 42.0f ), LUI_CENTER_CENTER );  // score
  m_table->AddColumn( lUICoord( 92.0f ), LUI_LEFT_CENTER );    // away name

	if((_MatchPlay == NULL) || (!_MatchPlay->AreLatestScoresAvailable()))
	{
		return;
	}

	int currentTime = _MatchPlay->GetTimePlayed();
	CFixtureList cFixtures;

	if (_MatchPlay->IsLeagueMatch())
	{
		_MatchPlay->GetDivision().DoBuildCurrentFixtureList( cFixtures );
	}
	else
	{
		_MatchPlay->GetCupRound().BuildCurrentNonUserFixtureList( cFixtures );
	}

	m_table->AddRows( cFixtures.GetSize() );

	StLiveResultStruct *pResults = FD_GAME_NEW StLiveResultStruct[cFixtures.GetSize()];
	for (int i = 0; i < cFixtures.GetSize(); i++)
	{
		pResults[i].iFixtureIndex = i;
		pResults[i].pFixtures = &cFixtures;
	}

	::qsort(pResults, cFixtures.GetSize(), sizeof(StLiveResultStruct), LiveResultSortFunc);

	for (int LoopCount = 0; LoopCount < cFixtures.GetSize(); LoopCount++)
	{
		// Highlight the user's result
		CFixture *pFixture = &cFixtures[pResults[LoopCount].iFixtureIndex];

		if( pFixture->IsUserMatch() == true )
		{
      m_table->SetRowBkgColour( LoopCount, L_ARGB(0xFF, 72, 134, 203) );
		}
		else
		{
      m_table->SetRowBkgColour( LoopCount, L_ARGB(0xFF, 0x50, 0x50, 0x50) );
		}

		m_table->SetText( LoopCount, 0, lUIText( pFixture->GetClubName(HOME) ) );
		m_table->SetText( LoopCount, 1, lUIText( pFixture->GoalsStr( currentTime ) ) );
		m_table->SetText( LoopCount, 2, lUIText( pFixture->GetClubName(AWAY) ) );
	}

	delete[] pResults;

	m_table->SetScrollPosition(prevScrollPosition, bDirection);
	m_table->GetScrollBar()->SetCurrentPosition( prevScrollBarPos, false );
}
