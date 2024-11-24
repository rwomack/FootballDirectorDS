//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : fdmainmenuui.cpp
// Description : FDMainMenuUI implementation
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifdef L_PLATFORM_NDS
//NOTE: Increase optimiser memory usage limit
#pragma opt_defuse_mem_limit 180
#endif

//#define AUTOLOAD

//#define SCREENSWITCH_DELAY

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
//#include <fdscript.h>
#include <fdgameworld.h>
#include <fdtext.h>
#include <fdcommandlineoptions.h>

#include <fdscreen.h>
#include <screens/fdsquadscreen.h>
#include <screens/fdaccountsscreen.h>
#include <screens/fdclubfixturesscreen.h>
#include <screens/fdcupfixturesscreen.h>
#include <screens/fdcupschedulescreen.h>
#include <screens/fdtacticsscreen.h>
#include <screens/fdtrainingscreen.h>
#include <screens/fdmatchdayscreen.h>
#include <screens/fdnewsscreen.h>
#include <screens/fdstatusscreen.h>
#include <screens/fdjobcentrescreen.h>
#include <screens/fdtransferlistscreen.h>
#include <screens/fdplayerinfoscreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdemployeeinfoscreen.h>
#include <screens/fdyouthinfoscreen.h>
#include <screens/fdmanagerinfoscreen.h>
#include <screens/fdyouthsquadscreen.h>
#include <screens/fdnewgamescreen.h>
#include <screens/fdselectteamscreen.h>
#include <screens/fdloadscreen.h>
#include <screens/fdsavescreen.h>
#include <screens/fdinfoscreen.h>
#include <screens/fdprogressscreen.h>
#include <screens/fdleaguefixturesscreen.h>
#include <screens/fdmatchintroscreen.h>
#include <screens/fdnegotiatescreen.h>
#include <screens/fdsackplayerscreen.h>
#include <screens/fdmatchsummaryscreen.h>
#include <screens/fdteamoftheweekscreen.h>
#include <screens/fdleaguetablescreen.h>
#include <screens/fdliveresultscreen.h>
#include <screens/fdliveleaguetablescreen.h>
#include <screens/fdtransferstatusscreen.h>
#include <screens/fdwagesscreen.h>
#include <screens/fdarrangefriendliesscreen.h>
#include <screens/fdoptions.h>
#include <screens/fdmonthlyawardscreen.h>
#include <screens/fdclubrecordsscreen.h>
#include <screens/fdleadingplayerscreen.h>
#include <screens/fdeditmodescreen.h>
#include <screens/fdeditnamescreen.h>
#include <screens/fdstrategyformationscreen.h>
#include <screens/fdstrategyrolesscreen.h>
#include <screens/fdclubinfosubscreen.h>

#include <playersearch.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdtextwidget.h>
#include <fdmenuwidget.h>
#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdbuttonrolloutwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>
#include <fdhighlightwidget.h>
#include <fdplayerblobwidget.h>
#include <fdfullscreenwidget.h>
#include <fdscrollbarwidget.h>

#include <fdeditmodedata.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#include <lemon/math/lmrand.h>

// INTEGRATION BEGIN

CExtString g_DebugStr;
CRandomNumber RandomNumber;
CFootballDirectorApp theApp;

extern bool DoMainLoop();

void ApplySaveDataChangesToWorldData(FDEditModeData &cData);

bool LoadText( CString &str, int id )
{
//  lLogFmt("LoadText %d\n", id);
  str = lText::Text( id );
  return true;
}

FDMainMenuUI * s_mainmenuUI = NULL;

CWorld *s_WorldData = NULL;
bool    s_bWorldDataUsed = false;
FDEditModeData s_EditModeData;

CWorld& WorldData()
{
  return *s_WorldData;
}

using namespace L_NAMESPACE_NAME;

FDEditModeData &EditModeData()
{
	return s_EditModeData;
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// FDMainMenuUI
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

FDMainMenuUI::FDMainMenuUI() : m_currentScreenIdx(SCREENCOMBO_INVALID), m_bBackShouldBeVisible(false),
								m_fHighlightTimer(0.0f), m_bGlow(false),
                m_pCurrentModalScreen(NULL), m_eSwitchingToScreen(SCREENCOMBO_INVALID), m_bGameOver(false)
{
  s_mainmenuUI = this;
}

FDMainMenuUI::~FDMainMenuUI()
{
}

void FDMainMenuUI::SetWorldDataUsed( bool b )      // set when world data has been active / loaded over
{
  s_bWorldDataUsed = b;
}

bool FDMainMenuUI::GetWorldDataUsed()      // set when world data has been active / loaded over
{
  return s_bWorldDataUsed;
}

void FDMainMenuUI::ApplyEditToWorldData()
{
  ApplySaveDataChangesToWorldData(m_EditModeData);
  s_EditModeData = m_EditModeData;
}

void FDMainMenuUI::ReinitWorldData(const bool bApplyEditModeDataFromOptions)
{
  if( s_WorldData != NULL )
  {
    if( !s_bWorldDataUsed )
    {
      // current data still valid
      lLogFmt("Using current data\n");

      if (bApplyEditModeDataFromOptions)
        ApplyEditToWorldData();

      return;
    }

    // Deinit
    lLogFmt("Deinit current data\n");
    DeinitWorldData();
  }

  // init new data
  lLogFmt("Init new data\n");
  InitWorldData();

  if (bApplyEditModeDataFromOptions)
  {
    lLogFmt("ApplyEditMode\n");
    ApplyEditToWorldData();
  }
}

void FDMainMenuUI::InitWorldData()
{
  lLogBreak();
  lPlatform_MemoryLeakDump();

  lLogFmt("Init world\n");
  s_WorldData = FD_GAME_NEW CWorld();

  lLogFmt("DoInit\n");
  s_WorldData->DoInitialise();

  s_bWorldDataUsed = false;

  lLogFmt("DoInit done\n");

  lPlatform_MemoryLeakDump();
  lLogBreak();
}

void FDMainMenuUI::DeinitWorldData()
{
  // integration work BEGIN
  lLogBreak();
  lPlatform_MemoryLeakDump();

  lLogFmt("Deinit world\n");
  delete s_WorldData;
  s_WorldData = NULL;

  lPlatform_MemoryLeakDump();
  lLogBreak();
  // integration work END
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// FDMainMenuUI functions
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void FDMainMenuUI::Init()
{
}

void FDMainMenuUI::Deinit()
{
}


void FDMainMenuUI::Activate( lUILayer * layer )
{
  m_bReadyToTick = false;
  m_layer = layer;

  lLogFmt("Start of MainMenuUI\n");
  lPlatform_MemoryLeakDump();

  s_WorldData = NULL;
  OptionsDefault();
  EditModeDefault();

  m_activeInputScreen = NULL;

  m_currentLayoutScreenIdx = SCREENCOMBO_INVALID;

  // mem dump
  lLogFmt("MainMenuUI: Post load\n");
  lPlatform_MemoryLeakDump();

  m_rootScreen    = FD_GAME_NEW lUIScreenWidget( NULL, 0, "root_screen" );
  m_rootScreenSub = FD_GAME_NEW lUIScreenWidget( NULL, 0, "root_screen_sub" );

  // *** Main ***

  m_mainScreen = FD_GAME_NEW lUIScreenWidget( m_rootScreen, 0, "main_screen" );
  m_mainScreenBg = FD_GAME_NEW lUISpriteWidget( m_mainScreen, 0, "bg" );

  m_splitter = FD_GAME_NEW lUISpriteWidget( m_mainScreen, 0, "splitter", FDResourceManager::GetSprite("splitter","splitter") );
  m_splitter->Hide();

  m_rootScreenBase = FD_GAME_NEW lUIScreenWidget( m_rootScreen, LUI_WIDGET_MOUSETRANSPARENT, "menu_screen" );

  // *** Main Sub ***

  m_mainScreenSub = FD_GAME_NEW lUIScreenWidget( m_rootScreenSub, 0, "main_screen_sub" );
  m_mainScreenSubBg = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "bg" );

  m_subScreenSpecial   = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "secpail" );  // behind title bar
  m_subScreenTitleBar  = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "titleBar" );
  m_subScreenTitleText = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "titleText" );

  m_subScreenCalendar       = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "cal", FDResourceManager::GetSprite( "uical", "calendar" ), LUI_TOP_LEFT,
    lUIPoint( 5, 0 ) );
  m_subScreenCalendarYear   = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "cal_year",  FDResourceManager::GetSprite( "uical", "year" ), LUI_TOP_CENTER,
    lUIPoint( 5 + 18, 30 ) );
  m_subScreenCalendarMonth  = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "cal_month",  FDResourceManager::GetSprite( "uical", "month" ), LUI_TOP_CENTER,
    lUIPoint( 5 + 18, 2 ) );
  m_subScreenCalendarDay    = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "cal_day",  FDResourceManager::GetSprite( "uical", "day" ), LUI_TOP_CENTER,
    lUIPoint( 5 + 18, 16 ) );

  m_subScreenCalendarDay->GetSprite()->SetTransparent(false);

  m_subScreenCalendar->Hide();
  m_subScreenCalendarYear->Hide();
  m_subScreenCalendarMonth->Hide();
  m_subScreenCalendarDay->Hide();

  // *** Main Options Menu ***

  m_mainmenuScreen = FD_GAME_NEW lUIScreenWidget( m_rootScreen, 0, "mainmenu_screen" );
  m_mainmenuScreen->Hide();

  lUISpriteWidget * menuBack = FD_GAME_NEW lUISpriteWidget( m_mainmenuScreen, 0, "menuback", FDResourceManager::GetSprite("ui","menu_overlay"), LUI_CENTER_CENTER, lUIPoint( 128, 192/2 ));

  m_contgameButton = FD_GAME_NEW FDButtonWidget( m_mainmenuScreen, 0, "Continue Game", FDResourceManager::GetSprite("ui","continue_game"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint( 128, 30+(35*0) ) );
  m_contgameButton->SetDisabledSprite( FDResourceManager::GetSprite("ui","continue_game_greyed_out") );
  m_contgameButton->SetSpriteOn( FDResourceManager::GetSprite("ui","continue_game-on") );

  FDButtonWidget * newgameButton = FD_GAME_NEW FDButtonWidget( m_mainmenuScreen, 0, "New Game", FDResourceManager::GetSprite("ui","create_new_game"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint( 128, 30+(35*1) ) );
  newgameButton->SetSpriteOn( FDResourceManager::GetSprite("ui","create_new_game-on") );
  FDButtonWidget * optionsButton = FD_GAME_NEW FDButtonWidget( m_mainmenuScreen, 0, "Options", FDResourceManager::GetSprite("ui","options"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint( 128, 30+(35*2) ) );
  optionsButton->SetSpriteOn( FDResourceManager::GetSprite("ui","options_on") );
  FDButtonWidget * editModeButton = FD_GAME_NEW FDButtonWidget(m_mainmenuScreen, 0, "Edit Mode", FDResourceManager::GetSprite("ui", "edit_mode"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint( 128, 30+(35*3) ));
  editModeButton->SetSpriteOn( FDResourceManager::GetSprite("ui","edit_mode_on") );

  // load options
  lLogFmt("Loading options\n");

  if( OptionsExist() )
  {
    lLogFmt("  Found - loading\n");
    if( OptionsLoad() )
    {
      lLogFmt("  Loaded options\n");
    } else
    {
      lLogFmt("  Error loading options\n");

      // save defaults
      lLogFmt("  Saving default\n");
      OptionsDefault();
      if( OptionsSave() )
      {
        lLogFmt("  Saved options (default)\n");
      } else
      {
        lLogFmt("  Error saving options\n");
      }
    }
  } else
  {
    lLogFmt("  No option data found\n");
  }

  OptionsApply();

  // load edit mode data
  if( EditModeExist() )
  {
    lLogFmt("  Found EditMode - loading\n");
    if( EditModeLoad() )
    {
      lLogFmt("  Loaded editmode\n");
    } else
    {
      lLogFmt("  Error loading editmode\n");

      // set defaults
      EditModeDefault();
    }
  } else
  {
    lLogFmt("  No editmode data found\n");
  }

  //

  newgameButton->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMainMenuNewButton );
  m_contgameButton->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMainMenuContinueButton );
  optionsButton->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMainMenuOptionsButton );
  editModeButton->SignalControlCommand.Connect(this, &FDMainMenuUI::OnMainMenuEditModeButton);

  m_mainmenuScreenSub = FD_GAME_NEW lUIScreenWidget( m_rootScreenSub, 0, "mainmenu_screen_sub" );
  m_mainmenuScreenSub->Hide();

  // Message box

  m_messageBoxBgSub = FD_GAME_NEW lUISpriteWidget( m_mainScreenSub, 0, "mbBgbar", FDResourceManager::GetSprite("message_box_small","message_box_small"), LUI_CENTER_CENTER, lUIPointPercent( 50, 60 ));
  m_messageBoxTextSub = FD_GAME_NEW lUITextWidget( m_mainScreenSub, 0, "mboxtext", "", LUI_CENTER_CENTER, lUIPointPercent( 50, 60 ), lUIPointPercent( 80, 40 ) );
  m_messageBoxTextSub->SetFont(FDResourceManager::GetFont("font16","font16"));
  m_messageBoxTextSub->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_messageBoxBgSub->Hide();
  m_messageBoxTextSub->Hide();

  m_messageBoxFade = FD_GAME_NEW FDFullScreenWidget( m_rootScreen, 0, "messageboxfade", L_ARGB( 50, 0, 0, 0 ) );
  m_messageBoxFade->Hide();

  m_messageBoxBg = FD_GAME_NEW lUISpriteWidget( m_messageBoxFade, 0, "mbBgbar", FDResourceManager::GetSprite("message_box_large","message_box_large"), LUI_CENTER_CENTER, lUIPoint( 127, 192/2 ));

  m_messageBoxText = FD_GAME_NEW FDTextWidget( m_messageBoxFade, 0, "mboxtext", "", LUI_CENTER_CENTER, lUIPointPercent( 50, 50 ), lUIPointPercent( 80, 40 ) );

  m_messageBoxOK = FD_GAME_NEW FDButtonWidget( m_messageBoxFade, 0, "mboxok", FDResourceManager::GetSprite("ui","button75"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 50, 85 ) );
  m_messageBoxOK->SetText( lUIText( IDS_DIALOG_OK ) );
  m_messageBoxOK->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMessageBoxButton );

  m_messageBoxYes = FD_GAME_NEW FDButtonWidget( m_messageBoxFade, 0, "mboxyes", FDResourceManager::GetSprite("ui","button75"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 30, 85 ) );
  m_messageBoxYes->SetText( lUIText( IDS_DIALOG_YES ) );
  m_messageBoxYes->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMessageBoxButton );

  m_messageBoxNo = FD_GAME_NEW FDButtonWidget( m_messageBoxFade, 0, "mboxno", FDResourceManager::GetSprite("ui","button75"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPointPercent( 70, 85 ) );
  m_messageBoxNo->SetText( lUIText( IDS_DIALOG_NO ) );
  m_messageBoxNo->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMessageBoxButton );

  // Keep a reference to the titles sprite bank around
  static lSharedPtr<lSpriteBank> s_titleBank = FDResourceManager::GetSpriteBank( "en_titles" );

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Init loading screen : NOTE: Unused currently
  //
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  m_loadScreen = FD_GAME_NEW FDLoadScreen( this );

  ShowProcessScreen( true, PROCESS_LOAD );
  ProcessingCallback( "LoadScreen" );

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Initialise game data
  //
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  InitWorldData();

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Initialise game screens
  //
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  m_newgameScreen            =  FD_GAME_NEW FDNewGameScreen( this );
  m_selectteamScreen         =  FD_GAME_NEW FDSelectTeamScreen( this );
  m_saveScreen               =  FD_GAME_NEW FDSaveScreen( this );
  m_progressScreen           =  FD_GAME_NEW FDProgressScreen( this );
  m_transferStatusScreen     =  FD_GAME_NEW FDTransferStatusScreen( this );
  m_trainingScreen           =  FD_GAME_NEW FDTrainingScreen( this );
  m_newsScreen               =  FD_GAME_NEW FDNewsScreen( this );
  m_squadScreen              =  FD_GAME_NEW FDSquadScreen( this );
  m_infoScreen               =  FD_GAME_NEW FDInfoScreen( this );
  m_playerInfoSubScreen      =  FD_GAME_NEW FDPlayerInfoSubScreen( this );
  m_playerInfoMainScreen     =  FD_GAME_NEW FDPlayerInfoScreen( this );
  m_clubRecordsScreen        =  FD_GAME_NEW FDClubRecordsScreen( this );
  m_youthInfoScreen          =  FD_GAME_NEW FDYouthInfoScreen( this );
  m_youthSquadScreen         =  FD_GAME_NEW FDYouthSquadScreen( this );
  m_managerInfoScreen        =  FD_GAME_NEW FDManagerInfoScreen( this );
  m_teamoftheweekScreen      =  FD_GAME_NEW FDTeamOfTheWeekScreen( this );
  m_accountsScreen           =  FD_GAME_NEW FDAccountsScreen( this );
  m_transferlistScreen       =  FD_GAME_NEW FDTransferListScreen( this );
  m_transferlisteditScreen   =  FD_GAME_NEW FDTransferListEditScreen( this );
  m_clubFixturesScreen       =  FD_GAME_NEW FDClubFixturesScreen( this );
  m_cupFixturesScreen        =  FD_GAME_NEW FDCupFixturesScreen( this );
  m_cupScheduleScreen        =  FD_GAME_NEW FDCupScheduleScreen( this );
  m_leagueFixturesScreen     =  FD_GAME_NEW FDLeagueFixturesScreen( this );
  m_leagueTableScreen        =  FD_GAME_NEW FDLeagueTableScreen( this );
  m_leadingPlayerScreen      =  FD_GAME_NEW FDLeadingPlayersScreen( this );
  m_tacticsScreen            =  FD_GAME_NEW FDTacticsScreen( this );
  m_negotiateScreen          =  FD_GAME_NEW FDNegotiateScreen( this );
  m_matchIntroScreen         =  FD_GAME_NEW FDMatchIntroScreen( this );
  m_jobCentreScreen          =  FD_GAME_NEW FDJobCentreScreen( this );
  m_employeeInfoScreen       =  FD_GAME_NEW FDEmployeeInfoScreen( this );
  m_sackPlayerScreen         =  FD_GAME_NEW FDSackPlayerScreen( this );
  m_wagesScreen              =  FD_GAME_NEW FDWagesScreen(this);
  m_arrangeFriendliesScreen  =  FD_GAME_NEW FDArrangeFriendlies( this );
  m_monthlyAwardScreen       =  FD_GAME_NEW FDMonthlyAwardScreen( this );
  m_options                  =  FD_GAME_NEW FDOptions(this);
  m_statusScreen             =  FD_GAME_NEW FDStatusScreen( this );
  m_editModeScreen           =  FD_GAME_NEW FDEditModeScreen(this);
  m_editNameScreen           =  FD_GAME_NEW FDEditNameScreen(this);
  m_strategyFormationScreen  =  FD_GAME_NEW FDStrategyFormationScreen(this);
  m_strategyRolesScreen      =  FD_GAME_NEW FDStrategyRolesScreen(this);
  m_clubInfoSubScreen        =  FD_GAME_NEW FDClubInfoSubScreen(this);

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Initialise background / menu elements
  //
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  // create background
  m_menuScreen = FD_GAME_NEW lUIScreenWidget( m_rootScreen, LUI_WIDGET_MOUSETRANSPARENT, "menu_screen" );

  // create fade controller
  m_fade     = FD_GAME_NEW FDFullScreenWidget( m_menuScreen, 0, "fade", L_ARGB( 200, 0, 0, 0 ) );
  m_fade->Hide();

  // create border - it shouldn't matter that these can be click through
  m_menuBar  = FD_GAME_NEW lUISpriteWidget( m_menuScreen, LUI_WIDGET_MOUSETRANSPARENT, "border", FDResourceManager::GetSprite("menu_bar","menu_bar") );
  m_menuBar2 = FD_GAME_NEW lUISpriteWidget( m_menuScreen, LUI_WIDGET_MOUSETRANSPARENT, "menubar2", FDResourceManager::GetSprite("menu_bar_2","menu_bar_2") );
  m_menuBar4 = FD_GAME_NEW lUISpriteWidget( m_menuScreen, LUI_WIDGET_MOUSETRANSPARENT, "menubar4", FDResourceManager::GetSprite("menu_bar_4","menu_bar_4") );

  // create clock and back buttons
  m_clock = FD_GAME_NEW FDButtonWidget( m_menuScreen, 0, "clock", FDResourceManager::GetSprite("uiicon","fast_forward"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,2) );
  m_back  = FD_GAME_NEW FDButtonWidget( m_rootScreen, 0, "back", FDResourceManager::GetSprite("uiicon","back"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,156) );
  m_clock->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMenuClock );
  m_back->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMenuBack );

  // create menu rollouts
  m_clubMenuBack = FD_GAME_NEW FDButtonWidget(m_menuScreen, 0, "clubback", FDResourceManager::GetSprite("uiicon", "menu_club"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(0, 47), lUIPoint(17, 17));
  m_leagueMenuBack = FD_GAME_NEW FDButtonWidget(m_menuScreen, 0, "leagueback", FDResourceManager::GetSprite("uiicon", "menu_comp"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(0, 69), lUIPoint(17, 17));
  m_adminMenuBack = FD_GAME_NEW FDButtonWidget(m_menuScreen, 0, "adminback", FDResourceManager::GetSprite("uiicon", "menu_admin"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(0, 91), lUIPoint(17, 17));
  m_gameMenuBack = FD_GAME_NEW FDButtonWidget(m_menuScreen, 0, "gameback", FDResourceManager::GetSprite("uiicon", "menu_game"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(0, 113), lUIPoint(17, 17));

  m_clubMenu   = FD_GAME_NEW FDMenuWidget( m_menuScreen, 0, "club",   FDResourceManager::GetSprite("uiicon","menu_club"), FDResourceManager::GetSprite("uiicon","menu_club_glow"), lUIPoint( 0, 47 ), LUI_TOP_LEFT, lUIPoint( 0, 0 ) );
  m_leagueMenu = FD_GAME_NEW FDMenuWidget( m_menuScreen, 0, "league", FDResourceManager::GetSprite("uiicon","menu_comp"), FDResourceManager::GetSprite("uiicon","menu_comp_glow"), lUIPoint( 0, 69 ), LUI_TOP_LEFT, lUIPoint( 0, 0 ) );
  m_adminMenu  = FD_GAME_NEW FDMenuWidget( m_menuScreen, 0, "admin",  FDResourceManager::GetSprite("uiicon","menu_admin"), FDResourceManager::GetSprite("uiicon","menu_admin_glow"), lUIPoint( 0, 91 ), LUI_TOP_LEFT, lUIPoint( 0, 0 ) );
  m_gameMenu   = FD_GAME_NEW FDMenuWidget( m_menuScreen, 0, "game",   FDResourceManager::GetSprite("uiicon","menu_game"), FDResourceManager::GetSprite("uiicon","menu_game_glow"), lUIPoint( 0, 113 ), LUI_TOP_LEFT, lUIPoint( 0, 0 ) );

  m_clubMenu->Hide();
  m_leagueMenu->Hide();
  m_adminMenu->Hide();
  m_gameMenu->Hide();

  m_clubMenuBack->Hide();
  m_leagueMenuBack->Hide();
  m_adminMenuBack->Hide();
  m_gameMenuBack->Hide();

  m_clubMenuBack->SignalControlCommand.Connect(this, &FDMainMenuUI::OnMenuBackingButtonPressed);
  m_leagueMenuBack->SignalControlCommand.Connect(this, &FDMainMenuUI::OnMenuBackingButtonPressed);
  m_adminMenuBack->SignalControlCommand.Connect(this, &FDMainMenuUI::OnMenuBackingButtonPressed);
  m_gameMenuBack->SignalControlCommand.Connect(this, &FDMainMenuUI::OnMenuBackingButtonPressed);

  m_clubMenu->SignalHot.Connect(this, &FDMainMenuUI::OnMenuRolledInOrOut);
  m_leagueMenu->SignalHot.Connect(this, &FDMainMenuUI::OnMenuRolledInOrOut);
  m_adminMenu->SignalHot.Connect(this, &FDMainMenuUI::OnMenuRolledInOrOut);
  m_gameMenu->SignalHot.Connect(this, &FDMainMenuUI::OnMenuRolledInOrOut);

  // create link button
  m_link  = FD_GAME_NEW FDButtonWidget( m_rootScreen, 0, "link", FDResourceManager::GetSprite("uiicon","link_off"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,139) );
  m_link->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMenuLink );

  // create help button
  m_help  = FD_GAME_NEW FDButtonWidget( m_rootScreen, 0, "help", FDResourceManager::GetSprite("uiicon","help"), FDButtonWidget::FDBUTTON_FLAG_NO_SOUND|lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,173) );
  m_help->SignalControlCommand.Connect( this, &FDMainMenuUI::OnMenuHelp );
  m_help->Hide();

  // Match screens
  m_matchDayScreen        = FD_GAME_NEW FDMatchDayScreen( this );
  m_matchSummaryScreen    = FD_GAME_NEW FDMatchSummaryScreen( this );

  //
  ShowMenuMenu( false );
  m_menuBar->Hide();
  m_menuBar2->Hide();
  m_menuBar4->Hide();

  // setup menus
  SetupMenus();

  // *** Menu Sub ***

  // create background
  m_menuScreenSub = FD_GAME_NEW lUIScreenWidget( m_rootScreenSub, 0, "menu_screen_sub" );

  // create fade controller for sub screen (we want to fade all the sub screen)
  m_fadeSub  = FD_GAME_NEW FDFullScreenWidget( m_rootScreenSub, 0, "fade", L_ARGB( 200, 0, 0, 0 ) );
  m_fadeSub->Hide();

  // tutorials

  m_tutorialHighlight     = FD_GAME_NEW FDHighlightWidget( m_rootScreen, 0,    "tutorialHighlight",    L_ARGB( 255, 0, 0, 0 ) );
  m_tutorialHighlightSub  = FD_GAME_NEW FDHighlightWidget( m_rootScreenSub, 0, "tutorialHighlightSub", L_ARGB( 255, 0, 0, 0 ) );

  m_tutorialBoxBg  = FD_GAME_NEW lUISpriteWidget( m_rootScreen, 0, "tutBG", FDResourceManager::GetSprite("uiicon", "tutorial"), LUI_CENTER_CENTER );
  m_tutorialText   = FD_GAME_NEW lUITextWidget( m_rootScreen, 0, "tutText", "", LUI_CENTER_CENTER, lUIPointPercent( 50, 60 ), lUIPointPercent( 95, 35 ) );
  m_tutorialSprite = FD_GAME_NEW lUISpriteWidget( m_rootScreen, 0, "tutSprite", FDResourceManager::GetSprite("uiicon", "tutorial"), LUI_CENTER_CENTER );

  m_tutorialBoxBgSub  = FD_GAME_NEW lUISpriteWidget( m_rootScreenSub, 0, "tutBGSub", FDResourceManager::GetSprite("uiicon", "tutorial"), LUI_CENTER_CENTER );
  m_tutorialTextSub   = FD_GAME_NEW lUITextWidget( m_rootScreenSub, 0, "tutTextSub", "", LUI_CENTER_CENTER, lUIPointPercent( 50, 60 ), lUIPointPercent( 95, 35 ) );
  m_tutorialSpriteSub = FD_GAME_NEW lUISpriteWidget( m_rootScreenSub, 0, "tutSprite", FDResourceManager::GetSprite("uiicon", "tutorial"), LUI_CENTER_CENTER );

  m_tutorialHighlight->Hide();
  m_tutorialHighlightSub->Hide();

  m_tutorialBoxBg->Hide();
  m_tutorialText->Hide();
  m_tutorialSprite->Hide();

  m_tutorialBoxBgSub->Hide();
  m_tutorialTextSub->Hide();
  m_tutorialSpriteSub->Hide();

  m_tutorialTapScreen = FD_GAME_NEW FDFullScreenWidget( m_rootScreen, 0, "tutorialTap", L_ARGB( 0, 0, 0, 0 ) );
  m_tutorialTapScreen->Hide();

  m_bTutorialActive = false;
  m_tutorialCurrentPage = -1;
  m_tutorialPageTime = 0.0f;

  // hide process screen
  ShowProcessScreen( false, PROCESS_LOAD );

  // start on main menu screen
  SwitchToScreenCombo( SCREENCOMBO_MAINMENU, false );

  m_bProcessingActive = false;

  CNewsItem::SetPlayerNamePreprocessFunc(FDEngineMakePlayerNameLink);
  CNewsItem::SetClubNamePreprocessFunc(FDEngineMakeClubNameLink);

  m_bReadyToTick = true;

#ifdef AUTOLOAD
  OnMainMenuContinueButton( m_contgameButton );
#endif
}

void FDMainMenuUI::Deactivate( lUILayer * )
{
}

lUIWidget * FDMainMenuUI::GetScreen()
{
  return m_rootScreen;
}

lUIWidget * FDMainMenuUI::GetSubScreen()
{
  return m_rootScreenSub;
}

void FDMainMenuUI::OptionsDefault()
{
  // setup default save data
  m_saveData.m_filter[0] = FDTransferFilter( 100000, 50000000 );
  m_saveData.m_filter[0].positionType = 4;
  m_saveData.m_filter[0]. flags |= CPlayerSearch::INCLUDEONTRANSFERLIST | CPlayerSearch::INCLUDEONLOANLIST;

  m_saveData.m_filter[1] = FDTransferFilter( 100000, 50000000 );
  m_saveData.m_filter[1].positionType = 2;
  m_saveData.m_filter[1]. flags |= CPlayerSearch::INCLUDEONTRANSFERLIST | CPlayerSearch::INCLUDEONLOANLIST;

  m_saveData.m_filter[2] = FDTransferFilter( 100000, 50000000 );
  m_saveData.m_filter[2].positionType = 1;
  m_saveData.m_filter[2]. flags |= CPlayerSearch::INCLUDEONTRANSFERLIST | CPlayerSearch::INCLUDEONLOANLIST;

  m_saveData.m_filter[3] = FDTransferFilter( 100000, 50000000 );
  m_saveData.m_filter[3].positionType = 0;
  m_saveData.m_filter[3]. flags |= CPlayerSearch::INCLUDEONTRANSFERLIST | CPlayerSearch::INCLUDEONLOANLIST;

  m_saveData.m_userPrefs.m_sfxVolume = 0.8f;
  m_saveData.m_userPrefs.m_crowdVolume = 0.6f;
  m_saveData.m_userPrefs.m_matchSpeed = 2;
  m_saveData.m_userPrefs.m_bInMatchSFX = true;
  m_saveData.m_userPrefs.m_bTutorialEnabled = true;

  m_saveData.m_bTutorialIntroViewed = false;
  for(int i = 0; i < SCREENCOMBO_MAX; i++)
  {
    m_saveData.m_bTutorialViewed[ i ] = false;
  }
}

void FDMainMenuUI::EditModeDefault()
{
  m_EditModeData.Clear();
}

void FDMainMenuUI::ResetTutorialData()
{
  m_saveData.m_bTutorialIntroViewed = false;
  for(int i = 0; i < SCREENCOMBO_MAX; i++)
  {
    m_saveData.m_bTutorialViewed[ i ] = false;
  }
}

void FDMainMenuUI::ResetEditModeData()
{
  m_EditModeData.Clear();
}


FDTransferFilter &FDMainMenuUI::GetTransferFilter( int filterNumber )
{
  if( filterNumber < 0 || filterNumber > 3 )
    return m_saveData.m_filter[0];

  return m_saveData.m_filter[filterNumber];
}

void FDMainMenuUI::SetTransferFilter(  int filterNumber, FDTransferFilter &filter )
{
  if( filterNumber < 0 || filterNumber > 3 )
    filterNumber = 0;

  m_saveData.m_filter[filterNumber] = filter;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Football Director App stuff
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

CFootballDirectorApp::CFootballDirectorApp()
{
}

// Overrides
BOOL      CFootballDirectorApp::InitInstance(void)
{
  return TRUE;
}

// Implementation
void          CFootballDirectorApp::InitDebugMessages(void)
{
}

CWorld&         CFootballDirectorApp::GetWorldData(void)
{
  return *s_WorldData;
}

CString         CFootballDirectorApp::LongDateStr(void)
{
  return GetWorldData().GetCurrentDate().LongDateStr();
}

bool          CFootballDirectorApp::HasOwnerName(const bool bDisplayWarning)
{
  return true;
}

CView*          CFootballDirectorApp::SwitchToView(eView nView)
{
  lLogFmt("SwitchToView %d\n", (int)nView);
  // 14/02 Isn't used by system currently
  // todo
  // return static_cast<CMainFrame*>(AfxGetMainWnd())->SwitchToView(_View);
  return NULL;
}

int           CFootballDirectorApp::WhichCountry(void)
{
  // todo
  //return m_WhichCountry;
  return 0;
}

void          CFootballDirectorApp::UpdateTitleBar(void)          {}
void          CFootballDirectorApp::SetWhichCountry(const int x)  {}
void          CFootballDirectorApp::OnHelp(void)                  {}
void          CFootballDirectorApp::OnExitApp(void)               {}
void          CFootballDirectorApp::OnOptions(void)               {}
void          CFootballDirectorApp::OnRegister(void)              {}
bool          CFootballDirectorApp::IsRegistered(const bool bDisplayWarning)
{
  return true;
}

char*         CFootballDirectorApp::szAddPath(char* _pFile)
{
  // append _pFile to data directory location

  //TODO HACK - return value is used immediately
  static char tempdata[128];
  #ifndef L_PLATFORM_NDS
  sprintf( tempdata, "nds/%s", _pFile );
  #else
  sprintf( tempdata, "%s", _pFile );
  #endif
  return tempdata;
}

CString         CFootballDirectorApp::AddGfxPath(CString& _szFile)
{
  // append _szFile to gfx directory location
  return _szFile;
}

const bool        CFootballDirectorApp::IsInitialised(void)
{
  return true;
}

CBasicDisplay&      CFootballDirectorApp::GetBasicDisplay()
{
  static CBasicDisplay display;
  return display;
}

// Player Actions
// Note: Unused - Possibly usless due to Playerscreen
//void          CFootballDirectorApp::OnViewOwnedPlayerInformation(void)       { lLogFmt("OnViewOwnedPlayerInformation\n"); }
//void          CFootballDirectorApp::OnViewNotOwnedPlayerInformation(void)    { lLogFmt("OnViewNotOwnedPlayerInformation\n"); }


// Youth Actions
// Note: Unused - Possibly usless due to youthscreen
//void          CFootballDirectorApp::OnViewOwnedYouthInformation(void)        { lLogFmt("OnViewOwnedYouthInformation\n"); }
//void          CFootballDirectorApp::OnViewNotOwnedYouthInformation(void)     { lLogFmt("OnViewNotOwnedYouthInformation\n"); }

// Fixture Actions
//void          CFootballDirectorApp::OnViewMatchResultInformation(void)       { lLogFmt("OnViewMatchResultInformation\n"); }

// Dialogs
void CFootballDirectorApp::OnSelectUserClubToManage(const ClubID _LastClubID )
{
	lLogFmt("OnSelectUserClubToManage\n");
	s_mainmenuUI->GetSelectTeamScreen()->SetNonSelectableTeam(_LastClubID);
  s_mainmenuUI->SwitchToScreenModal(SCREENCOMBO_SELECTNEWTEAM, false);
	s_mainmenuUI->GetSelectTeamScreen()->SetNonSelectableTeam(ClubID());
}

void CFootballDirectorApp::OnPlayerSearch()
  { lLogFmt("OnPlayerSearch\n");                        }              // Screen06



void CFootballDirectorApp::OnManagerRenewContract(CClub* _Club)
  { lLogFmt("OnManagerRenewContract\n");                }              // Screen47


void CFootballDirectorApp::OnUserManagerLeaveClubDlg(const CString &_str)
{
	lLogFmt("OnUserManagerLeaveClubDlg\n");

	// User has been sacked, and either needs to select a new team or
	// quit the game.
  UserMessageBox(_str, MB_OK | MB_ENDOFGAME);
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// In progress / Implemented
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

static bool DoModal( const char *screenName )
{
  if( s_mainmenuUI->GetDialogState() == FDMainMenuUI::DIALOG_STATUS_ACTIVE )
  {
    lLogFmt("DoModel: ERROR: Already in dialog active mode?\n");
    //NOTE: Continue anyway for now, maybe should assert?
  }

  lLogFmt("DoModal: %s\n", screenName);

  s_mainmenuUI->SetDialogState( FDMainMenuUI::DIALOG_STATUS_ACTIVE );

  while( DoMainLoop() )
  {
    if( s_mainmenuUI->GetDialogState() == FDMainMenuUI::DIALOG_STATUS_OK )
    {
      lLogFmt(" %s done - OK\n", screenName);
      return true;
    }

    if( s_mainmenuUI->GetDialogState() == FDMainMenuUI::DIALOG_STATUS_CANCEL )
    {
      lLogFmt(" %s done - Cancel\n", screenName);
      return false;
    }
  }

  return false;
}

void CFootballDirectorApp::OnHighlightMenuIcon( int index, int subIndex )
{
  lLogFmt("OnHighlightIcon %d (%d)", index, subIndex );
  s_mainmenuUI->HighlightMenuIcon( index, subIndex );
}

//void CFootballDirectorApp::OnKnockedOutCup( CString cupName )
//{
//  lLogFmt("OnKnockedOutCup\n");
//
//  s_mainmenuUI->GetInfoScreen()->SetCupKnockedOut( lString( cupName ) );
//  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_INFO, false );
//  DoModal( "Info" );
//}

void CFootballDirectorApp::OnWonManagerOfTheMonth(CString& _DivName)
{
  lLogFmt("OnWonMOM\n");

  s_mainmenuUI->GetInfoScreen()->SetWonMOM( lString( _DivName ) );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_INFO, false );
  DoModal( "Info" );
}

void CFootballDirectorApp::OnCupFinalResult( CString &cupName, bool bWin )
{
  lLogFmt("OnCupFinalResult\n");

  s_mainmenuUI->GetInfoScreen()->SetCupFinalResult( lString( cupName ), bWin );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_INFO, false );
  DoModal( "Info" );
}

void CFootballDirectorApp::OnEndOfSeason( CString &divName, bool bChampions, bool bPromoted, bool bRelegated, float normalisedPosition )
{
  lLogFmt("OnEndOfSeason - champions %d promoted %d relegated %d finalpos %.2f\n", bChampions, bPromoted, bRelegated, normalisedPosition);

  int state = FDInfoScreen::ENDSEASON_MIDTABLE;

  if( bChampions )
  {
    state = FDInfoScreen::ENDSEASON_CHAMPIONS;
  } else
  if( bPromoted )
  {
    state = FDInfoScreen::ENDSEASON_PROMOTION;
  } else
  if( bRelegated )
  {
    state = FDInfoScreen::ENDSEASON_RELEGATION;
  } else
  {
    // normalisedPosition being champions at 0.0, bottom at 1.0
    if( normalisedPosition > 0.7f )
    {
      state = FDInfoScreen::ENDSEASON_BOTTOM;
    } else if( normalisedPosition < 0.2f )
    {
      state = FDInfoScreen::ENDSEASON_TOP;
    }
  }

  s_mainmenuUI->GetInfoScreen()->SetEndOfSeason( lString( divName ), state);
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_INFO, false );
  DoModal( "Info" );
}

void CFootballDirectorApp::OnArrangeFriendlies(void)
{
  lLogFmt("OnArrangeFriendlies\n");

  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_ARRANGEFRIENDLIES, false );
  DoModal( lText::Text( IDS_SCREEN54 ) );
}              // Screen54

void CFootballDirectorApp::OnPlayerRequestTransfer(CPlayer* _Player)
{
  lLogFmt("OnPlayerRequestTransfer\n");

  s_mainmenuUI->GetNegotiateScreen()->SetOnPlayerRequestTransfer( _Player );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
  DoModal( lText::Text( IDS_PLAYERTRANSFERREQUEST) );
}              // Screen21

void CFootballDirectorApp::OnChangeTransferStatus()
{
  lLogFmt("OnChangeTransferStatus\n");
  s_mainmenuUI->GetPlayerInfoSubScreen()->SetPlayerInfo(WorldData().GetCurrentPlayer());
  s_mainmenuUI->GetTransferStatusScreen()->SetPlayer( WorldData().GetCurrentPlayer() );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_TRANSFERSTATUS, false );
  DoModal( lText::Text( IDS_SCREEN16 ) );
  s_mainmenuUI->PopScreen();
}              // Screen16

void CFootballDirectorApp::OnApproachClubLoanInPlayer(void)
{
  lLogFmt("OnApproachClubLoanInPlayer\n");

  s_mainmenuUI->GetNegotiateScreen()->SetOnApproachClubLoanInPlayer( WorldData().GetCurrentPlayer() );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
  DoModal( lText::Text( IDS_APPROACH_CLUB_LOAN )  );
  s_mainmenuUI->PopScreen();
}              // Screen24

void CFootballDirectorApp::OnMakeOfferForPlayer(void)
{
  lLogFmt("OnMakeOfferForPlayer\n");

  CTransferNegotiation* pNegotiation = WorldData().GetCurrentUser()->GetTransferNegotiationList()->DoFindNegotiationsForPlayer(WorldData().GetCurrentPlayer());
	if (pNegotiation == null)
	{
		s_mainmenuUI->GetNegotiateScreen()->SetOnMakeOfferForPlayer( WorldData().GetCurrentPlayer() );
    s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
    DoModal( lText::Text( IDS_MAKEOFFER )  );
		s_mainmenuUI->PopScreen();
	}
	else
	{
		CString strText;

    strText.Format(IDS_ALREADYNEGOTIATINGWITHPLAYER, FDMakeClubNameLink(&WorldData().GetClubByID(pNegotiation->GetSellClubID())).CStr(),
			FDMakePlayerNameLink(WorldData().GetPlayer(pNegotiation->GetPlayerID())).CStr());
    UserMessageBox(strText, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1) ;
	}
}              // Screen30

void          CFootballDirectorApp::DoPlayerContractNegotiations(CClub* _Club, CTransferNegotiation* _Negotiations)
{
  lLogFmt("DoPlayerContractNegotiations\n");
  WorldData().SetCurrentClub(_Club);
  s_mainmenuUI->GetNegotiateScreen()->SetNotOwnedPlayerContractNegotiations( _Negotiations );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
  DoModal( lText::Text( IDS_NEGOTIATE_PLAYER_CONTACT )  );
  //s_mainmenuUI->PopScreen();
}

void CFootballDirectorApp::OnNegotiateOutOfContractPlayerContract(void)
{
  lLogFmt("OnNegotiateOutOfContractPlayerContract\n");
  bool bAccepted = false;
	CContract PlayerContract;

  s_mainmenuUI->GetNegotiateScreen()->SetOnNegotiateOutOfContractPlayerContract( WorldData().GetCurrentPlayer(), WorldData().GetCurrentUserClub(), bAccepted, PlayerContract );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
  DoModal( lText::Text( IDS_NEGOTIATE_PLAYER_OUTOFCONTRACT)  );
  s_mainmenuUI->PopScreen();
}              // Screen23

void CFootballDirectorApp::OnNegotiatePlayerContract(void)
{
  lLogFmt("OnNegotiatePlayerContract\n");
  OnNegotiatePlayerContract(TEXT(""));
}

void CFootballDirectorApp::OnNegotiatePlayerContract(const CString &_strPrompt)
{
  lLogFmt("OnNegotiatePlayerContract\n");
  if (WorldData().GetCurrentPlayer()->GetContractEndDate().SeasonsRemaining() > 2 &&
		WorldData().GetCurrentClub()->GetFirstTeamSquad().IsPlayerInSquad(WorldData().GetCurrentPlayer()) == true)
	{
		CString strText;
    strText.Format(GetRandomNoContractResponse(), FDMakePlayerNameLink(WorldData().GetCurrentPlayer()).CStr());
    UserMessageBox(strText, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1) ;
	}
	else
	{
    s_mainmenuUI->GetNegotiateScreen()->SetOnNegotiatePlayerContract( WorldData().GetCurrentPlayer(), WorldData().GetCurrentClub(),  _strPrompt );
    s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
    DoModal( lText::Text( IDS_NEGOTIATE_PLAYER_CONTACT) );
	s_mainmenuUI->PopScreen();
	}

}              // Screen23

// Youth Actions
bool          CFootballDirectorApp::OnSackYouth(void)
{
  lLogFmt("OnSackYouth\n");

  CClub * club = WorldData().GetCurrentUserClub();

  int avail = club->GetFirstTeamSquad().DoCountAvailablePlayers();
  int availYouth = club->GetYouthList().GetSize();

  lLogFmt("OnSackYouth: %d available, %d club youth - need %d\n", avail, availYouth, PLAYERSINTEAM + 5);
  if( (avail + availYouth)
<= PLAYERSINTEAM + 5 )
  {
    UserMessageBox( "The board will not allow you to sack this youth as you would be unable to field a full team with your first team and youth squad players.", MB_ICONEXCLAMATION + MB_OK);
    return false;
  } else
  {
    // Note: this code could be called directly from youthscreen as afaik it isn't called anywhere else
    return WorldData().OnSackYouth();
  }
}

// NOTE: This function is called from youth squad screen when
//  a user promotes a youth
void          CFootballDirectorApp::OnPromoteYouth(void)
{
  lLogFmt("OnPromoteYouth\n");
  OnPromoteYouth(TEXT(""));
}

// NOTE: This is called from above and when a youth is nearing their contract end
void CFootballDirectorApp::OnPromoteYouth(const CString &_strPrompt)
{
  lLogFmt("OnPromoteYouth\n");
  s_mainmenuUI->GetNegotiateScreen()->SetOnPromoteYouth( WorldData().GetCurrentYouth(), _strPrompt );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
  DoModal( lText::Text( IDS_PROMOTE_YOUTH ) );
  s_mainmenuUI->PopScreen();
}              // Screen13

void          CFootballDirectorApp::OnHireYouth(void)
{
  lLogFmt("OnHireYouth\n");
  WorldData().OnHireYouth();
}

// Employee Actions
bool          CFootballDirectorApp::OnSackEmployee(void)
{
  lLogFmt("OnSackEmployee\n");
  return WorldData().OnSackEmployee();
}

void          CFootballDirectorApp::OnNegotiateEmployeeContract(void)
{
  lLogFmt("OnNegotiateEmployeeContract\n");

  if (WorldData().GetCurrentEmployee()->GetContractEndDate().SeasonsRemaining() > 3)
	{
    CString strMessage;
    strMessage.Format( GetRandomNoContractResponse(), WorldData().GetCurrentEmployee()->GetName());
    UserMessageBox(strMessage, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1) ;
	}
	else
	{
    s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
    DoModal( lText::Text( IDS_NEGOTIATE_EMPLOYEE_CONTACT ) );
	s_mainmenuUI->PopScreen();
	}
}

void          CFootballDirectorApp::OnHireEmployee(void)
{
  lLogFmt("OnHireEmployee\n");
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
  DoModal( lText::Text( IDS_HIRE_EMPLOYEE ) );
  s_mainmenuUI->PopScreen();
}

void CFootballDirectorApp::OnEmployeeContractRenewalNegotiations(CEmployee* _Employee, CString _Prompt)
{
  lLogFmt("OnEmployeeContractRenewalNegotiations\n");
  s_mainmenuUI->GetNegotiateScreen()->SetOnNegotiateEmployeeContract( _Employee,  _Prompt );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );

  DoModal( lText::Text( IDS_NEGOTIATE_EMPLOYEE_CONTACT ) );

  s_mainmenuUI->PopScreen();
}              // Screen14

// Club Actions
void          CFootballDirectorApp::OnViewSquadDetails(void)
{
  lLogFmt("OnViewSquadDetails\n");
  WorldData().SetCurrentClub( WorldData().GetCurrentUserClub() );
  s_mainmenuUI->GetSquadScreen()->SetSquadInfo( WorldData().GetCurrentUserClub() );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_SQUAD_CLUB, false );
}

// Screen12
void CFootballDirectorApp::OnReceiveOfferForPlayer(CClubTransferOffer* _TheOffer)
{
  lLogFmt("OnReceiveOfferForPlayer: %x\n", _TheOffer);

  s_mainmenuUI->GetNegotiateScreen()->SetOnReceiveOffer( _TheOffer );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEGOTIATION, false );
  DoModal( lText::Text( IDS_RECEIVEOFFERFORPLAYER ) );

  lLogFmt("Done OnReceiveOfferForPlayer\n");
}

// Screen35
bool CFootballDirectorApp::OnSackPlayer(void)
{
  lLogFmt("OnSackPlayer\n");

  CClub * club = WorldData().GetCurrentUserClub();

  int avail = club->GetFirstTeamSquad().DoCountAvailablePlayers();

  lLogFmt("OnSackPlayer: %d available - need %d\n", avail, PLAYERSINTEAM + 5);
  if( avail <= PLAYERSINTEAM + 5 )
  {
    UserMessageBox( "The board will not allow you to sack this player as you would be unable to field a full team.", MB_ICONEXCLAMATION + MB_OK);
  } else
  {
    // Can be called from Wage screen and Squad screen. As far as I know.
    s_mainmenuUI->GetSackPlayerScreen()->SetPlayer( WorldData().GetCurrentPlayer() );
    s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_SACKPLAYER, false );

    const bool bResult = DoModal( lText::Text( IDS_SCREEN35 ) );

    s_mainmenuUI->PopScreen();

    return bResult;
  }

  return false;
}

// Screen32
bool CFootballDirectorApp::DisplayMatchIntro(const bool _bShowSummary)
{
  lLogFmt("DisplayMatchIntro\n");

  if (_bShowSummary == true)
	{
    s_mainmenuUI->GetMatchIntroScreen()->SetIntroView();
    s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_MATCH_INTRO, false );

    if( DoModal( lText::Text( IDS_SCREEN32 ) ))
    {
      s_mainmenuUI->PopScreen();
      return true;
    }
    s_mainmenuUI->PopScreen();
  }

  return false;
}

// Screen65
void CFootballDirectorApp::OnDisplayMatch(CMatchPlay* _MatchPlay)
{
  lAssert( _MatchPlay != NULL );
  lLogFmt("OnDisplayMatch\n");

  s_mainmenuUI->GetMatchDayScreen()->SetMatchPlay( _MatchPlay );
  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_MATCH, false );

  DoModal( lText::Text( IDS_MATCH32 ) );

  s_mainmenuUI->PopScreen();
}

// Screen92
void CFootballDirectorApp::OnShowTeamOfTheWeek()
{
  lLogFmt("OnShowTeamOfTheWeek()\n");

  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_TEAMOFTHEWEEK, false );

  DoModal( lText::Text( IDS_SCREEN92 ) );
}


// Screen17
void CFootballDirectorApp::OnShowMatchSummary(CCup* _Cup, CFixture* _Fixture, bool bBackButtonToClose)
{
  lAssert( _Fixture != NULL );
  lLogFmt("OnShowMatchSummary\n");

  s_mainmenuUI->GetMatchSummaryScreen()->SetMatch( _Cup, _Fixture );
  s_mainmenuUI->GetMatchSummaryScreen()->ShowBackButton( bBackButtonToClose );

  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_MATCH_SUMMARY, false );

  DoModal( lText::Text( IDS_MATCH_SUMMARY ) );

  s_mainmenuUI->PopScreen();
}

void CFootballDirectorApp::OnShowSimProgressScreen()
{
  s_mainmenuUI->SwitchToScreenCombo(SCREENCOMBO_PROGRESS, false);
}

/*------------------------------------------------------------------------------
	Method:   	UserMessageBox
	Access:    	public
	Parameter: 	UINT Message
	Parameter: 	UINT Style
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int UserMessageBox(UINT MessageID, UINT Style)
{
  lLogFmt("MAINMENUUI: UserMessageBox\n");
  CString msg;
  msg.LoadString( MessageID );
  return s_mainmenuUI->UserMessageBox( msg, Style );
}

int FDMainMenuUI::UserMessageBox(UINT MessageID, UINT Style)
{
	return ::UserMessageBox(MessageID, Style);
}

/*------------------------------------------------------------------------------
	Method:   	UserMessageBox
	Access:    	public
	Parameter: 	CString& Message
	Parameter: 	UINT Style
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int UserMessageBox(const CString& Message, UINT Style)
{
  lLogFmt("MAINMENUUI: UserMessageBox style %d : %s\n", Style, Message.CStr());
  return s_mainmenuUI->UserMessageBox( Message, Style );
}

void FDMainMenuUI::OnMenuBackingButtonPressed(lUIControlWidget *pWidget)
{
	if (pWidget == m_clubMenuBack)
	{
		m_clubMenu->RollIn();
		pWidget->Hide();
	}
	else if (pWidget == m_leagueMenuBack)
	{
		m_leagueMenu->RollIn();
		pWidget->Hide();
	}
	else if (pWidget == m_adminMenuBack)
	{
		m_adminMenu->RollIn();
		pWidget->Hide();
	}
	else if (pWidget == m_gameMenuBack)
	{
		m_gameMenu->RollIn();
		pWidget->Hide();
	}
}

void FDMainMenuUI::OnMenuRolledInOrOut(FDMenuWidget *pWidget, int iRollType)
{
	if (iRollType < 0)
		return;

	m_clubMenuBack->Hide();
	m_leagueMenuBack->Hide();
	m_adminMenuBack->Hide();
	m_gameMenuBack->Hide();

	if (pWidget == m_clubMenu)
	{
		m_clubMenuBack->Show();
	}
	else if (pWidget == m_leagueMenu)
	{
		m_leagueMenuBack->Show();
	}
	else if (pWidget == m_adminMenu)
	{
		m_adminMenuBack->Show();
	}
	else if (pWidget == m_gameMenu)
	{
		m_gameMenuBack->Show();
	}
}

void ApplySaveDataChangesToWorldData(FDEditModeData &cData)
{
	// Testing changes.
	//s_WorldData->GetClub(0).SetName("Windowlickers");

  lLogFmt("Apply SaveDataChangesToWorldData\n");

	for (int i = 0; i < FDEditModeData::MAX_EDIT_CLUBS; i++)
	{
		if (cData.m_cClubIDs.Get(i) == (NOCLUB & 0x1FF))
			continue;

		lLogFmt(" %d: ID: %d Name: %s\n", i, cData.m_cClubIDs.Get(i), cData.m_cClubNames.GetString(i));

		s_WorldData->GetClubByID(ClubID(cData.m_cClubIDs.Get(i))).SetName(CString(cData.m_cClubNames.GetString(i)));
	}

	for (int i = 0; i < FDEditModeData::MAX_EDIT_PLAYERS; i++)
	{
		if (cData.m_cPlayerIDs.Get(i) == (NOPLAYER & 0xFFF))
			continue;

    lLogFmt(" %d: ID: %d Name: %s\n", i, cData.m_cPlayerIDs.Get(i), cData.m_cPlayerSurnames.GetString(i));

		// apply player names.
		CPlayer *pPlayer = s_WorldData->GetPlayer(cData.m_cPlayerIDs.Get(i));
		CNonDuplicateStringList &cSurnames = s_WorldData->GetSurnameList();
		const int iSurnameIndex = cSurnames.Add(CString(cData.m_cPlayerSurnames.GetString(i)));

		lAssert(iSurnameIndex >= 0);

		pPlayer->SetForename(CString(cData.m_cPlayerForenames.Get(i)));
		pPlayer->SetSurname((ushort)iSurnameIndex);
	}
}

FDEditModeData &FDMainMenuUI::GetSavedEditModeData()
{
  return m_EditModeData;
}

//////////////////////////////////////////////////////////////////////////
// Overrides the help text set when the UI switches to the current screen.
//////////////////////////////////////////////////////////////////////////
void StartAutoplayGame()
{
	s_mainmenuUI->ReinitWorldData(true);

	// select team
	ClubID cTeamToPlay;
	cTeamToPlay.id = FDCommandLineOptions::GetTeamToPlay();
	if (cTeamToPlay.id > WorldData().GetClubList().GetSize())
    cTeamToPlay.id = 69;                // Aldershot: Had some issues reported with previously

	WorldData().OnInitialiseUser( cTeamToPlay );

	// welcome human manager
	WorldData().GetCurrentUserClub()->DoHumanManagerWelcome();

	// skip to end of week
	WorldData().GetCountryList().DoEndOfWeek();

	// mark selected team
	s_mainmenuUI->GetSquadScreen()->SetSquadInfo( WorldData().GetCurrentUserClub() );

	// show news / status. Ensure that the player can't back up into team selection again.
	s_mainmenuUI->ClearScreenStack();
	s_mainmenuUI->ReinitWorldData(true);
	s_mainmenuUI->SetWorldDataUsed(true);

	if (FDCommandLineOptions::ShouldLoadGame())
	{
		s_mainmenuUI->GamedataLoad();
	}

  s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_NEWSSTATUS, false );
}

namespace L_NAMESPACE_NAME
{
  void FDClubLinkClick( int idx )
  {
    if (s_mainmenuUI->IsAnyMenuActive() == false )
	  {
	    ClubID cClubID;
      cClubID.id = idx;
	    s_mainmenuUI->GetSquadScreen()->SetSquadInfo( &WorldData().GetClubByID( cClubID ) );
        s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_SQUAD, true );
        s_mainmenuUI->OnMenuLink( NULL );
	  }
  }

  void FDPlayerLinkClick( int idx )
  {
    if (s_mainmenuUI->IsAnyMenuActive() == false )
	  {
        s_mainmenuUI->GetPlayerInfoSubScreen()->SetPlayerInfo( WorldData().GetPlayer( idx ) );
        s_mainmenuUI->GetPlayerInfoMainScreen()->SetPlayerInfo( WorldData().GetPlayer( idx ) );
      s_mainmenuUI->SwitchToScreenCombo( SCREENCOMBO_PLAYERPROFILE, true );
	    s_mainmenuUI->OnMenuLink( NULL );
	  }
  }

  void FDMatchLinkClick( unsigned int idx )
  {
    if (s_mainmenuUI->IsAnyMenuActive() == false )
	  {
      CCalendar TheDate( WorldData().GetCurrentDate() );
      TheDate.SetMatchDay( idx );

      CClub * pClub = WorldData().GetCurrentUserClub();

      CMatchInfo MatchDetails;
      if( pClub->IsMatchToday(TheDate, MatchDetails) == true )
      {
        if( MatchDetails.GetFixture() != NULL )
        {
          if( MatchDetails.GetFixture()->HasMatchBeenPlayed() == true )
          {
            theApp.OnShowMatchSummary( MatchDetails.GetCup(), MatchDetails.GetFixture(), true );
            s_mainmenuUI->OnMenuLink( NULL );
            //GetMainUI()->PopScreen();
          }
        }
      }

	    //s_mainmenuUI->OnMenuLink( NULL );
	  }
  }

  void FDLeagueLinkClick( unsigned int divIdx, unsigned int matchIdx, unsigned int date )
  {
    if (s_mainmenuUI->IsAnyMenuActive() == false )
	  {
      if( divIdx == -1 || matchIdx == -1 || date == -1 )
        return;

      CDivisionList &divisionList = WorldData().GetDivisionList();
      CDivision &division = divisionList.GetAt( divIdx );

      CSoccerResultList resultList;

      CCalendar TheDate( WorldData().GetCurrentDate() );
      TheDate.SetMatchDay( date );

      division.DoBuildLeagueFixtureList( /*TheDate*/ date, resultList );

      CSoccerResult* pResult = resultList.GetAt(matchIdx);

      if( pResult->GetFixture() != NULL )
      {
        if( pResult->GetFixture()->HasMatchBeenPlayed() == true )
        {
          theApp.OnShowMatchSummary( NULL, pResult->GetFixture(), true );
          s_mainmenuUI->OnMenuLink( NULL );
          //GetMainUI()->PopScreen();
        }
      }


	    //s_mainmenuUI->OnMenuLink( NULL );
	  }
  }

  void FDCupLinkClick( unsigned int cupIdx, unsigned int roundIdx, unsigned int matchIdx )
  {
    if (s_mainmenuUI->IsAnyMenuActive() == false )
	  {
      if( matchIdx == -1 || cupIdx == -1 || roundIdx == -1 )
        return;


      CCup * cup = WorldData().GetCup(cupIdx);

      if( cup == NULL )
      {
        lLogFmt("Update: invalid cup\n");
        return;
      }

      CCupRoundList &roundList = cup->GetRounds();

      if( roundList.GetSize() )
      {
        CCupRound &cupRound = roundList[ roundIdx ];


        CSoccerResultList resultList;
        CSoccerResult *result = NULL;

        if( cupRound.Is2Regions() == false )
        {
          cupRound.BuildReplayDisplayList(resultList);

          lLogFmt("Cup: %s Round: %s = %d\n", cup->GetName(), cupRound.GetName(), resultList.GetSize());
        }

        if( resultList.GetSize() > 0 )
        {
          if( cupRound.IsGroupMatch() == false )
          {
            resultList.SortByHomeClub();
          }

          result = resultList.GetAt(matchIdx);
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

          result = resultList.GetAt(matchIdx - rowCount);
        }

        if( result->GetFixture() != NULL )
        {
          if( result->GetFixture()->HasMatchBeenPlayed() == true )
          {
            theApp.OnShowMatchSummary( cup, result->GetFixture(), true );
            s_mainmenuUI->OnMenuLink( NULL );
            //GetMainUI()->PopScreen();
          }
        }

        //s_mainmenuUI->OnMenuLink( NULL );
      }
	  }
  }

  int GetRandomNoContractResponse()
  {
    int idx = rand() % 3;
    switch( idx )
    {
      case 0: return IDS_NOTNEGOTIATECONTRACT;
      case 1: return IDS_RESP_CONTRACT_HAPPY;
      case 2: return IDS_RESP_CONTRACT_CONTENT;
    }

    return IDS_RESP_CONTRACT_CONTENT;
  }

  // returns an ARGB colour
  uint GetKitColour( int index )
  {
    uint KitColourReferences[] = { RED, WHITE, BLUE, GREEN, SKYBLUE, MAROON, AMBER, TANGERINE, BLACK, YELLOW };
    return KitColourReferences[ index ];
  }

  const char *GetBadgeSpritePostfix( int index )
  {
    const char *ColourPostfix[] = { "RED",
                                    "WHITE",
                                    "DARKBLUE",//"BLUE",
                                    "GREEN",
                                    "SKYBLUE",
                                    "MAROON",
                                    "ORANGE", //"AMBER",
                                    "TANGERINE",
                                    "BLACK",
                                    "YELLOW" };
    return ColourPostfix[ index ];
  }



  // returns away colour based on passed in home team colour
  uint GetAwayColour( uint homecol )
  {
    if( homecol == BLACK )
    {
      return GREY;
    } else
    {
      return BLACK;
    }
  }


  // returns an ARGB colour
  uint GetInverseColour( uint colourIndex )
  {
    if( (colourIndex == 0 || colourIndex == 1 || colourIndex == 9 || colourIndex == 3) )
      return BLACK;
    else if( (colourIndex == 8 || colourIndex == 2 || colourIndex == 4)  )
      return WHITE;
    else
      return SKYBLUE;
  }

  // returns an ARGB colour
  uint GetInverseColour( uint colourIndex1, uint colourIndex2 )
  {
    //TODO : tweak...
    if( (colourIndex1 == 0 || colourIndex1 == 1 || colourIndex1 == 9 || colourIndex1 == 3) &&
        ( colourIndex2 == 0 ||  colourIndex2 == 1 ||  colourIndex2== 9 ||  colourIndex2 == 3) )
      return WHITE;
    else
    if( (colourIndex1 == 8 || colourIndex1 == 2 || colourIndex1 == 4) &&
        ( colourIndex2 == 8 ||  colourIndex2 == 2 ||  colourIndex2== 4) )
      return WHITE;
    else
      return WHITE;
  }

  int GetDefaultScrollBarWidth( bool bAgainstSide )
  {
    if( bAgainstSide )
      return 11;
    else
      return 8;
  }

  CCalendar GetSeasonStartDate()
  {
    CCalendar cCurrentDate(WorldData().GetCurrentDate());

    CCalendar TheDate( WorldData().GetCurrentDate() );
    WorldData().GetCurrentUserClub()->GetDivision().DoFindStartOfSeason(TheDate);

    CCalendar TheEndDate( WorldData().GetCurrentDate() );
    WorldData().GetCurrentUserClub()->GetDivision().DoFindEndOfSeason(TheEndDate);

    lLogFmt("Current: wk %d season %d matchday %d\n", cCurrentDate.GetWeek(), cCurrentDate.GetSeason(), cCurrentDate.GetMatchDay());
    lLogFmt("Start: wk %d season %d matchday %d\n", TheDate.GetWeek(), TheDate.GetSeason(), TheDate.GetMatchDay());
    lLogFmt("End:   wk %d season %d matchday %d\n", TheEndDate.GetWeek(), TheEndDate.GetSeason(), TheEndDate.GetMatchDay());

    if( cCurrentDate >= TheEndDate || cCurrentDate.GetSeason() != TheEndDate.GetSeason() )
    {
      TheDate = WorldData().GetCurrentDate();
      WorldData().GetCurrentUserClub()->GetDivision().DoFindStartOfNextSeason( TheDate );
      lLogFmt("Past end of season - finding start of next\n");
      lLogFmt("StartNext:   wk %d season %d matchday %d\n", TheDate.GetWeek(), TheDate.GetSeason(), TheDate.GetMatchDay());
    }

    return TheDate;
  }
}

