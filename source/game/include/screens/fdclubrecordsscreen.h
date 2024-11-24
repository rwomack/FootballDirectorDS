//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdclubrecordsscreen.h
// Description : FDClubRecordsScreen interface
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
#include <fdbackingwidget.h>

#ifndef FDCLUBRECORDSSSCREEN_H_INCLUDED
#define FDCLUBRECORDSSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDClubRecordsScreen - ClubFixtures screen (09)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDClubRecordsScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDClubRecordsScreen( FDMainMenuUI *parent );
  virtual ~FDClubRecordsScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDClubRecordsScreen interface
  void Update();

 private:

  void Init();
  void OnTypeCombo( FDComboWidget *, int idx );
  void OnClubCombo( FDComboWidget *, int idx );
  void OnDivisionCombo( FDComboWidget *, int idx );

  lUIScreenWidget       * m_clubrecordScreen;

  lUIScreenWidget       * m_clubrecordScreenSub;

  lUISpriteWidget *         m_backingStripes[10];
  lUISpriteWidget *         m_backingBars[10];

  lUISpriteWidget *         m_compBars[3];
  lUITextWidget *         m_compLabel[3];
  FDBackingWidget *         m_compBack[3];

  lUITextWidget         * m_recordLabel[10];
  lUITextWidget         * m_recordValue[10];

  FDComboWidget         * m_recordType;
  FDComboWidget         * m_divisionList;
  FDComboWidget         * m_clubList;

  lUITextWidget         * m_matchText1;
};




END_L_NAMESPACE
#endif

