//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmatchintroscreen.h
// Description : FDMatchIntroScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdbuttonrolloutwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>

#ifndef FDMATCHINTROSCREEN_H_INCLUDED
#define FDMATCHINTROSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE


//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDMatchIntroScreen -
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDMatchIntroScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDMatchIntroScreen( FDMainMenuUI *parent );
  virtual ~FDMatchIntroScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchIntroScreen interface
  void Update();
  virtual void AutoplaySkip();

  void SetIntroView();

  // States
  enum MatchIntroViewType
  {
    VIEW_INTRO = 0,
    VIEW_TEAMS,
    VIEW_SUBS,
    VIEW_REPORT,
    VIEW_OPPINFO,
  };

  MatchIntroViewType GetViewType() const
  {
	  return m_state;
  }

 private:

  void Init();

  void UpdateTeam();
  void UpdateSubs();
  void UpdateReport();

  void OnViewButton( lUIControlWidget * );
  void OnResultButton( lUIControlWidget * );
  void OnTeamsButton( lUIControlWidget * );
  void OnSubsButton( lUIControlWidget * );
  void OnOppositionButton( lUIControlWidget * );
  void OnBackButton( lUIControlWidget * );
  void OnCellSelect( FDTableWidget *, int row, int col);

  MatchIntroViewType      m_state;

  // Widgets

  lUIScreenWidget       * m_matchIntroScreen;
  lUIScreenWidget       * m_matchIntroScreenSub;

  FDButtonWidget        * m_matchTeamButton;
  FDButtonWidget        * m_matchSubButton;
  FDButtonWidget        * m_matchBackButton;

   FDButtonWidget        * m_matchOppositionButton;

  lUISpriteWidget *       m_backingStripes[2];
  lUISpriteWidget *       m_backingBars[2];
  lUISpriteWidget *       m_teamBoxBg;

  FDTableWidget *         m_table;

  //tacitcs
  lUISpriteWidget *       m_tacticsStripe;
  lUITextWidget *           m_tacticsText;
  FDTableWidget *         m_tacticsTable;

  FDButtonWidget        * m_matchViewButton;
  FDButtonWidget        * m_matchResultButton;

  FDButtonWidget              * m_fakeCommButton;
  FDButtonWidget              * m_fakeEventsButton;
  FDButtonWidget              * m_fakeSkillButton;
  FDButtonWidget              * m_fakeMenu;

  lUITextWidget *           m_compTitleText;
  lUITextWidget *           m_homeClubText;
  lUITextWidget *           m_vsText;
  lUITextWidget *           m_awayClubText;
  lUITextWidget *           m_stadiumText;

  lUISpriteWidget *       m_reportMessageBar;
  lUISpriteWidget *       m_reportTextStripe;
  lUITextWidget *			m_reportTitleText;
  lUITextWidget *			m_reportMessageText;
};


END_L_NAMESPACE
#endif

