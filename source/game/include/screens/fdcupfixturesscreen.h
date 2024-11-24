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



#ifndef FDCUPFIXTURESSSCREEN_H_INCLUDED
#define FDCUPFIXTURESSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDCupFixturesScreen - CupFixtures screen (20)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDCupFixturesScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDCupFixturesScreen( FDMainMenuUI *parent );
  virtual ~FDCupFixturesScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDCupFixturesScreen interface
  void Update();
  void SetInitialCup( int idx );

 private:

  void Init();
  void OnCupCombo( FDComboWidget *, int idx );
  void OnCupRoundCombo( FDComboWidget *, int idx );
  void OnMatchSelect( FDTableWidget *, int row, int col );


  lUIScreenWidget       * m_cupfixScreen;
  lUIScreenWidget       * m_cupfixScreenSub;

  FDTableWidget         * m_table;

  lArray<CMatchInfo>      m_matchList;
  lArray<CCalendar>       m_matchListDate;

  // cup selection
  FDComboWidget         * m_cupSelect;
  FDComboWidget         * m_cupRoundSelect;

  // top screen
  enum CupFixtureFieldType
  {
    CFFT_DATE_PLAYED,
    CFFT_VENUE,
    CFFT_ATTENDANCE,
    CFFT_MAX
  };

  lUITextWidget			* m_homeClubText;
  lUITextWidget			* m_awayClubText;
  lUITextWidget			* m_homeGoalsText;
  lUITextWidget			* m_awayGoalsText;
  lUISpriteWidget		* m_homeClubKit;
  lUISpriteWidget		* m_awayClubKit;
  lUISpriteWidget *   m_homekitBG;
  lUISpriteWidget *   m_awaykitBG;
  FDBarWidget *       m_splitterBar;

  lUISpriteWidget *m_backgroundStripes[CFFT_MAX];
  lUISpriteWidget *m_fieldStripes[CFFT_MAX];
  lUITextWidget *m_fieldLabels[CFFT_MAX];
  lUITextWidget *m_fieldValues[CFFT_MAX];

  lUITextWidget			* m_penaltyResults[2];
  lUISpriteWidget		* m_penaltyStripes[2];

  int                 m_initialCupIdx;


};




END_L_NAMESPACE
#endif

