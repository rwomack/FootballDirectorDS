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


#ifndef FDCLUBFIXTURESSSCREEN_H_INCLUDED
#define FDCLUBFIXTURESSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDClubFixturesScreen - ClubFixtures screen (18)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDClubFixturesScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDClubFixturesScreen( FDMainMenuUI *parent );
  virtual ~FDClubFixturesScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDClubFixturesScreen interface
  void Update();

 private:

  void OnMatchSelect( FDTableWidget *, int row, int col );
  void Init();

  enum ClubFixtureFieldType
  {
	  CFFT_DATE_PLAYED,
	  CFFT_VENUE,
	  CFFT_ATTENDANCE,
	  CFFT_COMPETITION,
	  CFFT_MAX
  };

  lUIScreenWidget       * m_clubfixScreen;
  lUIScreenWidget       * m_clubfixScreenSub;

  lUITextWidget			* m_penaltyResults[2];
  lUISpriteWidget		* m_penaltyStripes[2];

  FDTableWidget         * m_table;

  lUITextWidget			* m_homeClubText;
  lUITextWidget			* m_awayClubText;
  lUITextWidget			* m_homeGoalsText;
  lUITextWidget			* m_awayGoalsText;
  lUISpriteWidget		* m_homeClubKit;
  lUISpriteWidget		* m_awayClubKit;
  FDBarWidget *       m_splitterBar;
  lUISpriteWidget * m_homekitBG;
  lUISpriteWidget * m_awaykitBG;

  lUISpriteWidget *m_backgroundStripes[CFFT_MAX];
  lUISpriteWidget *m_fieldStripes[CFFT_MAX];
  lUITextWidget *m_fieldLabels[CFFT_MAX];
  lUITextWidget *m_fieldValues[CFFT_MAX];

  lArray<CMatchInfo>      m_matchList;
  lArray<CCalendar>       m_matchListDate;
};




END_L_NAMESPACE
#endif

