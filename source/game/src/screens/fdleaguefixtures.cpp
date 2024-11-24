//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDLeagueFixturesscreen.cpp
// Description : FDLeagueFixturesScreen implementation
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

#include <screens/fdleaguefixturesscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDLeagueFixturesScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDLeagueFixturesScreen::FDLeagueFixturesScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDLeagueFixturesScreen::~FDLeagueFixturesScreen()
{
}

void FDLeagueFixturesScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_leaguefixScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "leaguefix_screen" );
  m_leaguefixScreen->Hide();

  m_leaguefixScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "leaguefix_screen_sub" );
  m_leaguefixScreenSub->Hide();

  // top screen fixture info
  for (int i = 0; i < LFFT_MAX; i++)
  {
	  m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_leaguefixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * i)), lUIPoint(256, 12));
    m_fieldStripes[i] = FD_GAME_NEW lUISpriteWidget(m_leaguefixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT, lUIPoint(128, 91 + (14 * i)), lUIPoint(112, 12));
    m_fieldLabels[i] = FD_GAME_NEW lUITextWidget(m_leaguefixScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * i)), lUIPoint(108, 12));
    m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_leaguefixScreenSub, 0, "fieldvalue", lUIText(""), LUI_TOP_LEFT, lUIPoint(128, 91 + (14 * i)), lUIPoint(112, 12));

	  m_fieldLabels[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_fieldValues[i]->SetAlign(LUI_CENTER_CENTER);
  }

  m_fieldLabels[LFFT_DATE_PLAYED]->SetText(lUIText("MATCH DATE")); //TODO LOCALE
  m_fieldLabels[LFFT_VENUE]->SetText(lUIText(IDS_VENUE_NOCOLON));
  m_fieldLabels[LFFT_ATTENDANCE]->SetText(lUIText(IDS_ATTENDANCE));

  for (int i = 0; i < LFFT_MAX; i++)
  {
    m_fieldLabels[i]->SetTextToUpper();
  }


  m_homeClubText = FD_GAME_NEW lUITextWidget( m_leaguefixScreenSub, 0, "hometeam", lUIText(""), LUI_TOP_LEFT, lUIPoint( 59, 46 ), lUIPoint(125, 12) );
  m_awayClubText = FD_GAME_NEW lUITextWidget( m_leaguefixScreenSub, 0, "awayteam", lUIText(""), LUI_TOP_LEFT, lUIPoint(59, 69), lUIPoint(125, 12) );
  m_homeGoalsText = FD_GAME_NEW lUITextWidget( m_leaguefixScreenSub, 0, "homegoals", lUIText(""), LUI_TOP_LEFT, lUIPoint(172, 46), lUIPoint(25, 12) );
  m_awayGoalsText = FD_GAME_NEW lUITextWidget( m_leaguefixScreenSub, 0, "awaygoals", lUIText(""), LUI_TOP_LEFT, lUIPoint(172, 69), lUIPoint(25, 12) );

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

  m_splitterBar = FD_GAME_NEW FDBarWidget( m_leaguefixScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  m_splitterBar->SetPctA(0.0f);
  m_splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
  m_splitterBar->Hide();

  m_homekitBG = FD_GAME_NEW lUISpriteWidget( m_leaguefixScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"),
                                             LUI_TOP_LEFT, lUIPoint( 5, 42 ) );
  m_homekitBG->GetSprite()->SetAlphaEnable( true );
  lUIPoint pt = m_homekitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_homeClubKit = FD_GAME_NEW lUISpriteWidget( m_leaguefixScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, pt );
  m_homeClubKit->Hide();

  m_awaykitBG = FD_GAME_NEW lUISpriteWidget( m_leaguefixScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"),
                                             LUI_TOP_LEFT, lUIPoint( 210, 42 ) );
  m_awaykitBG->GetSprite()->SetAlphaEnable( true );
  pt = m_awaykitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_awayClubKit = FD_GAME_NEW lUISpriteWidget( m_leaguefixScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, pt );
  m_awayClubKit->Hide();

  // combo to select division + date
  m_divisionSelector = FD_GAME_NEW FDComboWidget( m_leaguefixScreen, 0, "lfdivisionselect", lUIText(""), LUI_CENTER_BOTTOM, lUIPoint( 138, 163 ), lUIPoint( 200, 19 ) );
  m_divisionSelector->SignalSelection.Connect( this, &FDLeagueFixturesScreen::OnDivisionSelect );
  m_divisionSelector->SetAllToUpper( true );

  // date selector
  m_dateSelector = FD_GAME_NEW FDComboWidget( m_leaguefixScreen, 0, "lfdateselect", lUIText(""), LUI_CENTER_TOP, lUIPoint( 138, 165 ), lUIPoint( 200, 19 ) );
  m_dateSelector->SignalSelection.Connect( this, &FDLeagueFixturesScreen::OnDateSelect );
  m_dateSelector->SetAllToUpper( true );

  // bottom screen table
  m_table = FD_GAME_NEW FDTableWidget( m_leaguefixScreen, 0, "leaguefixtable",
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELLBACKGROUND|
            FDTableWidget::TABLE_FLAG_CELL_SELECT|FDTableWidget::TABLE_FLAG_BACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 135 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_leaguefixScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 10), lUIPoint(GetDefaultScrollBarWidth(), 135));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar(pScrollBar);
  m_table->SignalCellSelect.Connect( this, &FDLeagueFixturesScreen::OnMatchSelect );

  //

  m_theDate.DoInitialise( WorldData().GetCurrentDate() );
}

void FDLeagueFixturesScreen::OnDivisionSelect(  FDComboWidget *, int index )
{
  lLogFmt("OnDivisionSelect %d\n", index);

  // set up date selector
  CDivisionList &divisionList = WorldData().GetDivisionList();
  CDivision &division = divisionList.GetAt( m_divisionSelector->GetSelectedItem() );

  CCalendar curDate = WorldData().GetCurrentDate();

  CCalendar firstDate = GetSeasonStartDate();
  CCalendar lastDate  = firstDate;
  division.DoFindEndOfSeason(lastDate);

  lLogFmt("Division %s: from %s to %s\n", division.GetName(), firstDate.ShortDateStr(), lastDate.ShortDateStr());

  // make up list
  m_dateSelector->ClearAll();
  m_dateList.Empty();
  while(1)
  {
    // Check that the date has matches
    CSoccerResultList resultList;
    division.DoBuildLeagueFixtureList( firstDate, resultList );
    if( resultList.GetSize() > 0 )
    {
      m_dateList.Push( firstDate );
      m_dateSelector->AddItem( lUIText( firstDate.ShortDateStr() ) );
    }

    if( firstDate == lastDate )
    {
      // reached end of fixture list
      break;
    }

    // get next date
    division.NextMatchDateThisSeason(firstDate);

    if( division.IsEndSeasonToday(firstDate.GetMatchDay()) )
    {
      break;
    }
  }

  // Select  the next date
  for( int i = 0; i < m_dateList.Size(); i++ )
  {
    if( m_dateList(i) > curDate )
    {
      m_dateSelector->SetSelectedItem(i, false);
      break;
    }
  }
}

void FDLeagueFixturesScreen::OnDateSelect(  FDComboWidget *, int index )
{
  lLogFmt("OnDateSelect %d\n", index);
  if( index != -1 )
  {
    m_theDate = m_dateList( index );
    lLogFmt(" Date: %s\n", m_theDate.ShortDateStr());
    UpdateTable();
  }
}

void FDLeagueFixturesScreen::UpdateTable()
{
  // build result list
  CDivisionList &divisionList = WorldData().GetDivisionList();
  CDivision &division = divisionList.GetAt( m_divisionSelector->GetSelectedItem() );

  m_resultList.RemoveList();
  division.DoBuildLeagueFixtureList( m_theDate, m_resultList );

  // set up table
  m_table->ClearAll();

  lLogFmt("%d results\n", m_resultList.GetSize());

  if( m_resultList.GetSize() > 0 )
  {
    m_table->AddColumn( lUICoord( 95 ), LUI_RIGHT_CENTER );   // date
    m_table->AddColumn( lUICoord( 40 ) );   // result
    m_table->AddColumn( lUICoord( 95 ), LUI_LEFT_CENTER );  // opponent name

    m_resultList.SortByHomeClub();
    m_table->AddRows( m_resultList.GetSize() );

    for (int LoopCount = 0; LoopCount < m_resultList.GetSize(); LoopCount++)
    {
     CSoccerResult* pResult = m_resultList.GetAt(LoopCount);

     m_table->SetBkgColour(LoopCount, 0, L_ARGB(0xFF, 116, 113, 113));
     m_table->SetBkgColour(LoopCount, 1, L_ARGB(0xFF, 82, 82, 82));
     m_table->SetBkgColour(LoopCount, 2, L_ARGB(0xFF, 116, 113, 113));

     // Highlight the user's result
     if( pResult->GetHomeClubDetails().IsUserControlledClub() == true ||
       pResult->GetAwayClubDetails().IsUserControlledClub() == true)
     {
       m_table->SetBkgColour( LoopCount, 0, L_ARGB(0xFF, 72, 134, 203) );
       m_table->SetBkgColour( LoopCount, 2, L_ARGB(0xFF, 72, 134, 203) );
     }

     const lString sHomeClubString = FDMakeClubNameLink(&WorldData().GetClubByID(pResult->GetHomeClubDetails().GetClubID()));
     const lString sAwayClubString = FDMakeClubNameLink(&WorldData().GetClubByID(pResult->GetAwayClubDetails().GetClubID()));

     m_table->SetText( LoopCount, 0, lUIText( sHomeClubString ) );
     //m_table->SetText( LoopCount, 1, lUIText( pResult->GoalsStr() ) );
     m_table->SetText( LoopCount, 1, lUIText( FDMakeLeagueLink( m_divisionSelector->GetSelectedItem(), LoopCount, &m_theDate, pResult->GoalsStr()) ) );
     m_table->SetText( LoopCount, 2, lUIText( sAwayClubString ) );
    }
  }
}

void FDLeagueFixturesScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int date     = (int)m_dateSelector->GetSelectedItem();
    int division = (int)m_divisionSelector->GetSelectedItem();
    int selectedFix = m_table->GetSelectedRow();

    state << date;
    state << division;
    state << selectedFix;

    lLogFmt("Saving date %d division %d fixture %d\n", date, division, selectedFix);
  } else
  {
    int date;
    int division;
    int selectedFix;

    state << date;
    state << division;
    state << selectedFix;

    lLogFmt("Loading date %d division %d fixture %d\n", date, division, selectedFix);
    m_divisionSelector->SetSelectedItem( division, false );
    m_dateSelector->SetSelectedItem( date,  false );
    m_table->SelectCell( selectedFix, 0 );
  }
}

void FDLeagueFixturesScreen::Update()
{
  if (IsForcedUpdateRequested() == false)
	  return;

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

  // Select  the next date
  CCalendar curDate = WorldData().GetCurrentDate();
  bool bSelectedDate = false;
  for( int i = 0; i < m_dateList.Size(); i++ )
  {
    if( m_dateList(i) > curDate )
    {
      m_dateSelector->SetSelectedItem(i, false);
	  bSelectedDate = true;
      break;
    }
  }

  if (bSelectedDate == false)
	  m_dateSelector->SetSelectedItem(0, false);

  // update table
  UpdateTable();

  SetForcedUpdateRequested(false);
}

void FDLeagueFixturesScreen::Prepare()
{
  //TODO LOCALE

  if (m_table->GetSelectedRow() == -1)
  {
	  HideDetailDisplay();
  }

  //GetMainUI()->HideSubMessage();
}

void FDLeagueFixturesScreen::DoShow()
{
  //
  m_leaguefixScreen->Show();
  m_leaguefixScreenSub->Show();
}

void FDLeagueFixturesScreen::DoHide()
{
  m_leaguefixScreen->Hide();
  m_leaguefixScreenSub->Hide();
}

void FDLeagueFixturesScreen::OnMatchSelect( FDTableWidget *, int row, int col )
{
  lLogFmt("Match %d selected\n", row);

  if( row == -1 )
  {
    HideDetailDisplay();
    return;
  }

  for (int i = 0; i < LFFT_MAX; i++)
  {
    m_backgroundStripes[i]->Show();
    m_fieldStripes[i]->Show();
    m_fieldLabels[i]->Show();
    m_fieldValues[i]->Show();
  }

  GetMainUI()->HideSubMessage();

  m_splitterBar->Show();

  m_homeClubKit->Show();
  m_awayClubKit->Show();

  m_homekitBG->Show();
  m_awaykitBG->Show();

  CSoccerResult *result = m_resultList.GetAt(row);
  CString str;

  CClub *pHomeClub = &WorldData().GetClubByID(result->GetHomeClubDetails().GetClubID());
  CClub *pAwayClub = &WorldData().GetClubByID(result->GetAwayClubDetails().GetClubID());

  if (( result->GetFixture() != NULL ) && result->GetFixture()->HasBeenSetupOrUsed())
  {
    m_fieldValues[LFFT_VENUE]->SetText(lUIText(result->GetFixture()->StadiumName().CStr()));
  } else
  {
    m_fieldValues[LFFT_VENUE]->SetText(lUIText(pHomeClub->GetStadium()->GetName().CStr()));
  }

  m_homeClubText->SetText(lUIText(pHomeClub->GetName().CStr()), true );
  m_awayClubText->SetText(lUIText(pAwayClub->GetName().CStr()), true );

  m_homeClubKit->GetSprite().Get()->SetFrame( pHomeClub->GetHomeKitID() );
  m_awayClubKit->GetSprite().Get()->SetFrame( pAwayClub->GetHomeKitID() );

  lLogFmt("League fixture %d - %s\n", row, m_theDate.LongDateStr().CStr());
  m_fieldValues[LFFT_DATE_PLAYED]->SetText(lUIText(m_theDate.LongDateStr().CStr()));

  if( result->GetFixture() != NULL )
  {
	  str.Format("%u", result->GetFixture()->GetAttendance());
	  m_fieldValues[LFFT_ATTENDANCE]->SetText(lUIText(str.CStr()));

	  str.Format("%d", (int)result->GetFixture()->GetNumberGoals(HOME));
	  m_homeGoalsText->SetText(lUIText(str.CStr()));

	  str.Format("%d", (int)result->GetFixture()->GetNumberGoals(AWAY));
	  m_awayGoalsText->SetText(lUIText(str.CStr()));
  } else
  {
    m_fieldValues[LFFT_ATTENDANCE]->SetText(lUIText(""));

    m_homeGoalsText->SetText(lUIText(""));
    m_awayGoalsText->SetText(lUIText(""));
  }

  /*if ((col == 1) && (result->GetFixture() != NULL))
  {
    // Open Summary screen based on table selection
    theApp.OnShowMatchSummary( NULL,  result->GetFixture(), true );
    //GetMainUI()->PopScreen();
  }*/
}

void FDLeagueFixturesScreen::HideDetailDisplay()
{
    for (int i = 0; i < LFFT_MAX; i++)
    {
      m_backgroundStripes[i]->Hide();
      m_fieldStripes[i]->Hide();
      m_fieldLabels[i]->Hide();
      m_fieldValues[i]->Hide();
    }

    m_homekitBG->Hide();
    m_awaykitBG->Hide();

    m_splitterBar->Hide();

    m_homeClubText->SetText(lUIText(""));
    m_awayClubText->SetText(lUIText(""));
    m_homeGoalsText->SetText(lUIText(""));
    m_awayGoalsText->SetText(lUIText(""));
    m_homeClubKit->Hide();
    m_awayClubKit->Hide();
    m_fieldValues[LFFT_DATE_PLAYED]->SetText(lUIText(""));
    m_fieldValues[LFFT_VENUE]->SetText(lUIText(""));
    m_fieldValues[LFFT_ATTENDANCE]->SetText(lUIText(""));
    GetMainUI()->DisplaySubMessage("Select Fixture"); //TODO LOCALE
}
