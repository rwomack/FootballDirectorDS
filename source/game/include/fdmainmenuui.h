//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmainmenuui.h
// Description : FDMainMenuUI interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDMAINMENUUI_H_INCLUDED
#define FDMAINMENUUI_H_INCLUDED

#include <ui/lui.h>
#include <framework/lsigslot.h>

#include "stdafx.h"
#include "Footballdirector.h"
#include "Division.h"
#include "MatchInfo.h"
#include <fdeditmodedata.h>

#include <fdsound.h>
#include <fdstatestorage.h>

//#ifdef L_PLATFORM_NDS
////NOTE: Force THUMB mode on UI / Exe side
//#include <nitro/code16.h>
//#endif

#ifdef L_PLATFORM_NDS
#pragma optimize_for_size on
#endif

BEGIN_L_NAMESPACE

// Pre-declared classes

class FDGameWorld;

// widgets
class lUIScreenWidget;
class lUISpriteWidget;
class lUIControlWidget;
class lUITextWidget;
class FDButtonWidget;
class FDComboWidget;
class FDMenuWidget;
class FDButtonRolloutWidget;
class FDTableWidget;
class FDFullScreenWidget;
class FDHighlightWidget;
class FDScrollBarWidget;
class FDTickerWidget;
class FDTextWidget;

// main ui
class FDMainMenuUI;
class FDScreen;

// screens
class FDAccountsScreen;
class FDClubFixturesScreen;
class FDCupFixturesScreen;
class FDCupScheduleScreen;              // 02
class FDJobCentreScreen;                // (39)
class FDNewGameScreen;
class FDNewsScreen;
class FDPlayerInfoScreen;
class FDPlayerInfoSubScreen;
class FDSelectTeamScreen;
class FDSquadScreen;
class FDStatusScreen;
class FDTacticsScreen;
class FDTrainingScreen;
class FDInfoScreen;
class FDTransferListScreen;
class FDTransferListEditScreen;
class FDTeamOfTheWeekScreen;// (92)
class FDEmployeeInfoScreen;// (60)
class FDYouthInfoScreen;// (59)
class FDManagerInfoScreen;
class FDYouthSquadScreen;// (41)
class FDLeagueFixturesScreen;           // (19)
class FDLeadingPlayersScreen;// (33)
class FDLoadScreen;
class FDSaveScreen;
class FDProgressScreen;
class FDSackPlayerScreen; // (35)
class FDLeagueTableScreen; // (50)
class FDNegotiateScreen;
class FDMatchIntroScreen;               // (32)
class FDMatchSummaryScreen;              // (17)
class FDMatchDayScreen; // 65
class FDClubRecordsScreen; // (09)
class FDLiveResultScreen;
class FDLiveLeagueTableScreen;
class FDLanguageScreen;
class FDOptions;
class FDWagesScreen;
class FDMonthlyAwardScreen; // (46)
class FDTransferStatusScreen; // 16, playerinfo on top
class FDArrangeFriendlies;
class FDEditModeScreen;
class FDEditNameScreen;
class FDStrategyFormationScreen;
class FDStrategyRolesScreen;

class FDClubInfoSubScreen;

enum
{
  MBOX_CLICK_NONE,
  MBOX_CLICK_OK,
  MBOX_CLICK_YES,
  MBOX_CLICK_NO,
};

//
// screen types
//
enum ScreenComboType
{
  SCREENCOMBO_MAINMENU = 0,
  SCREENCOMBO_LOADING,
  SCREENCOMBO_PROGRESS,
  SCREENCOMBO_NEWGAME,
  SCREENCOMBO_NEWGAME_SELECTTEAM,
  SCREENCOMBO_SELECTNEWTEAM,          // duplicate of SCREENCOMBO_NEWGAME_SELECTTEAM
  SCREENCOMBO_SQUAD,
  SCREENCOMBO_SQUAD_CLUB,
  SCREENCOMBO_NEWSSTATUS,
  SCREENCOMBO_ACCOUNTS,
  SCREENCOMBO_CLUBFIXTURES,
  SCREENCOMBO_CUPFIXTURES,
  SCREENCOMBO_CUPSCHEDULE,
  SCREENCOMBO_LEAGUEFIXTURES,
  SCREENCOMBO_LEAGUETABLE,
  SCREENCOMBO_TRANSFERLIST,
  SCREENCOMBO_TRANSFERLIST_EDIT,
  SCREENCOMBO_TRAINING,
  SCREENCOMBO_JOBCENTRE,
  SCREENCOMBO_TACTICS,
  SCREENCOMBO_TACTICS_IN_MATCH,
  SCREENCOMBO_MATCH_INTRO,
  SCREENCOMBO_MATCH_SUMMARY,
  SCREENCOMBO_MATCH,
  SCREENCOMBO_CLUBRECORDS,
  SCREENCOMBO_MESSAGEBOX_OK,
  SCREENCOMBO_MESSAGEBOX_YESNO,
  SCREENCOMBO_SAVE,
  SCREENCOMBO_SAVE_EDITMODE,
  SCREENCOMBO_SAVEDONE,
  SCREENCOMBO_YOUTHSQUAD,
  SCREENCOMBO_NEGOTIATION,
  SCREENCOMBO_SACKPLAYER,
  SCREENCOMBO_TEAMOFTHEWEEK,
  SCREENCOMBO_LEADINGPLAYERS,
  SCREENCOMBO_TRANSFERSTATUS,
  SCREENCOMBO_WAGESSCREEN,
  SCREENCOMBO_ARRANGEFRIENDLIES,
  SCREENCOMBO_LANGUAGE,
  SCREENCOMBO_MOTM,
  SCREENCOMBO_OPTIONS,
  SCREENCOMBO_OPTIONS_IN_GAME,

  SCREENCOMBO_PLAYERPROFILE,
//    SCREENCOMBO_YOUTHPROFILE,
//    SCREENCOMBO_EMPLOYEEPROFILE,
//    SCREENCOMBO_MANAGERPROFILE,

  SCREENCOMBO_EDITMODE,
  SCREENCOMBO_EDITNAME,

  SCREENCOMBO_STRATEGY_FORMATION,
  SCREENCOMBO_STRATEGY_ROLES,

  SCREENCOMBO_INFO,

  SCREENCOMBO_MAX,
  SCREENCOMBO_INVALID
};


//
// FDTransferFilter
//
struct FDTransferFilter
{
  FDTransferFilter( int _minPrice = 0, int _maxPrice = 0 )
  {
    minPrice = _minPrice;
    maxPrice = _maxPrice;
    minAge = MINIMUPLAYERAGE;
    maxAge = MAXIMUMPLAYERAGE;
    minSkill = MINPLAYSKILL;
    maxSkill = MAXPLAYSKILL;
    positionType = 1;
    flags = 0;
  }

  // Price
  uint minPrice;
  uint maxPrice;

  // Age
  byte minAge;
  byte maxAge;

  // Skill
  byte minSkill;
  byte maxSkill;

  // Position type
  byte positionType;

  // Flags
  byte flags; // CPlayerSearch::INCLUDEONTRANSFERLIST, INCLUDEONLOANLIST, INCLUDENONLISTED, INCLUDEINJURED
  // INCLUDEOUTOFCONTRACT, INCLUDESUSPENDED

};

struct FDPrefsData
{
  bool operator == (const FDPrefsData &rhs) const
  {
    return ( m_sfxVolume        == rhs.m_sfxVolume &&
             m_crowdVolume      == rhs.m_crowdVolume &&
             m_matchSpeed       == rhs.m_matchSpeed &&
             m_bInMatchSFX      == rhs.m_bInMatchSFX &&
             m_bTutorialEnabled == rhs.m_bTutorialEnabled );
  }

  bool operator != (const FDPrefsData &rhs) const
  {
    return !(*this == rhs);
  }

  float       m_sfxVolume;
  float       m_crowdVolume;
  int         m_matchSpeed : 4;
  bool        m_bInMatchSFX : 1;
  bool        m_bTutorialEnabled : 1;
};

// Savedata
struct FDSaveData
{
  FDPrefsData       m_userPrefs;
  FDTransferFilter  m_filter[4];

  bool              m_bTutorialIntroViewed;
  bool              m_bTutorialViewed[ SCREENCOMBO_MAX ];
};

class FDLoadSaveCallback
{
 public:
  virtual void LoadProgress( float pct ) {}
  virtual void SaveProgress( float pct ) {}
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMainMenuUI
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDMainMenuUI : public lUI, public lHasSlots<>
{
public:
  FDMainMenuUI();
  ~FDMainMenuUI();

  // lUI interface

  virtual void        Init();
  virtual void        Deinit();
  virtual void        Activate( lUILayer * );
  virtual void        Deactivate( lUILayer * );
  virtual lUIWidget * GetScreen();
  virtual lUIWidget * GetSubScreen();
  virtual void        Tick(float delta);

  void InitWorldData();
  void DeinitWorldData();
  void ApplyEditToWorldData();
  void ReinitWorldData(const bool bApplyEditModeDataFromOptions);
  void SetWorldDataUsed( bool b );      // set when world data has been active / loaded over
  bool GetWorldDataUsed();      // set when world data has been active / loaded over

  // menu items
  //void OnMenuButtonHot( FDButtonRolloutWidget *, int );
  //void OnMenuButtonCommand( FDButtonRolloutWidget *, int );

  void OnMenuButtonHot( FDMenuWidget *, int );
  void OnMenuButtonCommand( FDMenuWidget *, int );

  void OnMenuBack( lUIControlWidget * );
  void OnMenuClock( lUIControlWidget * );
  void OnMenuHelp( lUIControlWidget * );
  void OnMenuLink( lUIControlWidget * );

  // message box
  void OnMessageBoxButton( lUIControlWidget * );

  // main menu screen
  void OnMainMenuNewButton( lUIControlWidget * );
  void OnMainMenuContinueButton( lUIControlWidget * );
  void OnMainMenuOptionsButton( lUIControlWidget * );
  void OnMainMenuEditModeButton( lUIControlWidget * );

  //
  bool IsAnyMenuActive() const;
  void SwitchToScreenCombo( ScreenComboType screenIdx, const bool bInheritMenuState, bool bStoreLastState=true, bool bPopping=false );
  void SwitchToScreenModal( ScreenComboType screenIdx, const bool bInheritMenuState );
  ScreenComboType  GetScreenCombo() const { return m_currentScreenIdx; }
  const char *GetScreenName( ScreenComboType screenIdx );
  void ShowMenuMenu( bool b );

  void SetupMenus();
  void TickMenuHighlight( float delta );

  FDScreen *GetScreenClass( ScreenComboType screenIdx );

  lUIScreenWidget * GetMainScreenWidget()
  {
    return m_rootScreenBase;
  }

  lUIScreenWidget * GetSubScreenWidget()
  {
    return m_rootScreenSub;
  }

  enum
  {
    DIALOG_STATUS_ACTIVE,
    DIALOG_STATUS_OK,
    DIALOG_STATUS_CANCEL,
  };

  void SetDialogState(int s) { m_dialogState = s; }
  int GetDialogState() const { return m_dialogState; }
  void ShowProcessScreen( bool bShow, int processType, int processPct = 100 );
  int UserMessageBox( const CString& Message, unsigned int style );
  int UserMessageBox( unsigned int MessageID, unsigned int style );

  void SetLayout( bool bMainScreen, bool bSubScreen, ScreenComboType screenIdx );

  ///////////////////////////////

  bool GamedataLoad( FDLoadSaveCallback * = NULL );
  bool GamedataSave( FDLoadSaveCallback * = NULL );
  bool GamedataExist();
  bool GamedataWipe();

  void OptionsDefault();
  bool OptionsWipe();
  bool OptionsLoad();
  bool OptionsSave();
  bool OptionsExist();
  void OptionsApply();

  void EditModeDefault();
  bool EditModeWipe();
  bool EditModeLoad();
  bool EditModeSave( FDLoadSaveCallback * = NULL );
  bool EditModeExist();

  ///////////////////////////////

  FDTransferFilter &GetTransferFilter( int filterNumber );
  void SetTransferFilter(  int filterNumber, FDTransferFilter &filter );

  void ResetEditModeData();
  FDEditModeData &GetSavedEditModeData();

  //////////////////////////////////////////////////////////////////////////
  // Accessor functions.
  FDPlayerInfoScreen *GetPlayerInfoMainScreen()             { return m_playerInfoMainScreen; }
  FDPlayerInfoSubScreen *GetPlayerInfoSubScreen()           { return m_playerInfoSubScreen; }
  FDYouthInfoScreen *GetYouthInfoScreen()                   { return m_youthInfoScreen; }
  FDEmployeeInfoScreen *GetEmployeeInfoScreen()             { return m_employeeInfoScreen; }
  FDManagerInfoScreen *GetManagerInfoScreen()               { return m_managerInfoScreen; }
  FDTransferListEditScreen *GetTransferListEditScreen()     { return m_transferlisteditScreen; }
  FDSquadScreen *GetSquadScreen()                           { return m_squadScreen; }
  FDInfoScreen *GetInfoScreen()                             { return m_infoScreen; }
  FDNegotiateScreen *GetNegotiateScreen()                   { return m_negotiateScreen; }
  FDTransferStatusScreen *GetTransferStatusScreen()         { return m_transferStatusScreen; }
  FDSackPlayerScreen *GetSackPlayerScreen()                 { return m_sackPlayerScreen; }
  FDMatchDayScreen *GetMatchDayScreen()                     { return m_matchDayScreen; }
  FDMatchSummaryScreen *GetMatchSummaryScreen()             { return m_matchSummaryScreen; }
  FDEditNameScreen *GetEditNameScreen()                     { return m_editNameScreen; }
  FDStrategyFormationScreen *GetStrategyFormationScreen()   { return m_strategyFormationScreen; }
  FDStrategyRolesScreen *GetStrategyRolesScreen()           { return m_strategyRolesScreen; }
  FDSelectTeamScreen *GetSelectTeamScreen()                 { return m_selectteamScreen; }
  FDClubInfoSubScreen *GetClubInfoSubScreen()               { return m_clubInfoSubScreen; }
  FDFullScreenWidget *GetFade()                             { return m_fade; }
  FDFullScreenWidget *GetFadeSubScreen()                    { return m_fadeSub; }
  FDNewsScreen *GetNewsScreen()                             { return m_newsScreen; }
  FDTacticsScreen *GetTacticsScreen()                       { return m_tacticsScreen; }
  FDMatchIntroScreen          * GetMatchIntroScreen()       { return m_matchIntroScreen;
}
  FDNewGameScreen             * GetNewGameScreen()          { return m_newgameScreen;
}

  lUISpriteWidget * GetMenuBar2()  { return m_menuBar2; };
  lUISpriteWidget * GetMenuBar4()  { return m_menuBar4; };

  const FDSaveData &GetSaveData() const                 { return m_saveData; }
  FDSaveData &GetSaveData()                             { return m_saveData; }

  const FDEditModeData &GetEditModeData() const         { return m_EditModeData; }
  FDEditModeData &GetEditModeData()                     { return m_EditModeData; }

  // tutorial code
  void LoadTutorial( const char *filename );
  void EnableTutorial( ScreenComboType screenIdx, bool bViewTutorialIntro = false );
  void ResetTutorialData();
  void DisableTutorial();
  bool DoesTutorialExist( ScreenComboType screenIdx );
  void TickTutorial( float delta );
  void TutorialNextPage();

  // History/screen stack functions.
  void PopScreen();
  void ClearScreenStack();

  void ForceUpdateForAllScreensOnNextShow();


  // Highlight rollout menu
  void HighlightMenuIcon( int index, int subIndex = -1 );
  void UnhighlightAll();

  void SetTitleText( const char * textsprite );

  void DisplaySubMessage( char * text );
  void HideSubMessage();

private:

  // Struct for storing off information about how a screen combination was displayed.
  struct ScreenComboInterface
  {
	  ScreenComboInterface() {}
	  ScreenComboInterface(int i) {}

    ScreenComboType m_eScreen;
	  bool m_bMenuVisible : 1;
	  bool m_bPlayMusic : 1;
	  bool m_bBackVisible : 1;
	  bool m_bMenuInherited : 1;
	  bool m_bLinkVisible : 1;

    // state data used to store specific screen information
    FDStateStorage m_stateData;
  };

  void OnModalOK(lUIControlWidget *);
  void SwitchToScreenComboInternal(ScreenComboType screenIdx, const bool bInheritMenuState, bool bStoreLastState=true, bool bPopping=false);
  void SwitchToScreenModalInternal(ScreenComboType screenIdx, const bool bInheritMenuState);

  // menu handling
  void OnMenuRolledInOrOut(FDMenuWidget *pWidget, int iRollType);
  void OnMenuBackingButtonPressed(lUIControlWidget *pWidget);

    // select correct menu index
  void SelectCorrectMenuItem( ScreenComboType screenIdx );

  // screen stack
  Vector<ScreenComboInterface> m_ScreenStack;
  ScreenComboType         m_currentScreenIdx;
  ScreenComboType         m_currentLayoutScreenIdx;

  ScreenComboType     m_eSwitchingToScreen;
  bool                m_bSwitchingToScreenInheritMenuState;
  bool                m_bSwitchingToScreenStoreLastState;
  bool                m_bSwitchingToScreenPopping;
  bool                m_bSwitchingToScreenModal;
  FDScreen          * m_activeInputScreen;

  // save data
  FDSaveData m_saveData;
  FDEditModeData m_EditModeData;

  const char *            m_helpTextID;
  lUILayer              * m_layer;

  lUIScreenWidget       * m_rootScreen;
  lUIScreenWidget       * m_rootScreenBase;
  lUIScreenWidget       * m_rootScreenSub;

  FDFullScreenWidget    * m_fade;
  FDFullScreenWidget    * m_fadeHelp;
  FDFullScreenWidget    * m_fadeSub;

  // Main menu options screen

  lUIScreenWidget       * m_mainmenuScreen;
  lUIScreenWidget       * m_mainmenuScreenSub;

  // Menu screen

  lUIScreenWidget       * m_menuScreen;
  lUISpriteWidget       * m_menuBar;
  lUISpriteWidget       * m_menuBar2;
  lUISpriteWidget       * m_menuBar4;
  lUISpriteWidget       * m_splitter;
  lUITextWidget         * m_contextHeadHelp;
  lUITextWidget         * m_contextHelp;
  FDButtonWidget        * m_clock;
  FDButtonWidget        * m_back;
  FDButtonWidget        * m_help;
  FDButtonWidget        * m_link;
  FDButtonWidget        * m_clubMenuBack;
  FDButtonWidget        * m_leagueMenuBack;
  FDButtonWidget        * m_adminMenuBack;
  FDButtonWidget        * m_gameMenuBack;

  FDMenuWidget          * m_clubMenu;
  FDMenuWidget          * m_leagueMenu;
  FDMenuWidget          * m_adminMenu;
  FDMenuWidget          * m_gameMenu;

  FDMenuWidget          * m_lastHotMenu;

  // Message box
  FDFullScreenWidget    * m_messageBoxFade;
  FDTextWidget          * m_messageBoxText;
  FDButtonWidget        * m_messageBoxOK;
  FDButtonWidget        * m_messageBoxYes;
  FDButtonWidget        * m_messageBoxNo;
  lUISpriteWidget       * m_messageBoxBg;

  // Sub message box
  lUITextWidget         * m_messageBoxTextSub;
  lUISpriteWidget       * m_messageBoxBgSub;

  // Tutorial
  FDFullScreenWidget    * m_tutorialTapScreen;
  lUISpriteWidget       * m_tutorialBoxBg;
  lUISpriteWidget       * m_tutorialSprite;
  lUITextWidget         * m_tutorialText;
  lUISpriteWidget       * m_tutorialBoxBgSub;
  lUITextWidget         * m_tutorialTextSub;
  lUISpriteWidget       * m_tutorialSpriteSub;

  FDHighlightWidget     * m_tutorialHighlight;
  FDHighlightWidget     * m_tutorialHighlightSub;

  bool                 m_bTutorialActive;
  int                  m_tutorialCurrentPage;
  float                m_tutorialPageTime;


  // Menu sub screen
  lUIScreenWidget       * m_menuScreenSub;

  FDButtonWidget        * m_contgameButton;

  // Main screen
  lUIScreenWidget       * m_mainScreen;
  lUISpriteWidget       * m_mainScreenBg;

  lUISpriteWidget       * m_subScreenTitleBar;
  lUISpriteWidget       * m_subScreenTitleText;
  lUISpriteWidget       * m_subScreenSpecial;

  lUISpriteWidget       * m_subScreenCalendar;
  lUISpriteWidget       * m_subScreenCalendarYear;
  lUISpriteWidget       * m_subScreenCalendarMonth;
  lUISpriteWidget       * m_subScreenCalendarDay;

  lUIScreenWidget       * m_mainScreenSub;
  lUISpriteWidget       * m_mainScreenSubBg;

  FDTableWidget         * m_screenHelpTable;
  FDScrollBarWidget     * m_screenHelpTableScroll;

  // Screens
  FDLoadScreen                * m_loadScreen;
  FDSaveScreen                * m_saveScreen;
  FDProgressScreen            * m_progressScreen;
  FDLeadingPlayersScreen      * m_leadingPlayerScreen;
  FDPlayerInfoScreen          * m_playerInfoMainScreen;
  FDPlayerInfoSubScreen       * m_playerInfoSubScreen;
  FDYouthInfoScreen           * m_youthInfoScreen;
  FDManagerInfoScreen         * m_managerInfoScreen;
  FDEmployeeInfoScreen        * m_employeeInfoScreen;
  FDSquadScreen               * m_squadScreen;
  FDInfoScreen                * m_infoScreen;
  FDNegotiateScreen           * m_negotiateScreen;
  FDSackPlayerScreen          * m_sackPlayerScreen;
  FDAccountsScreen            * m_accountsScreen;
  FDClubFixturesScreen        * m_clubFixturesScreen;
  FDCupFixturesScreen         * m_cupFixturesScreen;
  FDCupScheduleScreen         * m_cupScheduleScreen;
  FDLeagueFixturesScreen      * m_leagueFixturesScreen;
  FDTransferListScreen        * m_transferlistScreen;
  FDTransferListEditScreen    * m_transferlisteditScreen;
  FDTrainingScreen            * m_trainingScreen;
  FDJobCentreScreen           * m_jobCentreScreen;
  FDYouthSquadScreen          * m_youthSquadScreen;
  FDTacticsScreen             * m_tacticsScreen;
  FDMatchIntroScreen          * m_matchIntroScreen;
  FDMatchDayScreen            * m_matchDayScreen;
  FDMatchSummaryScreen        * m_matchSummaryScreen;
  FDLeagueTableScreen         * m_leagueTableScreen;
  FDClubRecordsScreen         * m_clubRecordsScreen;
  FDStatusScreen              * m_statusScreen;
  FDNewsScreen                * m_newsScreen;
  FDTransferStatusScreen      * m_transferStatusScreen;
  FDLanguageScreen            * m_languageScreen;
  FDOptions                   * m_options;
  FDWagesScreen               * m_wagesScreen;
  FDArrangeFriendlies         * m_arrangeFriendliesScreen;
  FDMonthlyAwardScreen        * m_monthlyAwardScreen;
  FDEditModeScreen            * m_editModeScreen;
  FDEditNameScreen            * m_editNameScreen;
  FDStrategyFormationScreen	  * m_strategyFormationScreen;
  FDStrategyRolesScreen       * m_strategyRolesScreen;
  FDClubInfoSubScreen         * m_clubInfoSubScreen;
  FDNewGameScreen             * m_newgameScreen;
  FDSelectTeamScreen          * m_selectteamScreen;
  FDTeamOfTheWeekScreen       * m_teamoftheweekScreen;

  // variables
  bool          m_bProcessingActive;
  int           m_messageBoxClick;
  int           m_dialogState;
  bool          m_bHelpVisible;
  bool          m_bBackShouldBeVisible;

  float         m_fHighlightTimer;
  bool          m_bGlow;
  bool          m_bExitModal;
  FDScreen     *m_pCurrentModalScreen;

  bool          m_bReadyToTick;
  bool          m_bGameOver;
};

const char *GetRandomBackground();
int GetRandomNoContractResponse();
void StartAutoplayGame();

uint GetKitColour( int index );
const char *GetBadgeSpritePostfix( int index );
uint GetAwayColour( uint homecol );
uint GetInverseColour( uint colourIndex );
uint GetInverseColour( uint colourIndex1, uint colourIndex2 );
int GetDefaultScrollBarWidth( bool bAgainstSide=true );

CCalendar GetSeasonStartDate();

END_L_NAMESPACE
#endif


