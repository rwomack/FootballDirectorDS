//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// File        : fdmatchdayscreen.h
// Description : FDMatchDayXXXXXScreen interface
// Notes       :
//
//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdmenuwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>
#include <fdfullscreenwidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#ifndef FDMATCHDAYSCREEN_H_INCLUDED
#define FDMATCHDAYSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

class FDMarqueeWidget;
class FDDragWidget;
class FDTeleprinterWidget;

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// FDMatchDaySkillScreen - Match Day screens (65)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class FDMatchDaySkillScreen : public FDScreen
{
 public:
  FDMatchDaySkillScreen( FDMainMenuUI *parent );
  virtual ~FDMatchDaySkillScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchDaySkillScreen interface
  void Update( CFixture &Fixture );

 private:

  void Init();

  lUIScreenWidget       * m_matchSkillScreen;

  lUISpriteWidget *       m_longStripes[7];

  FDBarWidget *       m_skillBars[8];

  FDBarWidget           * m_possessionBar;

  lUITextWidget         * m_possA;
  lUITextWidget         * m_possB;
  lUITextWidget         * m_bartext;

};

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// FDMatchDayCommentaryScreen - Match Day screens (65)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class FDMatchDayCommentaryScreen : public FDScreen
{
 public:
  FDMatchDayCommentaryScreen( FDMainMenuUI *parent );
  virtual ~FDMatchDayCommentaryScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchDayScreen interface
  void Clear();
  void Update();
  void AddString( const CString &str, unsigned int colour );
  void SkipToEnd();

 private:

  void Init();

  lUIScreenWidget       * m_matchCommentaryScreen;
  FDTeleprinterWidget   * m_commtable;
};


//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// FDMatchDayTeamScreen - Match Day screens (65)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class FDMatchDayTeamScreen : public FDScreen
{
 public:
  FDMatchDayTeamScreen( FDMainMenuUI *parent );
  virtual ~FDMatchDayTeamScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchDayTeamScreen interface
  void Update( CMatchPlay* _MatchPlay );


 private:

  void Init();

  lUIScreenWidget       * m_matchTeamScreen;
  FDTableWidget         * m_teamstable;
};

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// FDMatchDaySubsScreen - Match Day screens (61)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class FDMatchDaySubsScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDMatchDaySubsScreen( FDMainMenuUI *parent );
  virtual ~FDMatchDaySubsScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchDaySubsScreen interface
  void Update( CMatchPlay* _MatchPlay );

 private:
  void OnFormationsButton(lUIControlWidget *);
  void OnRolesButton(lUIControlWidget *);
  void OnTacticsButton(lUIControlWidget *);
  void OnSwapButton(lUIControlWidget *);
  void OnRowSelect(FDTableWidget * pTable, int row );

  CMatchSubs&   SubsInfo() { return m_matchPlay->GetFixture().GetMatchSubs(); }

  void Init();

  lUIScreenWidget       * m_matchSubsScreen;

  FDTableWidget         * m_teamtable;
  FDTableWidget         * m_substable;

  FDButtonWidget		* m_formationsButton;
  FDButtonWidget		* m_rolesButton;
  FDButtonWidget    * m_tacticsButton;
  FDButtonWidget    * m_swapButton;

  lUITextWidget *     m_subsRemainingText;

  int m_selectedPlayerIdx;
  int m_selectedSubPlayerIdx;
  bool m_bSwap;

  CMatchPlay            * m_matchPlay;
};

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// FDMatchDayEventsScreen - Match Day screens (65)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class FDMatchDayEventsScreen : public FDScreen
{
 public:
  FDMatchDayEventsScreen( FDMainMenuUI *parent );
  virtual ~FDMatchDayEventsScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchDayScreen interface
  void Update( CFixture &Fixture );

  struct BarInfo
  {
    FDBarWidget   * m_bar;
    lUITextWidget * m_barText;
    lUITextWidget * m_barA;
    lUITextWidget * m_barB;
  };

 private:

  void Init();

  lUIScreenWidget       * m_matchEventsScreen;

  lArray<BarInfo>         m_barList;
  lUISpriteWidget *       m_longStripes[7];
};


//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// FDMatchDayScreen - Match Day screens  (65)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
class FDMatchDayScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDMatchDayScreen( FDMainMenuUI *parent );
  virtual ~FDMatchDayScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void AutoplaySkip();

  // FDMatchDayScreen interface
  void Update();
  void SetMatchPlay( CMatchPlay* _MatchPlay );
  void Tick( float delta );

  enum MatchDayViewType
	{
    VIEW_COMMENTARY,
    VIEW_SKILL,
    VIEW_LINEUPS,
    VIEW_EVENTS,
    VIEW_SUBS,
    //VIEW_RADAR,
    VIEW_LATESTSCORES,
    VIEW_LEAGUETABLE,
    VIEW_MAX,
	};

  void  SetView( MatchDayViewType eView );
  MatchDayViewType GetCurrentView() const;

 private:

  void Init();
  void OnTimer();

  void OnMenuButton( lUIControlWidget * );
  void OnDoneButton( lUIControlWidget * );

  void OnPlayButton( lUIControlWidget * );
  void OnPauseButton( lUIControlWidget * );
  void OnSkipButton( lUIControlWidget * );

  void OnMenuButtonHot( FDMenuWidget *, int );
  void OnMenuButtonCommand( FDMenuWidget *, int );
  void OnMenuBackingButton(lUIControlWidget *);

  void OnMarqueeDone(FDMarqueeWidget *pMarquee);

  void          DoPenaltyShootOut();
  void          SetMenuForFullTime( bool b );

  void          DoAddCommentary( const CString &strComment, COLORREF colorText = BLUE );
  void          DoUpdateCommentary();

  static void TableCellRenderCallback(FDTableWidget *pTable, void *pMatchDayScreen, luint32_t uiRow, luint32_t uiColumn, lUIRect cCellRect);
  static void SubstitutionMadeCallback(void *pMatchDayScreen, const eWhichTeam _Who, ushort _Player1SquadNumber, ushort _Player2SquadNumber);
  static void SkipMatchCommentaryCallback(void *pMatchDayScreen);
  static void SkipMatchPenaltyShootOutCallback(void *pMatchDayScreen);

  //void          DoBuildLineupGrid();
	void					DoCommenceHalf();
	void					DoEndOfHalf();
  //void          DoPlaySoundFX(const eSOUNDFX theFX);
  ///

  void RefreshPenaltyInfo();

  float                         m_timer;

  lUIScreenWidget             * m_matchSubScreen;

  lUIScreenWidget             * m_matchScreen;

  lUITextWidget               * m_homeName;
  lUITextWidget               * m_homeScore;
  lUITextWidget               * m_awayName;
  lUITextWidget               * m_awayScore;

  FDTableWidget               * m_matchtable;

  lUISpriteWidget *             m_homeKit;
  lUISpriteWidget *             m_awayKit;

  //lUITextWidget               * m_halftimeScore;

  lSharedPtr<lSprite>			m_redCardSprite;
  lSharedPtr<lSprite>     m_yelCardSprite;
  lSharedPtr<lSprite>     m_goalSprite;
  lSharedPtr<lSprite>     m_penHitSprite;
  lSharedPtr<lSprite>     m_penMissSprite;

  int m_liveTableMenuIdx;
  int m_liveScoreMenuIdx;

  FDButtonWidget              * m_playButton;
  FDButtonWidget              * m_pauseButton;

  FDButtonWidget              * m_skipButton;

  FDButtonWidget              * m_commButton;
  FDButtonWidget              * m_eventsButton;
  FDButtonWidget              * m_skillButton;
  FDMenuWidget                * m_menu;
  FDButtonWidget			  * m_menuBack;

  lUISpriteWidget       * m_dateBar;
  lUITextWidget         * m_subDateTime;

  lUISpriteWidget       * m_prematchMessageBar;
  lUITextWidget         * m_prematchMessageText;

  FDFullScreenWidget    * m_fadeEnd;

  FDFullScreenWidget    * m_fade;
  FDFullScreenWidget    * m_fadeSub;

  lUISpriteWidget       * m_menuBar4;

  // info
  MatchDayViewType              m_currentView;
  CMatchPlay                  * m_matchPlay;

  // screens to manage
  FDMatchDaySkillScreen       * m_skillScreen;
  FDMatchDayEventsScreen      * m_eventsScreen;
  FDMatchDayTeamScreen        * m_teamScreen;
  FDMatchDayCommentaryScreen  * m_commentaryScreen;
  FDMatchDaySubsScreen        * m_subsScreen;
  FDLiveLeagueTableScreen     * m_leagueScreen;
  FDLiveResultScreen          * m_resultsScreen;

  FDMarqueeWidget			  * m_marquee;
  FDMarqueeWidget			  * m_preMatchMarquee;
  FDButtonWidget              * m_matchDoneButton;
  lUITextWidget				  * m_matchDoneText;

  CString                       m_strLastComment;
  int							m_iLastMarqueeEvent;

  int m_currentRowHome;
  int m_currentRowAway;

  int m_currentShootOutHome;
  int m_currentShootOutAway;

  bool m_penaltyShootOutStarted;

  struct
  {
	bool                        m_SubstitutionRequested : 1;
	bool						m_bEndOfHalf : 1;
	bool                        m_bUserPause : 1;
	bool						m_bWaitForMarquee : 1;
	bool						m_bPenaltyHome : 1;
	bool						m_bSkippingMatch : 1;
	int                         m_iNonUserPauseCount : 12;
  };
};

END_L_NAMESPACE
#endif
