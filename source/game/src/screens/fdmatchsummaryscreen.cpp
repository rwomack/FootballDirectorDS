//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchSummary.cpp
// Description : FDMatchSummary implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdmatchsummaryscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchSummaryScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  TAG_FLAG_REDCARD_HOME = 0x001,
  TAG_FLAG_REDCARD_AWAY = 0x002,
  TAG_FLAG_YELCARD_HOME       = 0x004,
  TAG_FLAG_YELCARD_AWAY       = 0x008,
  TAG_FLAG_GOAL_HOME    = 0x010,
  TAG_FLAG_GOAL_AWAY    = 0x020,
  TAG_FLAG_PENSO_HIT    = 0x0040,
  TAG_FLAG_PENSO_MISS    = 0x0080,
  TAG_FLAG_PENSO_SAVED    = 0x0100,
};

FDMatchSummaryScreen::FDMatchSummaryScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  // init commentary
  m_skillScreen      = FD_GAME_NEW FDMatchDaySkillScreen( parent );
  m_eventsScreen     = FD_GAME_NEW FDMatchDayEventsScreen( parent );

  // init our selves last
  Init();
}

FDMatchSummaryScreen::~FDMatchSummaryScreen()
{
}

void FDMatchSummaryScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  //  - Sub screen

  m_matchSummaryScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), LUI_WIDGET_MOUSETRANSPARENT, "matchday_screen" );
  m_matchSummaryScreen->Hide();

  m_matchSummarySubScreen = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "matchday_screen_sub" );
  m_matchSummarySubScreen->Hide();

  m_homeName = FD_GAME_NEW lUITextWidget( m_matchSummarySubScreen, 0, "temp1", lUIText(""), LUI_LEFT_TOP, lUIPoint( 62, 46 ), lUIPoint( 120, 16 ) );
  m_homeName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_homeName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_awayName = FD_GAME_NEW lUITextWidget( m_matchSummarySubScreen, 0, "temp1", lUIText(""), LUI_LEFT_TOP, lUIPoint( 62, 68 ), lUIPoint( 120, 16 ) );
  m_awayName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_awayName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_homeScore = FD_GAME_NEW lUITextWidget( m_matchSummarySubScreen, 0, "temp1", lUIText(""), LUI_RIGHT_TOP, lUIPoint( 192, 46 ) );
  m_homeScore->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_homeScore->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_awayScore = FD_GAME_NEW lUITextWidget( m_matchSummarySubScreen, 0, "temp1", lUIText(""), LUI_RIGHT_TOP, lUIPoint( 192, 68 ) );
  m_awayScore->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_awayScore->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget *splitterBar = FD_GAME_NEW FDBarWidget(m_matchSummarySubScreen,0, "splitter", LUI_CENTER_CENTER,
                                                     lUIPoint(128, 65), lUIPoint(160, 1));

  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));


  lUISpriteWidget *homeKitBG = FD_GAME_NEW lUISpriteWidget( m_matchSummarySubScreen, 0, "homeShield", FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_LEFT_TOP,
                  lUIPoint( 5, 42 ) );
  homeKitBG->GetSprite()->SetAlphaEnable( true );
  lUISpriteWidget *awayKitBG = FD_GAME_NEW lUISpriteWidget( m_matchSummarySubScreen, 0, "awayShield", FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_LEFT_TOP,
                  lUIPoint( 210, 42 ));
  awayKitBG->GetSprite()->SetAlphaEnable( true );

  lUIPoint pt = homeKitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_homeKit = FD_GAME_NEW lUISpriteWidget( m_matchSummarySubScreen, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP,  pt );

  pt = awayKitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_awayKit = FD_GAME_NEW lUISpriteWidget( m_matchSummarySubScreen, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP,  pt );


  // table
  m_matchtable = FD_GAME_NEW FDTableWidget( m_matchSummarySubScreen, 0, "matchtable",
            FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_AUTO_SCROLL | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 0, 95 ), lUIPoint( 255, 192 - 95 ) );
  m_matchtable->SetCellRenderCallback(FDMatchSummaryScreen::TableCellRenderCallback, this);
  m_matchtable->SetRowGap(2);

  // create buttons
  m_menuBar2 = FD_GAME_NEW lUISpriteWidget( m_matchSummaryScreen, LUI_WIDGET_MOUSETRANSPARENT, "menubar2", FDResourceManager::GetSprite("menu_bar_2","menu_bar_2") );

  //m_commButton    = FD_GAME_NEW FDButtonWidget( m_matchSummaryScreen, 0, "comm", FDResourceManager::GetSprite("uiicon","menu_match_commentary"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(6,42) );
  m_eventsButton  = FD_GAME_NEW FDButtonWidget( m_matchSummaryScreen, 0, "summaryevents", FDResourceManager::GetSprite("uiicon","menu_events"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,    LUI_TOP_LEFT, lUIPoint(0,69) );
  m_skillButton   = FD_GAME_NEW FDButtonWidget( m_matchSummaryScreen, 0, "summaryskill", FDResourceManager::GetSprite("uiicon","menu_skills"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,     LUI_TOP_LEFT, lUIPoint(0,91) );

  m_matchDoneButton = FD_GAME_NEW FDButtonWidget( m_matchSummaryScreen, 0, "summaryDonebutton", FDResourceManager::GetSprite("uiicon","fast_forward"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_TOP, lUIPoint( 0, 2 ) );
  m_matchDoneBackButton = FD_GAME_NEW FDButtonWidget( m_matchSummaryScreen, 0, "summaryDonebutton", FDResourceManager::GetSprite("uiicon","back"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_TOP, lUIPoint( 0, 156 ) );

  //m_matchDoneButton->SetText( lUIText( IDS_DONE_CAPS ) );
  m_matchDoneButton->SignalControlCommand.Connect( this, &FDMatchSummaryScreen::OnDoneButton );
  m_matchDoneBackButton->SignalControlCommand.Connect( this, &FDMatchSummaryScreen::OnDoneButton );

  m_matchDoneButton->Hide();
  m_matchDoneBackButton->Hide();

  //m_commButton->SignalControlCommand.Connect( this, &FDMatchSummaryScreen::OnMenuButton );
  m_eventsButton->SignalControlCommand.Connect( this, &FDMatchSummaryScreen::OnMenuButton );
  m_skillButton->SignalControlCommand.Connect( this, &FDMatchSummaryScreen::OnMenuButton );

  m_redCardSprite = FDResourceManager::GetSprite("uiicon", "redcard");
  m_yelCardSprite = FDResourceManager::GetSprite("uiicon", "yellowcard");
  m_goalSprite = FDResourceManager::GetSprite("uiicon", "ball");
}

void FDMatchSummaryScreen::OnDoneButton( lUIControlWidget *button )
{
  lLogFmt("Match done button\n");
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

void FDMatchSummaryScreen::ShowBackButton( bool b )
{
  if( b )
  {
    m_matchDoneButton->Hide();
    m_matchDoneBackButton->Show();
  } else
  {
    m_matchDoneButton->Show();
    m_matchDoneBackButton->Hide();
  }
}

void FDMatchSummaryScreen::OnMenuButton( lUIControlWidget *button )
{
  if( button == m_eventsButton )
  {
    SetView( VIEW_EVENTS );
    //m_commButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_eventsButton->GetSprite()->SetColour( L_ARGB( 255, 255, 255, 255 ) );
    m_skillButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
  }

  if( button == m_skillButton )
  {
    SetView( VIEW_SKILL );
    //m_commButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_eventsButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_skillButton->GetSprite()->SetColour( L_ARGB( 255, 255, 255, 255 ) );
  }
}

void FDMatchSummaryScreen::DoShow()
{
  m_matchSummaryScreen->Show();
  m_matchSummarySubScreen->Show();

  //GetMainUI()->GetMenuBar2()->InsertBefore(m_eventsButton);
  SetView( m_currentView );
}

void FDMatchSummaryScreen::DoHide()
{
  m_matchSummarySubScreen->Hide();
  m_matchSummaryScreen->Hide();

  m_skillScreen->Hide();
  m_eventsScreen->Hide();
}

void FDMatchSummaryScreen::SetMatch( CCup* _Cup, CFixture* _Fixture )
{
  m_pFixture = _Fixture;
  SetView( VIEW_SKILL );
}

void FDMatchSummaryScreen::Update()
{
  if( m_pFixture == NULL )
  {
    return;
  }

//  int colA = L_ARGB( 255, 29, 9, 208 );
//  int colB = L_ARGB( 255, 211, 108, 5 );

	CString strPrint;

  // team name / score
  m_homeName->SetText( lUIText( m_pFixture->GetClub(HOME).GetName() ), true );
  m_homeName->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  m_awayName->SetText( lUIText( m_pFixture->GetClub(AWAY).GetName() ), true );
  m_awayName->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  strPrint.Format("%d", m_pFixture->GetNumberGoals(HOME));
  m_homeScore->SetText( lUIText( strPrint ) );
  m_homeScore->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  strPrint.Format("%d", m_pFixture->GetNumberGoals(AWAY));
  m_awayScore->SetText( lUIText( strPrint ) );
  m_awayScore->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  // team kits
  char tmp[256];

  m_homeKit->GetSprite().Get()->SetFrame( m_pFixture->GetClub(HOME).GetHomeKitID() );
  m_awayKit->GetSprite().Get()->SetFrame( m_pFixture->GetClub(AWAY).GetHomeKitID() );

  // half time score
  //strPrint.Format("(%d - %d)", m_pFixture->GetEventList()->HalfTimeHomeGoals(), m_pFixture->GetEventList()->HalfTimeAwayGoals());
  //m_halftimeScore->SetText( lUIText( strPrint ) );



  // only display if not a summary (or ended in pens)
  if( !m_pFixture->IsSummary() || m_pFixture->GetEventList()->WasPenaltyShootOut() )
  {
    // update top screen timeline for goals and sending offs
    m_pFixture->GetEventList()->SortByTime();
    int numEvents = m_pFixture->GetEventList()->DoCountInGameEvents();

    int rowidxA = 0, rowidxB = 0;
    CString strEvent;

    //int prevScrollPosition = 0;
    //bool bDirection = false;

    //m_matchtable->GetScrollPosition(prevScrollPosition, bDirection);
    m_matchtable->ClearAll();
    m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );  // gap
    m_matchtable->AddColumn( lUICoord( 15 ), LUI_CENTER_CENTER ); // Red card
    m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );  // gap
    m_matchtable->AddColumn( lUICoord( 99 ), LUI_LEFT_CENTER );
    m_matchtable->AddColumn( lUICoord( 5), LUI_CENTER_CENTER ); // gap
    m_matchtable->AddColumn( lUICoord( 99 ), LUI_RIGHT_CENTER );
    m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER ); // gap
    m_matchtable->AddColumn( lUICoord( 15 ), LUI_CENTER_CENTER ); // Red card
    m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );  // gap

/*
  m_matchtable->AddRows(8);
  for (int i = 0; i < 8; i++)
  {
    CString sLabel;
    sLabel.Format("Testing %d", i);
    m_matchtable->SetText(i, 3, lUIText(sLabel.CStr()));
    m_matchtable->SetText(i, 5, lUIText(sLabel.CStr()));
  }
 */

    for (int LoopCount = 0; LoopCount < m_pFixture->GetEventList()->GetSize(); LoopCount ++)
    {
      CMatchEvent* pEvent = static_cast<CMatchEvent*>(m_pFixture->GetEventList()->GetAt(LoopCount));
      if (pEvent->IsPenaltyShootOutEvent())
      {
        lLogFmt( "Penalty shoot out %s (%d)", pEvent->GetText(), pEvent->GetTime());
        continue;
      }

      switch( pEvent->GetEventType() )
      {
        case AWAYGOAL:
        case HOMEGOAL:
        case HOMEPENALTYGOAL:
        case AWAYPENALTYGOAL:
        {
          strEvent.Format( "%s (%d)", pEvent->GetText(), pEvent->GetTime());

          if (pEvent->IsHomeTeamEvent() == true)
          {
            if( rowidxA >= m_matchtable->GetRowCount() )
              m_matchtable->AddRows( 1 );

            m_matchtable->SetText( rowidxA, 3, lUIText( strEvent ) );
            m_matchtable->SetTextColour( rowidxA, 3, L_ARGB(255,255,255,255) );
            m_matchtable->SetTagDataForRow( rowidxA, m_matchtable->GetTagDataForRow(rowidxA) | TAG_FLAG_GOAL_HOME );
            rowidxA++;
          } else
          {
            if( rowidxB >= m_matchtable->GetRowCount() )
              m_matchtable->AddRows( 1 );

            m_matchtable->SetText( rowidxB, 5, lUIText( strEvent ) );
            m_matchtable->SetTextColour( rowidxB, 5, L_ARGB(255,255,255,255) );
            m_matchtable->SetTagDataForRow( rowidxB, m_matchtable->GetTagDataForRow(rowidxB) | TAG_FLAG_GOAL_AWAY );
            rowidxB++;
          }

          break;
        }

        case HOMEOWNGOAL:
        case AWAYOWNGOAL:
        {
          strEvent.Format( "%s OG (%d)", pEvent->GetText(), pEvent->GetTime());

          if (pEvent->IsHomeTeamEvent() == false)
          {
            if( rowidxA >= m_matchtable->GetRowCount() )
              m_matchtable->AddRows( 1 );

            m_matchtable->SetText( rowidxA, 3, lUIText( strEvent ) );
            m_matchtable->SetTextColour( rowidxA, 3, L_ARGB(255,255,255,255) );
            m_matchtable->SetTagDataForRow( rowidxA, m_matchtable->GetTagDataForRow(rowidxA) | TAG_FLAG_GOAL_HOME );
            rowidxA++;
          } else
          {
            if( rowidxB >= m_matchtable->GetRowCount() )
              m_matchtable->AddRows( 1 );

            m_matchtable->SetText( rowidxB, 5, lUIText( strEvent ) );
            m_matchtable->SetTextColour( rowidxB, 5, L_ARGB(255,255,255,255) );
            m_matchtable->SetTagDataForRow( rowidxB, m_matchtable->GetTagDataForRow(rowidxB) | TAG_FLAG_GOAL_AWAY );
            rowidxB++;
          }

          break;
        }


        case AWAYBOOKING:
        case HOMEBOOKING:
        case AWAYSENDINGOFF:
        case HOMESENDINGOFF:
        {
          bool bRed = true;
          if( pEvent->GetEventType() == AWAYBOOKING || pEvent->GetEventType() == HOMEBOOKING )
          {
            bRed = false;
          }

          strEvent.Format( "%s (%d)", pEvent->GetText(), pEvent->GetTime());

          if (pEvent->IsHomeTeamEvent() == true)
          {
            if( rowidxA >= m_matchtable->GetRowCount() )
              m_matchtable->AddRows( 1 );

            m_matchtable->SetText( rowidxA, 3, lUIText( strEvent ) );

            if( bRed )
            {
              m_matchtable->SetTextColour( rowidxA, 3, L_ARGB(255,255,0,0) );
              m_matchtable->SetTagDataForRow(rowidxA, m_matchtable->GetTagDataForRow(rowidxA) | TAG_FLAG_REDCARD_HOME);
            }
            else
            {
              m_matchtable->SetTextColour( rowidxA, 3, L_ARGB(255,255,255,0) );
              m_matchtable->SetTagDataForRow(rowidxA, m_matchtable->GetTagDataForRow(rowidxA) | TAG_FLAG_YELCARD_HOME);
            }

            rowidxA++;
          } else
          {
            if( rowidxB >= m_matchtable->GetRowCount() )
              m_matchtable->AddRows( 1 );

            m_matchtable->SetText( rowidxB, 5, lUIText( strEvent ) );

            if( bRed )
            {
              m_matchtable->SetTextColour( rowidxB, 5, L_ARGB(255,255,0,0) );
              m_matchtable->SetTagDataForRow(rowidxB, m_matchtable->GetTagDataForRow(rowidxB) | TAG_FLAG_REDCARD_AWAY);
            }
            else
            {
              m_matchtable->SetTextColour( rowidxB, 5, L_ARGB(255,255,255,0) );
              m_matchtable->SetTagDataForRow(rowidxB, m_matchtable->GetTagDataForRow(rowidxB) | TAG_FLAG_YELCARD_AWAY);
            }
            rowidxB++;
          }

          break;
        }

        default:
          break;
      }
    }

    // show penalty winner
    if( m_pFixture->GetEventList()->WasPenaltyShootOut() )
    {
      int home = m_pFixture->GetEventList()->NumberHomePenaltyShootOutGoals();
      int away = m_pFixture->GetEventList()->NumberAwayPenaltyShootOutGoals();

      lLogFmt("**\nPenalty Result Home: %d\nPenalty Result Away: %d\n**\n", home, away);

      if( home > away )
      {
        m_matchtable->AddRows( 2 );

        strEvent.Format( "WON %d-%d", home, away);
        m_matchtable->SetText( rowidxA, 3, lUIText(strEvent) );
        m_matchtable->SetTextColour( rowidxA, 3, L_ARGB(255,255,255,255) );
        rowidxA++;

        strEvent.Format( "  ON PENALTIES");
        m_matchtable->SetText( rowidxA, 3, lUIText(strEvent) );
        m_matchtable->SetTextColour( rowidxA, 3, L_ARGB(255,255,255,255) );
        rowidxA++;
      } else if( away > home )
      {
        m_matchtable->AddRows( 2 );

        strEvent.Format( "WON %d-%d", home, away);
        m_matchtable->SetText( rowidxB, 5, lUIText(strEvent) );
        m_matchtable->SetTextColour( rowidxB, 5, L_ARGB(255,255,255,255) );
        rowidxB++;

        strEvent.Format( "  ON PENALTIES");
        m_matchtable->SetText( rowidxB, 5, lUIText(strEvent) );
        m_matchtable->SetTextColour( rowidxB, 5, L_ARGB(255,255,255,255) );
        rowidxB++;
      }
      else
      {
        EXT_ASSERT(false);// shouldn't get here after match
      }
    }

    // Always have 7 rows
    if( !m_pFixture->IsSummary() )
    {
      if( m_matchtable->GetRowCount() < 7 )
      {
        m_matchtable->AddRows( 7 - m_matchtable->GetRowCount() );
      }
    }

    for( int i = 0; i < m_matchtable->GetRowCount(); i++ )
    {
      m_matchtable->SetBkgColour( i, 0, L_ARGB( 127, 130, 130, 130 ) );
      m_matchtable->SetBkgColour( i, 1, L_ARGB( 127, 80, 80, 80 ) );
      m_matchtable->SetBkgColour( i, 2, L_ARGB( 127, 130, 130, 130 ) );
      m_matchtable->SetBkgColour( i, 3, L_ARGB( 127, 80, 80, 80 ) );
      m_matchtable->SetBkgColour( i, 4, L_ARGB( 127, 130, 130, 130 ) );
      m_matchtable->SetBkgColour( i, 5, L_ARGB( 127, 80, 80, 80 ) );
      m_matchtable->SetBkgColour( i, 6, L_ARGB( 127, 130, 130, 130 ) );
      m_matchtable->SetBkgColour( i, 7, L_ARGB( 127, 80, 80, 80) );
      m_matchtable->SetBkgColour( i, 8, L_ARGB( 127, 130, 130, 130 ) );
    }

    //m_matchtable->SetScrollPosition(prevScrollPosition, bDirection);
  } else
  {
    m_matchtable->ClearAll();
  }
}


void  FDMatchSummaryScreen::SetView( int idx )
{
  m_skillScreen->Hide();
  m_eventsScreen->Hide();

  if( m_pFixture == NULL )
  {
    return;
  }

  switch( idx )
  {
    case VIEW_SKILL:
      m_skillScreen->Update( *m_pFixture );
      m_skillScreen->Show();
      break;
    case VIEW_EVENTS:
      m_eventsScreen->Update( *m_pFixture );
      m_eventsScreen->Show();
      break;
    default:
      break;
  }

  m_currentView = idx;
}

int   FDMatchSummaryScreen::GetCurrentView()
{
  return m_currentView;
}

void FDMatchSummaryScreen::AutoplaySkip()
{
  // Done
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

void FDMatchSummaryScreen::TableCellRenderCallback(FDTableWidget *pTable, void *pMatchSummaryScreen, luint32_t uiRow, luint32_t uiColumn, lUIRect cCellRect)
{
	if ((uiColumn != 1) && (uiColumn != 7))
		return;

	FDMatchSummaryScreen *pScreen = (FDMatchSummaryScreen *)pMatchSummaryScreen;

  luint32_t uiFlag = pTable->GetTagDataForRow(uiRow);

  if (((uiColumn == 1) && (uiFlag & TAG_FLAG_REDCARD_HOME)) ||
      ((uiColumn == 7) && (uiFlag & TAG_FLAG_REDCARD_AWAY)))
	{
		pScreen->m_redCardSprite->SetX(cCellRect.x);
		pScreen->m_redCardSprite->SetY(cCellRect.y);
		pScreen->m_redCardSprite->Render();
	}

  if (((uiColumn == 1) && (uiFlag & TAG_FLAG_YELCARD_HOME)) ||
      ((uiColumn == 7) && (uiFlag & TAG_FLAG_YELCARD_AWAY)))
	{
    pScreen->m_yelCardSprite->SetX(cCellRect.x);
    pScreen->m_yelCardSprite->SetY(cCellRect.y);
//    pScreen->m_redCardSprite->SetWidth(cCellRect.w);
//    pScreen->m_redCardSprite->SetHeight(cCellRect.h);
    pScreen->m_yelCardSprite->Render();
	}

  if (((uiColumn == 1) && (uiFlag & TAG_FLAG_GOAL_HOME)) ||
      ((uiColumn == 7) && (uiFlag & TAG_FLAG_GOAL_AWAY)))
	{
    pScreen->m_goalSprite->SetX(cCellRect.x);
    pScreen->m_goalSprite->SetY(cCellRect.y);
    pScreen->m_goalSprite->Render();
	}

}



