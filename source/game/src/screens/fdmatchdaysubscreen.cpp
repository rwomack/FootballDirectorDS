//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchDaySubscreen.cpp
// Description : FDMatchDayScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdmarqueewidget.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <ui/luispritewidget.h>

#include <screens/fdmatchdayscreen.h>
#include <screens/fdliveleaguetablescreen.h>
#include <screens/fdliveresultscreen.h>

using namespace L_NAMESPACE_NAME;

enum
{
  TAG_FLAG_REDCARD_HOME       = 0x001,
  TAG_FLAG_REDCARD_AWAY       = 0x002,
  TAG_FLAG_YELCARD_HOME       = 0x004,
  TAG_FLAG_YELCARD_AWAY       = 0x008,
  TAG_FLAG_GOAL_HOME          = 0x010,
  TAG_FLAG_GOAL_AWAY          = 0x020,
  TAG_FLAG_PENSO_HOME_HIT     = 0x040,
  TAG_FLAG_PENSO_AWAY_HIT     = 0x080,
  TAG_FLAG_PENSO_HOME_MISS    = 0x100,
  TAG_FLAG_PENSO_AWAY_MISS    = 0x200,
};


static int AddMenuButton( FDMenuWidget *menu, const char *spritePrefix, const char *spriteName )
{
  char tmp[256];

  sprintf( tmp, "%s_btn-on", spritePrefix);
  lSharedPtr<lSprite> buttonOn = FDResourceManager::GetSprite("uimenu", tmp);
  sprintf( tmp, "%s_btn-off", spritePrefix);
  lSharedPtr<lSprite> buttonOff = FDResourceManager::GetSprite("uimenu", tmp);

  sprintf( tmp, "%s_%s_desc-on", spritePrefix, spriteName);
  lSharedPtr<lSprite> textOn = FDResourceManager::GetSprite("uimenu", tmp);
  sprintf( tmp, "%s_%s_desc-off", spritePrefix, spriteName);
  lSharedPtr<lSprite> textOff = FDResourceManager::GetSprite("uimenu", tmp);

  sprintf( tmp, "%s_%s_icon", spritePrefix, spriteName);
  lSharedPtr<lSprite> icon = FDResourceManager::GetSprite("uimenu", tmp);

  sprintf( tmp, "%s_%s_icon_glow", spritePrefix, spriteName);
  lSharedPtr<lSprite> iconGlow = FDResourceManager::GetSprite("uimenu", tmp);

  return menu->AddButton( buttonOn, buttonOff, textOn, textOff, icon, iconGlow );
}

static bool s_penaltyShootOutEnd = false;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchDayScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMatchDayScreen::FDMatchDayScreen( FDMainMenuUI *parent ) : FDScreen( parent ), m_matchPlay( NULL ), m_currentView(VIEW_COMMENTARY), m_iNonUserPauseCount(0)
{
  // init commentary
  m_commentaryScreen = FD_GAME_NEW FDMatchDayCommentaryScreen( parent );
  m_skillScreen      = FD_GAME_NEW FDMatchDaySkillScreen( parent );
  m_eventsScreen     = FD_GAME_NEW FDMatchDayEventsScreen( parent );
  m_teamScreen       = FD_GAME_NEW FDMatchDayTeamScreen( parent );
  m_subsScreen       = FD_GAME_NEW FDMatchDaySubsScreen( parent );
  m_leagueScreen     = FD_GAME_NEW FDLiveLeagueTableScreen( parent );
  m_resultsScreen    = FD_GAME_NEW FDLiveResultScreen( parent );

  // init our selves last
  Init();
}

FDMatchDayScreen::~FDMatchDayScreen()
{
}

void FDMatchDayScreen::Init()
{
  CMatchPlay::SetMakeSubstitutionCallback(SubstitutionMadeCallback, this);
  CMatchPlay::SetSkipToEndOfMatchCommentaryCallback(SkipMatchCommentaryCallback, this);
  CMatchPlay::SetSkipToEndOfMatchPenaltyShootOutCallback(SkipMatchPenaltyShootOutCallback, this);

  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  //  - Sub screen

  m_matchScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), LUI_WIDGET_MOUSETRANSPARENT, "matchday_screen" );
  m_matchScreen->Hide();

  m_matchSubScreen = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "matchday_screen_sub" );
  m_matchSubScreen->Hide();

  m_homeName = FD_GAME_NEW lUITextWidget( m_matchSubScreen, 0, "temp1", lUIText(""), LUI_LEFT_TOP, lUIPoint( 62, 46 ), lUIPoint( 120, 16 )  );
  m_homeName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_homeName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_awayName = FD_GAME_NEW lUITextWidget( m_matchSubScreen, 0, "temp1", lUIText(""), LUI_LEFT_TOP, lUIPoint( 62, 68 ), lUIPoint( 120, 16 )  );
  m_awayName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_awayName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_homeScore = FD_GAME_NEW lUITextWidget( m_matchSubScreen, 0, "temp1", lUIText(""), LUI_RIGHT_TOP, lUIPoint( 192, 46 ) );
  m_homeScore->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_homeScore->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_awayScore = FD_GAME_NEW lUITextWidget( m_matchSubScreen, 0, "temp1", lUIText(""), LUI_RIGHT_TOP, lUIPoint( 192, 68 ) );
  m_awayScore->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_awayScore->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget *splitterBar = FD_GAME_NEW FDBarWidget(m_matchSubScreen,0, "splitter", LUI_CENTER_CENTER,
                                                     lUIPoint(128, 65), lUIPoint(160, 1));

  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));


  //m_halftimeScore = FD_GAME_NEW lUITextWidget( m_matchSubScreen, 0, "temp2", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 128, 62 ) );
  //m_halftimeScore->SetFont( FDResourceManager::GetFont("font10", "font10" ) );

  lUISpriteWidget *homeKitBG = FD_GAME_NEW lUISpriteWidget( m_matchSubScreen, 0, "homeShield", FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_LEFT_TOP,
                  lUIPoint( 5, 42) );
  homeKitBG->GetSprite()->SetAlphaEnable( true );
  lUISpriteWidget *awayKitBG = FD_GAME_NEW lUISpriteWidget( m_matchSubScreen, 0, "awayShield", FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_LEFT_TOP,
                  lUIPoint( 210, 42));
  awayKitBG->GetSprite()->SetAlphaEnable( true );

  lUIPoint pt = homeKitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_homeKit = FD_GAME_NEW lUISpriteWidget( m_matchSubScreen, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP,  pt );

  pt = awayKitBG->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_awayKit = FD_GAME_NEW lUISpriteWidget( m_matchSubScreen, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP,  pt );

  // table
  m_matchtable = FD_GAME_NEW FDTableWidget( m_matchSubScreen, 0, "matchtable",
            FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_AUTO_SCROLL | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 0, 95 ), lUIPoint( 255, 192 - 95 ) );
  m_matchtable->SetCellRenderCallback(FDMatchDayScreen::TableCellRenderCallback, this);
  m_matchtable->SetRowGap( 2 );

  m_prematchMessageBar = FD_GAME_NEW lUISpriteWidget(m_matchScreen, 0, "messagerBar", FDResourceManager::GetSprite("message_box_small", "message_box_small"), LUI_CENTER_LEFT,
   lUIPoint( 20, 96 ));

  m_prematchMessageText = FD_GAME_NEW lUITextWidget(m_matchScreen, 0, "reporttitle", lUIText("PRE-MATCH"), LUI_CENTER_CENTER, lUIPoint(138, 96 ));
  m_prematchMessageText->SetFont(FDResourceManager::GetFont( "font16", "font16" ) );
  m_prematchMessageText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_redCardSprite = FDResourceManager::GetSprite("uiicon", "redcard");
  m_yelCardSprite = FDResourceManager::GetSprite("uiicon", "yellowcard");
  m_goalSprite = FDResourceManager::GetSprite("uiicon", "ball");

  m_penHitSprite = FDResourceManager::GetSprite("uiicon", "penalty_green");
  m_penMissSprite = FDResourceManager::GetSprite("uiicon", "penalty_red");

  // create date/time
  m_dateBar = FD_GAME_NEW lUISpriteWidget( m_matchSubScreen, 0, "datebar", FDResourceManager::GetSprite("uiicon","speed1"), LUI_TOP_LEFT, lUIPoint( 0, 4 )  );
  m_subDateTime = FD_GAME_NEW lUITextWidget( m_matchSubScreen, 0, "timedate", lUIText("00" ), LUI_TOP_LEFT, lUIPoint( 5, 8 ) );
  m_subDateTime->SetFont( FDResourceManager::GetFont("fonttimer", "fonttimer" ) );
  m_subDateTime->GetFont()->SetFixedCharWidth( 9 );

  // add fades
  m_fadeEnd     = FD_GAME_NEW FDFullScreenWidget( GetMainScreenWidget(), 0, "fadeEnd", L_ARGB( 225, 0, 0, 0 ) );
  m_fadeEnd->Hide();

  m_fade     = FD_GAME_NEW FDFullScreenWidget( GetMainScreenWidget(), 0, "fade", L_ARGB( 225, 0, 0, 0 ) );
  m_fade->Hide();

  m_fadeSub     = FD_GAME_NEW FDFullScreenWidget( m_matchSubScreen, 0, "fade", L_ARGB( 225, 0, 0, 0 ) );
  m_fadeSub->Hide();

  // Process button
  m_matchDoneButton = FD_GAME_NEW FDButtonWidget( m_matchScreen, 0, "Donebutton", FDResourceManager::GetSprite("uiicon","fast_forward"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_TOP, lUIPoint( 0, 2 ) );
  m_matchDoneButton->SignalControlCommand.Connect( this, &FDMatchDayScreen::OnDoneButton );

  m_matchDoneText = FD_GAME_NEW lUITextWidget(m_fadeEnd, 0, "Donetext", lUIText(IDS_MATCH_DONE_TEXT), LUI_CENTER_CENTER, lUIPoint(128, 74));

  // setup match stuff
  m_pauseButton = FD_GAME_NEW FDButtonWidget( m_matchScreen, 0, "matchpause", FDResourceManager::GetSprite("uiicon","menu_match_pause"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,2) );
  m_playButton = FD_GAME_NEW FDButtonWidget( m_matchScreen, 0, "matchplay", FDResourceManager::GetSprite("uiicon","fast_forward"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,2) );
  m_pauseButton->Hide();

  m_skipButton = FD_GAME_NEW FDButtonWidget( m_matchScreen, 0, "matchskip", FDResourceManager::GetSprite("uiicon","menu_match_skip"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,156) );

  m_pauseButton->SignalControlCommand.Connect( this, &FDMatchDayScreen::OnPauseButton );
  m_playButton->SignalControlCommand.Connect( this, &FDMatchDayScreen::OnPlayButton );
  m_skipButton->SignalControlCommand.Connect( this, &FDMatchDayScreen::OnSkipButton );

  m_marquee = FD_GAME_NEW FDMarqueeWidget(m_matchSubScreen, 0, "marquee", LUI_CENTER_CENTER, lUIPoint(128, 96), lUIPoint(256, 45));
  m_marquee->SignalMessageComplete.Connect(this, &FDMatchDayScreen::OnMarqueeDone);

  // create buttons
  m_menuBar4 = FD_GAME_NEW lUISpriteWidget( m_matchScreen, LUI_WIDGET_MOUSETRANSPARENT, "matchmenubar4", FDResourceManager::GetSprite("menu_bar_4","menu_bar_4") );
  m_commButton    = FD_GAME_NEW FDButtonWidget( m_matchScreen, 0, "matchcomm", FDResourceManager::GetSprite("uiicon","menu_commentary"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,47) );
  m_eventsButton  = FD_GAME_NEW FDButtonWidget( m_matchScreen, 0, "matchevents", FDResourceManager::GetSprite("uiicon","menu_events"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,    LUI_TOP_LEFT, lUIPoint(0,69) );
  m_skillButton   = FD_GAME_NEW FDButtonWidget( m_matchScreen, 0, "matchskill", FDResourceManager::GetSprite("uiicon","menu_skills"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,     LUI_TOP_LEFT, lUIPoint(0,91) );

  //NOTE: If we want to move the commentary / events / skills to the menu there are icons for those also

  m_commButton->SignalControlCommand.Connect( this, &FDMatchDayScreen::OnMenuButton );
  m_eventsButton->SignalControlCommand.Connect( this, &FDMatchDayScreen::OnMenuButton );
  m_skillButton->SignalControlCommand.Connect( this, &FDMatchDayScreen::OnMenuButton );

  // create menu rollouts
  m_menu   = FD_GAME_NEW FDMenuWidget( m_matchScreen, 0, "matchmenurollout", FDResourceManager::GetSprite("uiicon","menu_match"), lSharedPtr<lSprite>(), lUIPoint( 0, 113 ),  LUI_TOP_LEFT, lUIPoint( 0, 0 ) );
  AddMenuButton( m_menu, "menu_match", "team_sheet" );
  AddMenuButton( m_menu, "menu_match", "substitutes" );
  m_liveTableMenuIdx = AddMenuButton( m_menu, "menu_match", "live_table" );
  m_liveScoreMenuIdx = AddMenuButton( m_menu, "menu_match", "live_score" );
  AddMenuButton( m_menu, "menu_match", "speed_up" );
  AddMenuButton( m_menu, "menu_match", "speed_down" );

  m_menuBack = FD_GAME_NEW FDButtonWidget(m_matchScreen, 0, "matchback", FDResourceManager::GetSprite("uiicon", "menu_match"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(0, 113), lUIPoint(17, 17));
  m_menuBack->Hide();
  m_menuBack->SignalControlCommand.Connect(this, &FDMatchDayScreen::OnMenuBackingButton);

  m_menu->SignalHot.Connect( this, &FDMatchDayScreen::OnMenuButtonHot );
  m_menu->SignalCommand.Connect( this, &FDMatchDayScreen::OnMenuButtonCommand );

  m_bEndOfHalf = false;
  m_currentShootOutAway = 0;
  m_currentShootOutHome = 0;
  m_bPenaltyHome = true;
  m_penaltyShootOutStarted = false;
  s_penaltyShootOutEnd = false;
}

void FDMatchDayScreen::OnDoneButton( lUIControlWidget *button )
{
  lLogFmt("Match done button\n");
  m_fadeEnd->Hide();
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );

  // Clear match info so we don't go back into it
  m_matchPlay = NULL;

  FDSound::ChangeCrowd( CROWD_STATE_NONE );
}

// Main menu controls
void FDMatchDayScreen::OnMenuButtonHot( FDMenuWidget *rollout, int index )
{
  lLogFmt("OnMenuButtonHot %d\n", index);
  // will receive -1 for when inactive
  // otherwise receive index of one clicked

  if( index == -1 )
  {
    m_fade->Hide();
    m_fade->BringToFront();
    //m_fadeSub->Hide();
	m_menuBack->Hide();
  } else
  {
    m_fade->Show();
    m_fade->InsertBefore( m_menuBar4 );
    //m_fadeSub->Show();
	m_menuBack->Show();
  }
}

//
// Main menu buttons
void FDMatchDayScreen::OnMenuButtonCommand( FDMenuWidget *rollout, int index )
{
  lLogFmt("OnMenuButtonCommand %d\n", index);

  bool bClick = false;

  m_prematchMessageText->Hide();
  m_prematchMessageBar->Hide();

  // Club
  if( index == 0 )
  {
    bClick = true;
    SetView( VIEW_LINEUPS );
  }

  if( index == 1 )
  {
    bClick = true;
    SetView( VIEW_SUBS );
  }

  if( index == 2 )
  {
    bClick = true;
    SetView( VIEW_LEAGUETABLE );
  }

  if( index == 3 )
  {
    bClick = true;
    SetView( VIEW_LATESTSCORES );
  }

  if( index == 4 )
  {
    // increase match speed
//    bClick = true;
    GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed++;
    if( GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed > 5 )
    {
      GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed = 5;
    }
    else
    {
      FDSound::PlaySoundEffect( FD_SFX_UI_SCROLL );
    }

    if( GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed >= 5 )
    {
      m_menu->SetButtonEnabled( 4, false );
      m_menu->SetButtonEnabled( 5, true );
    } else
    {
      m_menu->SetButtonEnabled( 4, true );
      m_menu->SetButtonEnabled( 5, true );
    }
  }

  if( index == 5 )
  {
    // decrease match speed
//    bClick = true;
    GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed--;
    if( GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed < 1 )
    {
      GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed = 1;
    }
    else
    {
      FDSound::PlaySoundEffect( FD_SFX_UI_SCROLL );
    }

    if( GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed <= 1 )
    {
      m_menu->SetButtonEnabled( 4, true );
      m_menu->SetButtonEnabled( 5, false );
    } else
    {
      m_menu->SetButtonEnabled( 4, true );
      m_menu->SetButtonEnabled( 5, true );
    }
  }

  if( index == 4 || index == 5 )
  {
    // update match speed sprite
    char tmp[32];
    sprintf( tmp, "speed%d", GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed);
    m_dateBar->SetSprite( FDResourceManager::GetSprite( "uiicon", tmp ) );
  }

  if( bClick )
  {
    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
  }
}

void FDMatchDayScreen::OnMenuBackingButton(lUIControlWidget *)
{
	m_menuBack->Hide();
	m_menu->RollIn();
}

void FDMatchDayScreen::OnMenuButton( lUIControlWidget *button )
{
  // Don't show any match menu item as selected
  m_menu->SetSelectedIndex( -1 );

  m_prematchMessageText->Hide();
  m_prematchMessageBar->Hide();

  if( button == m_commButton )
  {
    SetView( VIEW_COMMENTARY );
    m_commButton->GetSprite()->SetColour( L_ARGB( 255, 255, 255, 255 ) );
    m_eventsButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_skillButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_menu->RollIn();
  }

  if( button == m_eventsButton )
  {
    SetView( VIEW_EVENTS );
    m_commButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_eventsButton->GetSprite()->SetColour( L_ARGB( 255, 255, 255, 255 ) );
    m_skillButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_menu->RollIn();
  }

  if( button == m_skillButton )
  {
    SetView( VIEW_SKILL );
    m_commButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_eventsButton->GetSprite()->SetColour( L_ARGB( 255, 155, 155, 155 ) );
    m_skillButton->GetSprite()->SetColour( L_ARGB( 255, 255, 255, 255 ) );
    m_menu->RollIn();
  }
}

void FDMatchDayScreen::DoShow()
{
  m_matchScreen->Show();
  m_matchSubScreen->Show();

  GetMainUI()->HideSubMessage();


  SetView( m_currentView );
}

void FDMatchDayScreen::DoHide()
{
  m_matchSubScreen->Hide();
  m_matchScreen->Hide();

  m_skillScreen->Hide();
  m_eventsScreen->Hide();
  m_teamScreen->Hide();
  m_subsScreen->Hide();
  m_commentaryScreen->Hide();
  m_leagueScreen->Hide();
  m_resultsScreen->Hide();
}

void FDMatchDayScreen::Tick( float delta )
{
  if((m_matchPlay != NULL) && (m_matchPlay->GetMatchActionInfo().GetSoundFXID() != SOUND_MAX ) && (m_bSkippingMatch == false))
  {
    m_timer += 1.5f;

    if( GetMainUI()->GetSaveData().m_userPrefs.m_bInMatchSFX )
    {
      //NOTE: TODO: Needs proper integration with crowd system
      switch( m_matchPlay->GetMatchActionInfo().GetSoundFXID() )
      {
        case SOUND_BOO:
          lLogFmt("\nCROWD DONT AGREE\n");
          FDSound::PlaySoundEffect( FD_SFX_CROWD_CARD_DONTAGREE );
          break;

        case SOUND_CHEER:
          lLogFmt("\nCROWD APPROVAL\n");
          FDSound::PlaySoundEffect( FD_SFX_CROWD_CARD_APPROVAL );
          break;

        //case SOUND_CROWDLRG: break;
        //case SOUND_CROWDMED: break;
        //case SOUND_CROWDSML: break;

        case SOUND_FULLWHST:
          FDSound::PlaySoundEffect( FD_SFX_REF_WHISTLE_FULLTIME );
          break;

        case SOUND_GOALCHEE:
        {
          lLogFmt("\nCROWD GOAL CHEER\n");
          if( (rand() % 100) > 50 )
            FDSound::PlaySoundEffect( FD_SFX_CROWD_GOAL1 );
          else
            FDSound::PlaySoundEffect( FD_SFX_CROWD_GOAL2 );
          break;
        }

        case SOUND_MISS:
        {
          lLogFmt("\nMISS SOUND\n");
          if( (rand() % 100) > 50 )
            FDSound::PlaySoundEffect( FD_SFX_CROWD_CLOSESHOT1);
          else
            FDSound::PlaySoundEffect( FD_SFX_CROWD_CLOSESHOT2 );
          break;
        }

        case SOUND_ROAR: break;
        case SOUND_WHISTLE:
        {
          lLogFmt("\nREF WHISTLE\n");
          FDSound::PlaySoundEffect( FD_SFX_REF_WHISTLE );
          break;
        }

        case SOUND_BEEP: break;
      }
    }

    // clear sound
    m_matchPlay->GetMatchActionInfo().SetSoundFXID(SOUND_MAX);
  }

  if(m_matchPlay != NULL)
  {
    DoUpdateCommentary();
    if (m_bWaitForMarquee == true)
    {
      return;
    }

    m_timer -= delta;

    if( m_timer <= 0.0f )
    {
      //lLogFmt("Timer triggered\n");
      OnTimer();
    }
  }
}

void FDMatchDayScreen::SetMenuForFullTime( bool b )
{
  if( b )
  {
    m_menu->ChangeButtonText( m_liveTableMenuIdx, FDResourceManager::GetSprite("uimenu", "menu_match_table_desc-on"),
                                                  FDResourceManager::GetSprite("uimenu", "menu_match_table_desc-off") );

    m_menu->ChangeButtonText( m_liveScoreMenuIdx, FDResourceManager::GetSprite("uimenu", "menu_match_score_desc-on"),
                                                  FDResourceManager::GetSprite("uimenu", "menu_match_score_desc-off") );
  } else
  {
    m_menu->ChangeButtonText( m_liveTableMenuIdx, FDResourceManager::GetSprite("uimenu", "menu_match_live_table_desc-on"),
                                                  FDResourceManager::GetSprite("uimenu", "menu_match_live_table_desc-off") );

    m_menu->ChangeButtonText( m_liveScoreMenuIdx, FDResourceManager::GetSprite("uimenu", "menu_match_live_score_desc-on"),
                                                  FDResourceManager::GetSprite("uimenu", "menu_match_live_score_desc-off") );
  }
}

void FDMatchDayScreen::SetMatchPlay( CMatchPlay* _MatchPlay )
{
  lLogFmt("SetMatchPlay: %x\n", _MatchPlay);
  m_matchPlay = _MatchPlay;

  m_matchDoneButton->Hide();
  m_matchDoneText->Hide();
  m_fadeSub->Hide();

  m_commentaryScreen->Clear();

  // reset menu for start of match
  SetMenuForFullTime( false );

  // start paused
  m_iNonUserPauseCount = 1;
  m_bUserPause = false;
  m_SubstitutionRequested = false;
  m_timer = 0.0f;
  m_bWaitForMarquee = false;
  m_iLastMarqueeEvent = -1;
  m_currentShootOutAway = 0;
  m_currentShootOutHome = 0;
  m_bSkippingMatch = false;
  m_bPenaltyHome = true;
  m_penaltyShootOutStarted = false;
  s_penaltyShootOutEnd = false;

  SetView( VIEW_COMMENTARY );
  //SetView( VIEW_EVENTS );
  //SetView( VIEW_SKILL );

  // show pre-match
  m_prematchMessageText->Show();
  m_prematchMessageBar->Show();

  // enable buttons
  m_playButton->Enable();
  m_pauseButton->Enable();
  m_skipButton->Enable();
  m_skipButton->Show();
  m_pauseButton->Hide();
  m_playButton->Show();

  // Don't show any menu item as selected
  m_menu->SetSelectedIndex( -1 );

  // enable match speed buttons
  m_menu->SetButtonEnabled( 4, true );
  m_menu->SetButtonEnabled( 5, true );

  Update();
}

void   FDMatchDayScreen::DoCommenceHalf()
{
  if( m_matchPlay->IsMatchInProgress() == true )
	{
    m_timer = 0.8f - (GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed * 0.08f + 0.08f);
    m_matchPlay->DoCommenceHalf();
		m_iNonUserPauseCount--;
		EXT_ASSERT(m_iNonUserPauseCount >= 0);
	}
}

void   FDMatchDayScreen::DoEndOfHalf()
{
  lLogFmt("End of half\n");
  m_bEndOfHalf = true;
  m_playButton->Show();
  m_pauseButton->Hide();
  m_iNonUserPauseCount++;
}

void FDMatchDayScreen::OnPlayButton( lUIControlWidget * )
{
  if( !m_playButton->IsEnabled() )
  {
    return;
  }

  m_prematchMessageText->Hide();
  m_prematchMessageBar->Hide();

  lLogFmt(" OnPlay: Pause count %d\n", m_iNonUserPauseCount );

  if( m_bUserPause )
  {
    lLogFmt("Resuming after user pause\n");
    m_iNonUserPauseCount--;
    EXT_ASSERT(m_iNonUserPauseCount >= 0);

    CClub& CurrentClub = m_matchPlay->GetCurrentUserControlledClub();

    for( uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++ )
    {
      CPlayer* pPlayer = &CurrentClub.GetPlayer(LoopCount);
      if( pPlayer->IsInjured() )
      {
        m_SubstitutionRequested = true;
        break;
      }
    }

    if( !m_SubstitutionRequested )
    {
      if( GetCurrentView() == VIEW_SUBS )
        SetView( VIEW_COMMENTARY );
    }

  } else
  {
    lLogFmt("Commencing half\n");
    DoCommenceHalf();
  }

  m_playButton->Hide();
  m_pauseButton->Show();

  m_bUserPause = false;
}

void FDMatchDayScreen::OnPauseButton( lUIControlWidget * )
{
  if( !m_pauseButton->IsEnabled() )
  {
    return;
  }

  lLogFmt("Pause button\n");
  if( !m_bUserPause )
  {
    lLogFmt(" - pausing %d\n", m_iNonUserPauseCount);
    m_bUserPause = true;
    m_iNonUserPauseCount++;
    m_playButton->Show();
    m_pauseButton->Hide();
    lLogFmt(" set pausing %d\n", m_iNonUserPauseCount);
  }
}

void FDMatchDayScreen::OnSkipButton( lUIControlWidget * )
{
  if( !m_skipButton->IsEnabled() )
  {
    return;
  }

  m_prematchMessageText->Hide();
  m_prematchMessageBar->Hide();

  lLogFmt("Skip button\n");
  m_currentView = VIEW_COMMENTARY;
  m_bUserPause = false;
  m_bSkippingMatch = true;
  m_iNonUserPauseCount = 0;
  m_matchPlay->SkipToEndOfMatch();
  m_bWaitForMarquee = false;

  SetMenuForFullTime( true );

  m_skipButton->Disable();

  //GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
  //FDSound::ChangeCrowd( CROWD_STATE_NONE );
}

void FDMatchDayScreen::OnMarqueeDone(FDMarqueeWidget *pMarquee)
{
	m_bWaitForMarquee = false;
}

void FDMatchDayScreen::OnTimer()
{
  DoUpdateCommentary();

  if( m_matchPlay->GetMatchActionStage() != MATCHNOTSTARTED )
  {
    m_prematchMessageText->Hide();
    m_prematchMessageBar->Hide();
  }

  if( m_matchPlay->IsUserSubstitutionRequired() == true )
  {
    lLogFmt("Sub required\n");
    lLogFmt(" Pause count %d\n", m_iNonUserPauseCount );

    //CString str;
    //str.Format(IDS_PLAYERINJURED, m_matchPlay->GetPlayerName());
    //DoAddCommentary( str, GREEN );

    if (m_bSkippingMatch == false)
    {
      CClub &cInjuredPlayerClub = m_matchPlay->GetClubCurrentPosession();
      const eWhichTeam eTeamType = (&m_matchPlay->GetClub(HOME) == &cInjuredPlayerClub) ? HOME : AWAY;
      m_marquee->StartInjuryMessage(&cInjuredPlayerClub, &m_matchPlay->GetPlayer(eTeamType, m_matchPlay->GetMatchVars().GetLastInjuredPlayerSquadNumber()));
      m_commentaryScreen->SkipToEnd();
      m_bWaitForMarquee = true;
    }

    m_matchPlay->SetUserSubstitutionRequired(false);

    m_SubstitutionRequested = true;
    m_timer = 1.8f;
    return;
  }

  if( m_SubstitutionRequested == true )
  {
    //OnDoSubstitutions();
    lLogFmt("Doing lineups view - %d\n", m_iNonUserPauseCount);
    lLogFmt(" Pause count %d\n", m_iNonUserPauseCount );
    SetView( VIEW_SUBS );

    m_bUserPause = true;
    m_playButton->Show();
    m_pauseButton->Hide();

    m_SubstitutionRequested = false;
    lLogFmt(" Pause count now %d\n", m_iNonUserPauseCount );
  }

  // Pause match if on a user action page
  if( m_iNonUserPauseCount == 0 )
  {
    lLogFmt("Processing minute\n");

    if( m_matchPlay->GetMatchActionStage() != MATCHENDED &&
        m_matchPlay->GetMatchActionStage() != MATCHREQUIREPENALTYSHOOTOUT )
    {
      m_matchPlay->DoMinute();
    }

    switch (m_matchPlay->GetMatchActionStage())
    {
      case MATCHHALFTIME:
        DoEndOfHalf();
        // display play button
        // change title
        GetMainUI()->SetTitleText( "en_half_time" );

        if (m_bSkippingMatch == false)
        {
          m_marquee->StartCustomMessage( "Half Time" );
          m_bWaitForMarquee = true;
        }
        break;

      case MATCHENDOFNORMALTIME:
        DoEndOfHalf();
        // display play button
        break;

      case MATCHNOTSTARTED:
      case MATCHINPROGRESS:
        GetMainUI()->SetTitleText( "en_match" );
        m_timer = 0.8f - (GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed * 0.1f + 0.08f);
        m_bEndOfHalf = false;
        break;

      case MATCHEXTRATIMEHALFTIME:
        DoEndOfHalf();
        // display play button
        // change title
        GetMainUI()->SetTitleText( "en_half_time" );

        if (m_bSkippingMatch == false)
        {
          m_marquee->StartCustomMessage( "Half Time (ET)" );
          m_bWaitForMarquee = true;
        }
        break;


      case MATCHENDOFEXTRATIME:
        DoEndOfHalf();
        break;

      case MATCHREQUIREPENALTYSHOOTOUT:
        if( !m_penaltyShootOutStarted )
        {
          if (m_bSkippingMatch == false)
          {
            if( !m_bWaitForMarquee )
            {
              DoUpdateCommentary();
              DoAddCommentary( "The Penalty Shootout has started", WHITE );
              m_marquee->StartCustomMessage( "Penalty Shootout!" ); //TODO LOCALE
              m_bWaitForMarquee = true;

              // NOTE: SIMON: hide skip button during penalty shootout as skiping during causes an issue where it loops the final score
              m_skipButton->Disable();
              m_skipButton->Hide();
            }
          }
          m_penaltyShootOutStarted = true;
        }
        else if( !m_bWaitForMarquee )
        {
          DoPenaltyShootOut();
        }
        break;

      case MATCHENDED:
        DoEndOfHalf();

        // change title
        GetMainUI()->SetTitleText( "en_full_time" );

        // set menu for full time
        SetMenuForFullTime( true );

        // We actually want to show this message, even if the player is skipping to the end of the match.
        m_marquee->StartCustomMessage( "Full Time" ); // TODO LOCALE
        m_bWaitForMarquee = true;

        m_matchDoneButton->Show();

        // disable buttons
        m_playButton->Disable();
        m_pauseButton->Disable();
        m_skipButton->Disable();
        m_playButton->Hide();
        m_pauseButton->Hide();
        m_skipButton->Hide();

        // disable speed menu buttons
        m_menu->SetButtonEnabled( 4, false );
        m_menu->SetButtonEnabled( 5, false );
        break;

      default:
        EXT_ASSERT(false);
        break;
    }
  }
  else
  {
    // Match is paused so just keep feeding the timer until it restarts
    //lLogFmt("Match paused\n");
    m_timer = 0.1f;
  }

  // update other screens
  m_skillScreen->Update( m_matchPlay->GetFixture() );
  m_eventsScreen->Update( m_matchPlay->GetFixture() );
  m_teamScreen->Update( m_matchPlay );
  m_subsScreen->Update( m_matchPlay );
  m_resultsScreen->Update( m_matchPlay );
  m_leagueScreen->Update( m_matchPlay );

  //
  if( m_matchPlay->GetMatchActionStage() == MATCHREQUIREPENALTYSHOOTOUT || m_currentShootOutHome > 0 )
    RefreshPenaltyInfo();
  else
    Update();
}


void FDMatchDayScreen::Update()
{
  char tmp[32];
  sprintf( tmp, "speed%d", GetMainUI()->GetSaveData().m_userPrefs.m_matchSpeed);
  m_dateBar->SetSprite( FDResourceManager::GetSprite( "uiicon", tmp ) );

  if( m_matchPlay == NULL )
  {
    return;
  }

  if( GetMainUI()->GetSaveData().m_userPrefs.m_bInMatchSFX )
  {
    if( m_matchPlay->GetMatchActionStage() == MATCHNOTSTARTED )
    {
      FDSound::ChangeCrowd( CROWD_STATE_PREMATCH );
    } else
    {
      FDSound::ChangeCrowd( CROWD_STATE_NEUTRAL );
    }
  }

//  int colA = L_ARGB( 255, 29, 9, 208 );
//  int colB = L_ARGB( 255, 211, 108, 5 );

	CString strPrint;

  // team name / score
  m_homeName->SetText( lUIText( m_matchPlay->GetFixture().GetClub(HOME).GetName() ), true );
  m_homeName->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  m_awayName->SetText( lUIText( m_matchPlay->GetFixture().GetClub(AWAY).GetName() ), true );
  m_awayName->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  strPrint.Format("%d", m_matchPlay->GetFixture().GetNumberGoals(HOME));
  m_homeScore->SetText( lUIText( strPrint ) );
  m_homeScore->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  strPrint.Format("%d", m_matchPlay->GetFixture().GetNumberGoals(AWAY));
  m_awayScore->SetText( lUIText( strPrint ) );
  m_awayScore->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

  // team kits
  m_homeKit->GetSprite().Get()->SetFrame( m_matchPlay->GetFixture().GetClub(HOME).GetHomeKitID() );
  m_awayKit->GetSprite().Get()->SetFrame( m_matchPlay->GetFixture().GetClub(AWAY).GetHomeKitID() );

  // half time score
  /*if( !m_bEndOfHalf )
  {
    strPrint.Format("(0 - 0)");
  }
  else
  {
    strPrint.Format("(%d - %d)", m_matchPlay->GetFixture().GetEventList()->HalfTimeHomeGoals(), m_matchPlay->GetFixture().GetEventList()->HalfTimeAwayGoals());
  }
  m_halftimeScore->SetText( lUIText( strPrint ) );*/


  // time
  CString strTime;
  strTime.Format("%02d", m_matchPlay->GetTimePlayed());

  switch( m_matchPlay->GetMatchActionStage() )
  {
    case MATCHENDED:
    case MATCHEXTRATIMEHALFTIME:
    case MATCHHALFTIME:
    case MATCHENDOFNORMALTIME:
      // grey out when stopped
      m_subDateTime->SetTextColour( L_ARGB(255,190,190,190) );
      break;

    default:
      m_subDateTime->SetTextColour( L_ARGB(255,255,255,255) );
      break;
  }

  if (m_bUserPause)
  {
    // grey out when stopped
    m_subDateTime->SetTextColour( L_ARGB(255,200,200,200) );
  }

  m_subDateTime->SetText( lUIText( strTime ) );

  // update top screen timeline for goals and sending offs
  m_matchPlay->GetFixture().GetEventList()->SortByTime();
  int numEvents = m_matchPlay->GetFixture().GetEventList()->DoCountInGameEvents();

  int rowidxA = 0, rowidxB = 0;

  int prevScrollPosition = 0;
  bool bDirection = false;

  m_matchtable->GetScrollPosition(prevScrollPosition, bDirection);

  m_matchtable->ClearAll();
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );	// gap
  m_matchtable->AddColumn( lUICoord( 15 ), LUI_CENTER_CENTER );	// Red card
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );	// gap
  m_matchtable->AddColumn( lUICoord( 99 ), LUI_LEFT_CENTER );
  m_matchtable->AddColumn( lUICoord( 5), LUI_CENTER_CENTER ); // gap
  m_matchtable->AddColumn( lUICoord( 99 ), LUI_RIGHT_CENTER );
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER ); // gap
  m_matchtable->AddColumn( lUICoord( 15 ), LUI_CENTER_CENTER );	// Red card
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );	// gap

  // ***DEBUG: Use this to test bug #639
  /*
  m_matchtable->AddRows(10);
  for (int i = 0; i < 10; i++)
  {
    CString sLabel;
	sLabel.Format("Testing %d", i);
	m_matchtable->SetText(i, 3, lUIText(sLabel.CStr()));
	m_matchtable->SetText(i, 5, lUIText(sLabel.CStr()));
  }
  */

  CString strEvent;

  for (int LoopCount = 0; LoopCount < m_matchPlay->GetFixture().GetEventList()->GetSize(); LoopCount ++)
  {
    CMatchEvent* pEvent = static_cast<CMatchEvent*>(m_matchPlay->GetFixture().GetEventList()->GetAt(LoopCount));

    if (pEvent->IsPenaltyShootOutEvent())
    {
        //lLogFmt( "Penalty shoot out %s (%d)\n", pEvent->GetText(), pEvent->GetTime());
      continue;
    }

    CClub *pClub = &(pEvent->IsHomeTeamEvent() ? m_matchPlay->GetClub(HOME) : m_matchPlay->GetClub(AWAY));

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

				if ((LoopCount > m_iLastMarqueeEvent) && (m_bSkippingMatch == false))
				{
					m_marquee->StartGoalMessage(pClub);
					m_commentaryScreen->SkipToEnd();
					m_bWaitForMarquee = true;
					m_iLastMarqueeEvent = LoopCount;
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

				if ((LoopCount > m_iLastMarqueeEvent) && (m_bSkippingMatch == false))
				{
					m_marquee->StartOwnGoalMessage(pClub);
					m_commentaryScreen->SkipToEnd();
					m_bWaitForMarquee = true;
					m_iLastMarqueeEvent = LoopCount;
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

				if ((LoopCount > m_iLastMarqueeEvent) && (m_bSkippingMatch == false))
				{
          if( bRed )
            m_marquee->StartRedCardMessage(pClub, pEvent->GetName().GetName());
          else
            m_marquee->StartYellowCardMessage(pClub, pEvent->GetName().GetName());
					m_commentaryScreen->SkipToEnd();
					m_bWaitForMarquee = true;
					m_iLastMarqueeEvent = LoopCount;
				}

				break;
			}

			default:
				break;
		}
	}


	// Always have 7 rows
	if( m_matchtable->GetRowCount() < 7 )
	{
	  m_matchtable->AddRows(7 - m_matchtable->GetRowCount() );
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

	m_matchtable->SetScrollPosition(prevScrollPosition, bDirection);

  const bool bLiveMenuItemsAvailable = m_matchPlay->AreLatestScoresAvailable();
  m_menu->SetButtonEnabled(2, bLiveMenuItemsAvailable);
  m_menu->SetButtonEnabled(3, bLiveMenuItemsAvailable);
}


void          FDMatchDayScreen::DoAddCommentary( const CString &strComment, COLORREF colorText )
{
	CString strPrint;
	if( m_matchPlay->GetMatchActionStage() == MATCHREQUIREPENALTYSHOOTOUT )
	{
	  int time = m_matchPlay->GetTimePlayed() + m_currentShootOutHome + m_currentShootOutAway;
	  strPrint.Format("(%d) %s", time, strComment);
	}
	else
	{
	  strPrint.Format("(%d) %s", m_matchPlay->GetTimePlayed(), strComment);
	}

  if( m_matchPlay->GetMatchActionInfo().GetInformationStr().IsEmpty() != TRUE )
	{
    m_commentaryScreen->AddString( m_matchPlay->GetMatchActionInfo().GetInformationStr(), WHITE );
    m_matchPlay->GetMatchActionInfo().DoClearInformationStr();
	}
	else
	{
	  if( m_matchPlay->GetMatchActionStage() == MATCHREQUIREPENALTYSHOOTOUT  )
	  {
	    if( m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == HOMEPENALTYGOAL ||
	        m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == HOMEPENALTYSAVED ||
	        m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == HOMEPENALTYMISS )
	    {
	      colorText = GetKitColour( m_matchPlay->GetFixture().GetClub(HOME).GetHomeKit().GetPrimaryColour()-1 );
      } else
      if (m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == AWAYPENALTYGOAL ||
	        m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == AWAYPENALTYSAVED ||
	        m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == AWAYPENALTYMISS )
      {
        colorText = GetAwayColour( GetKitColour( m_matchPlay->GetFixture().GetClub(HOME).GetHomeKit().GetPrimaryColour()-1 ) );
      }
    }
    else if( m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == NOEVENT )
	  {
      colorText = WHITE;
	  }
	  else if( m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == HOMEOWNGOAL )
	  {
	    colorText = GetAwayColour( GetKitColour( m_matchPlay->GetFixture().GetClub(HOME).GetHomeKit().GetPrimaryColour()-1 ) );
	  }
	  else if( m_matchPlay->GetMatchActionInfo().GetCurrentEvent() == AWAYOWNGOAL )
	  {
	    colorText = GetKitColour( m_matchPlay->GetFixture().GetClub(HOME).GetHomeKit().GetPrimaryColour()-1 );
	  }
	  else if( (m_matchPlay->GetMatchActionInfo().GetCurrentEvent() % 2) == 1 )
	  {
	    colorText = GetKitColour( m_matchPlay->GetFixture().GetClub(HOME).GetHomeKit().GetPrimaryColour()-1 );
	  }
    else if( (m_matchPlay->GetMatchActionInfo().GetCurrentEvent() % 2) == 0)
    {
      //colorText = GetKitColour( m_matchPlay->GetFixture().GetClub(AWAY).GetHomeKit().GetPrimaryColour()-1 );
      colorText = GetAwayColour( GetKitColour( m_matchPlay->GetFixture().GetClub(HOME).GetHomeKit().GetPrimaryColour()-1 ) );
    }

    m_commentaryScreen->AddString( strPrint, colorText );
	}

  if( m_matchPlay->GetMatchActionInfo().GetCommentaryStr().IsEmpty() != TRUE &&
      m_strLastComment != m_matchPlay->GetMatchActionInfo().GetCommentaryStr() )
	{
    m_strLastComment = m_matchPlay->GetMatchActionInfo().GetCommentaryStr();
    DoAddCommentary( m_strLastComment, m_matchPlay->GetMatchActionInfo().GetCommentaryTextColor() );
	}
}

void FDMatchDayScreen::RefreshPenaltyInfo()
{
  int prevScrollPosition = 0;
  bool bDirection = false;

  m_matchtable->GetScrollPosition(prevScrollPosition, bDirection);

  m_matchtable->ClearAll();
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );	// gap
  m_matchtable->AddColumn( lUICoord( 15 ), LUI_CENTER_CENTER );	// Red card
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );	// gap
  m_matchtable->AddColumn( lUICoord( 99 ), LUI_CENTER_CENTER );
  m_matchtable->AddColumn( lUICoord( 5), LUI_CENTER_CENTER ); // gap
  m_matchtable->AddColumn( lUICoord( 99 ), LUI_CENTER_CENTER );
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER ); // gap
  m_matchtable->AddColumn( lUICoord( 15 ), LUI_CENTER_CENTER );	// Red card
  m_matchtable->AddColumn( lUICoord( 6 ), LUI_CENTER_CENTER );	// gap

  CPenaltyShootOut * penalty = m_matchPlay->GetPenaltyShootOut();

  bool bHome = true;
  int homeWins = 0, awayWins = 0;

  CClub * pHomeClub = &m_matchPlay->GetClub( HOME );
  CClub * pAwayClub = &m_matchPlay->GetClub( AWAY );


  m_matchtable->AddRows( 6 );

  for (int LoopCount = 0; LoopCount < m_currentShootOutHome; LoopCount ++)
  {
    int penaltyResult = -1;

    penaltyResult = penalty->GetHomePenaltyResult(LoopCount);

    CString str = WorldData().GetPlayer(pHomeClub->GetFirstTeamSquad().GetSquad(penalty->GetHomeTakerIndex(LoopCount)))->GetName();
    m_matchtable->SetText( LoopCount + 1, 3, lUIText( str.CStr() ) );
    m_matchtable->SetTextColour( LoopCount + 1, 3, L_ARGB(255,255,255,255) );
    uint flag = -1;
    switch(penaltyResult)
    {
      case 0: // Scored
        flag =  TAG_FLAG_PENSO_HOME_HIT;
        homeWins++;
        break;
      case 1: // Saved
      case 2: // Missed
        flag = TAG_FLAG_PENSO_HOME_MISS;
        break;
      default :
        EXT_ASSERT(false);
    }
    m_matchtable->SetTagDataForRow(LoopCount + 1, m_matchtable->GetTagDataForRow(LoopCount + 1) | flag);
  }

  for (int LoopCount = 0; LoopCount < m_currentShootOutAway; LoopCount ++)
  {
    int penaltyResult = -1;

    penaltyResult = penalty->GetAwayPenaltyResult(LoopCount);

    CString str = WorldData().GetPlayer(pAwayClub->GetFirstTeamSquad().GetSquad(penalty->GetAwayTakerIndex(LoopCount)))->GetName();
    m_matchtable->SetText( LoopCount + 1, 5, lUIText( str.CStr() ) );
    m_matchtable->SetTextColour( LoopCount + 1, 5, L_ARGB(255,255,255,255) );
    uint flag = -1;
    switch(penaltyResult)
    {
      case 0: // Scored
        flag =  TAG_FLAG_PENSO_AWAY_HIT;
        awayWins++;
        break;
      case 1: // Saved
      case 2: // Missed
        flag = TAG_FLAG_PENSO_AWAY_MISS;
        break;
      default :
        EXT_ASSERT(false);
    }
    m_matchtable->SetTagDataForRow(LoopCount + 1, m_matchtable->GetTagDataForRow(LoopCount + 1) | flag);
  }

  m_matchtable->SetTextNumber(0, 3, homeWins );
  m_matchtable->SetText(0, 4, lUIText("Penalty Score") );
  m_matchtable->SetTextToUpper(0, 4);
  m_matchtable->SetTextNumber(0, 5, awayWins );

	// Always have 7 rows
	if( m_matchtable->GetRowCount() < 7 )
	{
	  m_matchtable->AddRows(7 - m_matchtable->GetRowCount() );
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

	m_matchtable->SetBkgColour( 0, 4, L_ARGB( 127, 80, 80, 80 ) );

	m_matchtable->SetScrollPosition(prevScrollPosition, bDirection);
}

void          FDMatchDayScreen::DoPenaltyShootOut()
{
  lLogFmt("Penalty shootout\n");

  // Perform each shoot out commentary then end
  int penaltyResult = -1;

  CPenaltyShootOut * penalty = m_matchPlay->GetPenaltyShootOut();

  CClub * pClub = NULL;
  if( m_bPenaltyHome )
  {
    pClub = &m_matchPlay->GetClub( HOME );
  } else
  {
    pClub = &m_matchPlay->GetClub( AWAY );
  }

  if( m_bPenaltyHome && m_currentShootOutHome != 5 )
  {
    penalty->HomePenaltyCommentary( *m_matchPlay, m_currentShootOutHome, *pClub );
    penaltyResult = penalty->GetHomePenaltyResult(m_currentShootOutHome);
    m_currentShootOutHome++;
    m_bPenaltyHome = false;
  }
  else if( !m_bPenaltyHome && m_currentShootOutAway != 5 )
  {
    penalty->AwayPenaltyCommentary( *m_matchPlay, m_currentShootOutAway, *pClub );
    penaltyResult = penalty->GetAwayPenaltyResult(m_currentShootOutAway);
    m_currentShootOutAway++;
    m_bPenaltyHome = true;
  }


  if( m_currentShootOutAway != 5 && m_currentShootOutHome != 5)
  {
    if (m_bSkippingMatch == false)
    {
      switch(penaltyResult)
      {
        case 0: // Scored
          m_marquee->StartPenaltyGoalMessage( pClub );
          m_commentaryScreen->SkipToEnd();
          break;
        case 1: // Saved
          m_marquee->StartPenaltySaveMessage( pClub  );
          break;
        case 2: // Missed
          m_marquee->StartPenaltyMissMessage( pClub  );
          break;
        default :
          EXT_ASSERT(false);
      }
      m_bWaitForMarquee = true;
      m_timer = 5.0f;
    }
  }

  // Count up number of goals
  int awayResult = 0, homeResult = 0;
  for( int i = 0; i < m_currentShootOutHome; i++ )
  {
    if( penalty->GetHomePenaltyResult(i) == 0 )
      homeResult++;
  }
  for( int i = 0; i < m_currentShootOutAway; i++ )
  {
    if( penalty->GetAwayPenaltyResult(i) == 0 )
      awayResult++;
  }

  if( homeResult > awayResult )
  {
    int diff = homeResult - awayResult;
    if( (diff  == 3 && m_currentShootOutAway > 2) ||
        (diff > 1 && m_currentShootOutAway > 4 ) )
      s_penaltyShootOutEnd = true;
  }

  if( homeResult < awayResult )
  {
    int diff = awayResult - homeResult;
    if( (diff  == 3 && m_currentShootOutAway > 2) ||
        (diff > 1 && m_currentShootOutAway > 4 ) )
      s_penaltyShootOutEnd = true;
  }


  if( (m_currentShootOutAway == 5 && m_currentShootOutHome == 5) || s_penaltyShootOutEnd )
  {
    s_penaltyShootOutEnd = true;
    DoUpdateCommentary();

    uint winningColour;
    pClub = &m_matchPlay->GetClub( HOME );
    if( homeResult > awayResult )
    {
      winningColour = GetKitColour( pClub->GetHomeKitID() );
    }
    else
    {
      winningColour = GetAwayColour( GetKitColour( pClub->GetHomeKitID() ) );
    }

    DoAddCommentary( m_matchPlay->PenaltyShootoutWinnerStr(), winningColour );
    DoUpdateCommentary();


    lLogFmt("** END OF MATCH / PENALTY\n");
    if (m_bSkippingMatch == false)
    {
      m_matchPlay->DoMatchOver();
    }
  }
}

void          FDMatchDayScreen::DoUpdateCommentary()
{
  if( m_matchPlay->GetMatchActionInfo().GetInformationStr().IsEmpty() != TRUE )
	{
    m_commentaryScreen->AddString( m_matchPlay->GetMatchActionInfo().GetInformationStr(), WHITE );
    m_matchPlay->GetMatchActionInfo().DoClearInformationStr();
	}

  if( m_matchPlay->GetMatchActionInfo().GetCommentaryStr().IsEmpty() != TRUE &&
      m_strLastComment != m_matchPlay->GetMatchActionInfo().GetCommentaryStr() )
	{
    m_strLastComment = m_matchPlay->GetMatchActionInfo().GetCommentaryStr();
    DoAddCommentary( m_strLastComment, m_matchPlay->GetMatchActionInfo().GetCommentaryTextColor() );
	}
}

void  FDMatchDayScreen::SetView( MatchDayViewType eNewView )
{
  m_skillScreen->Hide();
  m_eventsScreen->Hide();
  m_teamScreen->Hide();
  m_subsScreen->Hide();
  m_resultsScreen->Hide();
  m_leagueScreen->Hide();
  m_commentaryScreen->Hide();

  switch (m_currentView)
  {
  case VIEW_SUBS:
	  m_marquee->SetPause(false);
	  break;
  case VIEW_SKILL:
  case VIEW_LINEUPS:
  case VIEW_EVENTS:
  case VIEW_LATESTSCORES:
  case VIEW_LEAGUETABLE:
  default:
	  // Do nothing.
	  break;
  }

  switch( eNewView )
  {
    case VIEW_COMMENTARY:
      m_commentaryScreen->Show();
      break;
    case VIEW_SKILL:
      m_skillScreen->Show();
      m_skillScreen->Update(m_matchPlay->GetFixture());
      break;
    case VIEW_LINEUPS:
      m_teamScreen->Show();
      m_teamScreen->Update(m_matchPlay);
      break;
    case VIEW_EVENTS:
      m_eventsScreen->Show();
      m_eventsScreen->Update(m_matchPlay->GetFixture());
      break;
    case VIEW_SUBS:
      m_subsScreen->Show();
      m_subsScreen->Update(m_matchPlay);

      // Don't want to pause if the match has ended
      if( m_matchPlay->GetMatchActionStage() != MATCHENDED )
      {
        if (m_bUserPause == false)
        {
          // Due to the start of the game, it is not user paused but still it has a
          // m_iNonUserPauseCount set to 1, which increments to 2 after OnPausebutton
          if( m_iNonUserPauseCount > 0 )
            m_iNonUserPauseCount = 0;

          OnPauseButton(m_pauseButton);
        }
        m_marquee->SetPause(true);
      }
      break;
    //TODO
    //case VIEW_RADAR,
    case VIEW_LATESTSCORES:
      m_resultsScreen->Show();
      break;
    case VIEW_LEAGUETABLE:
      m_leagueScreen->Show();
      break;
    default:
      break;
  }

  m_currentView = eNewView;
}

FDMatchDayScreen::MatchDayViewType FDMatchDayScreen::GetCurrentView() const
{
  return m_currentView;
}

void FDMatchDayScreen::TableCellRenderCallback(FDTableWidget *pTable, void *pMatchDayScreen, luint32_t uiRow, luint32_t uiColumn, lUIRect cCellRect)
{
	if ((uiColumn != 1) && (uiColumn != 7))
		return;

	FDMatchDayScreen *pScreen = (FDMatchDayScreen *)pMatchDayScreen;

  luint32_t uiFlag = pTable->GetTagDataForRow(uiRow);

  if (((uiColumn == 1) && (uiFlag & TAG_FLAG_REDCARD_HOME)) ||
      ((uiColumn == 7) && (uiFlag & TAG_FLAG_REDCARD_AWAY)))
	{
		pScreen->m_redCardSprite->SetX(cCellRect.x);
		pScreen->m_redCardSprite->SetY(cCellRect.y);
//    pScreen->m_redCardSprite->SetWidth(cCellRect.w);
//    pScreen->m_redCardSprite->SetHeight(cCellRect.h);
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
//    pScreen->m_goalSprite->SetWidth(cCellRect.w);
//    pScreen->m_goalSprite->SetHeight(cCellRect.h);
    pScreen->m_goalSprite->Render();
	}

	// Penalty shoot out lights/blobs
  if (((uiColumn == 1) && (uiFlag & TAG_FLAG_PENSO_HOME_HIT)) ||
      ((uiColumn == 7) && (uiFlag & TAG_FLAG_PENSO_AWAY_HIT)))
	{
    pScreen->m_penHitSprite->SetX(cCellRect.x);
    pScreen->m_penHitSprite->SetY(cCellRect.y);
    pScreen->m_penHitSprite->Render();
	}

  if (((uiColumn == 1) && (uiFlag & TAG_FLAG_PENSO_HOME_MISS)) ||
      ((uiColumn == 7) && (uiFlag & TAG_FLAG_PENSO_AWAY_MISS)))
	{
    pScreen->m_penMissSprite->SetX(cCellRect.x);
    pScreen->m_penMissSprite->SetY(cCellRect.y);
    pScreen->m_penMissSprite->Render();
	}
}

//////////////////////////////////////////////////////////////////////////
// Callback function that gets called whenever a substitution is made. This is
// used to trigger the substitution marquee.
//////////////////////////////////////////////////////////////////////////
void FDMatchDayScreen::SubstitutionMadeCallback(void *pMatchDayScreen, const eWhichTeam _Who, ushort _Player1SquadNumber, ushort _Player2SquadNumber)
{
	FDMatchDayScreen *pScreen = (FDMatchDayScreen *)pMatchDayScreen;

	if (pScreen->m_bSkippingMatch == true)
		return;

	CClub *pClub = &pScreen->m_matchPlay->GetClub(_Who);
	CPlayer *pPlayer1 = &pClub->GetFirstTeamSquad().GetPlayer(_Player1SquadNumber);
	CPlayer *pPlayer2 = &pClub->GetFirstTeamSquad().GetPlayer(_Player2SquadNumber);
	pScreen->m_marquee->StartSubstitutionMessage(pClub, pPlayer1, pPlayer2);
	pScreen->m_commentaryScreen->SkipToEnd();
	pScreen->m_bWaitForMarquee = true;
}

//////////////////////////////////////////////////////////////////////////
// Callback used when skipping to the end of a match, to capture all match commentary.
//////////////////////////////////////////////////////////////////////////
void FDMatchDayScreen::SkipMatchCommentaryCallback(void *pMatchDayScreen)
{
	FDMatchDayScreen *pScreen = (FDMatchDayScreen *)pMatchDayScreen;
	pScreen->DoUpdateCommentary();
}

void FDMatchDayScreen::AutoplaySkip()
{
	OnSkipButton(m_skipButton);
	DoUpdateCommentary();
	OnDoneButton(m_playButton);
}

//////////////////////////////////////////////////////////////////////////
// Callback used when skipping to the end of a match, to capture all penaltyshootout commentary.
//////////////////////////////////////////////////////////////////////////
void FDMatchDayScreen::SkipMatchPenaltyShootOutCallback(void *pMatchDayScreen)
{
	FDMatchDayScreen *pScreen = (FDMatchDayScreen *)pMatchDayScreen;

  pScreen->DoAddCommentary( "The Penalty Shootout has started", WHITE ); //TODO LOCALE
  pScreen->DoUpdateCommentary();
	while( s_penaltyShootOutEnd == false )
	{
	  pScreen->DoPenaltyShootOut();
	  pScreen->DoUpdateCommentary();
	}
}
