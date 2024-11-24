//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDCupFixturesscreen.cpp
// Description : FDCupFixturesScreen implementation
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

#include <screens/fdcupfixturesscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#include <SortedStringList.h>
#include <CupDisplayEvent.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDCupFixturesScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDCupFixturesScreen::FDCupFixturesScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  m_initialCupIdx = -1;
  Init();
}

FDCupFixturesScreen::~FDCupFixturesScreen()
{
}

void FDCupFixturesScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_cupfixScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "cupfix_screen" );
  m_cupfixScreen->Hide();

  m_table = FD_GAME_NEW FDTableWidget( m_cupfixScreen, 0, "cupfixtable",
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELLBACKGROUND|
            FDTableWidget::TABLE_FLAG_CELL_SELECT | FDTableWidget::TABLE_FLAG_CUT_OFF|FDTableWidget::TABLE_FLAG_BACKGROUND,
            LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 120 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_cupfixScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 10), lUIPoint(GetDefaultScrollBarWidth(), 120));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar(pScrollBar);
  m_table->SignalCellSelect.Connect( this, &FDCupFixturesScreen::OnMatchSelect );

  // cup / round
  m_cupSelect = FD_GAME_NEW FDComboWidget( m_cupfixScreen, 0, "cupcombo_combo", lUIText(""), LUI_CENTER_BOTTOM, lUIPoint( 138, 163 ), lUIPoint( 200, 19 ) );
  m_cupSelect->SetAllToUpper( true );

  CStringArray theList;
  WorldData().DoBuildCupNameList(theList);

  for(int i = 0; i < theList.GetSize(); i++)
  {
    m_cupSelect->AddItem( lUIText( theList.GetAt( i ) ) );
    lLogFmt("%d: %s - %d\n", i, theList.GetAt( i ), i);
  }

  m_cupSelect->SignalSelection.Connect( this, &FDCupFixturesScreen::OnCupCombo );

  // rounds
  m_cupRoundSelect = FD_GAME_NEW FDComboWidget( m_cupfixScreen, 0, "cupround_combo", lUIText(""), LUI_CENTER_TOP, lUIPoint( 138, 165 ), lUIPoint( 200, 19 ) );
  m_cupRoundSelect->SignalSelection.Connect( this, &FDCupFixturesScreen::OnCupRoundCombo );
  m_cupRoundSelect->SetAllToUpper( true );
  // top screen
  m_cupfixScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "cupfix_screen_sub" );
  m_cupfixScreenSub->Hide();

  // top screen fixture info
  for (int i = 0; i < CFFT_MAX; i++)
  {
    m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_cupfixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * i)), lUIPoint(256, 12));
    m_fieldStripes[i] = FD_GAME_NEW lUISpriteWidget(m_cupfixScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT, lUIPoint(128, 91 + (14 * i)), lUIPoint(112, 12));
    m_fieldLabels[i] = FD_GAME_NEW lUITextWidget(m_cupfixScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(0, 91 + (14 * i)), lUIPoint(108, 12));
    m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_cupfixScreenSub, 0, "fieldvalue", lUIText(""), LUI_TOP_LEFT, lUIPoint(128, 91 + (14 * i)), lUIPoint(112, 12));

	  m_fieldLabels[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_fieldValues[i]->SetAlign(LUI_CENTER_CENTER);
  }

  m_fieldLabels[CFFT_DATE_PLAYED]->SetText(lUIText("MATCH DATE"));
  m_fieldLabels[CFFT_VENUE]->SetText(lUIText(IDS_VENUE_NOCOLON));
  m_fieldLabels[CFFT_ATTENDANCE]->SetText(lUIText(IDS_ATTENDANCE));

  for (int i = 0; i < CFFT_MAX; i++)
  {
    m_fieldLabels[i]->SetTextToUpper();
  }


  m_homeClubText = FD_GAME_NEW lUITextWidget( m_cupfixScreenSub, 0, "hometeam", lUIText(""), LUI_TOP_LEFT, lUIPoint( 59, 46 ), lUIPoint(125, 12) );
  m_awayClubText = FD_GAME_NEW lUITextWidget( m_cupfixScreenSub, 0, "awayteam", lUIText(""), LUI_TOP_LEFT, lUIPoint(59, 69), lUIPoint(125, 12) );
  m_homeGoalsText = FD_GAME_NEW lUITextWidget( m_cupfixScreenSub, 0, "homegoals", lUIText(""), LUI_TOP_LEFT, lUIPoint(172, 46), lUIPoint(25, 12) );
  m_awayGoalsText = FD_GAME_NEW lUITextWidget( m_cupfixScreenSub, 0, "awaygoals", lUIText(""), LUI_TOP_LEFT, lUIPoint(172, 69), lUIPoint(25, 12) );

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

  m_splitterBar = FD_GAME_NEW FDBarWidget( m_cupfixScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  m_splitterBar->SetPctA(0.0f);
  m_splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
  m_splitterBar->Hide();

  for (int i = 0; i < 2; i++ )
  {
    m_penaltyStripes[i] = FD_GAME_NEW lUISpriteWidget( m_cupfixScreenSub, 0, "penstripe",  FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_TOP_LEFT,
      lUIPoint( 0, 160 + i * 14 ), lUIPoint( 256, 12) );
    m_penaltyResults[i] = FD_GAME_NEW lUITextWidget( m_cupfixScreenSub, 0, "penresult", lUIText(""), LUI_TOP_CENTER,
      lUIPoint(128, 160 + i * 14), lUIPoint(256, 12) );

    m_penaltyStripes[i]->Hide();
    m_penaltyResults[i]->Hide();
  }


  m_homekitBG = FD_GAME_NEW lUISpriteWidget( m_cupfixScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"),
                                             LUI_TOP_LEFT, lUIPoint( 5, 42 ) );
  m_homekitBG->GetSprite()->SetAlphaEnable( true );
  lUIPoint pt = m_homekitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_homeClubKit = FD_GAME_NEW lUISpriteWidget( m_cupfixScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, pt );
  m_homeClubKit->Hide();

  m_awaykitBG = FD_GAME_NEW lUISpriteWidget( m_cupfixScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"),
                                             LUI_TOP_LEFT, lUIPoint( 210, 42 ) );
  m_awaykitBG->GetSprite()->SetAlphaEnable( true );
  pt = m_awaykitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );

  m_awayClubKit = FD_GAME_NEW lUISpriteWidget( m_cupfixScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, pt );
  m_awayClubKit->Hide();

  // select first cup
  m_cupSelect->SetSelectedItem( 0, false );
}

void FDCupFixturesScreen::SetInitialCup( int idx )
{
  m_initialCupIdx = idx;
}

void FDCupFixturesScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int selectedCup  = m_cupSelect->GetSelectedItem();
    int selectedRnd  = m_cupRoundSelect->GetSelectedItem();
    int selectedFix  = m_table->GetSelectedRow();
    state << selectedCup;
    state << selectedRnd;
    state << selectedFix;
    lLogFmt("Saving cup %d rnd %d fix %d\n", selectedCup, selectedRnd, selectedFix);
  } else
  {
    int selectedCup;
    int selectedRnd;
    int selectedFix;
    state << selectedCup;
    state << selectedRnd;
    state << selectedFix;
    lLogFmt("Loading cup %d rnd %d fix %d\n", selectedCup, selectedRnd, selectedFix);
    m_cupSelect->SetSelectedItem(selectedCup, false);
    m_cupRoundSelect->SetSelectedItem(selectedRnd, false);
    m_table->SelectCell(selectedFix, 0);
  }
}

void FDCupFixturesScreen::Prepare()
{
  //TODO LOCALE
  GetMainUI()->DisplaySubMessage( "Select Fixture"); // or No Fixtures Drawn
}

void FDCupFixturesScreen::DoShow()
{
  m_cupfixScreen->Show();
  m_cupfixScreenSub->Show();
}

void FDCupFixturesScreen::DoHide()
{
  m_cupfixScreen->Hide();
  m_cupfixScreenSub->Hide();
}

void FDCupFixturesScreen::Update()
{
  if (IsForcedUpdateRequested() == false)
	  return;

  if( m_initialCupIdx != -1 )
  {
    lLogFmt("Setting initial cup %d\n", m_initialCupIdx);
    int cup = m_initialCupIdx;
    m_initialCupIdx = -1;               // clear once set
    m_cupSelect->SetSelectedItem( cup, false );
  }

  int cupIdx = m_cupSelect->GetSelectedItem();
  CCup * cup = WorldData().GetCup(cupIdx);

  if( cup == NULL )
  {
    lLogFmt("Update: invalid cup\n");
    return;
  }

  SetForcedUpdateRequested(false);

  CCupRoundList &roundList = cup->GetRounds();
  if( m_cupRoundSelect->GetSelectedItem() != -1  && roundList.GetSize() )
  {
    CCupRound &cupRound = roundList[ m_cupRoundSelect->GetSelectedItem() ];

    // Update match list
    m_table->ClearAll();

    m_table->AddColumn( lUICoord( 94 ), LUI_RIGHT_CENTER );  // home name
    m_table->AddColumn( lUICoord( 40 ), LUI_CENTER_CENTER );   // result
    m_table->AddColumn( lUICoord( 94 ), LUI_LEFT_CENTER );  // away name

    if ( cupRound.HasDrawBeenMade() == false )
    {
      lLogFmt("Draw hasn't been made yet!\n");
      m_table->AddRows( 1 );
      m_table->SetText( 0, 0, lUIText( IDS_NOT_DRAWN ) );
      m_table->SetText( 0, 2, lUIText( IDS_NOT_DRAWN ) );
      return;
    }

    CSoccerResultList resultList;

    if( cupRound.Is2Regions() == false )
    {
      cupRound.BuildReplayDisplayList(resultList);

      lLogFmt("Cup: %s Round: %s = %d\n", cup->GetName(), cupRound.GetName(), resultList.GetSize());

      if( cupRound.IsGroupMatch() == false )
      {
        resultList.SortByHomeClub();
      }

      m_table->AddRows( resultList.GetSize() );

      for (int LoopCount = 0; LoopCount < resultList.GetSize(); LoopCount++)
      {
        COLORREF TextCol;
        if( resultList.GetAt(LoopCount)->GetHomeClubDetails().IsUserControlledClub() == true || resultList.GetAt(LoopCount)->GetAwayClubDetails().IsUserControlledClub() == true )
        {
          TextCol = L_ARGB(128,0,0,128);
        }
        else
        {
          TextCol = L_ARGB(128,128,0,0);
        }

        //m_table->SetRowBkgColour( LoopCount, TextCol );
        m_table->SetBkgColour(LoopCount, 0, L_ARGB(0xFF, 166, 113, 113));
        m_table->SetBkgColour(LoopCount, 1, L_ARGB(0xFF, 102, 82, 82));
        m_table->SetBkgColour(LoopCount, 2, L_ARGB(0xFF, 166, 113, 113));

         // Highlight the user's result
        if( resultList.GetAt(LoopCount)->GetHomeClubDetails().IsUserControlledClub() == true ||
            resultList.GetAt(LoopCount)->GetAwayClubDetails().IsUserControlledClub() == true)
        {
          m_table->SetBkgColour( LoopCount, 0, L_ARGB(0xFF, 92, 134, 203) );
          m_table->SetBkgColour( LoopCount, 2, L_ARGB(0xFF, 92, 134, 203) );
        }

        const lString sHomeClubString = FDMakeClubNameLink(&WorldData().GetClubByID(resultList.GetAt(LoopCount)->GetHomeClubDetails().GetClubID()));
        const lString sAwayClubString = FDMakeClubNameLink(&WorldData().GetClubByID(resultList.GetAt(LoopCount)->GetAwayClubDetails().GetClubID()));

        m_table->SetText( LoopCount, 0, lUIText( sHomeClubString ) );
        //m_table->SetText( LoopCount, 1, lUIText( resultList.GetAt(LoopCount)->GoalsStr() ) );
        {
          m_table->SetText( LoopCount, 1,
            lUIText( FDEngineMakeCupLink( cupIdx, m_cupRoundSelect->GetSelectedItem(),
                        LoopCount,
                        resultList.GetAt(LoopCount)->GoalsStr().CStr() ) ) );
        }

        m_table->SetText( LoopCount, 2, lUIText( sAwayClubString ) );
      }
    }

    // store position in table and clear results from replay list
    int rowCount = resultList.GetSize();
    resultList.RemoveAll();

    // Normal second
    cupRound.BuildMatchDisplayList(resultList);

    lLogFmt("Cup: %s Round: %s = %d\n", cup->GetName(), cupRound.GetName(), resultList.GetSize());

    if( cupRound.IsGroupMatch() == false )
    {
      resultList.SortByHomeClub();
    }

    m_table->AddRows( resultList.GetSize() );

    for (int LoopCount = 0; LoopCount < resultList.GetSize(); LoopCount++)
    {
      COLORREF TextCol;
      if( resultList.GetAt(LoopCount)->GetHomeClubDetails().IsUserControlledClub() == true || resultList.GetAt(LoopCount)->GetAwayClubDetails().IsUserControlledClub() == true )
      {
        TextCol = L_ARGB(128,0,0,128);
      }
      else
      {
        TextCol = L_ARGB(128,0,0,0);
      }

      //m_table->SetRowBkgColour( rowCount + LoopCount, TextCol );
      m_table->SetBkgColour(rowCount + LoopCount, 0, L_ARGB(0xFF, 116, 113, 113));
      m_table->SetBkgColour(rowCount + LoopCount, 1, L_ARGB(0xFF, 82, 82, 82));
      m_table->SetBkgColour(rowCount + LoopCount, 2, L_ARGB(0xFF, 116, 113, 113));

      // Highlight the user's result
      if( resultList.GetAt(LoopCount)->GetHomeClubDetails().IsUserControlledClub() == true ||
        resultList.GetAt(LoopCount)->GetAwayClubDetails().IsUserControlledClub() == true)
      {
        m_table->SetBkgColour( rowCount + LoopCount, 0, L_ARGB(0xFF, 72, 134, 203) );
        m_table->SetBkgColour( rowCount + LoopCount, 2, L_ARGB(0xFF, 72, 134, 203) );
      }

      const lString sHomeClubString = FDMakeClubNameLink(&WorldData().GetClubByID(resultList.GetAt(LoopCount)->GetHomeClubDetails().GetClubID()));
      const lString sAwayClubString = FDMakeClubNameLink(&WorldData().GetClubByID(resultList.GetAt(LoopCount)->GetAwayClubDetails().GetClubID()));

      m_table->SetText( rowCount + LoopCount, 0, lUIText( sHomeClubString ) );

      {
        m_table->SetText( rowCount + LoopCount, 1,
          lUIText( FDEngineMakeCupLink( cupIdx, m_cupRoundSelect->GetSelectedItem(),
                      LoopCount,
                      resultList.GetAt(LoopCount)->GoalsStr().CStr() ) ) );
      }

      m_table->SetText( rowCount + LoopCount, 2, lUIText( sAwayClubString ) );
    }
  }
}

void FDCupFixturesScreen::OnCupCombo( FDComboWidget *, int idx )
{
  CCup * cup = WorldData().GetCup(idx);

  if( cup == NULL )
  {
    //TODO
    return;
  }

  //TODO LOCALE
  if( IsVisible() )
    GetMainUI()->DisplaySubMessage( "Select Fixture");

  SetForcedUpdateRequested(true);

  m_homeClubText->SetText(lUIText(""));
  m_awayClubText->SetText(lUIText(""));
  m_homeGoalsText->SetText(lUIText(""));
  m_awayGoalsText->SetText(lUIText(""));

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

  m_homekitBG->Hide();
  m_awaykitBG->Hide();

  m_homeClubKit->Hide();
  m_awayClubKit->Hide();

  m_splitterBar->Hide();

  for (int i = 0; i < 2; i++ )
  {
    m_penaltyStripes[i]->Hide();
    m_penaltyResults[i]->Hide();
  }

  // Setup rounds for cup
  CCupRoundList &roundList = cup->GetRounds();
  m_cupRoundSelect->ClearAll();

  for(int i = 0; i < roundList.GetSize(); i++)
  {
    m_cupRoundSelect->AddItem( lUIText( roundList[i].GetName() ) );
  }

  m_cupRoundSelect->SetSelectedItem( 0,  false );
  Update();
}

void FDCupFixturesScreen::OnCupRoundCombo( FDComboWidget *, int idx )
{
  SetForcedUpdateRequested(true);
  Update();
}

void FDCupFixturesScreen::OnMatchSelect( FDTableWidget * table, int row, int col )
{
  m_homeClubText->SetText(lUIText(""));
  m_awayClubText->SetText(lUIText(""));
  m_homeGoalsText->SetText(lUIText(""));
  m_awayGoalsText->SetText(lUIText(""));

  m_fieldValues[CFFT_DATE_PLAYED]->SetText(lUIText(""));
  m_fieldValues[CFFT_VENUE]->SetText(lUIText(""));
  m_fieldValues[CFFT_ATTENDANCE]->SetText(lUIText(""));

  m_homeClubKit->Hide();
  m_awayClubKit->Hide();

  m_splitterBar->Hide();

  for (int i = 0; i < 2; i++ )
  {
    m_penaltyStripes[i]->Hide();
    m_penaltyResults[i]->Hide();
  }

  if( row == -1 || table->GetRowCount() < 2)
  {
    return;
  }

  for (int i = 0; i < CFFT_MAX; i++)
  {
    m_backgroundStripes[i]->Show();
    m_fieldStripes[i]->Show();
	  m_fieldLabels[i]->Show();
	  m_fieldValues[i]->Show();
  }

  m_homekitBG->Show();
  m_awaykitBG->Show();

  m_splitterBar->Show();

  GetMainUI()->HideSubMessage();

  lLogFmt("Match %d selected\n", row);

  int cupIdx = m_cupSelect->GetSelectedItem();
  CCup * cup = WorldData().GetCup(cupIdx);

  if( cup == NULL )
  {
    lLogFmt("Update: invalid cup\n");
    return;
  }

  CCupRoundList &roundList = cup->GetRounds();
  if( m_cupRoundSelect->GetSelectedItem() != -1  && roundList.GetSize() )
  {
    CCupRound &cupRound = roundList[ m_cupRoundSelect->GetSelectedItem() ];

    if ( cupRound.HasDrawBeenMade() == false )
    {
      return;
    }

    // build info
    CSoccerResultList resultList;
    CSoccerResult *result = NULL;
    bool bReplay = true;

    // Replay list first
    if( cupRound.Is2Regions() == false )
    {
      cupRound.BuildReplayDisplayList(resultList);
    }

    if( row >= resultList.GetSize() || resultList.GetSize() == 0 )
      bReplay = false;

    if( bReplay )
    {
      if( cupRound.IsGroupMatch() == false )
      {
        resultList.SortByHomeClub();
      }

      result = resultList.GetAt(row);
    }
    else
    {
      int rowCount = resultList.GetSize();
      resultList.RemoveAll();
      cupRound.BuildMatchDisplayList(resultList);

      if( cupRound.IsGroupMatch() == false )
      {
        resultList.SortByHomeClub();
      }

      result = resultList.GetAt(row - rowCount);
    }

    // show information
    CString str;

    m_homeClubKit->Show();
    m_awayClubKit->Show();

    CClub *pHomeClub = &WorldData().GetClubByID(result->GetHomeClubDetails().GetClubID());
    CClub *pAwayClub = &WorldData().GetClubByID(result->GetAwayClubDetails().GetClubID());

    //TODO: Make sure stadium already correct
    if (( result->GetFixture() != NULL ) && result->GetFixture()->HasBeenSetupOrUsed())
    {
      m_fieldValues[CFFT_VENUE]->SetText(lUIText(result->GetFixture()->StadiumName().CStr()));
    } else
    {
      m_fieldValues[CFFT_VENUE]->SetText(lUIText(pHomeClub->GetStadium()->GetName().CStr()));
    }

    m_homeClubText->SetText(lUIText(pHomeClub->GetName().CStr()), true );
    m_awayClubText->SetText(lUIText(pAwayClub->GetName().CStr()), true );

    m_homeClubKit->GetSprite().Get()->SetFrame( pHomeClub->GetHomeKitID() );
    m_awayClubKit->GetSprite().Get()->SetFrame( pAwayClub->GetHomeKitID() );

    m_fieldValues[CFFT_DATE_PLAYED]->SetText(lUIText( cupRound.LongMatchDateStr() ));

    if( result->GetFixture() != NULL )
    {
      if( result->GetFixture()->HasMatchBeenPlayed() == true )
      {
        str.Format("%u", result->GetFixture()->GetAttendance());
        m_fieldValues[CFFT_ATTENDANCE]->SetText(lUIText(str.CStr()));

        str.Format("%d", (int)result->GetFixture()->GetNumberGoals(HOME));
        m_homeGoalsText->SetText(lUIText(str.CStr()));

        str.Format("%d", (int)result->GetFixture()->GetNumberGoals(AWAY));
        m_awayGoalsText->SetText(lUIText(str.CStr()));

        if( result->GetFixture()->GetEventList()->WasPenaltyShootOut() )
        {
          for (int i = 0; i < 2; i++ )
          {
            m_penaltyStripes[i]->Show();
            m_penaltyResults[i]->Show();
          }

          int homePenalties = result->GetFixture()->GetEventList()->NumberHomePenaltyShootOutGoals();
          int awayPenalties = result->GetFixture()->GetEventList()->NumberAwayPenaltyShootOutGoals();

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

          str.Format( "%d - %d", result->GetFixture()->GetEventList()->NumberHomePenaltyShootOutGoals(),
                                          result->GetFixture()->GetEventList()->NumberAwayPenaltyShootOutGoals() );

          m_penaltyResults[1]->SetText( lUIText( str.CStr() ) );
        }
      }
    }

    /*if( (col == 1) && result->GetFixture() != NULL )
    {
      if( result->GetFixture()->HasMatchBeenPlayed() == true )
      {
        // Open Summary screen based on table selection
        theApp.OnShowMatchSummary( cup, result->GetFixture(), true );
        //GetMainUI()->PopScreen();
      }
    }*/
  }

}


