//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
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
#include <fdcombowidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>



#ifndef FDLEAGUEFIXTURESSSCREEN_H_INCLUDED
#define FDLEAGUEFIXTURESSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDLeagueFixturesScreen - League Fixtures screen (19)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDLeagueFixturesScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDLeagueFixturesScreen( FDMainMenuUI *parent );
  virtual ~FDLeagueFixturesScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDLeagueFixturesScreen interface
  void Update();

 private:
  enum LeagueFixtureFieldType
  {
	  LFFT_DATE_PLAYED,
	  LFFT_VENUE,
	  LFFT_ATTENDANCE,
    LFFT_MAX
  };

  void UpdateTable();

  void OnMatchSelect( FDTableWidget *, int row, int col );
  void OnDivisionSelect( FDComboWidget *, int index );
  void OnDateSelect( FDComboWidget *, int index );
  void Init();

  void HideDetailDisplay();

  lUIScreenWidget       * m_leaguefixScreen;
  lUIScreenWidget       * m_leaguefixScreenSub;

  FDTableWidget         * m_table;

  lUITextWidget			* m_homeClubText;
  lUITextWidget			* m_awayClubText;
  lUITextWidget			* m_homeGoalsText;
  lUITextWidget			* m_awayGoalsText;
  lUISpriteWidget		* m_homeClubKit;
  lUISpriteWidget		* m_awayClubKit;

  lUISpriteWidget		* m_homekitBG ;
  lUISpriteWidget		* m_awaykitBG ;
  FDBarWidget *       m_splitterBar;

  lArray<CCalendar>       m_dateList;
  CSoccerResultList       m_resultList;

  FDComboWidget         * m_dateSelector;
  FDComboWidget         * m_divisionSelector;

  lUISpriteWidget *m_backgroundStripes[LFFT_MAX];
  lUISpriteWidget *m_fieldStripes[LFFT_MAX];
  lUITextWidget *m_fieldLabels[LFFT_MAX];
  lUITextWidget *m_fieldValues[LFFT_MAX];

  CCalendar               m_theDate;
};




END_L_NAMESPACE
#endif

