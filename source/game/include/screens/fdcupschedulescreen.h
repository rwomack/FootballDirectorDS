//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdcupschedulescreen.h
// Description : FDCupScheduleScreen interface
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


#ifndef FDCUPSCHEDULESCREEN_H_INCLUDED
#define FDCUPSCHEDULESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDCupScheduleScreen - CupSchedule screen (20)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDCupScheduleScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDCupScheduleScreen( FDMainMenuUI *parent );
  virtual ~FDCupScheduleScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );

  // FDCupScheduleScreen interface
  void Update();

 private:

  void Init();
  void OnCupCombo( FDComboWidget *, int idx );

  lUIScreenWidget       * m_cupschScreen;
  lUIScreenWidget       * m_cupschScreenSub;

  FDTableWidget         * m_table;
  FDComboWidget         * m_cupSelect;

  lUITextWidget         * m_cupName;
};




END_L_NAMESPACE
#endif

