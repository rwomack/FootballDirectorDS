//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchIntroscreen.cpp
// Description : FDMatchIntroScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscrollbarwidget.h>
#include <fdgameworld.h>
#include <fdtext.h>


#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdmatchIntroscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;


static int g_styleStrID[] =
{
  IDS_PLAYSTYLE1, IDS_PLAYSTYLE2, IDS_PLAYSTYLE3, IDS_PLAYSTYLE4,
  IDS_PLAYSTYLE5, IDS_PLAYSTYLE6, IDS_PLAYSTYLE7, IDS_PLAYSTYLE8,
  IDS_PLAYSTYLE9, IDS_PLAYSTYLE10
};

static int g_aggressionStrID[] =
{
  IDS_AGGRESSION1, IDS_AGGRESSION2, IDS_AGGRESSION3, IDS_AGGRESSION4,
  IDS_AGGRESSION5, IDS_AGGRESSION6, IDS_AGGRESSION7, IDS_AGGRESSION8,
  IDS_AGGRESSION9, IDS_AGGRESSION10
};

static int g_markingStrID[] =
{
  IDS_MARKING1, IDS_MARKING2, IDS_MARKING3,
  IDS_MARKING4, IDS_MARKING5
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchIntroScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMatchIntroScreen::FDMatchIntroScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDMatchIntroScreen::~FDMatchIntroScreen()
{
}

void FDMatchIntroScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_matchIntroScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "matchIntro_screen" );
  m_matchIntroScreen->Hide();

  // create buttons
  m_fakeCommButton    = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "comm", FDResourceManager::GetSprite("uiicon","menu_commentary"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,47) );
  m_fakeEventsButton  = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "events", FDResourceManager::GetSprite("uiicon","menu_events"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,    LUI_TOP_LEFT, lUIPoint(0,69) );
  m_fakeSkillButton   = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "skill", FDResourceManager::GetSprite("uiicon","menu_skills"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,     LUI_TOP_LEFT, lUIPoint(0,91) );
  m_fakeMenu   = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "match", FDResourceManager::GetSprite("uiicon","menu_match"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( 0, 113 ) );

  m_fakeCommButton->Disable();
  m_fakeEventsButton->Disable();
  m_fakeSkillButton->Disable();
  m_fakeMenu->Disable();

  // Intro

  m_matchBackButton = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "back", FDResourceManager::GetSprite("uiicon","back"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,156) );
  m_matchBackButton->SignalControlCommand.Connect( this, &FDMatchIntroScreen::OnBackButton );
  m_matchBackButton->Hide();

  // Match buttons
  m_matchViewButton = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "introbutton", FDResourceManager::GetSprite("ui","view_match"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 58, 30 ) );
  m_matchViewButton->SetSpriteOn( FDResourceManager::GetSprite("ui","view_match-on") );
  m_matchViewButton->SignalControlCommand.Connect( this, &FDMatchIntroScreen::OnViewButton );

  m_matchResultButton = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "resultbutton", FDResourceManager::GetSprite("ui","result_only"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 58, 45 ) );
  m_matchResultButton->SetSpriteOn( FDResourceManager::GetSprite("ui","result_only-on") );
  m_matchResultButton->SignalControlCommand.Connect( this, &FDMatchIntroScreen::OnResultButton );

  m_matchTeamButton = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "matchintroteamsbutton", FDResourceManager::GetSprite("ui","teams"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 58, 60 ) );
  m_matchTeamButton->SetSpriteOn( FDResourceManager::GetSprite("ui","teams-on") );
  m_matchTeamButton->SignalControlCommand.Connect( this, &FDMatchIntroScreen::OnTeamsButton );

  m_matchOppositionButton = FD_GAME_NEW FDButtonWidget( m_matchIntroScreen, 0, "matchintrooppobutton", FDResourceManager::GetSprite("ui","info"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 58, 75 ) );
  m_matchOppositionButton->SetSpriteOn( FDResourceManager::GetSprite("ui","info-on") );
  m_matchOppositionButton->SignalControlCommand.Connect( this, &FDMatchIntroScreen::OnOppositionButton );

  //

  m_table = FD_GAME_NEW FDTableWidget( m_matchIntroScreen, 0, "teamstable",
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_BACKGROUND|
            FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW|FDTableWidget::TABLE_FLAG_WORD_WRAP,
            LUI_TOP_LEFT, lUIPoint( 20, 40 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 120 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_matchIntroScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(248, 40), lUIPoint(GetDefaultScrollBarWidth(), 120));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar(pScrollBar);
  m_table->SignalCellSelect.Connect( this, &FDMatchIntroScreen::OnCellSelect );
  m_table->Hide();

  m_reportTextStripe = FD_GAME_NEW lUISpriteWidget(m_matchIntroScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
   lUIPoint( 20, 10 ), lUIPoint(236, 12));
  m_reportTextStripe->GetSprite()->SetWidth( 236 );
  m_reportTextStripe->Hide();

  m_reportTitleText = FD_GAME_NEW lUITextWidget(m_matchIntroScreen, 0, "reporttitle", lUIText(IDS_REPORT32), LUI_TOP_CENTER, lUIPoint(128, 10));
  m_reportTitleText->Hide();

  m_reportMessageBar = FD_GAME_NEW lUISpriteWidget(m_matchIntroScreen, 0, "messagerBar", FDResourceManager::GetSprite("message_box_small", "message_box_small"), LUI_CENTER_LEFT,
   lUIPoint( 20, 96 ));
  m_reportMessageBar->Hide();

  m_reportMessageText = FD_GAME_NEW lUITextWidget(m_matchIntroScreen, 0, "reporttitle", lUIText(IDS_REPORT_NOT_AVAILABLE), LUI_CENTER_CENTER, lUIPoint(128, 96 ));
  m_reportMessageText->SetFont(FDResourceManager::GetFont( "font16", "font16" ) );
  m_reportMessageText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_reportMessageText->Hide();

  m_matchIntroScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "matchIntro_screenSub" );
  m_matchIntroScreenSub->Hide();

  // Match text

  // backing stripes
  m_backingStripes[0] = FD_GAME_NEW lUISpriteWidget(m_matchIntroScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
		lUIPoint( 0, 56 + 0 * 14 ), lUIPoint(256, 24));
  m_backingStripes[0]->GetSprite()->SetWidth( 256 );

  m_backingBars[0] = FD_GAME_NEW lUISpriteWidget(m_matchIntroScreenSub, 0, "bar", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_CENTER,
    lUIPoint( 128, 56 + 0 * 14 ), lUIPoint(230, 24));

  m_backingStripes[1] = FD_GAME_NEW lUISpriteWidget(m_matchIntroScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
	lUIPoint( 0, 70 + 1 * 14 ), lUIPoint(256, 12));
  m_backingStripes[1]->GetSprite()->SetWidth( 256 );
  m_backingBars[1] = FD_GAME_NEW lUISpriteWidget(m_matchIntroScreenSub, 0, "bar", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_CENTER,
    lUIPoint( 128, 70 + 1 * 14 ), lUIPoint(140, 12));



  m_compTitleText = FD_GAME_NEW lUITextWidget( m_matchIntroScreenSub, 0, "", "", LUI_TOP_CENTER, lUIPoint( 128, 56 + 0 ), lUIPoint( 230, 24 ) );
  m_compTitleText->SetAlign(LUI_CENTER_CENTER);

  m_stadiumText = FD_GAME_NEW lUITextWidget( m_matchIntroScreenSub, 0, "", "", LUI_TOP_CENTER, lUIPoint( 128, 70 + 14 ), lUIPoint( 140, 12 ) );

  // team names
  m_teamBoxBg = FD_GAME_NEW lUISpriteWidget( m_matchIntroScreenSub, 0, "mbBgbar", FDResourceManager::GetSprite("message_box_small","message_box_small"), LUI_TOP_CENTER, lUIPoint( 127, 100 ));

  m_homeClubText = FD_GAME_NEW lUITextWidget( m_matchIntroScreenSub, 0, "", "", LUI_TOP_CENTER, lUIPoint( 127, 105 )  );
  m_vsText = FD_GAME_NEW lUITextWidget( m_matchIntroScreenSub, 0, "", "V", LUI_TOP_CENTER, lUIPoint( 127, 120 )  );
  m_awayClubText = FD_GAME_NEW lUITextWidget( m_matchIntroScreenSub, 0, "", "", LUI_TOP_CENTER, lUIPoint( 127, 135 )  );

  m_homeClubText->SetFont(FDResourceManager::GetFont("fontim", "fontim"));
  m_awayClubText->SetFont(FDResourceManager::GetFont("fontim", "fontim"));
  m_vsText->SetFont(FDResourceManager::GetFont("fontim", "fontim"));
  m_homeClubText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_awayClubText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_vsText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  // Tactics special
  m_tacticsStripe = FD_GAME_NEW lUISpriteWidget(m_matchIntroScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
   lUIPoint( 0, 66 ), lUIPoint(256, 12));
  m_tacticsStripe->GetSprite()->SetWidth( 256 );
  m_tacticsStripe->Hide();

  // TODO LOCALE
  m_tacticsText = FD_GAME_NEW lUITextWidget( m_matchIntroScreenSub, 0, "", lUIText("TACTICS"), LUI_TOP_CENTER, lUIPoint( 128, 66 ), lUIPoint( 140, 12 ) );
  m_tacticsText->Hide();
  m_tacticsTable = FD_GAME_NEW FDTableWidget( m_matchIntroScreenSub, 0, "teamstable",
          FDTableWidget::TABLE_FLAG_CELLBACKGROUND|
          FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW|FDTableWidget::TABLE_FLAG_WORD_WRAP,
          LUI_TOP_LEFT, lUIPoint( 0, 80 ), lUIPoint( 256, 90 ) );
  m_tacticsTable->SetRowGap( 2 );
  m_tacticsTable->Hide();

  m_state = VIEW_INTRO;
}

void FDMatchIntroScreen::OnViewButton( lUIControlWidget * )
{
  lLogFmt("MatchIntro: View Match\n");
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

void FDMatchIntroScreen::OnResultButton( lUIControlWidget * )
{
  lLogFmt("MatchIntro: Result Only\n");
  FDSound::ChangeCrowd( CROWD_STATE_NONE );
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_CANCEL );
}

void FDMatchIntroScreen::OnTeamsButton( lUIControlWidget * )
{
  lLogFmt("MatchIntro: Teams\n");
  m_state = VIEW_TEAMS;

  //GetMainUI()->SetHelpTextFilename("screen_matchintro_teams.txt");
  m_matchIntroScreenSub->Hide();

  m_table->Show();
  m_reportTitleText->Hide();
  m_reportTextStripe->Hide();


  m_matchTeamButton->Hide();
  m_matchOppositionButton->Hide();
  m_matchViewButton->Hide();
  m_matchResultButton->Hide();
  m_matchBackButton->Show();

  GetMainUI()->GetPlayerInfoSubScreen()->Show();

  Update();
}

void FDMatchIntroScreen::OnOppositionButton( lUIControlWidget * )
{
  lLogFmt("MatchIntro: Opposition\n");
  m_state = VIEW_OPPINFO;

  //GetMainUI()->SetHelpTextFilename("screen_matchintro_opposition.txt");
  m_table->Show();
  m_tacticsTable->Show();
  m_tacticsText->Show();
  m_tacticsStripe->Show();

  m_reportTitleText->Show();
  m_reportTextStripe->Show();
  m_compTitleText->Hide();
  m_homeClubText->Hide();
  m_vsText->Hide();
  m_awayClubText->Hide();
  m_stadiumText->Hide();

  m_teamBoxBg->Hide();

  for( int i = 0; i < 2; i++ )
  {
    m_backingStripes[i]->Hide();
    m_backingBars[i]->Hide();
  }


  m_matchOppositionButton->Hide();
  m_matchTeamButton->Hide();
  m_matchViewButton->Hide();
  m_matchResultButton->Hide();
  m_matchBackButton->Show();

  Update();
}

void FDMatchIntroScreen::OnBackButton( lUIControlWidget * )
{
  lLogFmt("MatchIntro: Back\n");
  m_state = VIEW_INTRO;

  //GetMainUI()->SetHelpTextFilename("screen_matchintro.txt");
  m_matchIntroScreenSub->Show();

  for( int i = 0; i < 2; i++ )
  {
    m_backingStripes[i]->Show();
    m_backingBars[i]->Show();
  }

  m_teamBoxBg->Show();

  m_table->Hide();
  m_tacticsTable->Hide();
  m_tacticsText->Hide();
  m_tacticsStripe->Hide();
  m_reportTitleText->Hide();
  m_reportTextStripe->Hide();
  m_reportMessageBar->Hide();
  m_reportMessageText->Hide();

  m_compTitleText->Show();
  m_homeClubText->Show();
  m_vsText->Show();
  m_awayClubText->Show();
  m_stadiumText->Show();

  m_matchTeamButton->Show();
  m_matchViewButton->Show();
  m_matchResultButton->Show();
  m_matchOppositionButton->Show();
  m_matchBackButton->Hide();

  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  GetMainUI()->HideSubMessage();

  Update();
}


void FDMatchIntroScreen::OnCellSelect( FDTableWidget *, int row, int col)
{
  lLogFmt("MatchIntro: Cell Selected, %d, %d\n", row, col );

  // minus header
  row--;

  if( row < 0 || col == 1 || col == 3 || m_state == VIEW_REPORT )
  {
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
	  GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);
	  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
    return;
  }

  GetMainUI()->HideSubMessage();

  CPlayer * pPlayer = NULL;

  if( m_state == VIEW_TEAMS )
  {
    if( col == 0 ) // HOME
    {
      pPlayer = &WorldData().GetMatchHandler().GetMatchPlay().GetPlayer(HOME, row);
    }
    else if( col == 4 ) // AWAY
    {
      pPlayer = &WorldData().GetMatchHandler().GetMatchPlay().GetPlayer(AWAY, row);
    }
  }

  if( m_state != VIEW_OPPINFO )
  {
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( pPlayer );

	if (pPlayer != NULL)
		GetMainUI()->GetPlayerInfoSubScreen()->Show();
	else
		GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  }
}

void FDMatchIntroScreen::Update()
{
  m_table->ClearAll();
  m_table->ResetScroll();

  m_tacticsTable->ClearAll();
  m_tacticsTable->ResetScroll();

  switch( m_state )
  {
    case VIEW_INTRO:
    {
      CFixture& Fixture = WorldData().GetMatchHandler().GetMatchPlay().GetFixture();
      lString str;

      str = WorldData().GetMatchHandler().GetMatchPlay().FullCompetitionTitle();
      m_compTitleText->SetText(lUIText(str));

	  // ***DEBUG - the longest competition name string I could find.
	  //m_compTitleText->SetText(lUIText("JOHNSTONES PAINT TROPHY REGIONAL QUARTER FINAL"));

      m_compTitleText->SetTextToUpper();

      str = Fixture.StadiumName();
      m_stadiumText->SetText(lUIText(str));
      m_stadiumText->SetTextToUpper();


      if (WorldData().GetMatchHandler().GetMatchPlay().IsLeagueMatch() == true && WorldData().GetMatchHandler().GetMatchPlay().GetDivision().GetNumberMatchesPlayedThisSeason() > 1)
    	{
    		str.Format("%s (%d)", Fixture.GetClub(HOME).GetName(), Fixture.GetClub(HOME).GetTableData().GetLeaguePosition() + 1);
    	}
    	else
    	{
    		str = Fixture.GetClub(HOME).GetName();
    	}
      m_homeClubText->SetText(lUIText(str));

    	if (WorldData().GetMatchHandler().GetMatchPlay().IsLeagueMatch() == true && WorldData().GetMatchHandler().GetMatchPlay().GetDivision().GetNumberMatchesPlayedThisSeason() > 1)
    	{
    		str.Format("%s (%d)", Fixture.GetClub(AWAY).GetName(), Fixture.GetClub(AWAY).GetTableData().GetLeaguePosition() + 1);
    	}
    	else
    	{
    		str = Fixture.GetClub(AWAY).GetName();
    	}
      m_awayClubText->SetText(lUIText(str));

      break;
    }
    case VIEW_TEAMS:
      UpdateTeam();
      break;
    case VIEW_REPORT:
    case VIEW_OPPINFO:
      UpdateReport();
      break;
  };
}

void FDMatchIntroScreen::UpdateTeam()
{
  GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
	GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);
	GetMainUI()->GetPlayerInfoSubScreen()->Hide();

	CClub &cHomeClub = WorldData().GetMatchHandler().GetMatchPlay().GetClub(HOME);
	CClub &cAwayClub = WorldData().GetMatchHandler().GetMatchPlay().GetClub(AWAY);

  // Move into place
  m_table->Move( lUIWidgetPosition( lUIPoint( 20, 20) ), lUIWidgetSize(  lUIPoint( 256 - 20, 144 )  ) );
  m_table->GetScrollBar()->Hide();

	// Fill table
  m_table->AddColumn( lUICoord( 92.0f ), LUI_RIGHT_CENTER );
  m_table->AddColumn( lUICoord( 15.0f ) );
  m_table->AddColumn( lUICoord( 24.0f ) );
  m_table->AddColumn( lUICoord( 15.0f ) );
  m_table->AddColumn( lUICoord( 92.0f ), LUI_LEFT_CENTER );

  int NumRows = PLAYERSINTEAM + 1;
	m_table->AllowCellSelection(true);
	m_table->AddRows(NumRows+1);

	m_table->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));


	// Headers
  m_table->SetText( 0, 0, lUIText( IDS_HOME_CAPS ));
  m_table->SetText( 0, 2, lUIText( IDS_NUMBER_ABBR ));
  m_table->SetText( 0, 4, lUIText( IDS_AWAY_CAPS ));

	lString str;
	for (int LoopCount = 0; LoopCount < NumRows-1; LoopCount++)
	{
		CPlayer *pPlayer1 = &WorldData().GetMatchHandler().GetMatchPlay().GetPlayer(HOME, LoopCount);
		CPlayer *pPlayer2 = &WorldData().GetMatchHandler().GetMatchPlay().GetPlayer(AWAY, LoopCount);

 //   bool bPlayer1Case = (pPlayer1->GetSkills().GetOverallSkill() > pPlayer2->GetSkills().GetOverallSkill()) ? true : false;
 //   bool bPlayer2Case = (pPlayer2->GetSkills().GetOverallSkill() > pPlayer1->GetSkills().GetOverallSkill()) ? true : false;

    m_table->SetBkgColour( LoopCount + 1, 0, PosnColour[pPlayer1->GetSkills().GetPosition()] );
    m_table->SetText(LoopCount + 1, 0, lUIText( FDMakePlayerNameLink(pPlayer1) ));

		str.Format( "%d", LoopCount + 1 );
		m_table->SetText(LoopCount + 1, 2, lUIText( str ));

		if( LoopCount < PLAYERSINTEAM )
		{
      m_table->SetBkgColour(LoopCount + 1, 2, L_ARGB(0xFF, 0x50, 0x50, 0x50));
		}
		else
		{
      m_table->SetBkgColour(LoopCount + 1, 2, L_ARGB(0xFF, 0x40, 0x40, 0x40));
		}

    m_table->SetBkgColour( LoopCount + 1, 4, PosnColour[pPlayer2->GetSkills().GetPosition()] );
    m_table->SetText(LoopCount + 1, 4, lUIText( FDMakePlayerNameLink(pPlayer2) ));

    m_table->SetBkgColour( LoopCount+1, 1, L_ARGB(80, 0x70, 0x70, 0x70) );
    m_table->SetBkgColour( LoopCount+1, 3, L_ARGB(80, 0x70, 0x70, 0x70) );

    str.Format("%d", pPlayer1->GetSkills().GetOverallSkill());
    m_table->SetText( LoopCount+1, 1, lUIText( str ) );

    str.Format( "%d", pPlayer2->GetSkills().GetOverallSkill());
    m_table->SetText( LoopCount+1, 3, lUIText( str ) );

	}
}

void FDMatchIntroScreen::UpdateReport()
{
  m_table->AllowCellSelection(false);

  // Move into place
  m_table->Move( lUIWidgetPosition( lUIPoint( 20, 40) ), lUIWidgetSize(  lUIPoint( 256 - 20 - 10, 120 )  ) );
  m_table->GetScrollBar()->Show();

  CFixture& Fixture = WorldData().GetMatchHandler().GetMatchPlay().GetFixture();
  CTactic curTactic;
  if( Fixture.GetClub( HOME ).IsUserControlled() != true )
  {
    curTactic = Fixture.GetClub( HOME ).GetTactics(); //WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();
  } else if( Fixture.GetClub( AWAY ).IsUserControlled() != true)
  {
    curTactic = Fixture.GetClub( AWAY ).GetTactics(); //WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();
  }


  lLogFmt("Current Tactics\n");
  lLogFmt("  %s - %d\n", curTactic.TitleStr(), curTactic.GetFamiliarity());


  CTeamTactic&	curTeamTactic = curTactic.GetTeamTactic();

  {
    // Tactics report
    m_tacticsTable->AddColumn(lUICoord( 128 ), LUI_CENTER_RIGHT); // label
    m_tacticsTable->AddColumn(lUICoord( 5 ), LUI_CENTER_CENTER); // gap
    m_tacticsTable->AddColumn(lUICoord( 100 ), LUI_CENTER_CENTER); // value
    m_tacticsTable->AddColumn(lUICoord( 24 ), LUI_CENTER_CENTER); // gap
    m_tacticsTable->AddRows( 6 );

    for ( int i = 0; i < 6; i++ )
    {
      m_tacticsTable->SetBkgColour(i, 0, L_ARGB(0xFF, 0x80, 0x80, 0x80));
      m_tacticsTable->SetBkgColour(i, 1, L_ARGB(0xFF, 0x80, 0x80, 0x80));
      m_tacticsTable->SetBkgColour(i, 2, L_ARGB(0xFF, 0x50, 0x50, 0x50));
      m_tacticsTable->SetBkgColour(i, 3, L_ARGB(0xFF, 0x80, 0x80, 0x80));
    }

    // formation
    lString text;
    m_tacticsTable->SetText(0, 0, lUIText(IDS_PLAYSTYLE7));
    m_tacticsTable->SetTextToUpper(0, 0);
    m_tacticsTable->SetText(0, 2, lUIText(curTactic.TitleStr()));

    // different styles
    m_tacticsTable->SetText(1, 0, lUIText(IDS_STYLEOFPLAY));
    m_tacticsTable->SetTextToUpper(1, 0);
    m_tacticsTable->SetText(1, 2, lUIText(g_styleStrID[curTeamTactic.GetStyleOfPlay()]));

    m_tacticsTable->SetText(2, 0, lUIText(IDS_AGGRESSION));
    m_tacticsTable->SetTextToUpper(2, 0);
    m_tacticsTable->SetText(2, 2, lUIText(g_aggressionStrID[curTeamTactic.GetAggression()]));

    m_tacticsTable->SetText(3, 0, lUIText(IDS_MARKSTYLE));
    m_tacticsTable->SetTextToUpper(3, 0);
    m_tacticsTable->SetText(3, 2, lUIText(g_markingStrID[curTeamTactic.GetMarkingStyle()]));

    m_tacticsTable->SetText(4, 0, lUIText("COUNTERATTACK"));
    m_tacticsTable->SetTextToUpper(4, 0);

    if( curTeamTactic.GetCounterAttack() )
      m_tacticsTable->SetText(4, 2, lUIText( "Yes" ));
    else
      m_tacticsTable->SetText(4, 2, lUIText( "No" ));

    m_tacticsTable->SetText(5, 0, lUIText("OFFSIDE TRAP"));
    m_tacticsTable->SetTextToUpper(5, 0);

    if( curTeamTactic.GetOffsideTrap() )
      m_tacticsTable->SetText(5, 2, lUIText( "Yes" ));
    else
      m_tacticsTable->SetText(5 , 2, lUIText( "No" ));

  }

  {
    // Get Report
    m_reportTitleText->Show();
    lArray<CString> stringList;
    WorldData().GetCurrentUser()->DoPreMatchReportFDL(&stringList);

    if( stringList.Size() == 0 )
    {
      m_table->AddColumn(lUICoordPercent( 0.025f ), LUI_CENTER_LEFT);
      m_table->AddColumn(lUICoordPercent( 0.95f ), LUI_CENTER_LEFT);
      m_table->AddColumn(lUICoordPercent( 0.025f ), LUI_CENTER_LEFT);
      m_table->AddRows( 1 );
      //m_reportTitleText->SetText(lUIText( IDS_REPORT_NOT_AVAILABLE ));
      m_table->Hide();
      m_reportTitleText->Hide();
      m_reportTextStripe->Hide();
      m_reportMessageBar->Show();
      m_reportMessageText->Show();
    } else
    {
      m_table->AddColumn(lUICoordPercent( 0.025f ), LUI_CENTER_LEFT);
      m_table->AddColumn(lUICoordPercent( 0.95f ), LUI_CENTER_LEFT);
      m_table->AddColumn(lUICoordPercent( 0.025f ), LUI_CENTER_LEFT);
      m_table->AddRows( 1+stringList.Size() );
      //m_reportTitleText->SetText(lUIText( IDS_REPORT32 ));
      m_reportTitleText->SetTextToUpper();
      for( int i = 0; i < stringList.Size(); i++ )
      {
        m_table->SetText(i, 1, lUIText( stringList(i) ));
      }
      m_reportMessageBar->Hide();
      m_reportMessageText->Hide();
    }
  }
}

void FDMatchIntroScreen::DoShow()
{
  lLogFmt("DoShow\n");
  m_matchIntroScreen->Show();
  m_matchIntroScreenSub->Show();

  m_tacticsTable->Hide();
  m_tacticsText->Hide();
  m_tacticsStripe->Hide();

//  m_state = VIEW_INTRO;

  GetMainUI()->GetMenuBar4()->InsertBefore(m_fakeCommButton);

  Update();

  if( GetMainUI()->GetSaveData().m_userPrefs.m_bInMatchSFX )
  {
    FDSound::ChangeCrowd( CROWD_STATE_PREMATCH );
  }
}

void FDMatchIntroScreen::SetIntroView()
{
  m_state = VIEW_INTRO;
}

void FDMatchIntroScreen::DoHide()
{
  m_matchIntroScreen->Hide();
  m_matchIntroScreenSub->Hide();
}


void FDMatchIntroScreen::AutoplaySkip()
{
  //if (::rand() % 2)
  //{
  //  // Result only
  //  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_CANCEL );
  //}
  //else
	{
		// View and then auto-sim.
		GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
	}
}
