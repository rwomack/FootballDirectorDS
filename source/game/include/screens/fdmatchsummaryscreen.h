//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchSummary.h
// Description : FDMatchSummary interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbuttonwidget.h>

#include <screens/fdmatchdayscreen.h>

#ifndef FDMATCHSUMMARYSCREEN_H_INCLUDED
#define FDMATCHSUMMARYSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDMatchSummaryScreen - 17
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDMatchSummaryScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDMatchSummaryScreen( FDMainMenuUI *parent );
  virtual ~FDMatchSummaryScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchSummaryScreen interface
  void Update();
  virtual void AutoplaySkip();

  void SetMatch( CCup* _Cup, CFixture* _Fixture );
  void ShowBackButton( bool b );

  enum
	{
    VIEW_NONE,
    VIEW_SKILL,
    VIEW_EVENTS,
    VIEW_TIMELINE,
    VIEW_MAX,
	};

  void  SetView( int );
  int   GetCurrentView();

 private:

  void Init();

  void OnMenuButton( lUIControlWidget * );
  void OnDoneButton( lUIControlWidget * );

  static void TableCellRenderCallback(FDTableWidget *pTable, void *pMatchDayScreen, luint32_t uiRow, luint32_t uiColumn, lUIRect cCellRect);

  lUIScreenWidget             * m_matchSummaryScreen;

  lUIScreenWidget             * m_matchSummarySubScreen;

  FDButtonWidget              * m_matchDoneButton;
  FDButtonWidget              * m_matchDoneBackButton;

  lUITextWidget               * m_homeName;
  lUITextWidget               * m_homeScore;
  lUITextWidget               * m_awayName;
  lUITextWidget               * m_awayScore;

  lUISpriteWidget *             m_homeKit;
  lUISpriteWidget *             m_awayKit;


  FDTableWidget               * m_matchtable;

  FDButtonWidget              * m_eventsButton;
  FDButtonWidget              * m_skillButton;
  FDButtonWidget              * m_timelineButton;

  lUISpriteWidget       * m_menuBar2;

  int                           m_currentView;

  // screens to manage
  FDMatchDaySkillScreen       * m_skillScreen;
  FDMatchDayEventsScreen      * m_eventsScreen;

  //CCup                  * m_pCup;
  //CString                 m_szTitle;
  CFixture                    * m_pFixture;
  lSharedPtr<lSprite>			m_redCardSprite;
  lSharedPtr<lSprite>     m_yelCardSprite;
  lSharedPtr<lSprite>     m_goalSprite;
};


END_L_NAMESPACE
#endif

