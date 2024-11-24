//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDClubFixturesscreen.cpp
// Description : FDClubFixturesScreen implementation
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
#include <fdtext.h>


#include <screens/fdclubfixturesscreen.h>
#include <screens/fdmatchsummaryscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDClubFixturesScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDClubFixturesScreen::FDClubFixturesScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDClubFixturesScreen::~FDClubFixturesScreen()
{
}

void FDClubFixturesScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_clubfixScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "clubfix_screen" );
  m_clubfixScreen->Hide();

  m_clubfixScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "clubfix_screen_sub" );
  m_clubfixScreenSub->Hide();

  // top screen fixture info
  for (int i = 0; i < CFFT_COMPETITION; i++)
  {
	  m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_clubfixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * i)), lUIPoint(256, 12));
	  m_fieldStripes[i] = FD_GAME_NEW lUISpriteWidget(m_clubfixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT, lUIPoint(128, 91 + (14 * i)), lUIPoint(112, 12));
      m_fieldLabels[i] = FD_GAME_NEW lUITextWidget(m_clubfixScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * i)), lUIPoint(108, 12));
      m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_clubfixScreenSub, 0, "fieldvalue", lUIText(""), LUI_TOP_LEFT, lUIPoint(128, 91 + (14 * i)), lUIPoint(112, 12));

	  m_fieldLabels[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_fieldValues[i]->SetAlign(LUI_CENTER_CENTER);
  }

  m_backgroundStripes[CFFT_COMPETITION] = FD_GAME_NEW lUISpriteWidget(m_clubfixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * 3)), lUIPoint(256, 12));
  m_fieldStripes[CFFT_COMPETITION] = FD_GAME_NEW lUISpriteWidget(m_clubfixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * 4)), lUIPoint(256, 12));
  m_fieldLabels[CFFT_COMPETITION] = FD_GAME_NEW lUITextWidget(m_clubfixScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * 3)), lUIPoint(256, 12));
  m_fieldValues[CFFT_COMPETITION] = FD_GAME_NEW lUITextWidget(m_clubfixScreenSub, 0, "fieldvalue", lUIText(""), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * 4)), lUIPoint(256, 12));

  m_fieldLabels[CFFT_COMPETITION]->SetAlign(LUI_CENTER_CENTER);
  m_fieldValues[CFFT_COMPETITION]->SetAlign(LUI_CENTER_CENTER);

  m_fieldLabels[CFFT_COMPETITION]->SetText(lUIText(IDS_COMPETITION));
  m_fieldLabels[CFFT_DATE_PLAYED]->SetText(lUIText("MATCH DATE")); //TODO LOCALE
  m_fieldLabels[CFFT_VENUE]->SetText(lUIText(IDS_VENUE_NOCOLON));
  m_fieldLabels[CFFT_ATTENDANCE]->SetText(lUIText(IDS_ATTENDANCE));

  for (int i = 0; i < CFFT_MAX; i++)
  {
    m_fieldLabels[i]->SetTextToUpper();

    m_backgroundStripes[i]->Hide();
	  m_fieldStripes[i]->Hide();
	  m_fieldLabels[i]->Hide();
	  m_fieldValues[i]->Hide();
  }

  m_homeClubText = FD_GAME_NEW lUITextWidget( m_clubfixScreenSub, 0, "hometeam", lUIText(""), LUI_TOP_LEFT, lUIPoint( 59, 46 ), lUIPoint(125, 12) );
  m_awayClubText = FD_GAME_NEW lUITextWidget( m_clubfixScreenSub, 0, "awayteam", lUIText(""), LUI_TOP_LEFT, lUIPoint(59, 69), lUIPoint(125, 12) );
  m_homeGoalsText = FD_GAME_NEW lUITextWidget( m_clubfixScreenSub, 0, "homegoals", lUIText(""), LUI_TOP_LEFT, lUIPoint(172, 46), lUIPoint(25, 12) );
  m_awayGoalsText = FD_GAME_NEW lUITextWidget( m_clubfixScreenSub, 0, "awaygoals", lUIText(""), LUI_TOP_LEFT, lUIPoint(172, 69), lUIPoint(25, 12) );

  m_homeClubText->SetFont( FDResourceManager::GetFont("font16", "font16" ) );
  m_awayClubText->SetFont( FDResourceManager::GetFont("font16", "font16" ) );
  m_homeGoalsText->SetFont( FDResourceManager::GetFont("font16", "font16" ) );
  m_awayGoalsText->SetFont( FDResourceManager::GetFont("font16", "font16" ) );
  m_homeClubText->SetAlign(LUI_CENTER_LEFT);
  m_awayClubText->SetAlign(LUI_CENTER_LEFT);
  m_homeGoalsText->SetAlign(LUI_CENTER_RIGHT);
  m_awayGoalsText->SetAlign(LUI_CENTER_RIGHT);

  m_homeClubText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_awayClubText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_homeGoalsText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_awayGoalsText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_splitterBar = FD_GAME_NEW FDBarWidget( m_clubfixScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  m_splitterBar->SetPctA(0.0f);
  m_splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
  m_splitterBar->Hide();

  m_homekitBG = FD_GAME_NEW lUISpriteWidget( m_clubfixScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"),
                                             LUI_TOP_LEFT, lUIPoint( 5, 42 ) );
  m_homekitBG->GetSprite()->SetAlphaEnable( true );
  lUIPoint pt = m_homekitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_homeClubKit = FD_GAME_NEW lUISpriteWidget( m_clubfixScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, pt );
  m_homeClubKit->Hide();

  m_awaykitBG = FD_GAME_NEW lUISpriteWidget( m_clubfixScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"),
                                             LUI_TOP_LEFT, lUIPoint( 210, 42 ) );
  m_awaykitBG->GetSprite()->SetAlphaEnable( true );
  pt = m_awaykitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_awayClubKit = FD_GAME_NEW lUISpriteWidget( m_clubfixScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, pt );
  m_awayClubKit->Hide();

  for (int i = 0; i < 2; i++ )
  {
    m_penaltyStripes[i] = FD_GAME_NEW lUISpriteWidget( m_clubfixScreenSub, 0, "penstripe",  FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_TOP_LEFT,
      lUIPoint( 0, 160 + i * 14 ), lUIPoint( 256, 12) );
    m_penaltyResults[i] = FD_GAME_NEW lUITextWidget( m_clubfixScreenSub, 0, "penresult", lUIText(""), LUI_TOP_CENTER,
      lUIPoint(128, 160 + i * 14), lUIPoint(256, 12) );

    m_penaltyStripes[i]->Hide();
    m_penaltyResults[i]->Hide();
  }

  // bottom screen table
  m_table = FD_GAME_NEW FDTableWidget( m_clubfixScreen, 0, "clubfixtable",
            /*FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|*/
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELLBACKGROUND|
            FDTableWidget::TABLE_FLAG_CELL_SELECT | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 20, 12 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 168 ) );

  FDScrollBarWidget *tableScroll = FD_GAME_NEW FDScrollBarWidget(m_clubfixScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(248, 12), lUIPoint(GetDefaultScrollBarWidth(), 168));
  tableScroll->AlignTo( m_table );
  m_table->SetScrollBar(tableScroll);
  m_table->SignalCellSelect.Connect( this, &FDClubFixturesScreen::OnMatchSelect );
}

void FDClubFixturesScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int scrollPos = m_table->GetScrollBar()->GetCurrentPosition();
    int selected  = m_table->GetSelectedRow();
    state << scrollPos;
    state << selected;
    lLogFmt("Saving scroll pos %d selected %d\n", scrollPos, selected);
  } else
  {
    int scrollPos;
    int selected;
    state << scrollPos;
    state << selected;
    lLogFmt("Loading scroll pos %d selected %d\n", scrollPos, selected);
    m_table->GetScrollBar()->SetCurrentPosition(scrollPos, true);
    m_table->SelectCell( selected, 0 );
  }
}


void FDClubFixturesScreen::Update()
{
	if (IsForcedUpdateRequested() == false)
		return;

	SetForcedUpdateRequested(false);

	m_table->ClearAll();

	m_table->AddColumn( lUICoord( 52 ), LUI_RIGHT_CENTER );   // date
	m_table->AddColumn( lUICoord( 115 ) );   //  opponent name
	m_table->AddColumn( lUICoord( 30 ), LUI_LEFT_CENTER );  //  result
	m_table->AddColumn( lUICoord( 15 ) );   // league position
	m_table->AddColumn( lUICoord( 15 ) );   //  venue

	eClubResultsType m_eMatchType = MATCHES_ALL;

	CClub * pCurrentClub = WorldData().GetCurrentUserClub();

	// add all matches
	CCalendar cCurrentDate(WorldData().GetCurrentDate());
  CCalendar TheDate = GetSeasonStartDate();

	m_matchList.Empty();
	m_matchListDate.Empty();

	bool bFoundNextMatch = false;
	int iRowToSelectAndHighlight = 0;
  int LeagueMatchesPlayed = 0;
	CMatchInfo MatchDetails;
	do
	{
		if((pCurrentClub->IsMatchToday(TheDate, MatchDetails) == true) &&
		   (MatchDetails.GetCompetitionType() != FRIENDLYMATCH))
		{
			int currentRow = m_table->AddRows(1);
			bool bHighlightAsNextMatch = false;

			if ((bFoundNextMatch == false) && (TheDate >= cCurrentDate))
			{
				bFoundNextMatch = true;
				bHighlightAsNextMatch = true;
				iRowToSelectAndHighlight = currentRow;
			}

			m_matchList.Push( MatchDetails );
			m_matchListDate.Push( TheDate );

			// date
			m_table->SetText( currentRow, 0, lUIText( TheDate.ShortDateStr() ) );

			// name / location
			if( MatchDetails.IsHomeClub(pCurrentClub) == true )
			{
				m_table->SetRowBkgColour( currentRow, L_ARGB(128,0,0,0) );

				m_table->SetText( currentRow, 1, lUIText( FDMakeClubNameLink( MatchDetails.GetOpponentClub(pCurrentClub), MatchDetails.GetAwayName().MakeUpper() ) ) );
        m_table->SetText( currentRow, 4, lUIText( lText::Text(IDS_HOMEOPPONENTS) ) );
			}
			else
			{
				m_table->SetRowBkgColour( currentRow, L_ARGB(128,0,0,128) );

				m_table->SetText( currentRow, 1, lUIText( FDMakeClubNameLink( MatchDetails.GetOpponentClub(pCurrentClub), MatchDetails.GetHomeName() ) ) );
        m_table->SetText( currentRow, 4, lUIText( lText::Text(IDS_AWAYOPPONENTS) ) );
			}

			if (bHighlightAsNextMatch)
			{
				m_table->SetRowBkgColour(currentRow, L_ARGB(128, 192, 128, 128));
			}

			// Display the result if it's been played
			if( MatchDetails.GetFixture() != null )
			{
				// NOTE: Friendly matches do not have a Fixture kept in MatchDetails yet
				// once this has change these comments will no longer matter
				if( MatchDetails.GetFixture()->HasMatchBeenPlayed() == true )
				{
					CString strPrint;
					strPrint.Format("%d-%d", MatchDetails.GetFixture()->GetNumberGoals(HOME), MatchDetails.GetFixture()->GetNumberGoals(AWAY));
					m_table->SetText( currentRow, 2, lUIText( FDEngineMakeMatchLink( &TheDate, strPrint ) ) );

          if( MatchDetails.GetFixture()->GetMatchType() != LEAGUEMATCH )
					{
						// Non-league fixture.
						m_table->SetText(currentRow, 3, lUIText("--"));
					}
					else
					{
						// League fixture.
            strPrint.Format("%d", pCurrentClub->GetLeaguePositionAfterMatch()->GetAt(LeagueMatchesPlayed) + 1 );
						m_table->SetText( currentRow, 3, lUIText( strPrint ) );
            LeagueMatchesPlayed++;
					}
				}
			}
		}

		TheDate.NextMatchDay();

	} while(pCurrentClub->GetDivision().GetMatchType(TheDate.GetMatchDay()) != DIVENDSEASON);

	if (bFoundNextMatch == true)
		m_table->CenterOnRow(iRowToSelectAndHighlight);
}

void FDClubFixturesScreen::Prepare()
{
  //TODO LOCALE
  GetMainUI()->DisplaySubMessage( "Select Fixture"); // or No Fixtures Drawn
}

void FDClubFixturesScreen::DoShow()
{
  m_clubfixScreen->Show();
  m_clubfixScreenSub->Show();
}

void FDClubFixturesScreen::DoHide()
{
  m_clubfixScreen->Hide();
  m_clubfixScreenSub->Hide();
}

extern CFootballDirectorApp theApp;

void FDClubFixturesScreen::OnMatchSelect( FDTableWidget *, int row, int col )
{
  lLogFmt("Match %d selected\n", row);

  GetMainUI()->DisplaySubMessage( "Select Fixture"); //TODO LOCALE

  m_homeClubText->SetText(lUIText(""));
  m_awayClubText->SetText(lUIText(""));
  m_homeGoalsText->SetText(lUIText(""));
  m_awayGoalsText->SetText(lUIText(""));

  m_splitterBar->Hide();

  m_fieldValues[CFFT_COMPETITION]->SetText(lUIText(""));
  m_fieldValues[CFFT_DATE_PLAYED]->SetText(lUIText(""));
  m_fieldValues[CFFT_VENUE]->SetText(lUIText(""));
  m_fieldValues[CFFT_ATTENDANCE]->SetText(lUIText(""));

  for (int i = 0; i < CFFT_MAX; i++)
  {
	  m_backgroundStripes[i]->Hide();
	  m_fieldStripes[i]->Hide();
	  m_fieldLabels[i]->Hide();
	  m_fieldValues[i]->Hide();
  }


  for (int i = 0; i < 2; i++ )
  {
    m_penaltyStripes[i]->Hide();
    m_penaltyResults[i]->Hide();
  }

  m_homekitBG->Hide();
  m_awaykitBG->Hide();

  if( row == -1 )
  {
    m_homeClubKit->Hide();
    m_awayClubKit->Hide();
    return;
  }

  GetMainUI()->HideSubMessage();

  m_splitterBar->Show();

  m_homekitBG->Show();
  m_awaykitBG->Show();
  m_homeClubKit->Show();
  m_awayClubKit->Show();

  for (int i = 0; i < CFFT_MAX; i++)
  {
	  m_backgroundStripes[i]->Show();
	  m_fieldStripes[i]->Show();
	  m_fieldLabels[i]->Show();
	  m_fieldValues[i]->Show();
  }

  CMatchInfo &match = m_matchList( row );
  CString str;

  CClub *pHomeClub = &WorldData().GetClubByID(match.GetHomeClubID());
  CClub *pAwayClub = &WorldData().GetClubByID(match.GetAwayClubID());
  m_fieldValues[CFFT_VENUE]->SetText(lUIText(match.GetStadiumName().CStr()));
  m_homeClubText->SetText(lUIText(pHomeClub->GetName().CStr()), true);
  m_awayClubText->SetText(lUIText(pAwayClub->GetName().CStr()), true);

  m_homeClubKit->GetSprite().Get()->SetFrame( pHomeClub->GetHomeKitID() );
  m_awayClubKit->GetSprite().Get()->SetFrame( pAwayClub->GetHomeKitID() );

  m_fieldValues[CFFT_COMPETITION]->SetText(lUIText( match.GetCompetitionTitle().CStr()));
  m_fieldValues[CFFT_DATE_PLAYED]->SetText(lUIText( m_matchListDate(row).LongDateStr().CStr() ));

  if( match.GetFixture() != NULL )
  {
    if( match.GetFixture()->HasMatchBeenPlayed() == true )
    {
      str.Format("%u", match.GetFixture()->GetAttendance());
      m_fieldValues[CFFT_ATTENDANCE]->SetText(lUIText(str.CStr()));

      str.Format("%d", (int)match.GetFixture()->GetNumberGoals(HOME));
      m_homeGoalsText->SetText(lUIText(str.CStr()));

      str.Format("%d", (int)match.GetFixture()->GetNumberGoals(AWAY));
      m_awayGoalsText->SetText(lUIText(str.CStr()));

      if( match.GetFixture()->GetEventList()->WasPenaltyShootOut() )
      {
        for (int i = 0; i < 2; i++ )
        {
          m_penaltyStripes[i]->Show();
          m_penaltyResults[i]->Show();
        }

        int homePenalties = match.GetFixture()->GetEventList()->NumberHomePenaltyShootOutGoals();
        int awayPenalties = match.GetFixture()->GetEventList()->NumberAwayPenaltyShootOutGoals();

        CString strClipped;

        if( homePenalties > awayPenalties )
          strClipped = pHomeClub->GetName().CStr();
        else if( homePenalties < awayPenalties )
          strClipped = pAwayClub->GetName().CStr();
        else
          EXT_ASSERT(false);            // shouldn't get here

          lSharedPtr<lFont> font = m_penaltyResults[0]->GetFont();

          int Width = 0, Height = 0;

          font->GetStringBound( Width, Height, strClipped.CStr() );

          if( Width > 70 )
          {
            bool bDone = false;

            while( !bDone )
            {
              font->GetStringBound( Width, Height, strClipped.CStr() );

              if( Width > 70 - 5 )
              {
                strClipped = strClipped.Left( strClipped.Size()-1 );
                if( strClipped.Size()  == 0 ) bDone = true;
              }
              else
              {
                bDone = true;
              }
            }
          }

        str.Format( "%s WON ON PENALTIES", strClipped.CStr() ); // TODO LOCALE

        m_penaltyResults[0]->SetText( lUIText( str.CStr() ) );

        str.Format( "%d - %d", match.GetFixture()->GetEventList()->NumberHomePenaltyShootOutGoals(),
                                        match.GetFixture()->GetEventList()->NumberAwayPenaltyShootOutGoals() );
        m_penaltyResults[1]->SetText( lUIText( str.CStr() ) );
      }
    }
  }

  /*if( (col == 2) && match.GetFixture() != NULL )
  {
    if( match.GetFixture()->HasMatchBeenPlayed() == true )
    {
      // Open Summary screen based on table selection
      theApp.OnShowMatchSummary( match.GetCup(), match.GetFixture(), true );
      //GetMainUI()->PopScreen();
    }
  }*/
}
